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
    double re_lower_left = -2;
    double im_lower_left = -2;
    double re_upper_right = 2;
    double im_upper_right = 2;
    double magnification = 1;
    bool verbose = false;

    //parse command line arguments
    int opt;
    while((opt = getopt(argc, argv, "vhi:x:y:l:u:z:")) != -1){
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
            case 'l':
                sscanf(optarg, "%lf+%lfi", &re_lower_left, &im_lower_left);
                break;
            case 'u':
                sscanf(optarg, "%lf+%lfi", &re_upper_right, &im_upper_right);
                break;
            case 'z':
                sscanf(optarg, "%lf", &magnification);
                if(magnification <= 0){
                    fprintf(stderr, "Invalid magnification %f, exitting\n", magnification);
                    return 1;
                }
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                fprintf(stderr, "Usage: %s [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-l lower_left] [-u upper_right]\n", argv[0]);
                return 0;
            default:
                fprintf(stderr, "Usage: %s [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-l lower_left] [-u upper_right]\n", argv[0]);
                return 1;
        }
    }

    const double complex lower_left = re_lower_left + im_lower_left * I;
    const double complex upper_right = re_upper_right + im_upper_right * I;

    grid_t* grid = create_grid(x_res, y_res, lower_left, upper_right);
    if(!grid) return 1;


    if(magnification != 1){
        if(verbose) printf("Magnification: %f\n", magnification);
        zoom_grid(grid, magnification);
    }


    mandelbrot_grid(grid, iterations);

    if(verbose)print_grid_info(grid);
    print_grid(grid, iterations);
}
