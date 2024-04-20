#include <stdlib.h>
#include <string.h>
#include "grids.h"

/*
 * Creates a grid for storing the results of the escape algorithm
 */
grid_t* create_grid(const size_t x, const size_t y){
    if(x <= 0 || y <= 0) return NULL;

    const size_t size = x * y;
    size_t* data = malloc(size * sizeof(size_t));
    if(!data) return NULL;

    grid_t* grid = malloc(sizeof(grid_t));
    if(!grid){
        free(data);
        return NULL;
    }

    grid->x = x;
    grid->y = y;
    grid->size = size;
    grid->data = data;

    return grid;
}

/*
 * Sets all entries of a grid to the value val
 */
void set_grid(grid_t* grid, const double val){
    if(!grid || !grid->data) return;
    memset(grid->data, val, grid->size);
}

/*
 * Creates a copy of a grid
 */
grid_t* copy_grid(const grid_t* grid){
    if(!grid || !grid->data) return NULL;

    grid_t* grid_copy = create_grid(grid->x, grid->y);
    if(!grid_copy) return NULL;

    memcpy(grid_copy->data, grid->data, grid->size);

    return grid_copy;
}

/*
 * Frees a grid and its members
 */
void free_grid(grid_t* grid){
    if(!grid) return;

    free(grid->data);
    free(grid);
}

/*
 * Checks if two grids are exactly equal
 * This may return incorrect values due to floating point arrithmetic errors 
 * that occur while the grid is being filled
 */
bool grid_equal(const grid_t* grid1, const grid_t* grid2){
    return grid1->x == grid2->x && grid1->y == grid2->y &&
        memcmp(grid1->data, grid2->data, grid1->size) == 0;
}

/*
 * Checks if two grids have a given maximum difference
 */
bool grid_allclose(const grid_t *grid1, const grid_t *grid2, const size_t max_error){
    if(grid1->x != grid2->x || grid1->y != grid2->y) return false;
    const size_t size = grid1->size;
    for(size_t i = 0; i < size; i++){
        if(abs(grid1->data[i] - grid2->data[i]) >= max_error) return false;
    }
    return true;
}
