#pragma once

#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include "grids.h"
#include "precision.h"

typedef union {
    CBASE degree;
    struct {
        complex_t constant;
        double radius;
    } cr ;
} grid_gen_params ;

typedef void (*fractal_generator)(grid_t* , const grid_gen_params* );

size_t mandelbrot(const CBASE complex z0, const size_t max_iterations);
void mandelbrot_grid(grid_t* grid, const grid_gen_params* params);

size_t tricorn(const CBASE complex z0, const size_t max_iterations);
void tricorn_grid(grid_t* grid, const grid_gen_params* params);

size_t burning_ship(const CBASE complex z0, const size_t max_iterations);
void burning_ship_grid(grid_t* grid, const grid_gen_params* params);

size_t multibrot(const CBASE complex z0, const size_t max_iterations, const double d);
void multibrot_grid(grid_t* grid, const grid_gen_params* params);

size_t multicorn(const CBASE complex z0, const size_t max_iterations, const double d);
void multicorn_grid(grid_t* grid, const grid_gen_params* params);

size_t julia(const CBASE complex z0, const CBASE complex c, const size_t max_iterations, const double R);
void julia_grid(grid_t* grid, const grid_gen_params* params);
