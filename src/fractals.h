#pragma once

#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include "grids.h"
#include "precision.h"

size_t mandelbrot(const CBASE complex z0, const size_t max_iterations);
void mandelbrot_grid(grid_t* grid,  const size_t max_iterations);

size_t multibrot(const CBASE complex z0, const size_t max_iterations, const double d);
void multibrot_grid(grid_t* grid, const size_t max_iterations, const double d);

size_t julia(const CBASE complex z0, const CBASE complex c, const size_t max_iterations, const double R);
void julia_grid(grid_t* grid, const size_t max_iterations, const CBASE complex c, const double R);
