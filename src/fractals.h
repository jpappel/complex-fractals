#pragma once

#ifndef __NVCC__
#include <complex.h>
#endif

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

#ifndef __NVCC__
size_t mandelbrot(const CBASE complex z0, const size_t max_iterations);
size_t tricorn(const CBASE complex z0, const size_t max_iterations);
size_t burning_ship(const CBASE complex z0, const size_t max_iterations);
size_t multibrot(const CBASE complex z0, const size_t max_iterations, const double d);
size_t multicorn(const CBASE complex z0, const size_t max_iterations, const double d);
size_t julia(const CBASE complex z0, const CBASE complex c, const size_t max_iterations, const double R);
#endif

#ifdef __cplusplus
extern "C" {
#endif
void mandelbrot_grid(grid_t* grid, const grid_gen_params* params);
void tricorn_grid(grid_t* grid, const grid_gen_params* params);
void burning_ship_grid(grid_t* grid, const grid_gen_params* params);
void multibrot_grid(grid_t* grid, const grid_gen_params* params);
void multicorn_grid(grid_t* grid, const grid_gen_params* params);
void julia_grid(grid_t* grid, const grid_gen_params* params);
#ifdef __cplusplus
}
#endif
