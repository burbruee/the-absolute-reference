#pragma once

// Functionality provided by the SH-2 compiler for the C language.
// TODO: Eliminate use of as many operator functions as possible. Keep the memory and string functions, though.

#include <stddef.h>
#include <stdint.h>

int32_t Div32s(int32_t denom, int32_t numer);
uint32_t Div32u(uint32_t denom, uint32_t numer);
int16_t Div16s(int16_t denom, int32_t numer);
int32_t Mod32s(int32_t denom, int32_t numer);
uint32_t Mod32u(uint32_t denom, uint32_t numer);
void MemCopy(size_t n, void* dest, void* src); // Where used, it was originally used; where string.h's memcpy is used, this wasn't originally used in the SH-2 code.
int32_t LeftShift(int32_t value, int32_t shifts);
int32_t RightShift(int32_t value, int32_t shifts);
int8_t StringNCompare(const char* s1, const char* s2, size_t n);
int16_t Mod16s(int16_t denom, int16_t numer);
void* MemSet(void* s, int c, size_t n);
