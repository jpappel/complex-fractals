#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the multibrot set
 * This is implementation closely matches mandelbrot
 * Note, only positive integer powers are supported
 */
size_t multibrot(const double complex z0, const size_t max_iterations, const uintmax_t d){
    double complex z = z0;
    double complex ztemp;
    size_t iteration = 0;
    while(cabs(z) <= 2 && iteration < max_iterations){
        ztemp = z;
        for(size_t i = 0; i < d; i ++){
            ztemp *= ztemp;
        }
        z = ztemp + z0;
        iteration++;
    }
    return iteration;
}

/*
 * Converts a grid point into an complex number
 */
double complex lattice_to_complex(const size_t index, const size_t x_res, const size_t y_res) {
    const double x_min = -2.0;
    const double x_max = 2.0;
    const double y_min = -2.0;
    const double y_max = 2.0;

    const double x_step = (x_max - x_min) / (double)x_res;
    const double y_step = (y_max - y_min) / (double)y_res;

    const size_t x_index = index % x_res;
    const size_t y_index = index / x_res;

    const double x = x_min + x_index * x_step;
    const double y = y_min + y_index * y_step;

    return x + y * I;
}


int main(const int argc, char *argv[]) {
    //default values
    size_t iterations = 1000;
    size_t x_res = 100;
    size_t y_res = 100;

    //parse command line arguments
    int opt;
    while((opt =getopt(argc, argv, "i:x:y:")) != -1){
        switch(opt){
            case 'i':
                iterations = strtoull(optarg, NULL, 10);
                break;
            case 'x':
                x_res = strtoull(optarg, NULL, 10);
                break;
            case 'y':
                y_res = strtoull(optarg, NULL, 10);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i iterations] [-x x_res] [-y y_res]\n", argv[0]);
                return 1;
        }
    }


    grid_t* grid = create_grid(x_res, y_res);
    if(!grid) return 1;


    const size_t size = grid->size;
    size_t* data = grid->data;
    for(size_t i = 0; i < size;i++){
        data[i] = multibrot(lattice_to_complex(i, x_res, y_res), iterations, 3);
    }

    for(size_t i = 0; i < size; i++){
        printf("%zu%s", data[i], (i % x_res == x_res - 1) ? "\n" : "\t");
    }
}
