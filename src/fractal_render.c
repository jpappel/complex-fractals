#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <setjmp.h>

#include "grids.h"
#include "precision.h"
#include "fractal_render.h"
#include "renderers.h"

#define BUFFER_SIZE 32

void print_usage(FILE* file, const char* program_name) {
    fprintf(file, "Usage: %s -i input.grid [-r renderer] [-o output.ext]\n", program_name);
}

void print_help(){
    printf("Options:\n"
           "  -i, --input <input grid>        the grid to be rendered, if the file name is '-' reads from stdin\n"
           "  -r, --renderer <renderer>       the renderer to use, defaults to the text renderer\n"
           "      renderers:   txt, png, gif (TODO, with additional features)\n"
           "  -d, --delay <delay>             the delay between animation frames in 1/100 s\n"
           "  -o, --output <output file>      the file to output the result of rendering, if not given defaults to fractal.out\n"
           "  -v, --verbose                   verbose output\n"
           "  -h, --help                      prints this help and exits\n"
          );
}

jmp_buf error_buffer;

/*
 * Error logger that causes used file streams to be closed
 */
void error_exit(const char* message, const char* issue) {
    if(issue){
        fprintf(stderr, "%s: %s\n", message, issue);
    }
    else {
        fprintf(stderr, "%s\n", message);
    }
    longjmp(error_buffer, 1);
}

/*
 * Cleanup io files and a grid
 */
void cleanup(FILE* output_file, FILE* input_file, grid_t* grid) {
    if(output_file) fclose(output_file);
    // closing stdin is not recommended, can supposedly cause odd behvaior
    if(input_file && input_file != stdin) fclose(input_file);
    if(grid) free_grid(grid);
}

/*
 * Wrapper for print_grid to meet renderer type
 */
void render_txt(FILE* output, const renderer_params* params){
    print_grid(output, params->grid);
}



int main(const int argc, char* argv[]){
    // defaults
    char* input_filename = "fractal.grid";
    char* output_filename = "fractal.txt";
    renderer_func renderer = render_txt;
    int anim_delay = 30;
    bool multigrid = false;
    bool verbose = false;
    renderer_params* params = malloc(sizeof(renderer_params));

    static struct option long_options[] = {
        {"input", required_argument, NULL, 'i'},
        {"renderer", required_argument, NULL, 'r'},
        {"delay", required_argument, NULL, 'd'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "i:r:o:d:vh", long_options, NULL)) != -1){
        switch(opt){
            case 'i':
                input_filename = optarg;
                break;
            case 'r':
                if(strcmp(optarg, "png") == 0){
                    renderer = render_png;
                }
                else if(strcmp(optarg, "txt") ==0 ){
                    renderer = render_txt;
                }
                else if(strcmp(optarg, "gif") == 0){
                    renderer = render_gif;
                    multigrid = true;
                }
                else {
                    fprintf(stderr, "Unrecognized renderer: %s, exitting", optarg);
                    exit(2);
                }
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'd':
                anim_delay = strtol(optarg, NULL, 10);
                if(anim_delay < 1){
                    fprintf(stderr, "Invalid frame delay: %d", anim_delay);
                    exit(2);
                }
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                print_usage(stdout, argv[0]);
                print_help();
                return 0;
            default:
                print_usage(stderr, argv[0]);
                fprintf(stderr, "See --help for more info\n");
                return 1;

        }
    }
    //TODO: logic to set output_filename if o flag is not used


    FILE* input_file = NULL;
    FILE* output_file = NULL;
    grid_t* grid = NULL;

    if (setjmp(error_buffer) != 0) {
        cleanup(output_file, input_file, grid);
        exit(EXIT_FAILURE);
    }

    if(strcmp(output_filename, "-") == 0){
        output_file = stdout;
    }
    else {
        output_file = fopen(output_filename, "wb");
        if (!output_file) { error_exit("Error opening output file", output_filename); }
    }

    if(!multigrid){
        if(strcmp(input_filename, "-") == 0){
            grid = read_grid(stdin);
            if (!grid) { error_exit("Error reading from stdin", NULL); }
        }
        else {
            input_file = fopen(input_filename, "rb");
            if(!input_file) { error_exit("Error opening input file", input_filename); }
            grid = read_grid(input_file);
            if(!grid) { error_exit("Error reading from file", input_filename); }
        }
        params->grid = grid;
    }
    else {
        grid_t** grids = malloc(BUFFER_SIZE * sizeof(grid_t*));
        char filename[256];
        size_t size = 0;
        FILE* file;
        if(strcmp(input_filename, "-") == 0){
            file = stdin;
        }
        else {
            file = fopen(input_filename, "r");
        }
        while(fgets(filename, sizeof(filename), file) != NULL && size < BUFFER_SIZE){
            //remove trailing newline from fgets
            filename[strcspn(filename, "\n")] = 0;
            input_file = fopen(filename, "rb");
            if (!input_file) { error_exit("Error opening input file", filename); }
            grids[size] = read_grid(input_file);
            if (!grids[size]) { error_exit("Error reading from file", filename); }
            size++;
        }
        //TODO: check grids to make sure they are have the same dimensions
        params->grid_array.delay = anim_delay;
        params->grid_array.size = size;
        params->grid_array.grids = grids;
    }


    if(verbose){
        print_grid_info(grid);
    }

    renderer(output_file, params);

    cleanup(output_file, input_file, grid);
    free(params);
    return 0;
}
