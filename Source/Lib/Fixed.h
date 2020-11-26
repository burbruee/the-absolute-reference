#pragma once

#include "Config.h"
#include <stdint.h>

// All fixed point types have an unsigned fractional part. For the fractional
// part, 0xFFFF is the highest position in a row, 0x0000 the lowest.
//
// For the signed fixed point types, a -1 integer part would be below row 0.
//
// The fixed point types are endian-dependent, which allows doing arithmetic in 
// normal integer expressions with the value field, in the same way you'd use 
// floating point values.

// Shorthand for initialization, guaranteed to initialize correctly, regardless of system endianness. Can be used for initializing any fixed point type, where valid.
#define FIXED(i, f) { .integer = (i), .fraction = (f) }

typedef union Fixed32 {
	struct {
#if BIGEND
		int16_t integer;
		uint16_t fraction;
#else
		uint16_t fraction;
		int16_t integer;
#endif
	};
	int32_t value;
} Fixed32;

#define F32(i, f) (Fixed32) { .integer = (int16_t)(i), .fraction = (uint16_t)(f) }
#define F32V(i, f) ((((int32_t)(i) & 0xFFFF) << 16) | ((uint16_t)(f)))

typedef union UFixed32 {
	struct {
#if BIGEND
		uint16_t integer;
		uint16_t fraction;
#else
		uint16_t fraction;
		uint16_t integer;
#endif
	};
	uint32_t value;
} UFixed32;

#define UF32(i, f) (Fixed32) { .integer = (uint16_t)(i), .fraction = (uint16_t)(f) }
#define UF32V(i, f) ((((uint32_t)(i) & 0xFFFFu) << 16) | ((uint16_t)(f)))

typedef union Fixed16 {
	struct {
#if BIGEND
		int8_t integer;
		uint8_t fraction;
#else
		uint8_t fraction;
		int8_t integer;
#endif
	};
	int16_t value;
} Fixed16;

#define F16(i, f) (Fixed16) { .integer = (int8_t)(i), .fraction = (uint8_t)(f) }
#define F16V(i, f) ((((int16_t)(i) & 0xFF) << 8) | ((uint8_t)(f)))

typedef union UFixed16 {
	struct {
#if BIGEND
		uint8_t integer;
		uint8_t fraction;
#else
		uint8_t fraction;
		uint8_t integer;
#endif
	};
	uint16_t value;
} UFixed16;

#define UF16(i, f) (UFixed16) { .integer = (uint8_t)(i), .fraction = (uint8_t)(f) }
#define UF16V(i, f) ((((uint16_t)(i) & 0xFFu) << 8) | ((uint8_t)(f)))
