
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <cuComplex.h>
#include <crt/math_functions.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define THRESHHOLD 25000
#define CUDACORES 2560
#define MAX_THREADS_PER_BLOCK 1024
#define PI2 6.2832
#define channelNumber 4
#define sampleSize 4

typedef cuComplex cplx;
__device__ __forceinline__ cplx cuexpf(cplx z)
{
    cplx res;
    float t = expf(z.x);
    sincosf(z.y, &res.y, &res.x);
    res.x *= t;
    res.y *= t;
    return res;
}

__device__ void _dft(cplx* buf, cplx* out, int N, dim3 blockIdx, dim3 blockDimx, dim3 gridDim, dim3 threadIdx)
{
    double PI = acosf(-1);

    int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;
    int side = gridDim.x * blockIdx.y * blockDim.x;
    int sideIndex = blockIdx.x * blockDim.x + threadNumInBlock;
    int globalIndex = side + sideIndex; //global thread number;

    for (int j = 0; j < N; j++)
    {
        int index_j = side + j;
        cplx exp = make_cuComplex(cos(((2 * PI) / N) * sideIndex * j), -1 * sin(((2 * PI) / N) * sideIndex * j));
        cplx res = cuCaddf(out[globalIndex], cuCmulf(buf[index_j], exp));
        out[globalIndex] = res;
    }
}

__device__ __forceinline__ void _dft2(cplx* buf, cplx* out, int n, int channelNb, int N)
{
    double PI =  acosf(-1);

    for (int channel = 0; channel < channelNumber; channel++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int index_i = channel * n + i;
                int index_j = channel * n + j;
                cplx exp = make_cuComplex(cos(((2 * PI) / N) * i * j), -1 * sin(((2 * PI) / N) * i * j));
                out[index_i] = cuCaddf(out[index_i], cuCmulf(buf[index_j], exp));
            }
        }
    }
}

__device__ __forceinline__ void _fft(cplx* buf, cplx* out, int n, int step)
{
    double PI = acosf(-1);
    if (step < n) {
        _fft(out, buf, n, step * 2);
        _fft(out + step, buf + step, n, step * 2);

        for (int j = 0; j < n; j += 2 * step) {

            cplx exp = cuCmulf(cuexpf(make_cuComplex(PI * j / n, -1)), out[j + step]);
            
            buf[j / 2] = cuCaddf(out[j], exp);
            buf[(j + n) / 2] = cuCsubf(out[j], exp);
        }
    }
}

void printbuf(cplx* buf_orig, cplx* buf, int size, int threshholdCount, int channelNb, int** x) {
    
    int n = 0;
    FILE* f;

    fopen_s(&f, "..\\out\\dftplots2.m", "a");

    // make this generic with channelnumber
    
        for (n = 0; n < size; ++n)
        {
            for (int channel = 0; channel < channelNumber; channel++)
            {
                fprintf(f, "%f ", cuCrealf(buf_orig[channel * size + n]));
                float dftMagnitude = sqrtf(cuCrealf(buf[channel * size + n]) * cuCrealf(buf[channel * size + n]) + 
                    cuCimagf(buf[channel * size + n]) * cuCimagf(buf[channel * size + n]));
                fprintf(f, "%f ", dftMagnitude);
            }
            fprintf(f, "%u\n", threshholdCount * size + n);
        }
        printf("Wrote %u magnitudes at %u channel(s)\n", size, channelNb);

    fclose(f);
}

__device__ __forceinline__ void fft(int* buf, cplx* out, int n)
{
    cplx* buf2 = (cplx*)malloc(sizeof(cplx) * n);
    for (int j = 1; j < n; j++) {
        printf("buf[%i]=%i\n", j, buf[j]);
        cplx one = make_cuComplex(buf[j], 0);
        buf2[j] = one;
    }
    for (int j = 0; j < n; j++)
    {
        printf("cOut[%i]=real(%f) im(%f) \n", j, cuCrealf(buf2[j]), cuCimagf(buf2[j]));
    }
    _fft(buf2, out, n, 1);
    
}

