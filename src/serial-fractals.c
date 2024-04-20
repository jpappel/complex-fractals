#include "fractals.h"
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

