#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t x;
    size_t y;
    size_t size;
    size_t* data;
} grid_t;

grid_t* create_grid(const size_t x, const size_t y);
void set_grid(grid_t* grid, const double val);
grid_t* copy_grid(const grid_t* grid);
void free_grid(grid_t* grid);
bool grid_equal(const grid_t* grid1, const grid_t* grid2);
bool grid_allclose(const grid_t* grid1, const grid_t* grid2, const size_t max_error);
