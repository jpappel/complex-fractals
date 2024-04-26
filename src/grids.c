#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "grids.h"

static inline bool equal_complex_t(const complex_t z1, const complex_t z2){
    return z1.re == z2.re && z1.im == z2.im;
}

/*
 * Creates a grid for storing the results of the escape algorithm
 */
grid_t* create_grid(const size_t x, const size_t y, const byte max_iterations, complex_t lower_left, complex_t upper_right){
    if(x <= 0 || y <= 0) return NULL;

    const size_t size = x * y;
    byte* data = malloc(size);
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
        .max_iterations = max_iterations,
        .lower_left = lower_left,
        .upper_right = upper_right,
        .data = data

    };

    return grid;
}

/*
 * Sets all entries of a grid to the value val
 */
void set_grid(grid_t* grid, const byte val){
    if(!grid || !grid->data) return;
    memset(grid->data, val, grid->size);
}

/*
 * Creates a copy of a grid
 */
grid_t* copy_grid(const grid_t* grid){
    if(!grid || !grid->data) return NULL;

    grid_t* grid_copy = create_grid(grid->x, grid->y, grid->max_iterations, grid->lower_left, grid->upper_right);
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
bool grid_equal(const grid_t* grid1_p, const grid_t* grid2_p){
    grid_t grid1 = *grid1_p;
    grid_t grid2 = *grid2_p;
    const bool lowers_equal = equal_complex_t(grid1.lower_left, grid2.lower_left);
    const bool uppers_equal = equal_complex_t(grid1.upper_right, grid2.upper_right);
    const bool dimensions_equal = grid1.x == grid2.x && grid1.y == grid2.y;
    return lowers_equal && uppers_equal && grid1.max_iterations == grid2.max_iterations &&
        dimensions_equal && memcmp(grid1.data, grid2.data, grid1.size) == 0;
}

/*
 * Checks if two grids have a given maximum difference
 */
bool grid_allclose(const grid_t* restrict grid1, const grid_t* restrict grid2, const byte max_error){
    if(grid1->x != grid2->x || grid1->y != grid2->y ||
            !equal_complex_t(grid1->lower_left, grid2->lower_left) ||
            !equal_complex_t(grid1->upper_right, grid2->upper_right)){
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
CBASE complex grid_to_complex(const grid_t* grid_p, const size_t index) {
    const grid_t grid = *grid_p;
    const size_t x_res = grid.x;
    const size_t y_res = grid.y;
    const CBASE x_min = grid.lower_left.re;
    const CBASE x_max = grid.upper_right.re;
    const CBASE y_min = grid.lower_left.im;
    const CBASE y_max = grid.upper_right.im;

    const CBASE x_step = (x_max - x_min) / (double)x_res;
    const CBASE y_step = (y_max - y_min) / (double)y_res;

    const size_t x_index = index % x_res;
    const size_t y_index = index / y_res;

    const CBASE x = x_min + x_index * x_step;
    const CBASE y = y_min + y_index * y_step;

    return x + y * I;
}

/*
 * Zoom a grid in or out based on its current center
 *
 * Resets all grid values to 0
 */
void zoom_grid(grid_t* restrict grid, const CBASE magnification){
    //FIXME: not impelemnted correctly
    set_grid(grid, 0);
    // const CBASE complex upper_right = grid->upper_right;
    const complex_t upper_right = grid->upper_right;
    // const CBASE complex lower_left = grid->lower_left;
    const complex_t lower_left = grid->lower_left;

    const CBASE inv2 = 1 / 2.0;
    const CBASE inv_mag = 1 / magnification;
    const complex_t center = {
        .re = inv2 * (lower_left.re + upper_right.re),
        .im = inv2 * (lower_left.im + lower_left.im)
    };
    const complex_t offset = {
        .re = inv_mag * (upper_right.re - lower_left.re),
        .im = inv_mag * (upper_right.im - lower_left.im)
    };

    grid->lower_left = (complex_t){
        .re = center.re - offset.re,
        .im = center.im - offset.im
    };
    grid->upper_right = (complex_t){
        .re = center.re + offset.re,
        .im = center.im + offset.im
    };
}

/*
 * Writes a grid to a file in the .grid format
 *
 * Returns 0 on success
 *
 * FIXME: add dynamic bounding box resolution by storing sizeof points in the file
 * The .grid format is a binary file format
 * The first 3 bytes of the file are a magic number defined in grids.h
 * The next 16 bytes are the grid dimensions (x then y)
 * The next 8 bytes is the max_iterations
 * The next 8 bytes are the size of a grid point in bytes
 * The next 2*precision bytes are the lower left and upper right corners
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
    const size_t precision = sizeof(complex_t);

    if(fwrite(magic_num, 1, 3, file) != 3) return GRID_WRITE_ERROR;

    if(fwrite(&grid->x, sizeof(size_t), 1, file) != 1 ||
       fwrite(&grid->y, sizeof(size_t), 1, file) != 1 ||
       fwrite(&grid->max_iterations, sizeof(byte), 1, file) != 1 ||
       fwrite(&precision, sizeof(size_t), 1, file) != 1 ||
       fwrite(&grid->lower_left, precision, 1, file) != 1 ||
       fwrite(&grid->upper_right, precision, 1, file) != 1){
        return GRID_WRITE_ERROR;
    }

    if(fwrite(grid->data, 1, grid->size, file) != grid->size){
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

    printf("Precision\t%zu\n", sizeof(CBASE));
    printf("x\t%zu\n", grid->x);
    printf("y\t%zu\n", grid->y);
    printf("size\t%zu\n", grid->size);
    printf("Max Iterations\t%hhu\n", grid->max_iterations);
    printf("lower_left\t"CFORMAT"+ "CFORMAT"I\n", grid->lower_left.re, grid->lower_left.im);
    printf("upper_right\t"CFORMAT"+ "CFORMAT"I\n", grid->upper_right.re, grid->upper_right.im);

    printf("Data is %s NULL\n", grid->data ? "not" : "");
}

/*
 * Attempts an ASCII print of the grid
 */
void print_grid(FILE* file, const grid_t* grid){
    const size_t size = grid->size;
    const size_t x_res = grid->x;
    const byte iterations = grid->max_iterations;
    const byte* data = grid->data;

    //TODO: set values in output buffer rather than multiple printf calls
    //      the buffer needs to be larger to hold newlines
    char* output_buffer = malloc(size + grid->y-1);
    if(!output_buffer){
        fprintf(stderr, "Failed to allocate output buffer for %zu points\n", size);
        return;
    }

    setvbuf(file, output_buffer, _IOFBF, size + grid->y - 1);

    const char point_types[] = { ' ', '.', '*', '%', '#'};
    size_t bin_width = iterations/3;
    size_t last_bin = iterations - bin_width;
    char point;

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
        fprintf(file ,"%c%s", point, (i % x_res == x_res - 1) ? "\n" : "");
    }
    fflush(file);
    free(output_buffer);
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

    jmp_buf file_read_error;

    if(read_count != 3){
        perror("Error reading file\n");
        return NULL;
    }
    if(magic_num[0] != 0xA6 || magic_num[1] != 0x00 || magic_num[2] != 0x5E){
        fprintf(stderr, "Error reading file, can't find magic number 0xA6005E\n");
        return NULL;
    }

    if(setjmp(file_read_error)){
        perror("Error reading file\n");
        return NULL;
    }

    size_t x = 0;
    size_t y = 0;
    byte max_iterations = 0;
    size_t precision = 0;
    if(fread(&x, sizeof(size_t), 1, file) != 1){ longjmp(file_read_error, 1); }
    if(fread(&y, sizeof(size_t), 1, file) != 1){ longjmp(file_read_error, 1); }
    if(fread(&max_iterations, sizeof(byte), 1, file) != 1){ longjmp(file_read_error, 1); }
    if(fread(&precision, sizeof(size_t), 1, file) != 1){ longjmp(file_read_error, 1) ; }

    if(precision != sizeof(complex_t)){
        fprintf(stderr, "File's precisions does not match programs: %zu != %zu\n", precision, sizeof(complex_t));
        longjmp(file_read_error, 1);
    }

    complex_t lower_left;
    complex_t upper_right;
    if(fread(&lower_left, sizeof(complex_t), 1, file) != 1){ longjmp(file_read_error, 1); }
    if(fread(&upper_right, sizeof(complex_t), 1, file) != 1){ longjmp(file_read_error, 1); }

    //TODO: look into mmaping the file to data, offseting by the bounding and resolution information
    //      this would likely require an alloc_grid function, similar to jeff's implementation in hw03
    grid_t* grid = create_grid(x, y, max_iterations, lower_left, upper_right);
    if(!grid){
        return NULL;
    }

    read_count = fread(grid->data, 1, grid->size, file);
    if(read_count != grid->size){
        fprintf(stderr, "Error reading file, expected %zu grid points but only found %zu\n", grid->size, read_count);
        free_grid(grid);
        return NULL;
    }

    return grid;
}
