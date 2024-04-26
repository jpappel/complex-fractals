#include <cuda_runtime.h>
#include <thrust/complex.h>
#include "fractals.h"

//These won't work/ need to be modifiied for the cuda version
//#include "grids.h"
//#include "fractals.h"

/*
 * Macro for checking CUDA errors
 */
#define CHECK(call) \
{ \
    const cudaError_t error = call; \
    if(error != cudaSuccess){ \
        fprintf(stderr, "Error: %s:%d, ", __FILE__, __LINE__); \
        fprintf(stderr, "Code: %d, Reason: %s\n", error, cudaGetErrorString(error)); \
    } \
}

#ifndef CBASE
#define CBASE double
#endif

#define BLOCK_SIZE_X 16
#define BLOCK_SIZE_Y 16

// this ain't pretty, but this logic should never change for kernels
#define SET_ROW_COL \
    const size_t row = blockIdx.y * blockDim.y + threadIdx.y; \
    const size_t col = blockIdx.x * blockDim.x + threadIdx.x; \
    if(row >= rows || col >= cols) return


__device__
thrust::complex<CBASE> grid_to_complex(const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t row, const size_t col, const size_t rows, const size_t cols){
    const CBASE x_min = lower_left.real();
    const CBASE x_max = upper_right.real();
    const CBASE y_min = lower_left.imag();
    const CBASE y_max = upper_right.imag();

    const CBASE x_step = (x_max - x_min) / (CBASE)cols;
    const CBASE y_step = (y_max - y_min) / (CBASE)rows;

    const CBASE x = x_min + col * x_step;
    const CBASE y = y_min + row * y_step;
    const thrust::complex<CBASE> z(x,y);
    return z;
}

__device__
byte mandelbrot(const thrust::complex<CBASE> z0, const byte max_iterations){
    thrust::complex<CBASE> z = z0;
    byte iteration = 0;
    while(thrust::abs(z) <= 2 && iteration < max_iterations){
        z = z*z + z0;
        iteration++;
    }
    return iteration;
}

__global__
void mandelbrot_kernel(byte* grid_data, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = mandelbrot(z, max_iterations);
}

__device__ 
byte tricorn(const thrust::complex<CBASE> z0, const byte max_iterations){
    thrust::complex<CBASE> z = z0;
    byte iteration = 0;
    while(thrust::abs(z) <= 2 && iteration < max_iterations){
        z = thrust::conj(z*z) + z0;
        iteration++;
    }
    return iteration;
}

__global__
void tricorn_kernel(byte* grid_data, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = tricorn(z, max_iterations);
}

__device__
byte burning_ship(const thrust::complex<CBASE> z0, const byte max_iterations){
    thrust::complex<CBASE> z = z0;
    thrust::complex<CBASE> z_mod;
    byte iteration = 0;
    while(thrust::abs(z) <= 2 && iteration < max_iterations){
        z_mod = thrust::complex<CBASE>(fabs(z.real()), fabs(z.imag()));
        z = z_mod * z_mod + z0;
        iteration++;
    }
    return iteration;
}

__global__
void burning_ship_kernel(byte* grid_data, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = burning_ship(z, max_iterations);
}

__device__
byte multibrot(const thrust::complex<CBASE> z0, const byte max_iterations, const double d){
    thrust::complex<CBASE> z = z0;
    byte iteration = 0;
    while(thrust::abs(z) <= 2 && iteration < max_iterations){
        z = thrust::pow(z, d) + z0;
        iteration++;
    }
    return iteration;
}

__global__
void multibrot_kernel(byte* grid_data, const double degree, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = multibrot(z, max_iterations, degree);
}

__device__
byte multicorn(const thrust::complex<CBASE> z0, const byte max_iterations, const double d){
    thrust::complex<CBASE> z = z0;
    byte iteration = 0;
    while(thrust::abs(z) <= 2 && iteration < max_iterations){
        z = thrust::conj(thrust::pow(z, d)) + z0;
        iteration++;
    }
    return iteration;
}

__global__
void multicorn_kernel(byte* grid_data, const double degree, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = multicorn(z, max_iterations, degree);
}

__device__
byte julia(const thrust::complex<CBASE> z0, const byte max_iterations, const thrust::complex<CBASE> c, const double R){
    thrust::complex<CBASE> z = z0;
    byte iteration = 0;
    while(thrust::abs(z) <= R && iteration < max_iterations){
        z = z*z + c;
        iteration++;
    }
    return iteration;
}

__global__
void julia_kernel(byte* grid_data, const thrust::complex<CBASE> constant, const double radius, const byte max_iterations, const thrust::complex<CBASE> lower_left, const thrust::complex<CBASE> upper_right, const size_t rows, const size_t cols){
    SET_ROW_COL;

    const auto z = grid_to_complex(lower_left, upper_right, row, col, rows, cols);

    grid_data[row*cols + col] = julia(z, max_iterations, constant, radius);
}

extern "C" {
void mandelbrot_grid(grid_t* grid, const grid_gen_params* params){
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    //TODO: find good sizes
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    //dim3 grid_size(0,0);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    mandelbrot_kernel<<<grid_size, block_size>>>(d_grid_data, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}

void tricorn_grid(grid_t* grid, const grid_gen_params* params){
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    tricorn_kernel<<<grid_size, block_size>>>(d_grid_data, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}

void burning_ship_grid(grid_t* grid, const grid_gen_params* params){
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    burning_ship_kernel<<<grid_size, block_size>>>(d_grid_data, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}

void multibrot_grid(grid_t* grid, const grid_gen_params* params){
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    const double degree = params->degree;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    multibrot_kernel<<<grid_size, block_size>>>(d_grid_data, degree, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}

void multicorn_grid(grid_t* grid, const grid_gen_params* params){
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    const double degree = params->degree;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    multicorn_kernel<<<grid_size, block_size>>>(d_grid_data, degree, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}

void julia_grid(grid_t* grid, const grid_gen_params* params){
    thrust::complex<CBASE> constant(params->cr.constant.re, params->cr.constant.im);
    const double radius = params->cr.radius;
    const size_t size = grid->size;
    const size_t rows = grid->y;
    const size_t cols = grid->x;
    const byte max_iterations = grid->max_iterations;
    thrust::complex<CBASE> lower_left(grid->lower_left.re, grid->lower_left.im);
    thrust::complex<CBASE> upper_right(grid->upper_right.re, grid->upper_right.im);

    byte* d_grid_data;
    CHECK(cudaMalloc(&d_grid_data, size*sizeof(byte)));
    dim3 block_size(BLOCK_SIZE_X, BLOCK_SIZE_Y);
    dim3 grid_size((cols + block_size.x - 1) / block_size.x, (rows + block_size.y - 1) / block_size.y);
    julia_kernel<<<grid_size, block_size>>>(d_grid_data, constant, radius, max_iterations, lower_left, upper_right, rows, cols);
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(grid->data, d_grid_data, size*sizeof(byte), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_grid_data));
    CHECK(cudaDeviceReset());
}
}
