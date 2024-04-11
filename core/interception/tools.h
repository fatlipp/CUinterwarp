#pragma once

#include <string.h>

extern "C"
{

// expands macro, if exists (i.e. cuCtxCreate => cuCtxCreate_v2, if it is #defined in <cuda.h>)
#define xstr(s) str(s)
#define str(s) #s

inline bool compText(const char *text, const char *text2)
{
    return strcmp(text, text2) == 0;
}

}