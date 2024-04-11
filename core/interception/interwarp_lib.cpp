#include "core/interception/tools.h"
#include "core/interception/interception_dlsym.h"

#include <stdio.h>
#include <dlfcn.h>

#include <iostream>

extern "C"
{

void* libCudaHandle = dlopen("libcuda.so.1", RTLD_NOW);

#define TRY_INTERCEPT(symbol, target)                                           \
    if (compText(symbol, xstr(target)))                                         \
    {                                                                           \
        *pfn = (void *)(&target);                                               \
        return CUDA_SUCCESS;                                                    \
    }

#define MAKE_CUDA_METHOD(symbol, params, ...)                                   \
CUresult CUDAAPI symbol params                                                  \
{                                                                               \
    if (libCudaHandle == nullptr)                                               \
    {                                                                           \
        std::cout << "Interception error (libCudaHandle is null): "             \
            << dlerror() << std::endl;                                          \
        return CUDA_ERROR_UNKNOWN;                                              \
    }                                                                           \
                                                                                \
    using symbol##Handler = CUresult(params);                                   \
    auto func = (symbol##Handler*)dlsym(libCudaHandle, xstr(symbol));           \
    if (func == nullptr)                                                        \
    {                                                                           \
        std::cout << "Interception method is not found: " << xstr(symbol) <<    \
                        ", error: " << dlerror() << std::endl;                  \
        return CUDA_ERROR_UNKNOWN;                                              \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        const auto res = func(__VA_ARGS__);                                     \
        std::cout << "Intercepted method: " << xstr(symbol) <<                  \
                        ", result: " << res << std::endl;                       \
        return res;                                                             \
    }                                                                           \
    return CUDA_SUCCESS;                                                        \
}


MAKE_CUDA_METHOD(cuDriverGetVersion, (int* driverVersion), 
    driverVersion)
MAKE_CUDA_METHOD(cuInit, (unsigned int flags), 
    flags)
MAKE_CUDA_METHOD(cuDeviceGet, (CUdevice *device, int ordinal), 
    device, ordinal)
MAKE_CUDA_METHOD(cuCtxGetDevice, (CUdevice *device), 
    device)
MAKE_CUDA_METHOD(cuCtxCreate, (CUcontext *pctx, unsigned int flags, CUdevice dev), 
    pctx, flags, dev)
MAKE_CUDA_METHOD(cuCtxGetCurrent, (CUcontext *pctx), 
    pctx)
MAKE_CUDA_METHOD(cuCtxSetCurrent, (CUcontext pctx), 
    pctx)
MAKE_CUDA_METHOD(cuDevicePrimaryCtxRetain, (CUcontext* pctx, CUdevice dev), 
    pctx, dev)
MAKE_CUDA_METHOD(cuMemAlloc, (CUdeviceptr *dptr, size_t bytesize), 
    dptr, bytesize)
MAKE_CUDA_METHOD(cuModuleGetFunction, (CUfunction *hfunc, CUmodule hmod, const char *name), 
    hfunc, hmod, name)
MAKE_CUDA_METHOD(cuLaunchKernel, (CUfunction f,
    unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ,
    unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ,
    unsigned int sharedMemBytes, CUstream hStream, void **kernelParams, void **extra), 
    f, gridDimX, gridDimY, gridDimZ, blockDimX, blockDimY, blockDimZ, sharedMemBytes, hStream, kernelParams, extra)
MAKE_CUDA_METHOD(cuModuleLoadData, (CUmodule *module, const void *image), 
    module, image)

CUresult GetMethodBySymbol(const char *symbol, void **pfn, int driverVersion, cuuint64_t flags,
                                CUdriverProcAddressQueryResult *symbolStatus)
{
    if (libCudaHandle == nullptr)
    {
        std::cout << "GetMethodBySymbol() failed (libCudaHandle is null): " << dlerror() << std::endl;
        return CUDA_ERROR_UNKNOWN;
    }

    if (symbol == nullptr || strlen(symbol) == 0)
    {
        std::cout << "Symbol is empty\n";
        return CUDA_ERROR_UNKNOWN;
    }

    TRY_INTERCEPT(symbol, cuInit);
    TRY_INTERCEPT(symbol, cuDeviceGet);
    TRY_INTERCEPT(symbol, cuDriverGetVersion);
    TRY_INTERCEPT(symbol, cuCtxGetDevice);
    TRY_INTERCEPT(symbol, cuCtxCreate);
    TRY_INTERCEPT(symbol, cuCtxGetCurrent);
    TRY_INTERCEPT(symbol, cuCtxSetCurrent);
    TRY_INTERCEPT(symbol, cuDevicePrimaryCtxRetain);
    TRY_INTERCEPT(symbol, cuMemAlloc);
    TRY_INTERCEPT(symbol, cuLaunchKernel);
    TRY_INTERCEPT(symbol, cuModuleGetFunction);
    TRY_INTERCEPT(symbol, cuModuleLoadData);

    // 1. try to find "symbol" + _v3
    char symbolExtV3[strlen(symbol) + 3] = {};
    strcat(symbolExtV3, symbol);
    strcat(symbolExtV3, "_v3");

    *pfn = dlsym(libCudaHandle, symbolExtV3);
    if (*pfn != nullptr)
    {
        return CUDA_SUCCESS;
    }

    // 2. try to find "symbol" + _v2
    char symbolExtV2[strlen(symbol) + 3] = {};
    strcat(symbolExtV2, symbol);
    strcat(symbolExtV2, "_v2");

    *pfn = dlsym(libCudaHandle, symbolExtV2);
    if (*pfn != nullptr)
    {
        return CUDA_SUCCESS;
    }

    // 3. try to find "symbol"
    *pfn = dlsym(libCudaHandle, symbol);
    if (*pfn != nullptr)
    {
        return CUDA_SUCCESS;
    }

    std::cout << "symbol not found: " << symbol << std::endl;
    return CUDA_ERROR_UNKNOWN;
}

}