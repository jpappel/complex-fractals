#include "util.h"

/*
 * Converts a grid point into an complex number
 */
double complex lattice_to_complex(const size_t index, const vec2 resolution) {
    const double x_min = -2.0;
    const double x_max = 2.0;
    const double y_min = -2.0;
    const double y_max = 2.0;

    const double x_step = (x_max - x_min) / (double)resolution.x;
    const double y_step = (y_max - y_min) / (double)resolution.y;

    const size_t x_index = index % resolution.x;
    const size_t y_index = index / resolution.x;

    const double x = x_min + x_index * x_step;
    const double y = y_min + y_index * y_step;

    return x + y * I;
}
