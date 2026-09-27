
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;


// --------------------------------------------------
// Count alive neighbours
// --------------------------------------------------
inline int countNeighbors(const vector<unsigned char>& grid,
                          int rows,
                          int cols,
                          int row,
                          int col)
{
    int count = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        // Skip rows outside the grid
        if (i < 0 || i >= rows)
            continue;

        for (int j = col - 1; j <= col + 1; j++)
        {
            // Skip columns outside the grid
            if (j < 0 || j >= cols)
                continue;

            // Do not count the current cell
            if (i == row && j == col)
                continue;

            count += grid[i * cols + j];
        }
    }

    return count;
}


// --------------------------------------------------
// Calculate next generation
// --------------------------------------------------
void nextGeneration(const vector<unsigned char>& grid,
                    vector<unsigned char>& nextGrid,
                    int rows,
                    int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int neighbors =
                countNeighbors(grid, rows, cols, i, j);

            int index = i * cols + j;

            // Current cell is alive
            if (grid[index] == 1)
            {
                // Survives with 2 or 3 neighbours
                nextGrid[index] =
                    (neighbors == 2 || neighbors == 3) ? 1 : 0;
            }

            // Current cell is dead
            else
            {
                // Becomes alive with exactly 3 neighbours
                nextGrid[index] =
                    (neighbors == 3) ? 1 : 0;
            }
        }
    }
}


// --------------------------------------------------
// Initialize grid
// --------------------------------------------------
void initializeGrid(vector<unsigned char>& grid,
                    int rows,
                    int cols)
{
    // Start with all cells dead
    fill(grid.begin(), grid.end(), 0);

    // Create a blinker pattern in the middle
    int centerRow = rows / 2;
    int centerCol = cols / 2;

    grid[centerRow * cols + centerCol] = 1;
    grid[(centerRow - 1) * cols + centerCol] = 1;
    grid[(centerRow + 1) * cols + centerCol] = 1;
}


// --------------------------------------------------
// Main function
// --------------------------------------------------
int main()
{
    // Grid size
    const int ROWS = 1000;
    const int COLS = 1000;

    // Number of iterations
    const int ITERATIONS = 100;


    // --------------------------------------------------
    // Allocate two grids
    // --------------------------------------------------

    vector<unsigned char> grid(ROWS * COLS);
    vector<unsigned char> nextGrid(ROWS * COLS);


    // --------------------------------------------------
    // Initialize grid
    // --------------------------------------------------

    initializeGrid(grid, ROWS, COLS);


    // --------------------------------------------------
    // Start timer
    // --------------------------------------------------

    auto start = high_resolution_clock::now();


    // --------------------------------------------------
    // Run Game of Life
    // --------------------------------------------------

    for (int iteration = 0;
         iteration < ITERATIONS;
         iteration++)
    {
        // Calculate next generation
        nextGeneration(
            grid,
            nextGrid,
            ROWS,
            COLS
        );

        // Swap grids instead of copying
        grid.swap(nextGrid);
    }


    // --------------------------------------------------
    // Stop timer
    // --------------------------------------------------

    auto end = high_resolution_clock::now();


    // Calculate execution time
    auto duration =
        duration_cast<milliseconds>(end - start);


    // --------------------------------------------------
    // Display results
    // --------------------------------------------------

    cout << "====================================" << endl;
    cout << "     Optimized CPU Game of Life" << endl;
    cout << "====================================" << endl;

    cout << "Grid Size       : "
         << ROWS << " x " << COLS << endl;

    cout << "Iterations      : "
         << ITERATIONS << endl;

    cout << "Execution Time  : "
         << duration.count()
         << " milliseconds" << endl;

    cout << "====================================" << endl;


    return 0;
}

