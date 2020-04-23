#pragma once

typedef enum VideoDimension {
	VIDEO_WIDTH = 320,
	VIDEO_HEIGHT = 240
} VideoDimension;

#define NUMPALCOLORS_BPP(bpp) NUMPALCOLORS_##bpp##BPP = 1 << (bpp)
typedef enum NumPalColors {
	NUMPALCOLORS_BPP(4),
	NUMPALCOLORS_BPP(8),
} NumPalColors;
