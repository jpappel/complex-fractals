#include "renderers.h"
#include <stdio.h>
#include "fractal_render.h"
#include <gd.h>

static inline byte scale_iterations(const byte max_iterations, const byte iteration){
    return (byte)((double)iteration / max_iterations * 255);
}

/*
 * Convert a grid into a gd image with true color
 * NOTE: modifying the size of colors will allow this function to use the 
 *       millions of colors the true colors support
 * As of now it is identical to converter, but should be changed
 */
gdImagePtr truecolor_converter(const grid_t* grid){
    const size_t width = grid->x;
    const size_t height = grid->y;
    const byte* data = grid->data;
    const byte max_iterations = grid->max_iterations;

    gdImagePtr img = gdImageCreateTrueColor(width, height);
    int colors[256];
    for(size_t i = 0; i < 255; i++){
        colors[i] = gdImageColorAllocate(img, 0, i, i/2);
    }

    colors[255] = gdImageColorAllocate(img, 0, 0, 0);

    for(size_t y = 0; y < height; y++){
        for(size_t x = 0; x < width; x++){
            byte iteration = data[y * width + x];
            byte scaled_iteration = scale_iterations(max_iterations, iteration);
            int color = colors[scaled_iteration];
            gdImageSetPixel(img, x, y, color);
        }
    }

    return img;
}

/*
 * Convert a grid into a gd image
 */
gdImagePtr converter(const grid_t* grid){
    const size_t width = grid->x;
    const size_t height = grid->y;
    const byte* data = grid->data;
    const byte max_iterations = grid->max_iterations;

    gdImagePtr img = gdImageCreate(width, height);
    int colors[256];
    for(size_t i = 0; i < 255; i++){
        colors[i] = gdImageColorAllocate(img, 0, i, i/2);
    }

    colors[255] = gdImageColorAllocate(img, 0, 0, 0);
    for(size_t y = 0; y < height; y++){
        for(size_t x = 0; x < width; x++){
            byte iteration = data[y * width + x];
            byte scaled_iteration = scale_iterations(max_iterations, iteration);
            int color = colors[scaled_iteration];
            gdImageSetPixel(img, x, y, color);
        }
    }

    return img;
}


void render_png(FILE *output, const renderer_params* params){
    gdImagePtr img = truecolor_converter(params->grid);
    gdImagePng(img, output);
    gdImageDestroy(img);
}

void render_gif(FILE* output, const renderer_params* params){
    const size_t size = params->grid_array.size;
    grid_t** grids = params->grid_array.grids;
    const int delay = params->grid_array.delay;

    gdImagePtr imgs[size];

    imgs[0] = converter(grids[0]);
    gdImageGifAnimBegin(imgs[0], output, 1, -1);
    gdImageGifAnimAdd(imgs[0], output, 0, 0, 0, delay, 1, NULL);

    for(size_t i = 1; i < size; i++){
        imgs[i] = converter(grids[i]);
        gdImagePaletteCopy(imgs[i], imgs[i-1]);
        gdImageGifAnimAdd(imgs[i], output, 0, 0, 0, delay, 1, imgs[i-1]);
    }

    gdImageGifAnimEnd(output);
    for(size_t i = 0; i < size; i++){
        gdImageDestroy(imgs[i]);
    }
}
