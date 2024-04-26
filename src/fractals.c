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

#define EXIT_BAD_ARGUMENT 2

#ifndef NUM_RUNS
#define NUM_RUNS 5
#endif

void print_usage(FILE* file, const char* program_name){
    fprintf(file, "Usage: %s [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-d degree] [-c constant] [-r radius] [-l lower_left] [-u upper_right] [-o output_grid] -f fractal\n", program_name);
}

void print_help(){
    printf("Options:\n"
            "  -i, --iterations <value>        the number of iterations (default: 25, max 255)\n"
            "  -x, --x-res <value>             the horizontal resolution (default: terminal width)\n"
            "  -y, --y-res <value>             the vertical resolution (default: terminal height)\n"
            "  -l, --lower-left <value>        Set the lower left corner of the fractal area (default: -2.0+-2.0i)\n"
            "  -u, --upper-right <value>       Set the upper right corner of the fractal area (default: 2.0+2.0i)\n"
            "  -z, --magnification <value>     Set the magnification factor (default: 1)\n"
            "  -d, --degree <value>            Set the degree for fractals that use it (default: 2)\n"
            "  -c, --constant <value>          Set the constant for fractals that use it (default: 0+0i)\n"
            "  -r, --radius <value>            Set the radius for fractals that use it (default: 2)\n"
            "  -o, --output <filename>         the output filename (default: fractal.grid)\n"
            "  -f, --fractal <type>            the fractal type (default: mandelbrot)\n"
            "      supported fractals: mandelbrot, tricorn, multibrot, multicorn, burning_ship, julia\n"
            "  -p, --performance               print performance info\n"
            "  -v, --verbose                   verbose output\n"
            "  -h, --help                      prints this help message\n"
            "\ndegree is mutually exclusive with constant and radius\n"
            "\nExits with a status code of 1 if the program encounters an error, exits with 2 if an argument is incorrect\n");
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

static inline void parse_complex(const char* string, complex_t* z){
    if(sscanf(string, CFORMAT "+" CFORMAT "i", &z->re, &z->im) != 2){
        fprintf(stderr, "Failed while parsing complex number: %s , is it formatted correctly?\n", string);
        exit(EXIT_FAILURE);
    };
}

int main(const int argc, char *argv[]) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //default values
    byte iterations = 25;
    size_t x_res = w.ws_col;
    size_t y_res = w.ws_row;
    complex_t lower_left = { .re = -2, .im = -2};
    complex_t upper_right = { .re = 2, .im = 2};
    CBASE magnification = 1;
    bool verbose = false;
    bool performance = false;

    //degree is mutually exclusive with constant and radius
    bool param_is_degree = false;
    bool param_is_cr = false;
    CBASE degree = 2;
    complex_t constant = { .re = 0, .im = 0};
    double radius = 2;

    char* fractal_name = "mandelbrot";
    fractal_generator generator = mandelbrot_grid;
    char* output_filename = "fractal.grid";

    grid_gen_params* params = malloc(sizeof(grid_gen_params));
    if(!params){
        fprintf(stderr, "Failed to allocate memory: %zu bytes\n", sizeof(grid_gen_params));
        exit(EXIT_FAILURE);
    }

    static struct option long_options[] = {
        {"iterations", required_argument, NULL, 'i'},
        {"x-res", required_argument, NULL, 'x'},
        {"y-res", required_argument, NULL, 'y'},
        {"lower-left", required_argument, NULL, 'l'},
        {"upper-right", required_argument, NULL, 'u'},
        {"magnification", required_argument, NULL, 'z'},
        {"degree", required_argument, NULL, 'd'},
        {"constant", required_argument, NULL, 'c'},
        {"radius", required_argument, NULL, 'r'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"performance", no_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {"fractal", required_argument, NULL, 'f'},
        {0, 0, 0, 0} // Termination element
    };

    unsigned long temp;
    //parse command line arguments
    int opt;
    while((opt = getopt_long(argc, argv, "i:x:y:l:u:z:d:c:r:o:vphf:", long_options, NULL)) != -1){
        switch(opt){
            case 'i':
                temp = strtoul(optarg, NULL, 10);
                if(temp > 255){
                    fprintf(stderr, "Iterations above maximum, setting to 255\n");
                    iterations = 255;
                }
                else {
                    iterations = temp;
                }
                break;
            case 'x':
                x_res = strtoull(optarg, NULL, 10);
                break;
            case 'y':
                y_res = strtoull(optarg, NULL, 10);
                break;
            case 'l':
                parse_complex(optarg, &lower_left);
                break;
            case 'u':
                parse_complex(optarg, &upper_right);
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'd':
                param_is_degree = true;
                if(param_is_cr){
                    fprintf(stderr, "--degree and --constant --radius are mutually exclusive, exiting\n");
                    exit(EXIT_BAD_ARGUMENT);
                }
                if(sscanf(optarg, CFORMAT, &degree) != 1){
                    fprintf(stderr, "Failed to parse degree: %s, exitting\n", optarg);
                    exit(EXIT_BAD_ARGUMENT);
                }
                break;
            case 'c':
                if(param_is_degree){
                    fprintf(stderr, "--degree and --constant --radius are mutually exclusive, exiting\n");
                    exit(EXIT_BAD_ARGUMENT);
                }
                parse_complex(optarg, &constant);
                param_is_cr = true;
                break;
            case 'r':
                if(param_is_degree){
                    fprintf(stderr, "--degree and --constant --radius are mutually exclusive, exiting\n");
                    exit(EXIT_BAD_ARGUMENT);
                }
                if(sscanf(optarg, CFORMAT, &radius) != 1){
                    fprintf(stderr, "Failed to parse radius: %s, exitting\n", optarg);
                    exit(EXIT_BAD_ARGUMENT);
                }
                param_is_cr = true;
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
                    if(param_is_cr){
                        fprintf(stderr, "multibrot requires a degree, not constant and radius, exitting\n");
                        exit(EXIT_BAD_ARGUMENT);
                    }
                    fractal_name = "multibrot";
                    generator = multibrot_grid;
                }
                else if(strncmp(optarg, "multicorn", strlen("multicorn")) == 0) {
                    if(param_is_cr){
                        fprintf(stderr, "multicorn requires a degree, not constant and radius, exitting\n");
                        exit(EXIT_BAD_ARGUMENT);
                    }
                    fractal_name = "multicorn";
                    generator = multicorn_grid;
                }
                else if(strncmp(optarg, "burning_ship", strlen("burning_ship")) == 0) {
                    fractal_name = "burning ship";
                    generator = burning_ship_grid;
                }
                else if(strncmp(optarg, "julia", strlen("julia")) == 0) {
                    if(param_is_degree){
                        fprintf(stderr, "julia requires a constant and a radius, not a degree, exitting\n");
                        exit(EXIT_BAD_ARGUMENT);
                    }
                    fractal_name = "julia";
                    generator = julia_grid;
                }
                else {
                    fprintf(stderr, "Invalid fractal type: %s, see --help for a list of supported fractals\n", optarg);
                    exit(EXIT_BAD_ARGUMENT);
                }
                break;
            case 'z':
                if(sscanf(optarg, CFORMAT, &magnification) != 1){
                    fprintf(stderr, "Failed to parse magnification: %s, exitting\n", optarg);
                    exit(EXIT_BAD_ARGUMENT);
                }
                if(magnification <= 0){
                    fprintf(stderr, "Invalid magnification "CFORMAT", exitting\n", magnification);
                    exit(EXIT_BAD_ARGUMENT);
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
                return 2;
        }
    }

    if(param_is_degree){
        params->degree = degree;
    }
    else {
        params->cr.constant = constant;
        params->cr.radius = radius;
    }

    grid_t* grid = create_grid(x_res, y_res, iterations, lower_left, upper_right);
    if(!grid) return 1;


    if(magnification != 1){
        zoom_grid(grid, magnification);
    }

    generator(grid, params);

    if(performance){
        double time = time_fractal(generator, grid, params);
        printf("%s,%s,%lf,"CFORMAT","CFORMAT",%lf,%hhu,%zu,%zu,",
                argv[0], fractal_name, degree, constant.re, constant.im, radius, iterations, x_res, y_res);
        printf(CFORMAT","CFORMAT","CFORMAT","CFORMAT",%f\n",
                lower_left.re, lower_left.im, upper_right.re, upper_right.im, time);
    }

    if(verbose){
        print_info(argv[0]);
        printf("Magnification:\t"CFORMAT"\n", magnification);
        print_grid_info(grid);
    }

    if(!performance){
        //uses "safer" versions of c string functions
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
    }

    free(params);
    free_grid(grid);

    return 0;
}
