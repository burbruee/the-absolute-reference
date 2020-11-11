#include "Video/Video.h"
#include "Video/SpriteInit.h"
#include "Video/HwSprite.h"
#include "Video/VideoDefs.h"
#include "Video/GameBg.h"
#include "Main/Frame.h"
#include "Lib/LibC.h"
#include "Lib/Fixed.h"
#include "Lib/Macros.h"
#include "PlatformTypes.h"
#include "HwData.h"
#include "BuildData/BuildData.h"
#include <assert.h>
#include <stdbool.h>

#define SPRITELAYER_FREE 0x0000

const int16_t UNK_60356C8[4] = { 0x00, 0x07, 0x0F, 0x1F };
STRUCT_607D218* UNK_606005C = NULL;
static void (**UNK_6060060)() = NULL;
static uint16_t* UNK_6060064 = NULL; // TODO: Appears to be a pointer to an array of four (u)int16_t.
static int16_t UNK_6060068 = 0; // NOTE: Appears to be a count of the number of active STRUCT_607D218 objects.
AddSpriteData SpriteAdders[64];
int16_t SpriteAdderNameTable[64];
STRUCT_6061932 UNK_6061932;

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

uint16_t NumVideoSetters = 0u;
VideoSetter VideoSetters[MAXVIDEOSETTERS];

typedef struct STRUCT_6064550 {
	Color* src;
	RAMDATA Color* dst;
	int16_t delay;
	int16_t frames;
	int16_t index;
} STRUCT_6064550;
static STRUCT_6064550 UNK_6064550[32];

// NOTE: Looks like this points to data that controls screen brightness during
// transitions; might also be used for darkening the attract mode ranking
// backgrounds. Also, UNK_602970C is used with this, which might be where the
// darkening is handled.
STRUCT_607D218* UNK_6064750 = NULL;

static STRUCT_607D218* UNK_607CF10[192];
static STRUCT_607D218* UNK_607D210 = NULL;
static STRUCT_607D218* UNK_607D214 = NULL;
static STRUCT_607D218 UNK_607D218[192];

static STRUCT_607D218* UNK_60AD218 = NULL;
static PauseMode NextPauseMode;
static bool UNK_60AD21E = false;
static int16_t UNK_60AD220;
static int16_t NumSpriteAdders;

STRUCT_60AD228 UNK_60AD228[11];

Bg Bgs[4];

// TODO: This could be BgSetter.
typedef struct STRUCT_60B0FE0 {
	void (*set)(void*, void*, void*, void*, void*);
	void* args[5];
} STRUCT_60B0FE0;

// TODO: This could be BgSetters.
static STRUCT_60B0FE0 UNK_60B0FE0[2][20];

static int16_t UNK_60B13A0[2];
static bool UNK_60B13A4;
static int16_t UNK_60B13AE;

void UNK_6023788() {
	// Empty.
}

void UNK_602378C() {
	// Empty.
}

void UNK_6023790() {
	NextPauseMode = PAUSEMODE_NOPAUSE;

	MemSet(UNK_607D218, 0, sizeof(UNK_607D218));

	STRUCT_607D218** var0 = UNK_607CF10;
	STRUCT_607D218* var1 = UNK_607D218;
	for (size_t i = 0u; i < lengthof(UNK_607CF10); i++) {
		*var0++ = var1++;
	}

	UNK_6060068 = 0;

	UNK_607D210 = NULL;
	UNK_607D214 = NULL;
}

void UNK_60237DE() {
	CurrentPauseMode = NextPauseMode;
	// TODO: Not sure of all the data types here. UNK_300 might be an array of 16-byte-size unions.
	if (UNK_6060068 != 0) {
		UNK_60AD218 = UNK_607D210;
		do {
			UNK_60AD21E = false;
			UNK_606005C = UNK_60AD218;
			if (CurrentPauseMode < PAUSEMODE_GAME) {
				int16_t i = UNK_60AD218->UNK_18;
				for (void* var1 = &UNK_60AD218->UNK_300[4 * (15 - i) + 3]; i > 0; i--, var1 += 4u) {
					UNK_6060064 = var1 - 3;
					UNK_6060060 = var1;
					(*(void(**)())var1)();
					if (UNK_60AD21E) {
						break;
					}
				}
			}
			else if (UNK_60AD218->UNK_300[4 * 15 + 2] != UNK_6023788) {
				int16_t i = UNK_60AD218->UNK_18;
				for (void* var1 = &UNK_60AD218->UNK_300[4 * (15 - i) + 3]; i > 1; i--, var1 += 4u) {
					UNK_6060064 = var1 - 3;
					UNK_6060060 = var1;
					(*(void(**)())var1)();
					if (UNK_60AD21E) {
						break;
					}
				}
				if (!UNK_60AD21E) {
					UNK_6060060 = (void(**)())&UNK_60AD218->UNK_300[4 * 15];
					((void(*)())UNK_60AD218->UNK_300[4 * 15])();
				}
			}
			UNK_606005C = UNK_60AD218->UNK_14;
			UNK_60AD218 = UNK_606005C;
		} while (UNK_606005C != NULL);
	}
}

void UNK_602392E(uintptr_t arg0, void (*arg1)()) {
	STRUCT_607D218* temp0 = UNK_606005C;

	if (UNK_6060068 != 0) {
		UNK_606005C = UNK_607D210;
		do {
			uint16_t i = UNK_606005C->UNK_18;
			for (void** var0 = &UNK_606005C->UNK_300[4 * (15 - i) + 3]; i != 0; i--, var0 += 4) {
				if ((uintptr_t)*var0 == arg0) {
					arg1();
					break;
				}
			}
			UNK_606005C = UNK_606005C->UNK_14;
		} while (UNK_606005C != NULL);
	}

	UNK_606005C = temp0;
}

void UNK_60239B8(uintptr_t arg0, uintptr_t arg1) {
	STRUCT_607D218* temp0 = UNK_606005C;
    
    if (UNK_6060068 != 0) {
        UNK_606005C = UNK_607D210;
        do {
            uint16_t i = UNK_606005C->UNK_18;
			for (void** var0 = &UNK_606005C->UNK_300[4 * (15 - i) + 3]; i != 0; i--, var0 += 4) {
                if ((uintptr_t)*var0 == arg0) {
                    *(uintptr_t*)var0 = arg1;
                }
            }
        } while (UNK_606005C != NULL);
    }

    UNK_606005C = temp0;
}

bool UNK_6023A0E(STRUCT_607D218* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4) {
	void (**var0)();
	void (*var1)();
	if (arg0->UNK_18 == 0u) {
		var0 = arg0->UNK_300[4 * 14 + 3];
		arg0->UNK_18++;
	}
	else {
		var0 = arg0->UNK_300[4 * 14 + 3];
		size_t i;
		for (i = 0u; i < arg0->UNK_18 && var1 != UNK_602378C; i++) {
			var1 = *var0;
			if (i > 14u) {
				return false;
			}
			var0 -= 4;
		}
		if (i >= arg0->UNK_18) {
			arg0->UNK_18++;
		}
	}

	*(var0 - 0) = arg1;
	*(var0 - 1) = (void*)(uintptr_t)arg4;
	*(var0 - 2) = (void*)(uintptr_t)arg3;
	*(var0 - 3) = (void*)(uintptr_t)arg2;
	return true;
}

