#pragma once

#include <stdio.h>
#include "grids.h"

typedef void (*renderer_func)(FILE*, const grid_t*);
