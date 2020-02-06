#pragma once

typedef enum VideoDimension {
	VIDEO_WIDTH = 320,
	VIDEO_HEIGHT = 240
} VideoDimension;

typedef enum Bpp {
	BPP4,
	BPP8
} Bpp;

typedef enum NumPalColors {
	NUMPALCOLORS_4BPP = 16 << (BPP4 << 2),
	NUMPALCOLORS_8BPP = 16 << (BPP8 << 2)
} NumPalColors;
