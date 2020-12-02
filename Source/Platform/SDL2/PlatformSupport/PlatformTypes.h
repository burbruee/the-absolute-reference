#pragma once

#include "SDL_pixels.h"
#include <stdlib.h>
#include <stdint.h>

#define ROMDATA
#define RAMDATA

typedef Uint32 Color;

#define COLOR(r, g, b, a) ( \
	((Color)((r) & 0xFFu) << 24) | \
	((Color)((g) & 0xFFu) << 16) | \
	((Color)((b) & 0xFFu) <<  8) | \
	((Color)((a) & 0xFFu) <<  0)   \
)

#define COLOR_GETR(color) ((uint8_t)(((color) >> 24) & 0xFFu))
#define COLOR_GETG(color) ((uint8_t)(((color) >> 16) & 0xFFu))
#define COLOR_GETB(color) ((uint8_t)(((color) >>  8) & 0xFFu))
#define COLOR_GETA(color) ((uint8_t)(((color) >>  0) & 0xFFu))
#define COLOR_SETR(color, r) ((color) = ((color) & 0x00FFFFFFu) | ((Color)((r) & 0xFFu) << 24))
#define COLOR_SETG(color, g) ((color) = ((color) & 0xFF00FFFFu) | ((Color)((g) & 0xFFu) << 16))
#define COLOR_SETB(color, b) ((color) = ((color) & 0xFFFF00FFu) | ((Color)((b) & 0xFFu) <<  8))
#define COLOR_SETA(color, a) ((color) = ((color) & 0xFFFFFF00u) | ((Color)((a) & 0xFFu) <<  0))

#define MAIN_DECLARATION int main(const int argc, const char* const* const argv)
