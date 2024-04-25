#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include "grids.h"
#include "precision.h"
#include "fractals.h"


void print_usage(FILE* file, char* program_name){
    fprintf(file, "Usage: %s [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-l lower_left] [-u upper_right]\n", program_name);
}

void print_help(){
    //TODO: add help
}

void print_info(){
    #ifdef EXTENDED_PRECISION
    printf("Compiled with long double float precision\n");
    #endif
    #ifndef EXTENDED_PRECISION
    printf("Compiled with double float precision\n");
    #endif
}

int main(const int argc, char *argv[]) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //default values
    size_t iterations = 100;
    size_t x_res = w.ws_col;
    size_t y_res = w.ws_row;
    CBASE re_lower_left = -2;
    CBASE im_lower_left = -2;
    CBASE re_upper_right = 2;
    CBASE im_upper_right = 2;
    CBASE magnification = 1;
    bool verbose = false;
    //TODO: allocate adequate size buffer
    bool output_to_file = true;
    char* output_filename = "fractal.grid";

    // TODO: have output format option
    // TODO: have output file


    static struct option long_options[] = {
        {"iterations", required_argument, NULL, 'i'},
        {"x-res", required_argument, NULL, 'x'},
        {"y-res", required_argument, NULL, 'y'},
        {"lower-left", required_argument, NULL, 'l'},
        {"upper-right", required_argument, NULL, 'u'},
        {"magnification", required_argument, NULL, 'z'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0} // Termination element
    };

    //parse command line arguments
    int opt;
    while((opt = getopt_long(argc, argv, "i:x:y:l:u:z:o:vh", long_options, NULL)) != -1){
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
                sscanf(optarg, CFORMAT"+"CFORMAT"i", &re_lower_left, &im_lower_left);
                break;
            case 'u':
                sscanf(optarg, CFORMAT"+"CFORMAT"i", &re_upper_right, &im_upper_right);
                break;
            case 'o':
                //TODO: check if can write to location
                output_to_file = true;
                //TODO: 
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
            case 'h':
                print_usage(stdout, argv[0]);
                return 0;
            default:
                print_usage(stderr, argv[0]);
                return 1;
        }
    }

    //const CBASE complex lower_left = re_lower_left + im_lower_left * I;
    const complex_t lower_left = { .re=re_lower_left, .im=im_lower_left};
    //const CBASE complex upper_right = re_upper_right + im_upper_right * I;
    const complex_t upper_right = { .re=re_upper_right, .im=im_upper_right};

    grid_t* grid = create_grid(x_res, y_res, iterations, lower_left, upper_right);
    if(!grid) return 1;


    if(magnification != 1){
        if(verbose) printf("Magnification: "CFORMAT"\n", magnification);
        zoom_grid(grid, magnification);
    }

    // params for different fractals
    const double degree = 3.0;
    // const CBASE complex constant = 0.285L + 0.01L*I;
    const complex_t constant = {
        .re = 0.285,
        .im = 0.01
    };
    // const CBASE complex constant = -0.835L -0.321L* I;
    const double radius = 100;

    // enum fractal f = BURNING_SHIP;
    enum fractal f = JULIA;
    switch(f){
        case MANDELBROT:
            mandelbrot_grid(grid);
            break;
        case TRICORN:
            tricorn_grid(grid);
            break;
        case MULTIBROT:
            multibrot_grid(grid, degree);
            break;
        case MULTICORN:
            multicorn_grid(grid, degree);
            break;
        case BURNING_SHIP:
            burning_ship_grid(grid);
            break;
        case JULIA:
            julia_grid(grid, constant, radius);
            break;
        default:
            fprintf(stderr, "Unrecognized fractal\n");
            return 1;
    }

    if(verbose)print_grid_info(grid);

    //write grid to file
    if(output_to_file){
        FILE* write_file = fopen("test.grid", "wb");
        int err = write_grid(write_file , grid);
        if(err == GRID_WRITE_ERROR){
            fprintf(stderr, "Error writing occured while writting to file %s\n", output_filename);
        }
        fclose(write_file);
    }

    free_grid(grid);
    // //attempt to read grid from file
    // FILE* read_file = fopen("test2.grid", "rb");
    // grid_t* grid2 = read_grid(read_file);
    // fclose(read_file);
    //
    // printf("Grids are %s equal\n", grid_equal(grid, grid2) ? "exactly" :"not exactly");

    return 0;
}
