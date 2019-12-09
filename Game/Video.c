#include "Video.h"
#include "Fixed.h"
#include "HwSprite.h"
#include "Macros.h"
#include "HwData.h"
#include "LibC.h"
#include "Frame.h"
#include "VideoDefs.h"
#include "PlatformTypes.h"
#include "Macros.h"

// NOTE: Video set functions must have three arguments, each of which is either
// a pointer or intptr_t/uintptr_t.
typedef struct VideoSetter {
	void (*set)(void*, void*, void*);
	void* args[3];
} VideoSetter;

#define MAXVIDEOSETTERS 32
static uint16_t NumVideoSetters;
static VideoSetter VideoSetters[MAXVIDEOSETTERS];

#define SPRITELAYER_FREE 0x0000

// Each index into this table is a sprite layer number. An element value of
// zero indicates the layer is free.
//
// Each element is both the name for the first sprite in a layer, and an index
// into SpriteLayerNames where any remaining sprite names for that layer
// start.
static int16_t SpriteLayers[128];

// Table of the sprite names in each layer that are after the first name.  Each
// element is both a sprite name and the index in the layer name table of the
// next sprite name in the layer. The end of a layer is marked with a zero
// valued name.
static int16_t SpriteLayerNames[MAXSPRITES];

int16_t NumSprites;

void AllocSpriteLayerNames(int16_t layer, uint16_t num) {
	// Write sprite names beyond the first name to the data table.
	// The new names will be appended onto the end of the table.
	int16_t numNames = num - 1;
	int16_t i;
	for (i = 0; i < numNames; i++) {
		SpriteLayerNames[NumSprites + i] = NumSprites + i + 1;
	}

	// Modify the layer data so the end of the data newly written to the
	// layer data table leads to the list of names that were previously in
	// the layer.
	SpriteLayerNames[NumSprites + i] = SpriteLayers[layer];

	// Set the first sprite name of the selected layer in the layer table.
	SpriteLayers[layer] = NumSprites;
}

void FreeSpriteLayer(uint16_t layer) {
	SpriteLayers[layer] = SPRITELAYER_FREE;
}

void InitSpriteAlpha() {
	for (size_t i = 0; i < lengthof(VideoControl.alpha); i++) {
		VideoControl.alpha[i] = 0x00;
	}
}

void _0x602419C() {
	// TODO
}

void _0x6024244() {
	// TODO
}

void _0x60243E8(struct_0x606006C *arg0) {
	// TODO
}

void _0x601471C(struct_0x606006C *arg0) {
	// TODO: Analyze this one first.
}

void WriteSpriteLayers() {
	// Convert from layers to a linear name sequence.
	int16_t nameIndex = SPRITE_FIRST;
	int16_t* currentLayer = SpriteLayerNames;
	for (int16_t layer = 0; layer < lengthof(SpriteLayers); layer++) {
		for (int16_t layerName = *currentLayer++; layerName != 0; nameIndex++) {
			SpriteNames[nameIndex] = layerName;
			layerName = SpriteLayerNames[layerName];
		}
	}

	// Terminate the name table.
	SpriteNames[nameIndex - 1] |= 0x4000;
}

void InitSpriteLayers() {
	// Set all layers as free.
	for (int16_t layer = 0; layer < lengthof(SpriteLayers); layer++) {
		SpriteLayers[layer] = SPRITELAYER_FREE;
	}
}

void SetVideo() {
	NumVideoSetters = NumVideoSetters < MAXVIDEOSETTERS ? NumVideoSetters : MAXVIDEOSETTERS;

	VideoSetter* setter = VideoSetters;
	for (uint16_t i = 0u; i < NumVideoSetters; i++) {
		setter->set(setter->args[0], setter->args[1], setter->args[2]);
	}

	NumVideoSetters = 0u;
}

typedef struct FixedColor {
	Fixed16 r, g, b;
} FixedColor;

