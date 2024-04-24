#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include "precision.h"

//grid write errors
#define GRID_NO_DATA 1
#define GRID_WRITE_ERROR 2

#define GRID_MAGIC_NUMBER 0xA6005E

// hack to  determine variable precision at compile time
typedef struct {
    CBASE re;
    CBASE im;
} complex_t;

typedef struct {
    size_t x;
    size_t y;
    size_t size;
    complex_t lower_left;
    complex_t upper_right;
    size_t* data;
} grid_t;

grid_t* create_grid(const size_t x, const size_t y, complex_t lower_left, complex_t upper_right);
void set_grid(grid_t* grid, const size_t val);
grid_t* copy_grid(const grid_t* grid);
void free_grid(grid_t* grid);
bool grid_equal(const grid_t* grid1, const grid_t* grid2);
bool grid_allclose(const grid_t* grid1, const grid_t* grid2, const size_t max_error);

CBASE complex grid_to_complex(const grid_t* grid, const size_t index);
void zoom_grid(grid_t* grid, const CBASE magnification);

void print_grid_info(const grid_t* grid);
void print_grid(const grid_t* grid, const size_t iterations);
int write_grid(FILE* file, const grid_t* grid);
grid_t* read_grid(FILE* file);
