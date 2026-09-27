#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <algorithm>

using namespace std;
using namespace chrono;

// Calculate the next generation
inline void nextGeneration(
    const vector<uint8_t>& current,
    vector<uint8_t>& next,
    int rows,
    int cols)
{
    // +2 because we keep a dead border around the grid
    int stride = cols + 2;

    for (int i = 1; i <= rows; i++)
    {
        int base = i * stride;

        for (int j = 1; j <= cols; j++)
        {
            int idx = base + j;

            // Count 8 neighbours
            int neighbours =
                current[idx - stride - 1] +
                current[idx - stride] +
                current[idx - stride + 1] +
                current[idx - 1] +
                current[idx + 1] +
                current[idx + stride - 1] +
                current[idx + stride] +
                current[idx + stride + 1];

            // Game of Life rules
            if (current[idx] == 1)
            {
                // Alive cell survives with 2 or 3 neighbours
                next[idx] = (neighbours == 2 || neighbours == 3) ? 1 : 0;
            }
            else
            {
                // Dead cell becomes alive with exactly 3 neighbours
                next[idx] = (neighbours == 3) ? 1 : 0;
            }
        }
    }
}


// Initialize the grid
void initializeGrid(
    vector<uint8_t>& grid,
    int rows,
    int cols)
{
    int stride = cols + 2;

    // Start with all cells dead
    fill(grid.begin(), grid.end(), 0);

    // Create a small blinker pattern in the middle
    int r = rows / 2 + 1;
    int c = cols / 2 + 1;

    grid[(r - 1) * stride + c] = 1;
    grid[r * stride + c] = 1;
    grid[(r + 1) * stride + c] = 1;
}


// Run Game of Life and return execution time
long long runGame(
    int rows,
    int cols,
    int iterations)
{
    int stride = cols + 2;

    // Continuous memory
    vector<uint8_t> grid(
        (rows + 2) * stride, 0);

    vector<uint8_t> nextGrid(
        (rows + 2) * stride, 0);

    initializeGrid(grid, rows, cols);

    auto start = high_resolution_clock::now();

    // Run 100 generations
    for (int iteration = 0;
         iteration < iterations;
         iteration++)
    {
        nextGeneration(
            grid,
            nextGrid,
            rows,
            cols);

        // Swap instead of copying the whole grid
        grid.swap(nextGrid);
    }

    auto end = high_resolution_clock::now();

    // Calculate checksum so compiler cannot remove the calculation
    long long aliveCells = 0;

    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            aliveCells +=
                grid[i * stride + j];
        }
    }

    cout << "Final alive cells: "
         << aliveCells << endl;

    return duration_cast<milliseconds>(
        end - start).count();
}


int main()
{
    const int ITERATIONS = 100;
    const int REPEATS = 3;

    // Test different grid sizes
    int sizes[] = {
        1000,
        1024,
        2048
    };

    cout << "====================================\n";
    cout << "Optimized CPU Game of Life\n";
    cout << "====================================\n";

    for (int size : sizes)
    {
        cout << "\nGrid Size: "
             << size << " x " << size << endl;

        long long totalTime = 0;

        for (int run = 1;
             run <= REPEATS;
             run++)
        {
            cout << "Run " << run << ": ";

            long long time =
                runGame(
                    size,
                    size,
                    ITERATIONS);

            cout << "Execution Time: "
                 << time << " ms"
                 << endl;

            totalTime += time;
        }

        double average =
            totalTime / (double)REPEATS;

        cout << "Average Time: "
             << average << " ms"
             << endl;
    }

    return 0;
}