typedef struct PalCycleData {
	PalCycleType type;
	int16_t palNum;

	int16_t perPalDelay;
	int16_t numPalFrames;

	int16_t steppingLength;
	int16_t step;
	int16_t endStep;

	FixedColor palFixed[NUMPALCOLORS_4BPP];
	FixedColor palFixedV[NUMPALCOLORS_4BPP];

	Color pal0[NUMPALCOLORS_4BPP];
	Color pal1[NUMPALCOLORS_4BPP];
} PalCycleData;

static PalCycleData PalCycleHeap[MAXPALCYCLES];

static bool NoPalCycles;

void InitPalCycles() {
	// Set all palette cycles as free.
	PalCycleData* cycle = PalCycleHeap;
	for (int16_t i = 0; i < lengthof(PalCycleHeap); i++, cycle++) {
		cycle->type = PALCYCLETYPE_FREE;
		cycle->palNum = -1;
	}
}

void UpdatePalCycles() {
	if (!NoPalCycles && CurrentPauseMode < PAUSEMODE_BG) {
		Color tempPal[NUMPALCOLORS_4BPP];
		PalCycleData* cycle = PalCycleHeap;
		for (uint16_t i = 0u; i < MAXPALCYCLES; i++, cycle++) {
			if (cycle->palNum >= 0 && --cycle->numPalFrames <= 0) {
				cycle->numPalFrames = cycle->perPalDelay;
				if (cycle->steppingLength > 0) {
					for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
						Color color;

						cycle->palFixed[j].r += cycle->palFixedV[j].r;
						COLOR_SETR(color, F16I(cycle->palFixed[j].r) & 0xFC);

						cycle->palFixed[j].g += cycle->palFixedV[j].g;
						COLOR_SETG(color, F16I(cycle->palFixed[j].g) & 0xFC);

						cycle->palFixed[j].b += cycle->palFixedV[j].b;
						COLOR_SETB(color, F16I(cycle->palFixed[j].b) & 0xFC);

						tempPal[j] = color;
					}

					for (size_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
						*&((RAMDATA Color*)PALRAM)[cycle->palNum * NUMPALCOLORS_4BPP + j] = tempPal[j];
					}

					cycle->step += cycle->steppingLength;
					if (cycle->step > cycle->endStep || cycle->step < 0) {
						switch (cycle->type) {
						case PALCYCLETYPE_UPSTOP:
							cycle->type = PALCYCLETYPE_FREE;
							cycle->palNum = -1;
							cycle->step = cycle->endStep;
							break;

						case PALCYCLETYPE_UPRESTART:
							cycle->step = 0;
							cycle->numPalFrames = cycle->perPalDelay;
							for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
								cycle->palFixed[j].r = F16(COLOR_GETR(cycle->pal0[j]) & 0xFC, 0x00);
								cycle->palFixed[j].g = F16(COLOR_GETG(cycle->pal0[j]) & 0xFC, 0x00);
								cycle->palFixed[j].b = F16(COLOR_GETB(cycle->pal0[j]) & 0xFC, 0x00);
							}
							break;

						case PALCYCLETYPE_BOUNCE:
							cycle->numPalFrames = cycle->perPalDelay;
							cycle->steppingLength *= -1;
							cycle->step = cycle->step >= 0 ? cycle->endStep : 0;
							break;

						case PALCYCLETYPE_DOWNRESTART:
							cycle->numPalFrames = cycle->perPalDelay;
							cycle->step = cycle->endStep;
							for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
								cycle->palFixed[j].r = F16(COLOR_GETR(cycle->pal1[j]) & 0xFC, 0x00);
								cycle->palFixed[j].g = F16(COLOR_GETG(cycle->pal1[j]) & 0xFC, 0x00);
								cycle->palFixed[j].b = F16(COLOR_GETB(cycle->pal1[j]) & 0xFC, 0x00);
							}
							break;

						case PALCYCLETYPE_DOWNSTOP:
							cycle->type = PALCYCLETYPE_FREE;
							break;

						default:
							break;
						}
					}
				}
			}
		}
	}
}

