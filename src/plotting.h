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
color get_color(const size_t iterations, const size_t max_iterations, const gradient map);

// for SoA
char get_red(const size_t iterations, const size_t max_iterations, const gradient map);
char get_green(const size_t iterations, const size_t max_iterations, const gradient map);
char get_blue(const size_t iterations, const size_t max_iterations, const gradient map);
colors_t* create_colors(size_t x, size_t y);
colors_t* copy_colors(const colors_t* colors);
void free_colors(colors_t* colors);
