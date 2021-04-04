#pragma once

#include "Lib/Fixed.h"
#include <stdint.h>

// TODO: These could be returning an Angle.
uint8_t UNK_602B4E4(int32_t, int32_t);
uint8_t UNK_602B5C8(int32_t, int32_t);

// Angles range 0 to 255.
// Convert radians angles: (radians_angle * 256) / 2pi
// Convert degrees angles: (degrees_angle * 256) / 360_degrees
typedef uint8_t Angle;
Fixed32 Sin(Angle angle);
Fixed32 Cos(Angle angle);

extern uint32_t RandSeed;
uint32_t Rand(uint32_t upperBound);
