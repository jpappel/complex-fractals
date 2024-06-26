#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef __NVCC__
#include <complex.h>
#endif
#include "precision.h"

//grid write errors
#define GRID_NO_DATA 1
#define GRID_WRITE_ERROR 2

#define GRID_MAGIC_NUMBER 0xA6005E

// hack to allow variable precision at compile time
typedef struct {
    CBASE re;
    CBASE im;
} complex_t;

// I've gone back and forth on the size for a grid point, eventually I settled on a byte
// ideally it should just be its own type which can be configured similarly to precision.h
// but it does not seem worth
typedef unsigned char byte;

typedef struct {
    size_t x;
    size_t y;
    size_t size;
    byte max_iterations;
    complex_t lower_left;
    complex_t upper_right;
    byte* data;
} grid_t;

grid_t* create_grid(const size_t x, const size_t y, const byte max_iterations, complex_t lower_left, complex_t upper_right);
void set_grid(grid_t* grid, const byte val);
grid_t* copy_grid(const grid_t* grid);
void free_grid(grid_t* grid);
bool grid_equal(const grid_t* grid1, const grid_t* grid2);
// not useful
bool grid_allclose(const grid_t* grid1, const grid_t* grid2, const byte max_error);

#ifndef __NVCC__
CBASE complex grid_to_complex(const grid_t* grid, const size_t index);
#endif

void zoom_grid(grid_t* grid, const CBASE magnification);

void print_grid_info(const grid_t* grid);
void print_grid(FILE* file, const grid_t* grid);
int write_grid(FILE* file, const grid_t* grid);
grid_t* read_grid(FILE* file);
