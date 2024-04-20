#pragma once

#include <complex.h>
#include <stddef.h>
#include <stdint.h>

size_t mandelbrot(const double complex z0, const size_t max_iterations);
size_t multibrot(const double complex z0, const size_t max_iterations, const uintmax_t d);
size_t julia(const double R, const double complex z0, const double complex c, const size_t max_iterations);
