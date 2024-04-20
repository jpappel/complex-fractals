#include <stdlib.h>
#include <string.h>
#include "plotting.h"

/*
 * Gets the corresponding color for a given number of iterations according to a gradient
 */
color get_color(size_t iterations, size_t max_iterations, gradient map){
    size_t val = (max_iterations - iterations) / max_iterations;
    color color;

    //TODO: map the interval of [0,max_iterations] to a value given a gradient
    return color;
}

/*
 * Gets the associated "red" value for a given number of iterations according to a gradient
 */
char get_red(size_t iterations, size_t max_iterations, gradient map){
    char red;

    return red;
}

/*
 * Gets the associated "green" value for a given number of iterations according to a gradient
 */
char get_green(size_t iterations, size_t max_iterations, gradient map){
    char green;

    return green;
}

/*
 * Gets the associated "blue" value for a given number of iterations according to a gradient
 */
char get_blue(size_t iterations, size_t max_iterations, gradient map){
    char blue;

    return blue;
}

colors_t* create_colors(size_t x, size_t y){
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

    colors->x = x;
    colors->y = y;
    colors->size = x*y;
    colors->red = red;
    colors->green = green;
    colors->blue = blue;

    return colors;
}

colors_t* copy_colors(colors_t* colors){
    if(!colors) return NULL;

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
