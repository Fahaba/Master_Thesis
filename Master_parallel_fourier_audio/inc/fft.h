#ifndef __FFT_H_
#define __FFT_H_

#include <stdio.h>
#include <math.h>
#include <cuComplex.h>
//#include <complex.h>

typedef cuComplex cplx;
//typedef _Dcomplex cplx;
__device__ __forceinline__ void fft(int* buf, cplx* out, int n);
// declarations go here

__device__ __forceinline__ void _fft(cplx* buf, cplx* out, int n, int step);
//void fft(int* buf, int n);
void printbuf(int* buf_orig, cplx* buf, int size);


#endif