void UNK_6023A98(STRUCT_607D218* arg0, void (*arg1)(), void* arg2, void* arg3) {
	void** var1;
	uint8_t var0;
	if (arg0->UNK_18 == 0u) {
		var0 = 0u;
		var1 = &arg0->UNK_300[4 * 14 + 3];
		arg0->UNK_18++;
	}
	else {
		var1 = &arg0->UNK_300[4 * 14 + 3];
		size_t i = 0u;
		while (i < arg0->UNK_18 && *var1 != UNK_602378C) {
			var1 -= 4;
		}
		var0 = i;
		if (i >= arg0->UNK_18) {
			arg0->UNK_18++;
		}
	}
	*var1 = arg1;

	size_t i;
	void** var3 = var1 - 1;
	void** var4;
	for (i = 0u; i < 4u; i++) {
		if (arg0->UNK_1C[i] == 0u) {
			arg0->UNK_1C[i] = var0 + 1u;
			// TODO: This is some strange pointer math, and might have to be completely changed for the code to work.
			var4 = arg0->UNK_100[i];
			*var3 = (void*)((uintptr_t)var4 | (i << 28));
			var3 = var1 - 2;
			break;
		}
	}
	if (i < 4u) {
		for (size_t i = 0u; i < 32u; i++, var4++) {
			*var4 = NULL;
		}
		*(var3 - 0) = arg3;
		*(var3 - 1) = arg2;
	}
}

#if 0
void UNK_6023B76(STRUCT_607D218* arg0) {
	// TODO
}

void UNK_6023BC4(STRUCT_607D218* arg0, void (*arg1)()) {
	// TODO
}

void UNK_6023C3E(STRUCT_607D218* arg0, void* arg1) {
	// TODO
}

void* UNK_6023C8A(STRUCT_607D218* arg0, void* arg1) {
	// TODO
	return NULL;
}

int32_t UNK_6023CBC(void (*arg0)(), uint32_t arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7) {
	// TODO
	return 0u;
}
#endif

STRUCT_607D218* UNK_6023DAE(STRUCT_607D218* arg0) {
	if (UNK_6060068 >= lengthof(UNK_607CF10) - 1) {
		return NULL;
	}

	STRUCT_607D218* var0 = UNK_607CF10[UNK_6060068];
	MemSet(var0, 0u, sizeof(STRUCT_607D218));

	if (arg0 != NULL) {
		var0->UNK_0 = arg0;
		var0->UNK_C = arg0->UNK_4;
		if (arg0->UNK_4 != NULL) {
			arg0->UNK_4->UNK_8 = var0;
		}
		arg0->UNK_4 = var0;
	}

	if (UNK_6060068 == 0) {
		var0->UNK_10 = NULL;
		var0->UNK_14 = NULL;
		UNK_607D210 = var0;
	}
	else {
		var0->UNK_10 = UNK_607D214;
		var0->UNK_14 = NULL;
		UNK_607D214->UNK_14 = var0;
	}

	if (UNK_60AD220 == 0) {
		UNK_60AD220 = 1;
	}

	UNK_607D214 = var0;
	var0->UNK_1A = UNK_60AD220++;

	UNK_6060068++;

	return var0;
}

void UNK_6023E5A(STRUCT_607D218* arg0) {
	STRUCT_607D218* var0 = arg0->UNK_0;
	STRUCT_607D218* var1 = arg0->UNK_4;
	STRUCT_607D218* var2 = arg0->UNK_8;
	STRUCT_607D218* var3 = arg0->UNK_C;

	if (var1 == NULL) {
		if (var2 == NULL) {
			if (var0 != NULL) {
				var0->UNK_4 = var3;
			}
		}
		else {
			var2->UNK_C = var3;
		}
		if (var3 != NULL) {
			var3->UNK_8 = var2;
		}
	}
	else {
		if (var2 == NULL) {
			if (var0 != NULL) {
				var0->UNK_4 = var1;
			}
		}
		else {
			var2->UNK_C = var1;
		}
		var1->UNK_8 = var2;
		for (var1->UNK_0 = var0; var1->UNK_C != NULL; var1->UNK_0 = var0, var1 = var1->UNK_C);
		if (var3 != NULL) {
			var1->UNK_C = var3;
			var3->UNK_8 = var1;
		}
	}
	if (arg0 == UNK_607D210) {
		UNK_607D210 = arg0->UNK_14;
	}
	if (arg0 == UNK_607D214) {
		UNK_607D214 = arg0->UNK_10;
	}
	arg0->UNK_1A = 0u;
	if (arg0->UNK_10 != NULL) {
		arg0->UNK_10->UNK_14 = arg0->UNK_14;
	}
	if (arg0->UNK_14 != NULL) {
		arg0->UNK_14->UNK_10 = arg0->UNK_10;
	}
	UNK_6060068--;
	UNK_607CF10[UNK_6060068] = arg0;
}

STRUCT_607D218* UNK_6023EFE(void (**arg0)(), STRUCT_607D218* arg1) {
    STRUCT_607D218* var0 = UNK_606005C;
    if (arg1 == (STRUCT_607D218 *)~(intptr_t)0) {
        arg1 = UNK_606005C;
    }
    STRUCT_607D218* var1 = UNK_6023DAE(arg1);
    if (var1 == NULL) {
        var1 = NULL;
        var0 = UNK_606005C;
    }
    else {
		void (*fun0)();
		void (*fun1)();
		void (*fun2)();

        if (arg0[3] == NULL) {
            fun0 = UNK_6023788;
        }
        else {
            fun0 = arg0[3];
        }

        UNK_606005C = var1;
        var1->UNK_300[4 * 15 + 3] = fun0;

        if (arg0[1] == NULL) {
            fun1 = UNK_6023788;
        }
        else {
            fun1 = arg0[1];
        }

        var1->UNK_300[4 * 15 + 2] = fun1;

        if (arg0[0] == NULL) {
            fun2 = UNK_6023788;
        }
        else {
            fun2 = arg0[0];
        }

        var1->UNK_300[4 * 14 + 3] = fun2;
        var1->UNK_18 = 1;

        if (arg0[2] != NULL) {
            arg0[2]();
        }
    }
    UNK_606005C = var0;
    return var1;
}

#if 0
void* UNK_6023FA4(void (**arg0)(), STRUCT_607D218* arg1) {
	// TODO
	return NULL;
}
#endif

void UNK_6024030(STRUCT_607D218* arg0) {
	STRUCT_607D218* var0 = UNK_606005C;
	UNK_606005C = arg0;
	((void (*)())arg0->UNK_300[4 * 15 + 3])();
	UNK_6023E5A(arg0);
	if (arg0 == var0) {
		UNK_60AD21E = true;
	}
	UNK_606005C = var0;
}

void UNK_602406E() {
	if (UNK_6060068 != 0) {
		for (STRUCT_607D218* var0 = UNK_607D210, * var1; var0 != NULL; var0 = var1) {
			var1 = var0->UNK_14;
			UNK_6024030(var0);
		}
	}
}

void UNK_60240A8(void* arg0) {
	if (UNK_6060068 != 0) {
		for (STRUCT_607D218* var0 = UNK_607D210; var0 != NULL; var0 = var0->UNK_14) {
			STRUCT_607D218* var1 = var0->UNK_14;
			void** var2 = &var0->UNK_300[4 * 14 + 3];
			for (size_t i = 0u; i < var0->UNK_18; i++, var2 -= 4) {
				if (*var2 == arg0) {
					UNK_6024030(arg0);
					break;
				}
			}
		}
	}
}

void SetNextPauseMode(PauseMode nextPauseMode) {
	NextPauseMode = nextPauseMode;
}

void DisablePause() {
	NextPauseMode = PAUSEMODE_NOPAUSE;
}

PauseMode GetNextPauseMode() {
	return NextPauseMode;
}

void AllocSpriteLayerNames(int16_t layer, int16_t num) {
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
	for (size_t i = 0; i < NUMALPHAS; i++) {
		Alpha[i] = 0x00;
	}
}

