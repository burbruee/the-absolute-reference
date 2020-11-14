#pragma once

#include "Config.h"
#include <stdint.h>

// All fixed point types have an unsigned fractional part. For the fractional
// part, 0xFFFF is the highest position in a row, 0x0000 the lowest.
//
// For the signed fixed point types, a -1 integer part would be below row 0.
//
// The I/F macros produce lvalues, so you can use them in expressions as well
// as assign to the macro.
//
// The RVALUE macros are for rvalues, such as function return values, and can't
// be used for assignment to the indivual parts.
//
// The fixed point types are endian-dependent, which allows doing arithmetic in
// normal integer expressions with them, in the same way you'd use floating
// point values.

typedef int32_t Fixed32;
#define F32(i, f) (((int32_t)(i) << 16) | ((uint16_t)(f)))
#define F32I(v) (*(( int16_t*)(&v) + !BIGEND))
#define F32F(v) (*((uint16_t*)(&v) +  BIGEND))
#define F32IRVALUE(v) ((int16_t)((v) >> 16))
#define F32FRVALUE(v) ((uint16_t)(v))

typedef uint32_t UFixed32;
#define UF32(i, f) (((uint32_t)(i) << 16) | ((uint16_t)(f)))
#define UF32I(v) (*((uint16_t*)(v) + !BIGEND))
#define UF32F(v) (*((uint16_t*)(v) +  BIGEND))
#define UF32IRVALUE(v) ((uint16_t)((v) >> 16))
#define UF32FRVALUE(v) ((uint16_t)(v))

typedef int16_t Fixed16;
#define F16(i, f) (((int16_t)(i) << 8) | ((uint8_t)(f)))
#define F16I(v) (*(( int8_t*)(&v) + !BIGEND))
#define F16F(v) (*((uint8_t*)(&v) +  BIGEND))
#define F16IRVALUE(v) ((int8_t)((v) >> 16))
#define F16FRVALUE(v) ((uint8_t)(v))

typedef uint16_t UFixed16;
#define UF16(i, f) (((uint16_t)(i) << 8) | ((uint8_t)(f)))
#define UF16I(v) (*((uint8_t*)(&v) + !BIGEND))
#define UF16F(v) (*((uint8_t*)(&v) +  BIGEND))
#define UF16IRVALUE(v) ((uint8_t)((v) >> 16))
#define UF16FRVALUE(v) ((uint8_t)(v))
