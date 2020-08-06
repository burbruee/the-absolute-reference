#pragma once

typedef enum VideoDimension {
	VIDEO_WIDTH = 320,
	VIDEO_HEIGHT = 240
} VideoDimension;

typedef enum Bpp {
	BPP_4,
	BPP_8
} Bpp;

#define NUMPALCOLORS_BPP(bpp) NUMPALCOLORS_##bpp##BPP = 1 << (bpp)
typedef enum NumPalColors {
	NUMPALCOLORS_BPP(4),
	NUMPALCOLORS_BPP(8),
} NumPalColors;