cudaError_t cudaFourierTransform(cplx* input, cplx* complexOut, int channelNb, int size);

__global__ void addKernel(cplx* x, cplx* out, int channelNummber, long size)
{
    //unsigned int id = threadIdx.x + blockIdx.x * blockDim.x;
    _dft(x, out, size, blockIdx, blockDim, gridDim, threadIdx);
}

//FFT(x) {
//    n = length(x);
//    if (n == 1) return x;
//    m = n / 2;
//    X = (x_{ 2j })_ { j = 0 }^{m - 1};
//    Y = (x_{ 2j + 1 })_ { j = 0 }^{m - 1};
//    X = FFT(X);
//    Y = FFT(Y);
//    U = (X_{ k mod m })_ { k = 0 }^{n - 1};
//    V = (g^{ -k }Y_{ k mod m })_ { k = 0 }^{n - 1};
//    return U + V;
//}

int convertToInt(char* buf, int bufsize)
{
    /*int shift = 8;
    int res = buf[0] & 0xFF;
    for (int i = 0; i < bufsize; ++i)
    {
        res |= (buf[i] & i < (bufsize - 1) ? 0xFF : buf[i]) << shift;
        shift += 8;
    }

    return res;*/

    int shift = 8;
    int ret = buf[0] & 0xFF;
    for (int i = 1; i < bufsize; ++i)
    {
        char one = buf[1];
        ret |= (buf[i] & i < (bufsize - 1) ? 0xFF : buf[i]) << shift;
        shift += 8;
    }
    return ret;
}

int sendBuf(SOCKET s, cplx* complx, int threshhold, int channelNb)
{
    float* mag = (float*)malloc(sizeof(float) * threshhold * channelNumber);
    int msgsize = 1024;
    char* msg = (char*)malloc(sizeof(char) * msgsize);
    msg[0] = '[';
    int length = 1;
    for (int c = 0; c < channelNumber; c++)
    {
        for (int samp = 0; samp < threshhold; samp++)
        {
            float ftMagnitude = sqrtf(
                  cuCrealf(complx[c * threshhold + samp]) 
                * cuCrealf(complx[c * threshhold + samp]) 
                + cuCimagf(complx[c * threshhold + samp]) 
                * cuCimagf(complx[c * threshhold + samp]));
            mag[c * threshhold + samp] = ftMagnitude;
            
            int len = snprintf(NULL, 0, "%f", ftMagnitude) +1;

            if (length + len > msgsize)
            {
                msgsize += 100;
                msg = (char*)realloc(msg, msgsize * sizeof(char));
            }

            if (c == channelNumber - 1 && samp == threshhold-1)
                sprintf(msg + length, "%f]", ftMagnitude);
            else
                sprintf(msg + length, "%f,", ftMagnitude);
            
            length += len;
        }
    }
    // build message
    char datalength[1024];
    sprintf(datalength, "%u|", strlen(msg));
    send(s, datalength, strlen(datalength), 0);
    send(s, msg, strlen(msg), 0);

    return 0;
}

bool initSocket(SOCKET& s, char* host, int port)
{
    struct sockaddr_in server;
    WSADATA wsa;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return false;
    }

    printf("Initialized.");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //Connect to remote server
    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("Could not create connection : %d", WSAGetLastError());
        puts("connect error");
        return false;
    }

    puts("Connected");
    return true;
}

