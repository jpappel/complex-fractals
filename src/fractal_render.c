#include <stdio.h>
#include "grids.h"
#include "precision.h"
#include "fractal_render.h"

int main(const int argc, char* argv[]){
    char* input_file = "test.grid";
    FILE* file = fopen(input_file, "rb");
    grid_t* grid = read_grid(file);
    fclose(file);

    if(!grid) return 1;

    print_grid_info(grid);

    free_grid(grid);
    return 0;
}
