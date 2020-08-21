#pragma once

// Functionality provided by the SH-2 compiler for the C language.
// TODO: Eliminate use of as many operator functions as possible. Keep the memory and string functions, though.

#include "Config.h"
#include <stddef.h>
#include <stdint.h>

int32_t Div32s(int32_t denom, int32_t numer);
uint32_t Div32u(uint32_t denom, uint32_t numer);
int16_t Div16s(int16_t denom, int32_t numer);
int32_t Mod32s(int32_t denom, int32_t numer);
uint32_t Mod32u(uint32_t denom, uint32_t numer);

#ifdef USESTDC
#include <string.h>
#define MemCopy(n, dst, src) ((void)memcpy(dst, src, n))
#else
void MemCopy(size_t n, void* dst, void* src);
#endif

int32_t LeftShift(int32_t value, int32_t shifts);
int32_t RightShift(int32_t value, int32_t shifts);
#ifdef USESTDC
#include <string.h>
#define StringNCompare(s1, s2, n) ((int8_t)strncmp((s1), (s2), (n)))
#else
int8_t StringNCompare(const char* s1, const char* s2, size_t n);
#endif
int16_t Mod16s(int16_t denom, int16_t numer);

#ifdef USESTDC
#include <string.h>
#define MemSet(s, c, n) memset(s, c, n)
#else
void* MemSet(void* s, int c, size_t n);
#endif
