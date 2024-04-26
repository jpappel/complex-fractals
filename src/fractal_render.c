#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <setjmp.h>

#include "grids.h"
#include "precision.h"
#include "fractal_render.h"

void print_usage(FILE* file, const char* program_name) {
    fprintf(file, "Usage: %s -i input.grid [-r renderer] [-o output.ext]\n", program_name);
}

void print_help(){
    printf("Options:\n"
           "  -i, --input <input grid>        the grid to be rendered, if the file name is '-' reads from stdin\n"
           "  -r, --renderer <renderer>       the renderer to use, defaults to the text renderer\n"
           "      renderers:   txt, png (TODO), gif (TODO, with additional features)\n"
           "  -o, --output <output file>      the file to output the result of rendering, if not given defaults to output.<EXT>\n"
           "                                  where <EXT> is the renderer used\n"
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


int main(const int argc, char* argv[]){
    char* input_filename = "fractal.grid";
    char* output_filename = "fractal.txt";
    renderer_func renderer = print_grid;
    bool verbose = false;

    static struct option long_options[] = {
        {"input", required_argument, NULL, 'i'},
        {"renderer", required_argument, NULL, 'r'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "i:r:o:vh", long_options, NULL)) != -1){
        switch(opt){
            case 'i':
                input_filename = optarg;
                break;
            case 'r':
                //TODO: update with other renderers
                renderer = print_grid;
                break;
            case 'o':
                output_filename = optarg;
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

    if(strcmp(input_filename, "-") == 0){
        grid = read_grid(stdin);
        if (!grid) { error_exit("Error reading from stdin", NULL); }
    }
    else {
        input_file = fopen(input_filename, "rb");
        if (!input_file) { error_exit("Error opening input file", input_filename); }
        grid = read_grid(input_file);
        if (!grid) { error_exit("Error reading from file", input_filename); }
    }

    if(verbose){
        print_grid_info(grid);
    }

    renderer(output_file, grid);

    cleanup(output_file, input_file, grid);

    return 0;
}
