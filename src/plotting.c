#include <stdlib.h>
#include <string.h>
#include "plotting.h"

/*
 * Gets the corresponding color for a given number of iterations according to a gradient
 */
color get_color(const size_t iterations, const size_t max_iterations, const gradient map){
    const size_t val = (max_iterations - iterations) / max_iterations;
    color color;

    //TODO: map the interval of [0,max_iterations] to a value given a gradient
    return color;
}

/*
 * Gets the associated "red" value for a given number of iterations according to a gradient
 */
char get_red(const size_t iterations, const size_t max_iterations, const gradient map){
    char red;

    return red;
}

/*
 * Gets the associated "green" value for a given number of iterations according to a gradient
 */
char get_green(const size_t iterations, const size_t max_iterations, const gradient map){
    char green;

    return green;
}

/*
 * Gets the associated "blue" value for a given number of iterations according to a gradient
 */
char get_blue(const size_t iterations, const size_t max_iterations, const gradient map){
    char blue;

    return blue;
}

colors_t* create_colors(const size_t x, const size_t y){
    if(x <= 0 || y <= 0) return NULL;

    char* red = malloc(x*y);
    char* green = malloc(x*y);
    char* blue = malloc(x*y);
    
    if(!red || !green || !blue){
        free(red); free(green); free(blue);
        return NULL;
    }

    colors_t* colors = malloc(sizeof(colors_t));
    if(!colors){
        free(red); free(green); free(blue);
        return NULL;
    }

    *colors = (colors_t){
        .x = x,
        .y = y,
        .size = x*y,
        .red = red,
        .green = green,
        .blue = blue
    };

    return colors;
}

colors_t* copy_colors(const colors_t* colors){
    if(!colors || !colors->red  || !colors->green || !colors->blue) return NULL;

    colors_t* colors_copy = create_colors(colors->x, colors->y);
    if(!colors_copy) return NULL;

    const size_t size = colors->size;

    memcpy(colors_copy->red, colors->red, size);
    memcpy(colors_copy->green, colors->green, size);
    memcpy(colors_copy->blue, colors->blue, size);

    return colors_copy;
}

void free_colors(colors_t* colors){
    if(!colors) return;

    free(colors->red);
    free(colors->green);
    free(colors->blue);
    free(colors);
}