void InitSpriteAdders() {
	AddSprite(1, NULL);
	InitSpriteAlpha();
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x67u;
	for (int16_t i = 0; i < lengthof(SpriteAdders); i++) {
		SpriteAdders[i].type = 0u;
		SpriteAdders[i].UNK_2C = 0x8000u;
		SpriteAdderNameTable[i] = i;
	}

	UNK_6061932.tempSprite[6] = 0x003Fu;
	UNK_6061932.UNK_12 = 0x7Du;
	UNK_6061932.UNK_14 = 0x7Fu;
	NumSpriteAdders = 0;
}

void UNK_6024244() {
	int16_t* var0 = SpriteAdderNameTable;
	for (int16_t i = 0u; i < NumSpriteAdders; i++, var0++) {
		int32_t var1 = *var0;
		AddSpriteData* var2 = &SpriteAdders[var1];
		switch (var2->type) {
		case ADDSPRITE_1:
			while (CurrentPauseMode < PAUSEMODE_BG && --var2->UNK_34 <= 0) {
				int16_t var3;
				var2->UNK_34 = var2->UNK_36;
				if (!(var2->UNK_2C & 0x800)) {
					if (var2->animFrame < var2->UNK_28 - 1) {
						var3 = var2->animFrame + 1;
					}
					else {
						if (!(var2->UNK_2C & 0x200)) {
							if (!(var2->UNK_2C & 0x400)) {
								var2->animFrame = var2->UNK_28 - 1;
								var2->type = 2u;
							}
							else {
								var2->animFrame--;
								var2->UNK_2C |= 0x800;
							}
							break;
						}
						var3 = var2->UNK_38;
					}
				}
				else {
					if (var2->UNK_38 < var2->animFrame) {
						var3 = var2->animFrame;
					}
					else {
						if (!(var2->UNK_2C & 0x200)) {
							if (!(var2->UNK_2C & 0x400)) {
								var2->animFrame = var2->UNK_38;
								var2->type = 2u;
							}
							else {
								var2->animFrame++;
								var2->UNK_2C &= ~0x0800;
							}
							break;
						}
						var3 = var2->UNK_28;
					}
					var3--;
				}
				var2->animFrame = var3;
				break;
			}

		case ADDSPRITE_2:
			if (!(var2->UNK_2C & 0x8000)) {
				if (ScreenTimeOdd) {
					if ((var2->UNK_2C & 0x20) || CurrentPauseMode >= PAUSEMODE_GAME) {
						UNK_60243E8(var2);
					}
				}
				else {
					if ((var2->UNK_2C & 0x10) || CurrentPauseMode >= PAUSEMODE_GAME) {
						UNK_60243E8(var2);
					}
				}
			}
			break;

		case ADDSPRITE_9:
			if (!(var2->UNK_2C & 0x8000)) {
				UNK_602471C(var2);
			}
			break;

		default:
			break;
		}
	}
}

void UNK_60243E8(AddSpriteData* arg0) {
	int16_t numSprites;
	const ObjectData* object;

	object = arg0->data;
	for (int16_t frame = 0; frame < arg0->animFrame; frame++) {
		object += OBJECT_GETNUMSPRITES(object);
	}
	numSprites = OBJECT_GETNUMSPRITES(object);
	assert(numSprites > 0);
	AllocSpriteLayerNames(arg0->layer, numSprites);

	if (arg0->UNK_30 != 0) {
		arg0->UNK_30--;
	}

	if (arg0->UNK_32 < 1) {
		if (arg0->UNK_32 < 0) {
			arg0->UNK_32++;
		}
	}
	else {
		arg0->UNK_32--;
	}

	bool var0 = false;
	bool var1 = false;
	if (!(arg0->UNK_2C & 0x2000u)) {
		if (arg0->UNK_2C & 0x1000u) {
			var0 = arg0->UNK_32 == 0;
			if (var0) {
				arg0->UNK_32 = 8;
			}
			arg0->UNK_2C &= ~0x1000u;
		}
		else if (arg0->UNK_32 < 0) {
			if (arg0->UNK_32 % 2 != 0) {
				var0 = true;
			}
		}
		else if (CurrentPauseMode == PAUSEMODE_NOPAUSE && (arg0->UNK_2C & 3u) != 0 && arg0->UNK_30 == 0) {
			var1 = true;
			arg0->UNK_30 = UNK_60356C8[arg0->UNK_2C & 3];
		}
	}
	object += (size_t)numSprites - 1;
	for (int16_t i = 0; i < numSprites; i++, object--) {
		if (((*object)[0] & 0x8000u) == 0 || ((ScreenTimeOdd + 1) & arg0->UNK_2F) == 0 ) {
			int16_t offsetY = OBJECT_GETY(object);
			int16_t offsetX = OBJECT_GETX(object);

			// verticalHorizontal at least has flags indicating if the object should be
			// flipped horizontally (0x0080) and/or vertically (0x8000); there
			// could be more flags in it.
			if (arg0->verticalHorizontal & 0x8000) {
				// BUG: The original did a "& 0xFu" on the height-plus-one
				// value, but that's not strictly correct; for a 16-tile sized
				// sprite, that'd result in a 0-tile flipping offset applied.
				offsetY = (OBJECT_GETH(object) + 1) * -16 - offsetY;
			}
			if (arg0->verticalHorizontal & 0x0080) {
				// BUG: The original did a "& 0xFu" on the height-plus-one
				// value, but that's not strictly correct; for a 16-tile sized
				// sprite, that'd result in a 0-tile flipping offset applied.
				offsetX = (OBJECT_GETW(object) + 1) * -16 - offsetX;
			}
			OBJECT_SETY(&UNK_6061932.tempSprite, arg0->y + offsetY);
			OBJECT_SETX(&UNK_6061932.tempSprite, arg0->x + offsetX);

			uint8_t sprPriVertical = (*object)[2] >> 8;
			uint8_t horizontal = (*object)[3] >> 8;
			uint8_t bgPri = (arg0->flipXBgPri << 4) & 0x30u;
			uint8_t bgPriHorizontal = (bgPri | horizontal) & 0xCFu;

			if (arg0->verticalHorizontal & 0x8000) {
				if (sprPriVertical & 0x80) {
					sprPriVertical &= 0x7f;
				}
				else {
					sprPriVertical |= 0x80;
				}
			}
			UNK_6061932.tempSprite[2] = (((uint16_t)sprPriVertical) << 8) | arg0->scaleY;
			if (arg0->verticalHorizontal & 0x80) {
				if (horizontal & 0x80) {
					bgPriHorizontal &= 0x4Fu;
				}
				else {
					bgPriHorizontal |= 0x80u;
				}
			}
			UNK_6061932.tempSprite[3] = (((uint16_t)bgPriHorizontal) << 8) | arg0->scaleX;
			if (var0) {
				if (OBJECT_GETSPRPRI(object) & 2) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, 16u);
				}
				else if (arg0->palNum == 0) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
				}
				else {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
				}
			}
			else if (var1) {
				if (OBJECT_GETSPRPRI(object) & 2) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, 32u);
				}
				else {
					if (arg0->palNum == 0) {
						OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
					}
					else {
						OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
					}
				}
			}
			else if (arg0->palNum == 0) {
				OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
			}
			else {
				OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
			}
			UNK_6061932.tempSprite[4] = (UNK_6061932.tempSprite[4] & 0xFF00u) | ((*object)[4] & 0x8Fu) | (arg0->bppAlphaTileTop & 0x70u);
			UNK_6061932.tempSprite[5] = (*object)[5];
			SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
			NumSprites++;
		}
	}
}

