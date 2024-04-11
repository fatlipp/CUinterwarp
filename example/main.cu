#include "main.h"

#include <iostream>
#include <cuda_runtime.h>
#include <cuda.h>

#include <string>
#include <fstream>
#include <iostream>

#ifndef checkCudaErrors
#define checkCudaErrors(err) __checkCudaErrors(err, __FILE__, __LINE__)

// These are the inline versions for all of the SDK helper functions
inline void __checkCudaErrors(cudaError_t err, const char *file, const int line)
{
    if (CUDA_SUCCESS != err)
    {
        const char *errorStr = NULL;
        fprintf(stderr,
                "checkCudaErrors() Driver API error = %04d \"%s\" from file <%s>, "
                "line %i.\n",
                err, errorStr, file, line);
        exit(EXIT_FAILURE);
    }
}
#endif

__global__ void Do(int *data, int size)
{
    data[threadIdx.x] = threadIdx.x;
}

void Alloc()
{
    std::cout << "Alloc start Cuda" << std::endl;
    int *mem;
    cudaMalloc((void **)&mem, sizeof(int) * 1024);

    Do<<<1, 1024>>>(mem, 1024);
    checkCudaErrors(cudaDeviceSynchronize());

    int *mem2 = (int *)malloc(sizeof(int) * 1024);
    checkCudaErrors(cudaMemcpy(mem2, mem, sizeof(int) * 1024, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaDeviceSynchronize());

    std::cout << "" << mem2[0] << " - " << mem2[1] << " - " << mem2[2] << std::endl;

    std::cout << "Alloc end Cuda" << std::endl;
}
