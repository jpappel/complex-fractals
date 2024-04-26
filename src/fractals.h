#pragma once

#include <stdint.h>
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
byte mandelbrot(const CBASE complex z0, const byte max_iterations);
byte tricorn(const CBASE complex z0, const byte max_iterations);
byte burning_ship(const CBASE complex z0, const byte max_iterations);
byte multibrot(const CBASE complex z0, const byte max_iterations, const double d);
byte multicorn(const CBASE complex z0, const byte max_iterations, const double d);
byte julia(const CBASE complex z0, const CBASE complex c, const byte max_iterations, const double R);
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
