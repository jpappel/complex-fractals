#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grids.h"

/*
 * Creates a grid for storing the results of the escape algorithm
 */
grid_t* create_grid(const size_t x, const size_t y, long double complex lower_left, long double complex upper_right){
    if(x <= 0 || y <= 0) return NULL;

    const size_t size = x * y;
    size_t* data = malloc(size * sizeof(size_t));
    if(!data){
        fprintf(stderr, "Error allocating %zu grid points for grid\n", size);
        return NULL;
    }

    grid_t* grid = malloc(sizeof(grid_t));
    if(!grid){
        fprintf(stderr, "Error allocating grid\n");
        free(data);
        return NULL;
    }

    *grid = (grid_t){
        .x = x,
        .y = y,
        .size = x*y,
        .lower_left = lower_left,
        .upper_right = upper_right,
        .data = data

    };

    return grid;
}

/*
 * Sets all entries of a grid to the value val
 */
void set_grid(grid_t* grid, const size_t val){
    if(!grid || !grid->data) return;
    memset(grid->data, val, grid->size);
}

/*
 * Creates a copy of a grid
 */
grid_t* copy_grid(const grid_t* grid){
    if(!grid || !grid->data) return NULL;

    grid_t* grid_copy = create_grid(grid->x, grid->y, grid->lower_left, grid->upper_right);
    if(!grid_copy) return NULL;

    memcpy(grid_copy->data, grid->data, grid->size);

    return grid_copy;
}

/*
 * Frees a grid and its members
 */
void free_grid(grid_t* grid){
    if(!grid) return;

    free(grid->data);
    free(grid);
}

/*
 * Checks if two grids are exactly equal
 * This may return incorrect values due to floating point arrithmetic errors 
 * that occur while the grid is being filled
 */
bool grid_equal(const grid_t* grid1, const grid_t* grid2){
    return grid1->x == grid2->x && grid1->y == grid2->y &&
        grid1->lower_left == grid2->lower_left && grid1->upper_right == grid2->upper_right &&
        memcmp(grid1->data, grid2->data, grid1->size) == 0;
}

/*
 * Checks if two grids have a given maximum difference
 */
bool grid_allclose(const grid_t *grid1, const grid_t *grid2, const size_t max_error){
    if(grid1->x != grid2->x || grid1->y != grid2->y ||
            grid1->lower_left != grid2->lower_left || grid1->upper_right != grid2->upper_right){
        return false;
    }
    const size_t size = grid1->size;
    for(size_t i = 0; i < size; i++){
        //FIXME: figure out how to handle difference between two unsigned values
        //       possibly cast them to sized longs?
        if(abs(grid1->data[i] - grid2->data[i]) >= max_error) return false;
    }
    return true;
}


/*
 * Converts a grid point into the corresponding complex number
 */
long double complex grid_to_complex(const grid_t* grid, const size_t index) {
    const size_t x_res = grid->x;
    const size_t y_res = grid->y;
    const long double x_min = creal(grid->lower_left);
    const long double x_max = creal(grid->upper_right);
    const long double y_min = cimag(grid->lower_left);
    const long double y_max = cimag(grid->upper_right);

    const long double x_step = (x_max - x_min) / (double)x_res;
    const long double y_step = (y_max - y_min) / (double)y_res;

    const size_t x_index = index % x_res;
    const size_t y_index = index / y_res;

    const long double x = x_min + x_index * x_step;
    const long double y = y_min + y_index * y_step;

    return x + y * I;
}

/*
 * Zoom a grid in or out based on its current center
 *
 * Resets all grid values to 0
 */
void zoom_grid(grid_t* grid, const double magnification){
    set_grid(grid, 0);
    const long double complex upper_right = grid->upper_right;
    const long double complex lower_left = grid->lower_left;

    const long double complex center = (lower_left + upper_right) / 2.0;
    const long double complex offset = (upper_right - lower_left) / magnification;

    grid->lower_left = center - offset;
    grid->upper_right = center + offset;
}

/*
 * Writes a grid to a file in the .grid format
 *
 * Returns 0 on success
 *
 * The .grid format is a binary file format
 * The first 3 bytes of the file are a magic number defined in grids.h
 * The next 16 bytes are the grid dimensions (x then y)
 * Following are 32 bytes are the bounding of the complex region (lower_left then upper_right)
 * The rest of the file is the data for the grid, which should be exactly x*y*8 bytes
 */
