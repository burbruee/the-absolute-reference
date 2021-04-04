#pragma once

// Functionality provided by the SH-2 compiler for the C language.

#include "Config.h"
#include <stddef.h>
#include <stdint.h>

#ifdef USESTDC
#include <string.h>
#define MemCopy(n, dst, src) ((void)memcpy((dst), (src), (n)))
#else
void MemCopy(size_t n, void* dst, void* src);
#endif

#ifdef USESTDC
#include <string.h>
#define StringNCompare(s1, s2, n) ((int8_t)strncmp((s1), (s2), (n)))
#else
int8_t StringNCompare(const char* s1, const char* s2, size_t n);
#endif

#ifdef USESTDC
#include <string.h>
#define MemSet(s, c, n) memset((s), (c), (n))
#else
void* MemSet(void* s, int c, size_t n);
#endif
