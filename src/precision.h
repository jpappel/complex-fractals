/*
 * Header that allows conditional compilation into double floating point precisions ot extended double floating point precision
 *
 * CUDA does not support long double so cuda-fractals may fail to compile or have unexpected behavior if linked with code that was compiled with EXTENDED_PRECISION
 */
#pragma once

#ifdef EXTENDED_PRECISION

#define CBASE long double
#define CREAL creall
#define CIMAG cimagl
#define CPOW cpowl
#define CONJ conjl
#define CABS cabsl
#define CFORMAT "%Lf"

#endif
#ifndef EXTENDED_PRECISION

#define CBASE double
#define CREAL creal
#define CIMAG cimag
#define CPOW cpow
#define CONJ conj
#define CABS cabs
#define CFORMAT "%lf"

#endif
