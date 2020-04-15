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

#define SPRITELAYER_FREE 0x0000

unknown_type* _0x606005C = NULL;
static void (**_0x6060060)() = NULL;
static uint16_t* _0x6060064 = NULL;
static uint16_t _0x6060068 = 0u;
struct_0x606006C _0x606006C[64];
int16_t _0x606106C[64];

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

// NOTE: Video set functions must have three arguments, each of which is either
// a pointer or intptr_t/uintptr_t.
typedef struct VideoSetter {
	void (*set)(void*, void*, void*);
	void* args[3];
} VideoSetter;

#define MAXVIDEOSETTERS 32
static uint16_t NumVideoSetters;
static VideoSetter VideoSetters[MAXVIDEOSETTERS];

// NOTE: Looks like this points to data that controls screen brightness during
// transitions; might also be used for darkening the attract mode ranking
// backgrounds. Also, _0x602970C is used with this, which might be where the
// darkening is handled.
unknown_type* _0x6064750 = NULL;

static unknown_type* _0x607D210 = NULL;
static unknown_type* _0x607D214 = NULL;
static unknown_type _0x607D218[192];

static unknown_type* _0x60AD218 = NULL;
static PauseMode NextPauseMode;
static uint16_t _0x60AD21E = 0u;
static int16_t _0x60AD224;

void _0x6023788() {
	// Empty.
}

void _0x602378C() {
	// Empty.
}

void _0x6023790() {
	// TODO
}

void _0x60237DE() {
	CurrentPauseMode = NextPauseMode;
	if (_0x6060068 != 0) {
		do {
			_0x60AD21E = 0u;
			_0x606005C = _0x60AD218;
			if (CurrentPauseMode < PAUSEMODE_GAME) {
				int16_t var0 = _0x60AD218->_0x18;
				void** var1 = &_0x60AD218->_0x300[(1 - var0 + 14) * 4 + 3];
				while (var0 > 0) {
					_0x6060064 = var1 - 3;
					_0x6060060 = var1;
					((void(*)())*var1)();
					if (_0x60AD21E != 0) {
						break;
					}
					var0--;
					var1 += 4;
				}
			}
			else if (_0x60AD218->_0x3F8 != _0x6023788) {
				int16_t var0 = _0x60AD218->_0x18;
				void** var1 = &_0x60AD218->_0x300[(1 - var0 + 14) * 4 + 3];
				while (var0 > 1) {
					_0x6060064 = var1 - 3;
					_0x6060060 = var1;
					((void(*)())*var1)();
					if (_0x60AD21E != 0) {
						break;
					}
					var0--;
					var1 += 4;
				}
				if (_0x60AD21E == 0) {
					_0x6060060 = &_0x60AD218->_0x3F8;
					_0x60AD218->_0x3F8();
				}
			}
			_0x606005C = _0x60AD218->_0x14;
			_0x60AD218 = _0x606005C;
		} while (_0x606005C != NULL);
	}
}

void _0x602392E(int32_t arg0, void (*arg1)()) {
	// TODO
}

void _0x60239B8(int32_t arg0, int32_t arg1) {
	// TODO
}

void _0x6023A0E(unknown_type* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4) {
	// TODO
}

void* _0x6023A98(unknown_type* arg0, void* arg1, void* arg2, void* arg3) {
	// TODO
	return NULL;
}

void _0x6023B76(unknown_type* arg0) {
	// TODO
}

void _0x6023BC4(unknown_type* arg0, void (*arg1)()) {
	// TODO
}

void _0x6023C3E(unknown_type* arg0, void* arg1) {
	// TODO
}

void* _0x6023C8A(unknown_type* arg0, void* arg1) {
	// TODO
	return NULL;
}

int32_t _0x6023CBC(void (*arg0)(), uint32_t arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7) {
	// TODO
	return 0u;
}

void* _0x6023DAE(unknown_type* arg0) {
	// TODO
	return NULL;
}

void _0x6023E5A(unknown_type* arg0) {
	// TODO
	return NULL;
}

void* _0x6023EFE(unknown_type* arg0, unknown_type* arg1) {
	// TODO
	return NULL;
}

void* _0x6023FA4(void (**arg0)(), unknown_type* arg1) {
	// TODO
	return NULL;
}

void _0x6024030(unknown_type* arg0) {
	// TODO
}

void _0x602406E() {
	// TODO
}

void _0x60240A8(void* arg0) {
	// TODO
}

void SetNextPauseMode(PauseMode nextPauseMode) {
	NextPauseMode = nextPauseMode;
}

void DisablePause() {
	NextPauseMode = PAUSEMODE_NOPAUSE;
}

void GetNextPauseMode() {
	return NextPauseMode;
}

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

	// Set the first sprite name of the selected layer in the layer table to
	// the start of the new names.
	SpriteLayers[layer] = NumSprites;
}

void FreeSpriteLayer(uint16_t layer) {
	SpriteLayers[layer] = SPRITELAYER_FREE;
}