void NewPalCycle(uint8_t palNum, Color* pal0, Color* pal1, int16_t perPalDelay, PalCycleType type, uint8_t steppingLength, uint8_t endStep) {
	size_t heapIndex = MAXPALCYCLES;

	// Check if there's already a cycle for palNum.
	for (uint16_t i = 0u; i < MAXPALCYCLES; i++) {
		if (PalCycleHeap[i].palNum == palNum) {
			heapIndex = i;
			break;
		}
	}

	// If there's no cycle for palNum already, look for a free cycle.
	if (heapIndex == MAXPALCYCLES) {
		for (uint16_t i = 0u; i < MAXPALCYCLES; i++) {
			if (PalCycleHeap[i].type <= PALCYCLETYPE_FREE) {
				heapIndex = i;
			}
		}
	}

	// BUG: There's no handling of the case where a free cycle wasn't found in
	// the SH-2 code. So a safeguard has been added here. TAP only ever
	// allocates at most three palette cycles, so the bug never shows up.
	if (heapIndex == MAXPALCYCLES) {
		return;
	}

	PalCycleData* cycle = &PalCycleHeap[heapIndex];

	cycle->palNum = palNum;
	cycle->numPalFrames = 0;
	cycle->type = type;
	cycle->endStep = endStep;

	switch (type) {
	case PALCYCLETYPE_UPSTOP:
	case PALCYCLETYPE_UPRESTART:
	case PALCYCLETYPE_BOUNCE:
		cycle->perPalDelay = perPalDelay;
		cycle->step = 0;
		cycle->steppingLength = steppingLength;
		break;

	case PALCYCLETYPE_DOWNRESTART:
	case PALCYCLETYPE_DOWNSTOP:
		cycle->perPalDelay = perPalDelay;
		cycle->step = endStep;
		cycle->steppingLength = -steppingLength;
		break;

	default:
		break;
	}

	for (size_t i = 0u; i < NUMPALCOLORS_4BPP; i++) {
		cycle->pal0[i] = pal0[i];
	}
	for (size_t i = 0u; i < NUMPALCOLORS_4BPP; i++) {
		cycle->pal1[i] = pal1[i];
	}

	for (size_t i = 0u; i < NUMPALCOLORS_4BPP; i++) {
		#define INITCOMPONENT(c, C) \
		if (cycle->steppingLength > 0) { \
			cycle->palFixed[i].##c = F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00); \
		} \
		else { \
			cycle->palFixed[i].##c = F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00); \
		} \
		cycle->palFixedV[i].##c = steppingLength * (F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00) - F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00))

		INITCOMPONENT(r, R);
		INITCOMPONENT(g, G);
		INITCOMPONENT(b, B);
	}
}

void FreePalCycles(uint16_t palNum) {
	if (palNum > MAXPALS - 1u) {
		for (uint16_t i = 0; i < MAXPALCYCLES; i++) {
			PalCycleHeap[i].type = PALCYCLETYPE_FREE;
			PalCycleHeap[i].palNum = -1;
		}
	}
	else {
		for (uint16_t i = 0u; i < MAXPALCYCLES; i++) {
			if (PalCycleHeap[i].palNum == palNum) {
				PalCycleHeap[i].palNum = -1;
				PalCycleHeap[i].type = PALCYCLETYPE_FREE;
			}
		}
	}
}

void DisablePalCycles() {
	NoPalCycles = true;
}

void EnablePalCycles() {
	NoPalCycles = false;
}

static void SetVideoSetPal(uintptr_t palNum, uintptr_t numPals, const Color* pal) {
	palNum = (uint8_t)palNum;
	numPals = (uint8_t)numPals;
	for (int16_t i = 0; i < numPals; i++) {
		for (size_t j = 0u; j < 16u; j++) {
			*(RAMDATA Color*)&PALRAM[palNum * 16 * 4 + j * 4] = pal[j];
		}
	}
}

void SetPal(uint8_t palNum, uint8_t numPals, const Color* pal) {
	VideoSetters[NumVideoSetters].set = SetVideoSetPal;
	VideoSetters[NumVideoSetters].args[0] = (uintptr_t)palNum;
	VideoSetters[NumVideoSetters].args[1] = (uintptr_t)numPals;
	VideoSetters[NumVideoSetters++].args[2] = pal;
}