void UNK_602471C(AddSpriteData* arg0) {
	const ObjectData* object = arg0->data;
	for (int16_t frame = 0; frame < arg0->animFrame; frame++) {
		object += OBJECT_GETNUMSPRITES(object);
	}
	int16_t numSprites = OBJECT_GETNUMSPRITES(object);
	AllocSpriteLayerNames(arg0->layer, numSprites);

	if (arg0->UNK_30 != 0) {
		arg0->UNK_30--;
	}
	if (arg0->UNK_32 < 1) {
		if (arg0->UNK_32 < 0) {
			arg0->UNK_32++;
		}
	}
	else {
		arg0->UNK_32--;
	}

	bool var0 = false;
	bool var1 = false;
	if (!(arg0->UNK_2C & 0x2000u)) {
		if (arg0->UNK_2C & 0x1000u) {
			var0 = arg0->UNK_32 == 0;
			if (var0) {
				arg0->UNK_32 = 8;
			}
			arg0->UNK_2C &= ~0x1000u;
		}
		else if (arg0->UNK_32 < 0) {
			if (arg0->UNK_32 % 2) {
				var0 = true;
			}
		}
		else if (CurrentPauseMode == PAUSEMODE_NOPAUSE && (arg0->UNK_2C & 3) && arg0->UNK_30 == 0) {
			var1 = true;
			arg0->UNK_30 = UNK_60356C8[arg0->UNK_2C & 3];
		}
	}

	object += (size_t)numSprites - 1;
	for (int16_t i = 0; i < numSprites; i++, object--) {
		if (((*object)[0] & 0x8000u) == 0u || ((ScreenTimeOdd + 1) & arg0->UNK_2F) == 0u) {
			int16_t offsetY = OBJECT_GETY(object);
			int16_t offsetX = OBJECT_GETX(object);
			if (arg0->verticalHorizontal & 0x8000u) {
				// BUG: The original did a "& 0xFu" on the height-plus-one
				// value, but that's not strictly correct; for a 16-tile sized
				// sprite, that'd result in a 0-tile flipping offset applied.
				offsetY = (OBJECT_GETH(object) + 1) * -16 - offsetY;
			}
			if (arg0->verticalHorizontal & 0x0080u) {
				// BUG: The original did a "& 0xFu" on the height-plus-one
				// value, but that's not strictly correct; for a 16-tile sized
				// sprite, that'd result in a 0-tile flipping offset applied.
				offsetX = (OBJECT_GETW(object) + 1) * -16 - offsetX;
			}
			OBJECT_SETY(&UNK_6061932.tempSprite, arg0->y + offsetY);
			OBJECT_SETX(&UNK_6061932.tempSprite, arg0->x + offsetX);
			uint8_t sprPriVertical = (*object)[2] >> 8;
			uint8_t horizontal = (*object)[3] >> 8;
			uint8_t bgPri = (arg0->flipXBgPri << 4) & 0x30u;
			uint8_t bgPriHorizontal = (bgPri | horizontal) & 0xCFu;
			if (arg0->verticalHorizontal & 0x8000u) {
				if (sprPriVertical & 0x80u) {
					sprPriVertical &= ~0x80u;
				}
				else {
					sprPriVertical |= 0x80u;
				}
			}
			UNK_6061932.tempSprite[2] = ((uint16_t)sprPriVertical << 8) | arg0->scaleY;
			if (arg0->verticalHorizontal & 0x0080u) {
				if (horizontal & 0x80u) {
					bgPriHorizontal &= 0x4Fu;
				}
				else {
					bgPriHorizontal |= 0x80u;
				}
			}
			UNK_6061932.tempSprite[3] = ((uint16_t)bgPriHorizontal << 8) | arg0->scaleX;
			if (var0) {
				if (OBJECT_GETSPRPRI(object) & 0x2u) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, 16u);
				}
				else if (arg0->palNum == 0) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
				}
				else {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
				}
			}
			else if (var1) {
				if (OBJECT_GETSPRPRI(object) & 2u) {
					OBJECT_SETPALNUM(&UNK_6061932.tempSprite, 32u);
				}
				else {
					if (arg0->palNum == 0u) {
						OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
					}
					else {
						OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
					}
				}
			}
			else if (arg0->palNum == 0) {
				OBJECT_SETPALNUM(&UNK_6061932.tempSprite, OBJECT_GETPALNUM(object));
			}
			else {
				OBJECT_SETPALNUM(&UNK_6061932.tempSprite, arg0->palNum);
			}
			UNK_6061932.tempSprite[4] = ((uint16_t)UNK_6061932.tempSprite[4] & 0xFF00u) | ((*object)[4] & 0x008Fu) | (arg0->bppAlphaTileTop & 0x70u);
			UNK_6061932.tempSprite[5] = (*object)[5];
			SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
			NumSprites++;
		}
	}
}

