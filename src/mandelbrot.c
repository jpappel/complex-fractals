#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "plotting.h"

/*
 * Computes the number of iterations it takes for a point z0 to diverge
 * if the return value is equal to max_iterations, the point lies within the mandelbrot set
 * This is an implementation the esacpe algorithm
 */
size_t mandelbrot(double complex z0, size_t max_iterations) {
  double complex z = 0.0 + 0.0*I;
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
size_t multibrot(double complex z0, size_t max_iterations, uintmax_t d){
    double complex z = 0.0 + 0.0*I;
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


int main(const int argc, const char *argv[]) {
    double complex z = 1.0 + 0.0*I;
    size_t result = mandelbrot(z, 1000);
    printf("Input: %f+%fI\n", creal(z), cimag(z));
    printf("result: %zu\n", result);
}
