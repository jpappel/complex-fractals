#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "grids.h"
#include "fractals.h"


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


    const double complex lower_left = -2 + -2*I;
    const double complex upper_right = 2 + 2*I;
    grid_t* grid = create_grid(x_res, y_res, lower_left, upper_right);
    if(!grid) return 1;


    const size_t size = grid->size;
    size_t* data = grid->data;
    for(size_t i = 0; i < size;i++){
        data[i] = multibrot(grid_to_complex(grid, i), iterations, 3);
    }

    for(size_t i = 0; i < size; i++){
        printf("%zu%s", data[i], (i % x_res == x_res - 1) ? "\n" : "\t");
    }
}
