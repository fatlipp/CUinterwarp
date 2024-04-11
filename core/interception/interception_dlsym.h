#pragma once

#include "core/interception/tools.h"

#include <cuda.h>

#include <dlfcn.h>

extern "C"
{

CUresult GetMethodBySymbol(const char *symbol, void **pfn, int driverVersion, cuuint64_t flags,
                                CUdriverProcAddressQueryResult *symbolStatus);

void *__libc_dlsym(void *map, const char *name);
void *__libc_dlopen_mode(const char *name, int mode);

using normalDlsymFn = void*(void *, const char *);

void *dlsym(void *handle, const char *symbol)
{
    if (compText(symbol, xstr(cuGetProcAddress)))
    {
        return (void *)&GetMethodBySymbol;
    }

    static auto normalDsym = (normalDlsymFn*)__libc_dlsym(
        __libc_dlopen_mode("libdl.so.2", RTLD_LAZY), "dlsym");
    return (*normalDsym)(handle, symbol);
}

}