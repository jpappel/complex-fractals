#pragma once

#include <complex.h>
#include <stddef.h>

typedef struct {
    size_t x;
    size_t y;
} vec2;

double complex lattice_to_complex(const size_t index, const vec2 resolution);

