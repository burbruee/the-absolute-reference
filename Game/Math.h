#pragma once

#include "Fixed.h"
#include <stdint.h>

// TODO: Could be returning an Angle.
uint8_t _0x602B4E4(int32_t, int32_t);

uint32_t _0x602B5C8(int32_t, int32_t);

// Angles range 0 to 255.
// Convert radians angles: (radians_angle * 256) / 2pi
// Convert degrees angles: (degrees_angle * 256) / 360_degrees
typedef uint8_t Angle;
Fixed32 Sin(Angle angle);
Fixed32 Cos(Angle angle);

extern uint32_t RandSeed;
uint32_t Rand(uint32_t upperBound);