void InitSpriteAlpha() {
	for (size_t i = 0; i < NUMALPHAVALUES; i++) {
		AlphaValues[i] = 0x00;
	}
}

void _0x602419C() {
	// TODO
}

void _0x6024244() {
	int16_t* var0 = _0x606106C;
	for (int16_t i = 0u; i < _0x60AD224; i++, var0++) {
		int32_t var1 = *var0;
		struct_0x606006C* var2 = &_0x606006C[var1];
		switch (var2->_0x2B) {
		case 0u:
			while (CurrentPauseMode < PAUSEMODE_BG && --var2->_0x34 < 1) {
				int16_t var3;
				var2->_0x34 = var2->_0x36;
				if (!(var2->_0x2C & 0x800)) {
					if (var2->_0x16 < var2->_0x28 - 1) {
						var3 = var2->_0x16 + 1;
					}
					else {
						if (!(var2->_0x2C & 0x200)) {
							if (!(var2->_0x2C & 0x400)) {
								var2->_0x16 = var2->_0x28 - 1;
								var2->_0x2B = 2u;
							}
							else {
								var2->_0x16--;
								var2->_0x2C |= 0x800;
							}
							break;
						}
						var3 = var2->_0x38;
					}
				}
				else {
					if (var2->_0x38 < var2->_0x16) {
						var3 = var2->_0x16;
					}
					else {
						if (!(var2->_0x2C & 0x200)) {
							if (!(var2->_0x2C & 0x400)) {
								var2->_0x16 = var2->_0x38;
								var2->_0x2B = 2u;
							}
							else {
								var2->_0x16++;
								var2->_0x2C &= ~0x0800;
							}
							break;
						}
						var3 = var2->_0x28;
					}
					var3--;
				}
				var2->_0x16 = var3;
				break;
			}

		case 2u:
			if (!(var2->_0x2C & 0x8000)) {
				if (NumScreenFramesOdd) {
					if ((var2->_0x2C & 0x20) || CurrentPauseMode >= PAUSEMODE_GAME) {
						_0x60243E8(var2);
					}
				}
				else {
					if ((var2->_0x2C & 0x10) || CurrentPauseMode >= PAUSEMODE_GAME) {
						_0x60243E8(var2);
					}
				}
			}
			break;

		case 9u:
			if (!(var2->_0x2C & 0x8000)) {
				_0x602471C(var2);
			}
			break;

		default:
			break;
		}
	}
}

void _0x60243E8(struct_0x606006C* arg0) {
	// TODO
}