void WriteSpriteLayers() {
	// Convert from layers to a linear name sequence.
	int16_t nameIndex = SPRITE_FIRST;
    SpriteNames[0] = 0u;
    SpriteNames[1] = 1u;
    int16_t* currentLayer = SpriteLayers;
    for (uint16_t layer = 0u; layer < lengthof(SpriteLayers); layer++) {
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
    for (uint16_t layer = 0u; layer < lengthof(SpriteLayers); layer++) {
		SpriteLayers[layer] = SPRITELAYER_FREE;
	}
}

int16_t AllocSpriteAdder() {
	int16_t i = SpriteAdderNameTable[NumSpriteAdders];
	NumSpriteAdders++;
	SpriteAdders[i].flipXBgPri = 1u;
    SpriteAdders[i].verticalHorizontal = 0u;
    SpriteAdders[i].scaleX = UNSCALED;
    SpriteAdders[i].scaleY = UNSCALED;
    SpriteAdders[i].palNum = 0u;
    SpriteAdders[i].bppAlphaTileTop = 0u;
    SpriteAdders[i].layer = 16u;
    SpriteAdders[i].type = 1u;
    SpriteAdders[i].UNK_2C = 0xA030u;
    SpriteAdders[i].UNK_2E = 0u;
    SpriteAdders[i].UNK_30 = 0;
    SpriteAdders[i].UNK_32 = 0;
    SpriteAdders[i].UNK_34 = 0;
    SpriteAdders[i].animFrame = 0;
    SpriteAdders[i].UNK_36 = 0;
    SpriteAdders[i].UNK_28 = 0;
    return i;
}

void FreeSpriteAdder(int16_t arg0) {
	if ((-1 < arg0) && (arg0 < 0x40)) {
		SpriteAdders[arg0].type = ADDSPRITE_0;
		SpriteAdders[arg0].UNK_2C = 0x8000u;
		int32_t var1 = 0;
		int16_t var0 = SpriteAdderNameTable[0];
		while (var0 != arg0) {
			var1 += 1;
			var0 = SpriteAdderNameTable[var1];
		}
		NumSpriteAdders += -1;
		if (var1 != NumSpriteAdders) {
			int16_t* var2 = &SpriteAdderNameTable[var1];
			// Swap *i and SpriteAdderNameTable[NumSpriteAdders].
			SpriteAdderNameTable[NumSpriteAdders] ^= *var2;
			*var2 ^= SpriteAdderNameTable[NumSpriteAdders];
			SpriteAdderNameTable[NumSpriteAdders] ^= *var2;
		}
	}
	return;
}

void UNK_6024C3C(int16_t spriteAdderName, int16_t y, int16_t x, const ObjectData* objectTable) {
	if (spriteAdderName >= 0 && spriteAdderName < lengthof(SpriteAdders)) {
		SpriteAdders[spriteAdderName].data = objectTable;
		SpriteAdders[spriteAdderName].y = y;
		SpriteAdders[spriteAdderName].x = x;
		if (SpriteAdders[spriteAdderName].type == ADDSPRITE_2) {
			SpriteAdders[spriteAdderName].type = ADDSPRITE_2;
		}
		else {
			SpriteAdders[spriteAdderName].type = ADDSPRITE_9;
		}
		SpriteAdders[spriteAdderName].animFrame = 0;
	}
}

// TODO: Could be InitBgIndex().
void UNK_6024E5C(int16_t i) {
	Bgs[i].UNK_2 = 0;
	Bgs[i].UNK_4 = 1;
	Bgs[i].UNK_6 = 0;
	Bgs[i].UNK_A = 3;
	Bgs[i].UNK_C = 0;
	Bgs[i].darkness = 0;
	Bgs[i].UNK_10 = 0;
	Bgs[i].UNK_14 = 0;
	Bgs[i].UNK_30 = 0;
	Bgs[i].UNK_34 = 0;
	Bgs[i].UNK_8 = 0;
	Bgs[i].UNK_40 = 0;
	Bgs[i].UNK_44 = 0;
	Bgs[i].UNK_4C = 0;

	for (size_t k = 0u; k < lengthoffield(Bg, UNK_18); k++) {
		Bgs[i].UNK_18[k] = -1;
	}

	for (size_t k = 0u; k < lengthoffield(Bg, UNK_38); k++) {
		Bgs[i].UNK_38[k] = 0;
	}
}

void UNK_6024ED8() {
	BgMapBank[0] = 10u;
	BgMapBank[1] = 12u;
	BgMapBank[2] = 14u;
	BgMapBank[3] = 16u;

	BgMapSetting[0] = 0u;
	BgMapSetting[1] = 0u;

	UNK_2405FFEB |= 0x40u;

	UNK_60B13A4 = false;

	for (size_t i = 0u; i < lengthof(Bgs); i++) {
		Bgs[i].UNK_0 = 0;
		UNK_6024E5C((int16_t)i);
	}

	for (size_t i = 0u; i < lengthof(UNK_60AD228); i++) {
		UNK_60AD228[i].UNK_0 = 0u;
		for (size_t bank = 0u; bank < 2u; bank++) {
			// TODO: Create a RAM macro here, once this is better understood.
			UNK_60AD228[i].UNK_4[bank] = &GRAPHICSRAM[(0x5000u + i * 0x1000u + bank * 0x800u) / sizeof(uint32_t)];
			UNK_60AD228[i].UNK_C[bank] = (uint16_t)(bank + i * 2 + 10u);
		}
		UNK_60AD228[i].UNK_10 = NULL;
		UNK_60AD228[i].UNK_14 = 0;
		UNK_60AD228[i].UNK_60 = NULL;
		UNK_60AD228[i].UNK_64 = 0;
	}

	for (size_t i = 0u; i < lengthof(UNK_60B0FE0); i++) {
		for (size_t k = 0u; k < lengthof(*UNK_60B0FE0); k++) {
			MemSet(&UNK_60B0FE0[i][k], 0, sizeof(STRUCT_60B0FE0));
		}
	}
}

void UNK_6025078() {
	if (CurrentPauseMode != PAUSEMODE_NOPAUSE) {
		return;
	}

	STRUCT_60B0FE0* var0 = UNK_60B0FE0[!UNK_60B13A4];
	for (size_t i = 0u; i < lengthof(*UNK_60B0FE0); i++) {
		if (var0[i].set != NULL) {
			var0[i].set(
				var0[i].args[0],
				var0[i].args[1],
				var0[i].args[2],
				var0[i].args[3],
				var0[i].args[4]);
			var0[i].set = NULL;
		}
	}
	UNK_60B13A0[!UNK_60B13A4] = 0;
}

void UNK_60251B8(int16_t bgIndex, uint32_t* arg1, uint32_t* arg2) {
	for (size_t i = 0; i < 0xE0u; i += 8u) {
		for (size_t j = 0; j < 8u; j++) {
			arg1[i + j] = Bgs[bgIndex].UNK_50[i + j + 0x00u];
		}
		for (size_t j = 0; j < 8u; j++) {
			arg2[i + j] = Bgs[bgIndex].UNK_50[i + j + 0xE0u];
		}
	}
}

void UNK_602523C() {
	if (CurrentPauseMode == PAUSEMODE_NOPAUSE) {
		UNK_60B13A4 = !UNK_60B13A4;
		VideoSetters[NumVideoSetters++].set = UNK_602526A;
	}
}

void UNK_602526A(void* unused0, void* unused1, void* unused2) {
	uint8_t bgMapBank0, bgMapBank1, bgMapBank2, bgMapBank3;
	uint8_t bgMapSettings01 = 0u, bgMapSettings23 = 0u;

#define BGSET(bgIndex, tilemapBank, tilemapSettings) \
	do { \
		if (Bgs[bgIndex].UNK_0 == 1 && Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6] >= 0) { \
			const int16_t spriteAdderName = Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6]; \
			const int16_t palNum = UNK_60AD228[spriteAdderName].UNK_54; \
			const int16_t i = Bgs[bgIndex].UNK_8; \
			uint16_t tilemapBankIndex; \
			if (UNK_60AD228[spriteAdderName].UNK_56 == 0) { \
				tilemapBankIndex = (palNum + 1) % 2; \
			} \
			else { \
				UNK_60AD228[spriteAdderName].UNK_54 = (palNum + 1) % 2; \
				UNK_60AD228[spriteAdderName].UNK_56 = 0; \
				tilemapBankIndex = palNum; \
			} \
			tilemapBank = (UNK_60AD228[i].UNK_C[tilemapBankIndex] >> 8) & 0xFFu; \
			if (Bgs[bgIndex].UNK_10 == 0) { \
				UNK_60AD228[i].UNK_4[tilemapBankIndex][0xFCu + bgIndex] = \
					(UNK_60AD228[spriteAdderName].UNK_18[palNum] << 16) | \
					(UNK_60AD228[spriteAdderName].UNK_20[palNum] & 0x1FFu); \
				UNK_60AD228[i].UNK_4[tilemapBankIndex][0x1FCu + bgIndex] = \
					((uint32_t)Bgs[bgIndex].UNK_A << 24) | \
					UNK_60AD228[spriteAdderName].UNK_C[tilemapBankIndex] | \
					(Bgs[bgIndex].darkness << 8) | \
					(Bgs[bgIndex].UNK_C << 15); \
			} \
			else { \
				tilemapBank |= 0x80u; \
				uint32_t* var3 = UNK_60AD228[i].UNK_4[tilemapBankIndex]; \
				UNK_60251B8(1, var3, var3 + 0x100u); \
			} \
			if (!(UNK_60AD228[spriteAdderName].UNK_10->UNK_0->header.tileInfo & BGMAPTILEINFO_BIT21)) { \
				tilemapSettings |= 0x40u >> ((bgIndex % 2u) * 4u); \
			} \
			if (Bgs[bgIndex].UNK_2 != 0) { \
				tilemapSettings |= 0x80u >> ((bgIndex % 2u) * 4u); \
			} \
		} \
		else { \
			tilemapBank = 10u; \
		} \
	} while (false)

	BGSET(0u, bgMapBank0, bgMapSettings01);
	BGSET(1u, bgMapBank1, bgMapSettings01);
	BGSET(2u, bgMapBank2, bgMapSettings23);
	BGSET(3u, bgMapBank3, bgMapSettings23);

#undef BGSET

	BgMapBank[0] = bgMapBank0;
	BgMapBank[1] = bgMapBank1;
	BgMapBank[2] = bgMapBank2;
	BgMapBank[3] = bgMapBank3;
	BgMapSetting[0] = bgMapSettings01;
	BgMapSetting[1] = bgMapSettings23;
}

// TODO: Could be AllocBg(); returns the background index allocated or -1 on
// failure.
int32_t UNK_60257EE() {
	int32_t i;

	for (i = 0; i < lengthof(Bgs); i++) {
		if (Bgs[i].UNK_0 == 0) {
			break;
		}
	}

	if (i < 4) {
		UNK_6024E5C(i);
		Bgs[i].UNK_0 = 1;
	}
	else {
		i = -1;
	}

	return i;
}

