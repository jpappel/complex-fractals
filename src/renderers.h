#pragma once

#include <gd.h>
#include <stdio.h>
#include "fractal_render.h"
#include "grids.h"

void render_png(FILE* output, const renderer_params* params);
void render_gif(FILE* output, const renderer_params* params);