int main()
{
    SOCKET s_vis;
    SOCKET s_rcv;

    if (!initSocket(s_vis, "127.0.0.1", 1338))
        return -1;
    if (!initSocket(s_rcv, "127.0.0.1", 1337))
        return -1;

    int** sampleData = (int**)malloc(sizeof(int*) * channelNumber);
    for (int i = 0; i < channelNumber; i++)
        sampleData[i] = (int*)malloc(sizeof(int) * THRESHHOLD);

    int arr_pos = 0;
    long start = 0;
    int rcvSize = (channelNumber * sampleSize + channelNumber - 1 + 1) * THRESHHOLD;
    char* rcvBuf = (char*)malloc(sizeof(char) * rcvSize);
    char* startptr, *endptr;
    while (true)
    {
        if (arr_pos >= THRESHHOLD)
        {
            cplx* complexOut = (cplx*)malloc(sizeof(cplx) * channelNumber * THRESHHOLD);
            cplx* helper = (cplx*)malloc(sizeof(cplx) * channelNumber * THRESHHOLD);

            for (int i = 0; i < channelNumber; i++)
            {
                for (int j = 0; j < THRESHHOLD; j++)
                {
                    int sample = sampleData[i][j];
                    helper[i * THRESHHOLD + j] = make_cuComplex(sampleData[i][j], 0);
                }
            }
            // cuda
            cudaFourierTransform(helper, complexOut, channelNumber, THRESHHOLD);
            sendBuf(s_vis, complexOut, THRESHHOLD, channelNumber);

            start = arr_pos;
            arr_pos = 0;

            // realloc sampleData array
            free(sampleData);
            sampleData = (int**)malloc(sizeof(int*) * channelNumber);
            for (int i = 0; i < channelNumber; i++)
                sampleData[i] = (int*)malloc(sizeof(int) * THRESHHOLD);
        }
        

        int bytes = 0;

        while (bytes < rcvSize)
        {
            int recvBytes = recv(s_rcv, rcvBuf + bytes, rcvSize - bytes, 0);
            bytes += recvBytes;
        }

        char delim[] = "\n";
        
        unsigned count = 0;
        startptr = endptr = (char*)rcvBuf;

        //                                                  2               4   = 8 +      " "        "\n"
        char* channelStr = (char*)malloc(sizeof(char) * (channelNumber * sampleSize + (sampleSize-1) + 1));
        while ((endptr = strchr(startptr, '\n')))
        {
            sprintf(channelStr, "%.*s", (int)(endptr - startptr + 1), startptr);
            char* perChannel = strtok(channelStr, " ");
            for (int c = 0; c < channelNumber; c++)
            {
                int channelData = (int)strtol(perChannel, NULL, 16);
                perChannel = strtok(NULL, " ");
                sampleData[c][arr_pos] = channelData;
            }
            startptr = endptr + 1;
            arr_pos++;
        }
        
    }
    free(rcvBuf);
    return 0;
}

cudaError_t cudaFourierTransform(cplx* x, cplx* complexOut, int channelNb, int size)
{
    cudaError_t cudaStatus;

    cplx* dev_c;
    cplx* dev_out;
    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!");
        goto Error;
    }

    // Allocate GPU buffers
    cudaStatus = cudaMalloc((void**)&dev_out, sizeof(cplx) * channelNumber * size);
    cudaStatus = cudaMalloc((void**)&dev_c, sizeof(cplx) * channelNumber * size);
    cudaStatus = cudaMemcpy(dev_c, x, sizeof(cplx) * channelNumber * size, cudaMemcpyHostToDevice);

    int maxThreadsPerBlock = 1024;
    int threadCluster = (int)ceilf((float)size / maxThreadsPerBlock);
    int actualThreadsPerBlock = size;

    if (threadCluster)
        actualThreadsPerBlock = (int)ceilf((float)size / threadCluster);
    
    dim3 block_dim(actualThreadsPerBlock, 1, 1);
    dim3 grid_dim(threadCluster, channelNumber, 1);

    // Launch a kernel on the GPU with one thread for each element.
    addKernel <<<grid_dim, block_dim >>> (dev_c, dev_out, channelNumber, size);
    
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(complexOut, dev_out, sizeof(cplx) * size * channelNumber, cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    printf("DFT done with sitze %u\n", size);

Error:
    cudaFree(dev_out);
    cudaFree(dev_c);

    return cudaStatus;
}