int write_grid(FILE* restrict file, const grid_t *grid){
    if(grid->size == 0 || !grid->data ){
        return GRID_NO_DATA;
    }

    unsigned char magic_num[3];
    magic_num[0] = 0xA6;
    magic_num[1] = 0x00;
    magic_num[2] = 0x5E;
    // for(size_t i = 0; i < 3; i++){
    //     magic_num[i] = (GRID_MAGIC_NUMBER >> (2*i)) & 0xFF;
    // }

    if(fwrite(magic_num, 1, 3, file) != 3) return GRID_WRITE_ERROR;

    if(fwrite(&grid->x, sizeof(size_t), 1, file) != 1 ||
       fwrite(&grid->y, sizeof(size_t), 1, file) != 1 ||
       fwrite(&grid->lower_left, sizeof(long double complex), 1, file) != 1 ||
       fwrite(&grid->upper_right, sizeof(long double complex), 1, file) != 1){
        return GRID_WRITE_ERROR;
    }

    if(fwrite(grid->data, sizeof(size_t), grid->size, file) != grid->size){
        return GRID_WRITE_ERROR;
    }

    return 0;
}

/*
 * Prints info about a grid to stdout
 */
void print_grid_info(const grid_t* grid){
    if(!grid){
        printf("Grid is NULL!\n");
        return;
    }

    printf("x\t%zu\n", grid->x);
    printf("y\t%zu\n", grid->y);
    printf("size\t%zu\n", grid->size);
    printf("lower_left\t%f + %fI\n", creal(grid->lower_left), cimag(grid->lower_left));
    printf("upper_right\t%f + %fI\n", creal(grid->upper_right), cimag(grid->upper_right));

    printf("Data is %s NULL\n", grid->data ? "not" : "");
}

/*
 * Attempts an ASCII print of the grid
 */
void print_grid(const grid_t* grid, const size_t iterations){
    const size_t size = grid->size;
    const size_t x_res = grid->x;
    const size_t* data = grid->data;

    // char* output_buffer = malloc(size);
    // if(!output_buffer){
    //     fprintf(stderr, "Failed to allocate output buffer for %zu points\n");
    //     return;
    // }

    const char point_types[] = { ' ', '.', '*', '%', '#'};
    size_t bin_width = iterations/3;
    size_t last_bin = iterations - bin_width;
    char point;

    //TODO: set values in output buffer rather than multiple printf calls
    for(size_t i = 0; i < size; i++){
        const size_t value = data[i];
        if(value == iterations){
            point = point_types[4];
        }
        else if(value == 0){
            point = point_types[0];
        }
        else if(value <= bin_width){
            point = point_types[1];
        }
        else if(value >= last_bin){
            point = point_types[3];
        }
        else {
            point = point_types[2];
        }
        printf("%c%s", point, (i % x_res == x_res - 1) ? "\n" : "");
    }
}

/*
 * Creates a grid from a .grid file, reading the amount of data as specified by the file
 * For more details on the .grid format see write_grid
 *
 * Ignores remainder of file if it has finished reading but is not at the end of the file
 */
grid_t* read_grid(FILE* restrict file){
    // Make sure the file has a magic goose (GRID_MAGIC_NUMBER)
    unsigned char magic_num[3];
    size_t read_count = fread(magic_num, 1, 3, file);

    if(read_count != 3){
        perror("Error reading file\n");
        return NULL;
    }
    if(magic_num[0] != 0xA6 || magic_num[1] != 0x00 || magic_num[2] != 0x5E){
        fprintf(stderr, "Error reading file, can't find magic number 0xA6005E\n");
        return NULL;
    }

    size_t x = 0;
    size_t y = 0;
    read_count = fread(&x, sizeof(size_t), 1, file);
    if(read_count != 1){
        perror("Error reading file\n");
        return NULL;
    }
    read_count = fread(&y, sizeof(size_t), 1, file);
    if(read_count != 1){
        perror("Error reading file\n");
        return NULL;
    }

    long double complex lower_left = 0;
    long double complex upper_right = 0; 
    read_count = fread(&lower_left, sizeof(long double complex), 1, file);
    if(read_count != 1){
        perror("Error reading file\n");
        return NULL;
    }
    read_count = fread(&upper_right, sizeof(long double complex), 1, file);
    if(read_count != 1){
        perror("Error reading file\n");
        return NULL;
    }

    //TODO: look into mmaping the file to data, offseting by the bounding and resolution information
    //      this would likely require an alloc_grid function, similar to jeff's implementation in hw03
    grid_t* grid = create_grid(x, y, lower_left, upper_right);
    if(!grid){
        return NULL;
    }

    read_count = fread(grid->data, sizeof(size_t), grid->size, file);
    if(read_count != grid->size){
        fprintf(stderr, "Error reading file, expected %zu grid points but only found %zu\n", grid->size, read_count);
        free_grid(grid);
        return NULL;
    }

    return grid;
}
