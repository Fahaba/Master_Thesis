#include "dft.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define N 16
#define PI2 6.2832

void dft2(int32_t* x, int size)
{
    // time and frequency domain data arrays
    int n, k;             // indices for time and frequency domains
    //float x[N];           // discrete-time signal, x
    float* Xre = (float*)malloc(sizeof(float) * size);
    float* Xim = (float*)malloc(sizeof(float) * size); // DFT of x (real and imaginary parts)
    float* P = (float*)malloc(sizeof(float) * size);           // power spectrum of x

    printf("test %u", x[123]);

    // Calculate DFT of x using brute force
    for (k = 0; k < size; ++k)
    {
        // Real part of X[k]
        Xre[k] = 0;
        for (n = 0; n < size; ++n) Xre[k] += x[n] * cos(n * k * PI2 / size);

        // Imaginary part of X[k]
        Xim[k] = 0;
        for (n = 0; n < size; ++n) Xim[k] -= x[n] * sin(n * k * PI2 / size);

        // Power at kth frequency bin
        P[k] = Xre[k] * Xre[k] + Xim[k] * Xim[k];
    }

    // Output results to MATLAB / Octave M-file for plotting
    FILE* f;
    fopen_s(&f, "dftplots.m", "w");
    fprintf(f, "n = [0:%d];\n", N - 1);
    fprintf(f, "x = [ ");
    for (n = 0; n < size; ++n) fprintf(f, "%f ", x[n]);
    fprintf(f, "];\n");
    fprintf(f, "Xre = [ ");
    for (k = 0; k < size; ++k) fprintf(f, "%f ", Xre[k]);
    fprintf(f, "];\n");
    fprintf(f, "Xim = [ ");
    for (k = 0; k < size; ++k) fprintf(f, "%f ", Xim[k]);
    fprintf(f, "];\n");
    fprintf(f, "P = [ ");
    for (k = 0; k < size; ++k) fprintf(f, "%f ", P[k]);
    fprintf(f, "];\n");
    fprintf(f, "subplot(3,1,1)\nplot(n,x)\n");
    fprintf(f, "xlim([0 %d])\n", N - 1);
    fprintf(f, "subplot(3,1,2)\nplot(n,Xre,n,Xim)\n");
    fprintf(f, "xlim([0 %d])\n", N - 1);
    fprintf(f, "subplot(3,1,3)\nstem(n,P)\n");
    fprintf(f, "xlim([0 %d])\n", N - 1);
    fclose(f);
}