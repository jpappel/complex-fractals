#include "fractals.h"

#include <omp.h>
#include "fractals.h"
#include "precision.h"

/*
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the mandelbrot set
 * This should be identical to the version found in serial-fractals.c
 */
size_t mandelbrot(const CBASE complex z0, const size_t max_iterations){
    CBASE complex z = z0;
    size_t iteration = 0;
    while(CABS(z) <= 2 && iteration < max_iterations){
        z = z*z + z0;
        iteration++;
    }

    return iteration;
}

/*
 * Fills a grid with mandelbrot values
 */
void mandelbrot_grid(grid_t* restrict grid, const size_t max_iterations){
    const size_t size = grid->size;
    size_t* data = grid->data;

    #pragma omp parallel for default(none) shared(data, size, grid, max_iterations) schedule(dynamic)
    for(size_t i = 0; i < size; i++){
        data[i] = mandelbrot(grid_to_complex(grid, i), max_iterations);
    }
}

/*
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the multibrot set
 * This should be identical to the version found in serial-fractals.c
 */
size_t multibrot(const CBASE complex z0, const size_t max_iterations, const double d){
    CBASE complex z = z0;
    size_t iteration = 0;
    while(CABS(z) <= 2 && iteration < max_iterations){
        z = CPOW(z, d) + z0;
        iteration++;
    }
    return iteration;
}


/*
 * Fills a grid with multibrot values
 */
void multibrot_grid(grid_t* restrict grid, const size_t max_iterations, const double d){
    const size_t size = grid->size;
    size_t* data = grid->data;

    #pragma omp parallel for default(none) shared(data, size, grid, max_iterations, d) schedule(dynamic)
    for(size_t i = 0; i < size; i ++){
        data[i] = multibrot(grid_to_complex(grid, i), max_iterations, d);
    }
}

/*
 * Computes ????? for a julia set
 * implementation of https://en.wikipedia.org/wiki/Julia_set#Pseudocode
 *
 * This behaves weirdly, needs a very small number of iterations to be visibile
 */

size_t julia(const CBASE complex z0, const CBASE complex c, const size_t max_iterations, const double R){
    double complex z = z0;

    size_t iteration = 0;
    while(CABS(z) < R && iteration < max_iterations){
        z = z * z + c;
        iteration++;
    }
    return iteration;
}

void julia_grid(grid_t* restrict grid, const size_t max_iterations, const CBASE complex c, const double R){
    const size_t size = grid->size;
    size_t* data = grid->data;
    #pragma omp parallel for default(none) shared(data, size, grid, max_iterations, c, R) schedule(dynamic)
    for(size_t i = 0; i < size; i++){
        data[i] = julia(grid_to_complex(grid, i), c, max_iterations, R);
    }
}
