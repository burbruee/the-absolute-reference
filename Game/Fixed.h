#pragma once

#include "Config.h"
#include <stdint.h>

// Fixed point values have a signed integer part and unsigned fractional part.
// So a -1 integer part could be a row below the matrix. But for both negative
// and nonnegative rows, a fractional part of 0xFFFF would be the highest
// position within those rows, and 0x0000 the lowest.
//
// The fixed point I/F macros produce lvalues, so you can use them in
// expressions as well as assign to the macro.
//
// The fixed point types are endian-dependent, which allows doing arithmetic in
// normal integer expressions with them, in the same way you'd use floating
// point values.

typedef int32_t Fixed32;
#define F32(i, f) (((int32_t)(i) << 16) | ((uint32_t)(f) & 0xFFFFu))
#define F32I(v) (*(( int16_t*)(v) + !BIGEND))
#define F32F(v) (*((uint16_t*)(v) +  BIGEND))

typedef uint32_t UFixed32;
#define UF32(i, f) (((uint32_t)(i) << 16) | ((uint32_t)(f) & 0xFFFFu))
#define UF32I(v) (*((uint16_t*)(v) + !BIGEND))
#define UF32F(v) (*((uint16_t*)(v) +  BIGEND))

typedef int16_t Fixed16;
#define F16(i, f) (((int16_t)(i) << 8) | ((uint16_t)(f) & 0xFFu))
#define F16I(v) (*(( int8_t*)(v) + !BIGEND))
#define F16F(v) (*((uint8_t*)(v) +  BIGEND))

typedef uint16_t UFixed16;
#define UF16(i, f) (((uint16_t)(i) << 8) | ((uint16_t)(f) & 0xFFu))
#define UF16I(v) (*((uint8_t*)(v) + !BIGEND))
#define UF16F(v) (*((uint8_t*)(v) +  BIGEND))