int32_t UNK_6025918() {
	int32_t bgIndex;

	for (bgIndex = 1; bgIndex < 11; bgIndex++) {
		if (UNK_60AD228[bgIndex].UNK_0 == 0u) {
			break;
		}
	}

	if (bgIndex < 11) {
		UNK_60AD228[bgIndex].UNK_0 = 1u;
		UNK_60AD228[bgIndex].UNK_10 = NULL;
		UNK_60AD228[bgIndex].UNK_60 = NULL;
		UNK_60AD228[bgIndex].UNK_54 = 0;
		UNK_60AD228[bgIndex].UNK_56 = 0;
		UNK_60AD228[bgIndex].UNK_4C = 0u;
		UNK_60AD228[bgIndex].UNK_50 = 0;
		UNK_60AD228[bgIndex].UNK_58 = 0;

		for (int16_t i = 0; i < 2; i++) {
			UNK_60AD228[bgIndex].UNK_18[i] = 0;
			UNK_60AD228[bgIndex].UNK_20[i] = 0;
			UNK_60AD228[bgIndex].UNK_28[i] = 0;
			UNK_60AD228[bgIndex].UNK_30[i] = 0;
			UNK_60AD228[bgIndex].UNK_34[i] = 0;
			UNK_60AD228[bgIndex].UNK_38[i] = 0;
			UNK_60AD228[bgIndex].UNK_3C[i] = 0;
			UNK_60AD228[bgIndex].UNK_40[i] = 0;
			UNK_60AD228[bgIndex].UNK_46[i] = 0;
		}
	}
	else {
		bgIndex = -1;
	}

	return bgIndex;
}

int32_t UNK_6025AE4(int16_t bgIndex, GameBg* gameBg) {
	int16_t var0;
	if (Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6] < 0) {
		var0 = UNK_6025918();
		Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6] = var0;
	}
	else {
		var0 = Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6];
	}
	UNK_60AD228[var0].UNK_10 = gameBg->UNK_0;
	UNK_60AD228[bgIndex].UNK_14 = 0;
	
	STRUCT_GameBg_0* var1 = gameBg->UNK_0;
	for (const BgMap* bgMap = var1->UNK_0; bgMap != NULL; var1++, bgMap = var1->UNK_0) {
		UNK_60AD228[var0].UNK_14 += var1->UNK_0->header.UNK_6 * var1->UNK_4;
	}

	UNK_60AD228[var0].UNK_4C = 0;
	UNK_60AD228[var0].UNK_50 = 0;

	return var0;
}

void UNK_6025B9A(int16_t bgIndex, GameBg* gameBg, int32_t arg2, int32_t arg3) {
	int32_t var0 = UNK_6025AE4(bgIndex, gameBg);
	UNK_60AD228[var0].UNK_4C += arg2;
	UNK_60AD228[var0].UNK_50 += arg3;
}

void UNK_60267E4(int16_t bgIndex) {
	const int16_t var0 = Bgs[bgIndex].UNK_6;
	const int16_t var1 = Bgs[bgIndex].UNK_18[var0];
	if (var1 >= 0) {
		UNK_6026870(bgIndex, var0, UNK_60AD228[var1].UNK_54);
	}
}

void UNK_602682A(int16_t arg0, int16_t arg1, int16_t arg2, bool arg3) {
	if (arg3) {
		UNK_60B13AE = 1;
		UNK_6026870(arg0, arg1, arg2);
		UNK_60B13AE = 0;
	}
	else {
		UNK_6026870(arg0, arg1, arg2);
	}
}

void UNK_6026870(int16_t bgIndex, int16_t arg1, int16_t arg2) {
	const int16_t var1 = Bgs[bgIndex].UNK_18[arg1];
	if (var1 != -1) {
		UNK_60AD228[var1].UNK_56 = 1;
		const int32_t var2 = (-UNK_60AD228[var1].UNK_18[arg2] - 16) / 16;
		const int32_t var3 = (-UNK_60AD228[var1].UNK_20[arg2] - 16) / 16;
		const int32_t var5 =
			(-UNK_60AD228[var1].UNK_18[arg2] - 16) / 16 - UNK_60AD228[var1].UNK_4C / 16;
		int32_t var6 =
			(-UNK_60AD228[var1].UNK_28[arg2] - 16) / 16 - UNK_60AD228[var1].UNK_50 / 16;

		arg1 = 1;
		if (arg1) {
			int16_t var0 = 0;
			int16_t var4 = 0;
			for (int32_t i = 0; i < 32; i++, var6++) {
				uint32_t* var7 = &UNK_60AD228[var1].UNK_4[arg2][(i + var3) & 0x1Fu];
				const STRUCT_GameBg_0* var8 = UNK_6026AAC(bgIndex, var6, &var4, &var0);
				if (var8->UNK_0->header.tileInfo & BGMAPTILEINFO_PERTILEPAL) {
					const BgMap32* const bgMap32 = (const BgMap32* const)var8->UNK_0;
					int32_t var10 = var5;
					int32_t var11 = var2;
					for (int32_t j = 0; j < 16; j++, var10++, var11++) {
						int32_t var12 = var10 % var8->UNK_0->header.UNK_4;
						if (var12 < 0) {
							var12 += var8->UNK_0->header.UNK_4;
						}
						var7[(var11 & 0xFu) * 32] = bgMap32->names[var4 * bgMap32->header.UNK_4 + var12];
					}
				}
				else {
					const BgMap16* const bgMap16 = (const BgMap16* const)var8->UNK_0;
					int32_t var10 = var5;
					int32_t var11 = var2;
					for (int32_t j = 0; j < 16; j++, var10++, var11++) {
						int32_t var12 = var10 % var8->UNK_0->header.UNK_4;
						if (var12 < 0) {
							var12 += var8->UNK_0->header.UNK_4;
						}
						var7[(var11 & 0xFu) * 32] = var8->UNK_0->header.tileInfo | bgMap16->names[var4 * bgMap16->header.UNK_4 + var12];
					}
				}
			}
		}

		UNK_60AD228[var1].UNK_38[arg2] %= 16;
		UNK_60AD228[var1].UNK_3C[arg2] %= 16;
	}
}

STRUCT_GameBg_0* UNK_6026AAC(int16_t bgIndex, int16_t arg1, int16_t* arg2, int16_t* arg3) {
	const int16_t var0 = Bgs[bgIndex].UNK_18[Bgs[bgIndex].UNK_6];
	STRUCT_GameBg_0* struct0;
	if (var0 < 0) {
		struct0 = NULL;
	}
	else {
		assert(var0 >= 0 && var0 < lengthof(UNK_60AD228));
		struct0 = UNK_60AD228[var0].UNK_10;
		if (struct0->UNK_0 != NULL) {
			*arg2 = arg1;
			STRUCT_GameBg_0* var1 = struct0;
			while (*arg2 < 0) {
				if (UNK_60AD228[var0].UNK_60 == NULL) {
					*arg2 += UNK_60AD228[var0].UNK_14;
				}
				else {
					*arg2 += UNK_60AD228[var0].UNK_64;
					struct0 = UNK_60AD228[var0].UNK_60;
					var1 = struct0;
				}
			}
			while (*arg2 >= (int16_t)struct0->UNK_0->header.UNK_6 * struct0->UNK_4) {
				*arg2 -= struct0->UNK_0->header.UNK_6 * struct0->UNK_4;
				struct0++;
				if (struct0->UNK_0 == NULL) {
					struct0 = var1;
					if (UNK_60AD228[var0].UNK_60 != NULL) {
						struct0 = UNK_60AD228[var0].UNK_60;
					}
				}
			}
			*arg3 = (int16_t)struct0->UNK_0->header.UNK_6 - *arg2 - 1;
			*arg2 %= (int16_t)struct0->UNK_0->header.UNK_6;
		}
	}
	return struct0;
}

void UNK_6026FCA(int16_t bgIndex, int16_t arg1) {
	Bgs[bgIndex].UNK_2 = arg1;
}

