#include <stddef.h>

#pragma once

typedef struct {
    char red;
    char green;
    char blue;
} color;

typedef struct {
    size_t x;
    size_t y;
    size_t size;
    char* red;
    char* green;
    char* blue;
} colors_t;

typedef char gradient;

// for AoS
color get_color(size_t iterations, size_t max_iterations, gradient map);

// for SoA
char get_red(size_t iterations, size_t max_iterations, gradient map);
char get_green(size_t iterations, size_t max_iterations, gradient map);
char get_blue(size_t iterations, size_t max_iterations, gradient map);
colors_t* create_colors(size_t x, size_t y);
colors_t* copy_colors(colors_t* colors);
void free_colors(colors_t* colors);
