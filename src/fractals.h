#pragma once

#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include "grids.h"

size_t mandelbrot(const long double complex z0, const size_t max_iterations);
void mandelbrot_grid(grid_t* grid,  const size_t max_iterations);

size_t multibrot(const long double complex z0, const size_t max_iterations, const double d);
void multibrot_grid(grid_t* grid, const size_t max_iterations, const double d);

size_t julia(const double R, const long double complex z0, const long double complex c, const size_t max_iterations);