void UNK_6026FDC(int16_t arg0, int16_t arg1) {
	Bgs[arg0].UNK_A = arg1;
}

void SetBgDarkness(int16_t bgIndex, int16_t darkness) {
	Bgs[bgIndex].darkness = darkness;
}

void SetScanlinesBank(uint8_t bankNum) {
	VideoSetters[NumVideoSetters++].set = VideoSetScanlinesBank;
}

void VideoSetScanlinesBank(void* unused0, void* unused1, void* unused2) {
	ScanlinesBank[0] = 8u;
}

void UNK_6029498(int16_t arg0) {
	VideoSetters[NumVideoSetters].set = UNK_60294C0;
	VideoSetters[NumVideoSetters++].args[0] = (void*)(uintptr_t)arg0;
}

void UNK_60294C0(void* arg0, void* unused1, void* unused2) {
	// TODO: Make a define for register 0x0B.
	VIDEOREGS[0x0B] = (VIDEOREGS[0x0B] & 0xF8u) | ((uintptr_t)arg0 & 0x07u);
}

void SetBackdropColor(Color color) {
	VideoSetters[NumVideoSetters].set = VideoSetBackdropColor;
	VideoSetters[NumVideoSetters++].args[0] = (void*)(uintptr_t)color;
}

void VideoSetBackdropColor(void* color, void* unused1, void* unused2) {
	RAMDATA Color* backdropLineColor = BACKDROPRAM;
	const Color colorLocal = (Color)(uintptr_t)color;
	for (size_t i = 0; i < NUMBACKDROPLINES; i += 16u, backdropLineColor += 16u) {
		backdropLineColor[0] = colorLocal;
		backdropLineColor[1] = colorLocal;
		backdropLineColor[2] = colorLocal;
		backdropLineColor[3] = colorLocal;
		backdropLineColor[4] = colorLocal;
		backdropLineColor[5] = colorLocal;
		backdropLineColor[6] = colorLocal;
		backdropLineColor[7] = colorLocal;
		backdropLineColor[8] = colorLocal;
		backdropLineColor[9] = colorLocal;
		backdropLineColor[10] = colorLocal;
		backdropLineColor[11] = colorLocal;
		backdropLineColor[12] = colorLocal;
		backdropLineColor[13] = colorLocal;
		backdropLineColor[14] = colorLocal;
		backdropLineColor[15] = colorLocal;
	}
}

static void UNK_602970C();
static void UNK_602975E();
static void (* UNK_60356D0[4])() = {
	UNK_602970C,
	UNK_602970C,
	NULL,
	UNK_602975E
};

void UNK_6029546(int16_t arg0, int16_t arg1, int16_t arg2, int16_t arg3) {
	if (arg0 != 4) {
		if (UNK_6064750 == NULL) {
			UNK_6064750 = UNK_6023EFE(UNK_60356D0, NULL);
		}
		if (arg3 & 0x8000) {
			UNK_6064750->UNK_30 = 1u;
		}
		else {
			UNK_6064750->UNK_30 = 0u;
		}
		UNK_6064750->UNK_2C = F32(0, 0x0000u);
		F32I(UNK_6064750->UNK_2C) = 0x80;
		UNK_6064750->UNK_2C /= arg1;
		VideoSetters[NumVideoSetters++].set = VideoSetScanlinesBank;
		if ((arg3 & 7) != 0) {
			UNK_6029498(arg3 & 7);
		}
		UNK_6064750->UNK_1A = arg0;
		if (arg0 == 0) {
			UNK_6064750->backdropColor = 0xFFu;
			if (arg2 == 0) {
				UNK_6064750->UNK_24 = 0x80;
			}
			else {
				UNK_6064750->UNK_24 = arg2;
			}
			UNK_6064750->UNK_2C = -UNK_6064750->UNK_2C;
		}
		else {
			if (arg0 == 1) {
				UNK_6064750->backdropColor = ~0u;
				if (arg2 == 0) {
					UNK_6064750->UNK_24 = 0x80;
				}
				else {
					UNK_6064750->UNK_24 = arg2;
				}
				UNK_6064750->UNK_2C = -UNK_6064750->UNK_2C;
			}
			else {
				if (arg0 == 2) {
					UNK_6064750->backdropColor = COLOR(0, 0, 0, 0);
					if (arg2 == 0) {
						UNK_6064750->UNK_24 = 0;
					}
					else {
						UNK_6064750->UNK_24 = arg2;
					}
				}
				else {
					if (arg0 == 3) {
						UNK_6064750->backdropColor = COLOR(0xFF, 0xFF, 0xFF, 0x00);
						if (arg2 == 0) {
							UNK_6064750->UNK_24 = 0;
						}
						else {
							UNK_6064750->UNK_24 = arg2;
						}
					}
				}
			}
		}
		UNK_6064750->UNK_28 = F32(0, 0x0000u);
		F32I(UNK_6064750->UNK_28) = UNK_6064750->UNK_24;
		SetBackdropColor(UNK_6064750->backdropColor);
	}
	else if (UNK_6064750 != NULL) {
		UNK_6024030(UNK_6064750);
		UNK_6064750 = NULL;
	}
}

static void UNK_602970C() {
	UNK_6064750->UNK_28 += UNK_6064750->UNK_2C;
	if (UNK_6064750->UNK_2C <= F32(0, 0x0000u)) {
		if (F32I(UNK_6064750->UNK_28) <= 0) {
			UNK_6024030(UNK_606005C);
			return;
		}
	}
	else if (F32I(UNK_6064750->UNK_28) >= 128) {
		UNK_6024030(UNK_606005C);
		return;
	}
	else {
		UNK_6064750->backdropColor = (UNK_6064750->backdropColor & ~0xFF) | F32I(UNK_6064750->UNK_28);
		SetBackdropColor(UNK_6064750->backdropColor);
		return;
	}
}

static void UNK_602975E() {
	if (F32I(UNK_6064750->UNK_2C) <= 0) {
		if (UNK_6064750->UNK_30 == 0u) {
			UNK_6029498(0);
			UNK_6064750->backdropColor = COLOR(0x11, 0x11, 0x11, 0xFF);
		}
		else {
			UNK_6064750->backdropColor &= COLOR(0xFF, 0xFF, 0xFF, 0x00);
		}
	}
	else {
		UNK_6064750->backdropColor |= COLOR(0x00, 0x00, 0x00, 0xFF);
	}
	SetBackdropColor(UNK_6064750->backdropColor);
	UNK_6064750 = NULL;
}

void SetVideo() {
	NumVideoSetters = NumVideoSetters < MAXVIDEOSETTERS ? NumVideoSetters : MAXVIDEOSETTERS;

	VideoSetter* setter = VideoSetters;
	for (uint16_t i = 0u; i < NumVideoSetters; i++) {
		setter->set(setter->args[0], setter->args[1], setter->args[2]);
	}

	NumVideoSetters = 0u;
}

void InitVideoSetters() {
	NumVideoSetters = 0u;
}

