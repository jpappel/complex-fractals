#include "fractals.h"
#include "grids.h"
/*
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the mandelbrot set
 * This is an implementation the escape algorithm
 */
size_t mandelbrot(const double complex z0, const size_t max_iterations) {
  double complex z = z0;
  size_t iteration = 0;

  while (cabs(z) <= 2 && iteration < max_iterations) {
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
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the multibrot set
 * This is implementation closely matches mandelbrot, but uses cpow which might degrade performance.
 */
size_t multibrot(const double complex z0, const size_t max_iterations, const double d){
    double complex z = z0;
    size_t iteration = 0;
    while(cabs(z) <= 2 && iteration < max_iterations){
        z = cpowl(z, d) + z0;
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
 * Computes ????? for a julia set
 * implementation of https://en.wikipedia.org/wiki/Julia_set#Pseudocode
 */
size_t julia(const double R, const double complex z0, const double complex c, const size_t max_iterations){
    //FIXME: I'm notsure if this is currently implemented correctly
    if(R*R - R >= cabs(z0)) return 0;
    double complex z = z0;

    size_t iteration = 0;
    while(cabs(z) < R && iteration < max_iterations){
        z = z * z + c;
        iteration++;
    }
    return iteration;
}

