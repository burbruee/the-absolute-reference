#pragma once

#include <stdint.h>

#define ROMDATA
#define RAMDATA

// TODO: Rewrite using SDL2 types.

typedef uint32_t Color;
#define COLOR(r, g, b) ( \
	( \
		((Color)(r) << 24) | \
		((Color)(g) << 16) | \
		((Color)(b) <<  8) \
	) & 0xFFFFFF00 \
)
#define COLOR_GETR(color) (uint8_t)(((color) >> 24) & 0xFF)
#define COLOR_GETG(color) (uint8_t)(((color) >> 16) & 0xFF)
#define COLOR_GETB(color) (uint8_t)(((color) >>  8) & 0xFF)
#define COLOR_SETR(color, r) ((color) = ((color) & 0x00FFFFFF) | (Color)(r & 0xFF))
#define COLOR_SETG(color, g) ((color) = ((color) & 0xFF00FFFF) | (Color)(g & 0xFF))
#define COLOR_SETB(color, b) ((color) = ((color) & 0xFFFF00FF) | (Color)(b & 0xFF))