// TODO: Appears to fill background RAM.
void UNK_6029814(uint32_t subtractedData, uint32_t addedData, uint8_t start, uint8_t end) {
	// Check if the start and size need to be swapped.
	if (start > end) {
		// Swap start and end.
		end ^= start;
		start ^= end;
		end ^= start;

		// Swap subtractedData and addedData.
		addedData ^= subtractedData;
		subtractedData ^= addedData;
		addedData ^= subtractedData;
	}
	uint32_t size = end - start;

	// Write background data.
	uint32_t num3 = 0;
	uint32_t num2 = 0;
	uint32_t num1 = 0;
	for (RAMDATA uint32_t* bgData = &BGRAM[start]; start < end; start++, bgData++) {
		// Bits [31, 24].
		num3 += ((addedData >> 24) & 0xFFu) - ((subtractedData >> 24) & 0xFFu);
		uint32_t offset3 = num3 / size;

		// Bits [23, 16].
		num2 += ((addedData >> 16) & 0xFFu) - ((subtractedData >> 16) & 0xFFu);
		uint32_t offset2 = num2 / size;

		// Bits [15, 8].
		num1 += ((addedData >>  8) & 0xFFu) - ((subtractedData >>  8) & 0xFFu);
		uint32_t offset1 = num1 / size;

		// Bits [7, 0] are set to zero.
		*bgData =
			(((((subtractedData >>  8) & 0xFFu) + offset1) <<  8) & 0x0000FF00u) |
			(((((subtractedData >> 16) & 0xFFu) + offset2) << 16) & 0x00FF0000u) |
			(((((subtractedData >> 24) & 0xFFu) + offset3) << 24) & 0xFF000000u);
	}
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
				if (cycle->stride >= 0) {
					for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
						Color color = COLOR(0, 0, 0, 0);

						cycle->palFixed[j].r -= cycle->palFixedV[j].r;
						COLOR_SETR(color, F16I(cycle->palFixed[j].r) & 0xFC);

						cycle->palFixed[j].g -= cycle->palFixedV[j].g;
						COLOR_SETG(color, F16I(cycle->palFixed[j].g) & 0xFC);

						cycle->palFixed[j].b -= cycle->palFixedV[j].b;
						COLOR_SETB(color, F16I(cycle->palFixed[j].b) & 0xFC);

						tempPal[j] = color;
					}
				}
				else {
					for (uint16_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
						Color color = COLOR(0, 0, 0, 0);

						cycle->palFixed[j].r += cycle->palFixedV[j].r;
						COLOR_SETR(color, F16I(cycle->palFixed[j].r) & 0xFC);

						cycle->palFixed[j].g += cycle->palFixedV[j].g;
						COLOR_SETG(color, F16I(cycle->palFixed[j].g) & 0xFC);

						cycle->palFixed[j].b += cycle->palFixedV[j].b;
						COLOR_SETB(color, F16I(cycle->palFixed[j].b) & 0xFC);

						tempPal[j] = color;
					}
				}

				for (size_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
					PALRAM[(size_t)cycle->palNum * NUMPALCOLORS_4BPP + j] = tempPal[j];
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

void NewPalCycle(uint8_t palNum, const Color* pal0, const Color* pal1, int16_t perPalDelay, PalCycleType type, int8_t stride, uint8_t endStep) {
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
	// the SH-2 code. So a safeguard has been addedData here. TAP only ever
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
			cycle->palFixed[i].c = F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00); \
		} \
		else { \
			cycle->palFixed[i].c = F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00); \
		} \
		cycle->palFixedV[i].c = stride * (F16(COLOR_GET##C(cycle->pal0[i]) & 0xFC, 0x00) - F16(COLOR_GET##C(cycle->pal1[i]) & 0xFC, 0x00))

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

static void VideoSetPal(void* palNumArg, void* numPalsArg, void* palArg);

void SetPal(const uint8_t palNum, const uint8_t numPals, const Color* pal) {
	VideoSetters[NumVideoSetters].set = VideoSetPal;
	VideoSetters[NumVideoSetters].args[0] = (void*)(uintptr_t)palNum;
	VideoSetters[NumVideoSetters].args[1] = (void*)(uintptr_t)numPals;
	VideoSetters[NumVideoSetters++].args[2] = (void*)pal;
}
static void VideoSetPal(void* palNumArg, void* numPalsArg, void* palArg) {
	const uint8_t palNum = (uint8_t)(uintptr_t)palNumArg;
	const uint8_t numPals = (uint8_t)(uintptr_t)numPalsArg;
	const Color* const pal = (const Color*)palArg;
	for (size_t i = 0u; i < numPals; i++) {
		for (size_t c = 0u; c < NUMPALCOLORS_4BPP; c++) {
			PALRAM[(palNum + i) * NUMPALCOLORS_4BPP + c] = pal[i * NUMPALCOLORS_4BPP + c];
		}
	}
}

static void VideoSetPalReverse(void*, void*, void*);

void PalSetReverse(const uint8_t palNum, const uint8_t numPals, const Color* pal) {
	VideoSetters[NumVideoSetters].set = VideoSetPalReverse;
	VideoSetters[NumVideoSetters].args[0] = (void*)(uintptr_t)palNum;
	VideoSetters[NumVideoSetters].args[1] = (void*)(uintptr_t)numPals;
	VideoSetters[NumVideoSetters++].args[2] = (void*)pal;
}

static void VideoSetPalReverse(void* palNumArg, void* numPalsArg, void* palArg) {
	const uint8_t palNum = (uint8_t)(uintptr_t)palNumArg;
	const uint8_t numPals = (uint8_t)(uintptr_t)numPalsArg;
	const Color* pal = (const Color*)palArg;
	for (int32_t i = 0; i < numPals; i++) {
		for (int32_t c = 0; c < NUMPALCOLORS_4BPP; c++) {
			PALRAM[(palNum + i) * NUMPALCOLORS_4BPP + c] = pal[i * -NUMPALCOLORS_4BPP + c];
		}
	}
}

static void VideoSetPalList(void* palNumArg, void* palListArg, void* unused2);

void SetPalList(uint8_t palNum, const Color** palList) {
	VideoSetters[NumVideoSetters].set = VideoSetPalList;
	VideoSetters[NumVideoSetters].args[0] = (void*)(uintptr_t)palNum;
	VideoSetters[NumVideoSetters++].args[1] = (void*)palList;
}

static void VideoSetPalList(void* palNumArg, void* palListArg, void* unused2) {
	uint8_t palNum = (uint8_t)(uintptr_t)palNumArg;
	const Color** palList = (const Color**)palListArg;

	int16_t i = 0;
	for (bool notEnd = true; notEnd; i++, palNum++) {
		if (palList[i] != (Color*)(intptr_t)-1) {
			if (palList[i] == NULL) {
				notEnd = false;
			}
			else {
				for (int16_t c = 0; c < NUMPALCOLORS_4BPP; c++) {
					PALRAM[(size_t)palNum * NUMPALCOLORS_4BPP + c] = palList[i][c];
				}
			}
		}
	}
}

void UNK_602AA16() {
	UNK_60294C0((void*)(uintptr_t)7u, NULL, NULL);
	VideoSetBackdropColor((void*)(uintptr_t)0u, NULL, NULL);
}

void UNK_602AA4C() {
	UNK_60294C0((void*)(uintptr_t)0x07u, NULL, NULL);
	VideoSetBackdropColor((void*)(uintptr_t)0x000000FFu, NULL, NULL);
}

void UNK_602AA64() {
	STRUCT_6064550* var0 = UNK_6064550;
	for (size_t i = 0u; i < lengthof(UNK_6064550); i++) {
		if (var0->src != NULL) {
			if (--var0->frames == 0) {
				*var0->dst = var0->src[var0->index] & COLOR(0xFF, 0xFF, 0xFF, 0x00);
				const int16_t var2 = COLOR_GETA(var0->src[var0->index]) == 0x00 ? var0->index + 1 : 0;
				var0->index = var2;
				var0->frames = var0->delay;
			}
		}
	}
}

void UNK_602AB9E() {
	for (size_t i = 0u; i < lengthof(UNK_6064550); i++) {
		UNK_6064550[i].src = NULL;
	}
}

void UNK_602AC68(int16_t* arg0) {
	int16_t var0 = arg0[0];
	for (int16_t* var1 = arg0; var0 == 0x00A1; var1 += 4, var0 = var1[0]) {
		// TODO: Figure out a way to translate the construction of a pointer here to portable code.
		//SetPalList((uint8_t)var1[1], ((int32_t)var1[2] << 16) | var1[3]);
	}
}
