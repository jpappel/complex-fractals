#pragma once

#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include "grids.h"
#include "precision.h"

enum fractal {
    MANDELBROT, // IMPLEMENTED IN SERIAL SHARED
    TRICORN, // IMPLEMENTED IN SERIAL SHARED
    MULTIBROT, // IMPLEMENTED IN SERIAL SHARED
    MULTICORN, // IMPLEMENTED IN SERIAL SHARED
    BURNING_SHIP, // IMPLEMENTED IN SERIAL SHARED
    //NEWTON,  // MIGHT NEVER BE IMPLEMENTED, REQUIRES SPECIAL COLORING
    JULIA // IMPLEMENTED IN SERIAL SHARED
};

size_t mandelbrot(const CBASE complex z0, const size_t max_iterations);
void mandelbrot_grid(grid_t* grid, const size_t max_iterations);

size_t tricorn(const CBASE complex z0, const size_t max_iterations);
void tricorn_grid(grid_t* grid, const size_t max_iterations);

size_t burning_ship(const CBASE complex z0, const size_t max_iterations);
void burning_ship_grid(grid_t* grid, const size_t max_iterations);

size_t multibrot(const CBASE complex z0, const size_t max_iterations, const double d);
void multibrot_grid(grid_t* grid, const size_t max_iterations, const double d);

size_t multicorn(const CBASE complex z0, const size_t max_iterations, const double d);
void multicorn_grid(grid_t* grid, const size_t max_iterations, const double d);

size_t julia(const CBASE complex z0, const CBASE complex c, const size_t max_iterations, const double R);
void julia_grid(grid_t* grid, const size_t max_iterations, const complex_t c, const double R);
