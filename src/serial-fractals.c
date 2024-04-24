#include "fractals.h"
#include <math.h>
#include "precision.h"
#include "grids.h"

/*
 * Computes the number of iterations it takes for a point z0 to become unbounded
 * if the return value is equal to max_iterations, the point lies within the mandelbrot set
 * This is an implementation the escape algorithm
 */
size_t mandelbrot(const CBASE complex z0, const size_t max_iterations) {
  CBASE complex z = z0;
  size_t iteration = 0;

  while (CABS(z) <= 2 && iteration < max_iterations) {
    z = z * z + z0;
    iteration++;
  }
  return iteration;
}

/*
 * Fills a grid with mandelbrot values
 */
void mandelbrot_grid(grid_t* grid, const size_t max_iterations){
    const size_t size = grid->size;
    size_t* data = grid->data;

    for(size_t i = 0; i < size; i++){
        data[i] = mandelbrot(grid_to_complex(grid, i), max_iterations);
    }
}

/*
 * Computes the number of iterations it takes for a point z0 to become unbounded
 * if the return value is equal to max_iterations, the point lies within the tricorn set
 * This is nearly identical to mandelbrot, except for the complex conjugate
 */
size_t tricorn(const CBASE complex z0, const size_t max_iterations){
    CBASE complex z = z0;
    size_t iteration = 0;
    while(CABS(z) <= 2 && iteration < max_iterations){
        z = CONJ(z * z) + z0;
        iteration++;
    }
    return iteration;
}

/*
 * Fills a grid with tricorn values
 */
void tricorn_grid(grid_t* grid, const size_t max_iterations){
    const size_t size = grid->size;
    size_t* data = grid->data;

    for(size_t i = 0; i < size; i++){
        data[i] = tricorn(grid_to_complex(grid, i), max_iterations);
    }
}

/*
 * Computes the number of iterations it takes for a point z0 to become unbounded
 * if the return value is equal to max_iterations, the point lies within the burningship set (oh no! I hope they have fire safety gear)
 */
size_t burning_ship(const CBASE complex z0, const size_t max_iterations) {
  CBASE complex z = z0;
  CBASE complex z_mod;
  size_t iteration = 0;

  while (CABS(z) <= 2 && iteration < max_iterations) {
    z_mod = RABS(CREAL(z)) + RABS(CIMAG(z))*I;
    z = z_mod * z_mod + z0;
    iteration++;
  }
  return iteration;
}

/*
 * Fills a grid with burning_ship values
 */
void burning_ship_grid(grid_t* grid, const size_t max_iterations){
    const size_t size = grid->size;
    size_t* data = grid->data;

    for(size_t i = 0; i < size; i++){
        data[i] = burning_ship(grid_to_complex(grid, i), max_iterations);
    }
}

/*
 * Computes the number of iterations it takes for a point z0 to become unbounded
 * if the return value is equal to max_iterations, the point lies within the multibrot set
 * This is implementation closely matches mandelbrot, but uses cpow which might degrade performance.
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
void multibrot_grid(grid_t* grid, const size_t max_iterations, const double d){
    const size_t size = grid->size;
    size_t* data = grid->data;
    for(size_t i = 0; i < size; i ++){
        data[i] = multibrot(grid_to_complex(grid, i), max_iterations, d);
    }
}

/*
 * Computes the number ofiterations it takes for a point z0 to become unbounded
 * if the return value is equal to max_iterations, the point lies within the multicorn set
 * This function is to tricorn as multibrot is to mandelbrot
 */
size_t multicorn(const CBASE complex z0, const size_t max_iterations, const double d){
    CBASE complex z = z0;
    size_t iteration = 0;
    while(CABS(z) <= 2 && iteration < max_iterations){
        z = CONJ(CPOW(z, d)) + z0;
        iteration++;
    }
    return iteration;
}

/*
 * Fills a grid with multicorn values
 */
void multicorn_grid(grid_t* grid, const size_t max_iterations, const double d){
    const size_t size = grid->size;
    size_t* data = grid->data;
    for(size_t i = 0; i < size; i ++){
        data[i] = multicorn(grid_to_complex(grid, i), max_iterations, d);
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

void julia_grid(grid_t* grid, const size_t max_iterations, const complex_t constant, const double R){
    const size_t size = grid->size;
    const CBASE complex c = constant.re + constant.im * I;
    size_t* data = grid->data;
    for(size_t i = 0; i <size; i++){
        data[i] = julia(grid_to_complex(grid, i), c, max_iterations, R);
    }
}
