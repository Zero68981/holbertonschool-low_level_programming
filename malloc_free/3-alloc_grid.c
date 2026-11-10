#include "main.h"
#include <stdlib.h>

/**
 * alloc_grid - creates a 2D array of integers, all set to 0
 * @width: number of columns
 * @height: number of rows
 *
 * Return: pointer to the grid, or NULL on failure
 */
int **alloc_grid(int width, int height)
{
int **grid;
int i, j;

/**checks on width and height */
if (width <= 0 || height <= 0)
return (NULL);

/**Allocating the array of row pointers*/
grid = malloc(sizeof(int *) * height);
if (grid == NULL)
return (NULL);

/**Allocating each row*/
for (i = 0; i < height; i++)
{
grid[i] = malloc(sizeof(int) * width);

/**Handling failure in the middle*/
if (grid[i] == NULL)
{
for (j = 0; j < i; j++)
free(grid[j]);
free(grid);
return (NULL);
}

/**Filling with zeros*/
for (j = 0; j < width; j++)
grid[i][j] = 0;
}

/**Returning the result*/
return (grid);
}
