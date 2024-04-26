#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#include "grids.h"
#include "precision.h"
#include "fractals.h"

#ifndef NUM_RUNS
#define NUM_RUNS 5
#endif

void print_usage(FILE* file, const char* program_name){
    fprintf(file, "Usage: %s [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-l lower_left] [-u upper_right] [-o output_grid] -f fractal\n", program_name);
}

void print_help(){
    printf("Options:\n"
           "  -i, --iterations <value>        the number of iterations (default: 100)\n"
           "  -x, --x-res <value>             the horizontal resolution (default: terminal width)\n"
           "  -y, --y-res <value>             the vertical resolution (default: terminal height)\n"
           "  -l, --lower-left <value>        Set the lower left corner of the fractal area (default: -2-2i)\n"
           "  -u, --upper-right <value>       Set the upper right corner of the fractal area (default: 2+2i)\n"
           "  -z, --magnification <value>     Set the magnification factor (default: 1)\n"
           "  -o, --output <filename>         the output filename (default: fractal.grid)\n"
           "  -f, --fractal <type>            the fractal type (default: mandelbrot)\n"
           "      supported fractals: mandelbrot, tricorn, multibrot, multicorn, burning_ship, julia\n"
           "  -p, --performance               print performance info\n"
           "  -v, --verbose                   verbose output\n"
           "  -h, --help                      prints this help message\n");
}

void print_info(const char* program_name){
    #ifdef EXTENDED_PRECISION
    printf("Compiled with long double float precision\n");
    #endif
    #ifndef EXTENDED_PRECISION
    printf("%s complied with double float precision\n", program_name);
    #endif
}

double time_fractal(fractal_generator generator, grid_t* grid, grid_gen_params* params){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(size_t i = 0; i < NUM_RUNS; i++){
        generator(grid, params);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) * 1.0e-9) / NUM_RUNS;
}

int main(const int argc, char *argv[]) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //default values
    size_t iterations = 100;
    size_t x_res = w.ws_col;
    size_t y_res = w.ws_row;
    complex_t lower_left = { .re = -2, .im = -2};
    complex_t upper_right = { .re = 2, .im = 2};
    CBASE magnification = 1;
    bool verbose = false;
    bool performance = false;
    grid_gen_params* params = NULL;
    char* fractal_name = "mandelbrot";
    fractal_generator generator = mandelbrot_grid;
    char* output_filename = "fractal.grid";

    static struct option long_options[] = {
        {"iterations", required_argument, NULL, 'i'},
        {"x-res", required_argument, NULL, 'x'},
        {"y-res", required_argument, NULL, 'y'},
        {"lower-left", required_argument, NULL, 'l'},
        {"upper-right", required_argument, NULL, 'u'},
        {"magnification", required_argument, NULL, 'z'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"performance", no_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {"fractal", required_argument, NULL, 'f'},
        {0, 0, 0, 0} // Termination element
    };

    //parse command line arguments
    int opt;
    while((opt = getopt_long(argc, argv, "i:x:y:l:u:z:o:vphf:", long_options, NULL)) != -1){
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
                sscanf(optarg, CFORMAT"+"CFORMAT"i", &lower_left.re, &lower_left.im);
                break;
            case 'u':
                sscanf(optarg, CFORMAT"+"CFORMAT"i", &upper_right.re, &upper_right.im);
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'f':
                if(strncmp(optarg, "mandelbrot", strlen("mandelbrot")) == 0) {
                    fractal_name = "mandelbrot";
                    generator = mandelbrot_grid;
                }
                else if(strncmp(optarg, "tricorn", strlen("tricorn")) == 0) {
                   fractal_name = "tricorn";
                   generator = tricorn_grid;
                }
                else if(strncmp(optarg, "multibrot", strlen("multibrot")) == 0) {
                   fractal_name = "multibrot";
                   generator = multibrot_grid;
                   params = malloc(sizeof(grid_gen_params));
                   params->degree = 3;
                }
                else if(strncmp(optarg, "multicorn", strlen("multicorn")) == 0) {
                   fractal_name = "multicorn";
                   generator = multicorn_grid;
                   params = malloc(sizeof(grid_gen_params));
                   params->degree = 3;
                }
                else if(strncmp(optarg, "burning_ship", strlen("burning_ship")) == 0) {
                   fractal_name = "burning ship";
                   generator = burning_ship_grid;
                }
                else if(strncmp(optarg, "julia", strlen("julia")) == 0) {
                   fractal_name = "julia";
                   generator = julia_grid;
                   params = malloc(sizeof(grid_gen_params));
                   params->cr.radius = 100;
                   params->cr.constant = (complex_t){ .re = 0.285, .im = 0.01 };
                }
                else {
                    fprintf(stderr, "Invalid fractal type: %s, see --help for a list of supported fractals\n", optarg);
                    return 1;
                }
                break;
            case 'z':
                sscanf(optarg, CFORMAT, &magnification);
                if(magnification <= 0){
                    fprintf(stderr, "Invalid magnification "CFORMAT", exitting\n", magnification);
                    return 1;
                }
                break;
            case 'v':
                verbose = true;
                break;
            case 'p':
                performance = true;
                break;
            case 'h':
                print_usage(stdout, argv[0]);
                print_help();
                return 0;
            default:
                print_usage(stderr, argv[0]);
                return 1;
        }
    }

    grid_t* grid = create_grid(x_res, y_res, iterations, lower_left, upper_right);
    if(!grid) return 1;


    if(magnification != 1){
        zoom_grid(grid, magnification);
    }

    generator(grid, params);

    if(performance){
        double time = time_fractal(generator, grid, params);
        printf("%s,%s,%zu,%zu,%zu,"
                CFORMAT","CFORMAT","CFORMAT","CFORMAT",%f\n",
                argv[0], fractal_name, iterations, x_res, y_res,
                lower_left.re, lower_left.im, upper_right.re, upper_right.im, time);
    }

    if(verbose){
        print_info(argv[0]);
        printf("Magnification:\t"CFORMAT"\n", magnification);
        print_grid_info(grid);
    }

    //use "safer" versions of c string functions
    //likely aren't necessary unless a user can pass non-null terminated strings as arguments, but that would likely break something up in getopt
    if(output_filename[0] == '-' && strnlen(output_filename, 16) == 1){
        if(write_grid(stdout, grid) == GRID_WRITE_ERROR){
            fprintf(stderr, "Error occured while writting to file %s\n", output_filename);
        }
    }
    else {
        FILE* file = fopen(output_filename, "wb");
        if(!file){
            perror("Error occured while trying to write");
        }
        else if(write_grid(file, grid) == GRID_WRITE_ERROR){
            fprintf(stderr, "Error occured while writting to file %s\n", output_filename);
        }
        fclose(file);
    }

    free(params);
    free_grid(grid);

    return 0;
}
