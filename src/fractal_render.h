#pragma once

#include <stdio.h>
#include <gd.h>

#include "grids.h"

typedef union {
    grid_t* grid;
    struct {
        size_t size;
        int delay;
        grid_t** grids;
    } grid_array;
} renderer_params;
typedef void (*renderer_func)(FILE*, const renderer_params*);
typedef gdImagePtr (*grid_image_converter)(grid_t*);