void _0x602471C(struct_0x606006C* arg0) {
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

void _0x6026FCA(uint16_t arg0, uint16_t arg1) {
	// TODO
}

void _0x60294C0(uint8_t arg0) {
	// TODO: Make a define for register 0x0B.
	VIDEOREGS[0x0B] = (VIDEOREGS[0x0B] & 0xF8u) | (arg0 & 0x07u);
}

void VideoSetBackdropColor(uintptr_t color, void* unused1, void* unused2) {
	RAMDATA Color* backdropLineColor = BACKDROPRAM;
	for (size_t i = 0; i < NUMBACKDROPLINES; i += 16u, backdropLineColor += 16u) {
		backdropLineColor[0] = (Color)color;
		backdropLineColor[1] = (Color)color;
		backdropLineColor[2] = (Color)color;
		backdropLineColor[3] = (Color)color;
		backdropLineColor[4] = (Color)color;
		backdropLineColor[5] = (Color)color;
		backdropLineColor[6] = (Color)color;
		backdropLineColor[7] = (Color)color;
		backdropLineColor[8] = (Color)color;
		backdropLineColor[9] = (Color)color;
		backdropLineColor[10] = (Color)color;
		backdropLineColor[11] = (Color)color;
		backdropLineColor[12] = (Color)color;
		backdropLineColor[13] = (Color)color;
		backdropLineColor[14] = (Color)color;
		backdropLineColor[15] = (Color)color;
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

void ResetVideoSetters() {
	NumVideoSetters = 0u;
}

void _0x6029814(uint32_t arg0, uint32_t arg1, uint8_t arg2, uint8_t arg3) {
	// TODO
}

typedef struct FixedColor {
	Fixed16 r, g, b;
} FixedColor;

typedef struct PalCycle {
	PalCycleType type;
	int16_t palNum;

	int16_t perPalDelay;
	int16_t palFrames;

	int16_t stride;
	int16_t step;
	int16_t endStep;

	FixedColor palFixed[NUMPALCOLORS_4BPP];
	FixedColor palFixedV[NUMPALCOLORS_4BPP];

	Color pal0[NUMPALCOLORS_4BPP];
	Color pal1[NUMPALCOLORS_4BPP];
} PalCycle;

static PalCycle PalCycleHeap[MAXPALCYCLES];

static bool NoPalCycles;

void InitPalCycles() {
	// Set all palette cycles as free.
	PalCycle* cycle = PalCycleHeap;
	for (int16_t i = 0; i < lengthof(PalCycleHeap); i++, cycle++) {
		cycle->type = PALCYCLETYPE_FREE;
		cycle->palNum = -1;
	}
}

void UpdatePalCycles() {
	if (!NoPalCycles && CurrentPauseMode < PAUSEMODE_BG) {
		Color tempPal[NUMPALCOLORS_4BPP];
		PalCycle* cycle = PalCycleHeap;
		for (uint16_t i = 0u; i < MAXPALCYCLES; i++, cycle++) {
			if (cycle->palNum >= 0 && --cycle->palFrames <= 0) {
				cycle->palFrames = cycle->perPalDelay;
				if (cycle->stride > 0) {
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
						Palettes[cycle->palNum * NUMPALCOLORS_4BPP + j] = tempPal[j];
					}

					cycle->step += cycle->stride;
					if (cycle->step > cycle->endStep || cycle->step < 0) {
						switch (cycle->type) {
						case PALCYCLETYPE_UPSTOP:
							cycle->type = PALCYCLETYPE_FREE;
							cycle->palNum = -1;
							cycle->step = cycle->endStep;
							break;

						case PALCYCLETYPE_UPRESTART:
							cycle->step = 0;
							cycle->palFrames = cycle->perPalDelay;
							for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
								cycle->palFixed[j].r = F16(COLOR_GETR(cycle->pal0[j]) & 0xFC, 0x00);
								cycle->palFixed[j].g = F16(COLOR_GETG(cycle->pal0[j]) & 0xFC, 0x00);
								cycle->palFixed[j].b = F16(COLOR_GETB(cycle->pal0[j]) & 0xFC, 0x00);
							}
							break;

						case PALCYCLETYPE_BOUNCE:
							cycle->palFrames = cycle->perPalDelay;
							cycle->stride *= -1;
							cycle->step = cycle->step >= 0 ? cycle->endStep : 0;
							break;

						case PALCYCLETYPE_DOWNRESTART:
							cycle->palFrames = cycle->perPalDelay;
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

void NewPalCycle(uint8_t palNum, Color* pal0, Color* pal1, int16_t perPalDelay, PalCycleType type, uint8_t stride, uint8_t endStep) {
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

	PalCycle* cycle = &PalCycleHeap[heapIndex];

	cycle->palNum = palNum;
	cycle->palFrames = 0;
	cycle->type = type;
	cycle->endStep = endStep;

	switch (type) {
	case PALCYCLETYPE_UPSTOP:
	case PALCYCLETYPE_UPRESTART:
	case PALCYCLETYPE_BOUNCE:
		cycle->perPalDelay = perPalDelay;
		cycle->step = 0;
		cycle->stride = stride;
		break;

	case PALCYCLETYPE_DOWNRESTART:
	case PALCYCLETYPE_DOWNSTOP:
		cycle->perPalDelay = perPalDelay;
		cycle->step = endStep;
		cycle->stride = -stride;
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
		if (cycle->stride > 0) { \
			cycle->palFixed[i].##c = F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00); \
		} \
		else { \
			cycle->palFixed[i].##c = F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00); \
		} \
		cycle->palFixedV[i].##c = stride * (F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00) - F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00))

		INITCOMPONENT(r, R);
		INITCOMPONENT(g, G);
		INITCOMPONENT(b, B);
	}
}

void FreePalCycles(uint16_t palNum) {
	if (palNum > NUMPALS - 1u) {
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

static void VideoSetPal(uintptr_t palNum, uintptr_t numPals, const Color* pal);
void SetPal(uint8_t palNum, uint8_t numPals, const Color* pal) {
	VideoSetters[NumVideoSetters].set = VideoSetPal;
	VideoSetters[NumVideoSetters].args[0] = (uintptr_t)palNum;
	VideoSetters[NumVideoSetters].args[1] = (uintptr_t)numPals;
	VideoSetters[NumVideoSetters++].args[2] = pal;
}
static void VideoSetPal(uintptr_t palNum, uintptr_t numPals, const Color* pal) {
	palNum = (uint8_t)palNum;
	numPals = (uint8_t)numPals;
	for (int16_t i = 0; i < numPals; i++) {
		for (size_t j = 0u; j < 16u; j++) {
			Palettes[palNum * 16 * 4 + j * 4] = pal[j];
		}
	}
}

void _0x602AA4C() {
	_0x60294C0(0x07u);
	VideoSetBackdropColor(0x000000FFu, NULL, NULL);
}

struct {
	Color offset_0;
	RAMDATA Color* offset_4;
	int16_t offset_8;
	int16_t offset_A;
	int16_t offset_C;
} _0x6064550[32];

void _0x602AB9E() {
	for (size_t i = 0; i < lengthof(_0x6064550); i++) {
		_0x6064550[i].offset_0 = 0x00000000u;
	}
}
