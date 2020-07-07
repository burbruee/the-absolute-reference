#include "Video.h"
#include "GameBg.h"
#include "SpriteInit.h"
#include "Fixed.h"
#include "HwSprite.h"
#include "Macros.h"
#include "HwData.h"
#include "LibC.h"
#include "Frame.h"
#include "VideoDefs.h"
#include "BuildData.h"
#include "HwData.h"
#include "PlatformTypes.h"
#include "Macros.h"

#define SPRITELAYER_FREE 0x0000

const int16_t _0x60356C8[4] = { 0x00, 0x07, 0x0F, 0x1F };
struct_0x607D218* _0x606005C = NULL;
static void (**_0x6060060)() = NULL;
static uint16_t* _0x6060064 = NULL;
static int16_t _0x6060068 = 0;
struct_0x606006C _0x606006C[64];
int16_t _0x606106C[64];
struct_0x6061932 _0x6061932;

// Each index into this table is a sprite layer number. An element value of
// zero indicates the layer is free.
//
// Each element is both the name for the first sprite in a layer, and an index
// into SpriteLayerNames where any remaining sprite names for that layer
// bgStart.
static int16_t SpriteLayers[128];

// Table of the sprite names in each layer that are after the first name.  Each
// element is both a sprite name and the index in the layer name table of the
// next sprite name in the layer. The bgEnd of a layer is marked with a zero
// valued name.
static int16_t SpriteLayerNames[MAXSPRITES];

int16_t NumSprites;

// NOTE: Video set functions must have three arguments, each of which is either
// a pointer or intptr_t/uintptr_t.
typedef struct VideoSetter {
	void (*set)(void*, void*, void*);
	void* args[3];
} VideoSetter;

uint16_t NumVideoSetters;
VideoSetter VideoSetters[MAXVIDEOSETTERS];

// NOTE: Looks like this points to data that controls screen brightness during
// transitions; might also be used for darkening the attract mode ranking
// backgrounds. Also, _0x602970C is used with this, which might be where the
// darkening is handled.
struct_0x607D218* _0x6064750 = NULL;

static struct_0x607D218* _0x607CF10[192];
static struct_0x607D218* _0x607D210 = NULL;
static struct_0x607D218* _0x607D214 = NULL;
static struct_0x607D218 _0x607D218[192];

static struct_0x607D218* _0x60AD218 = NULL;
static PauseMode NextPauseMode;
static uint16_t _0x60AD21E = 0u;
static int16_t _0x60AD220;
static int16_t _0x60AD224;

Bg Bgs[4];

typedef struct struct_0x60B0FE0 {
	void (*update)(void*, void*, void*, void*, void*);
	void* args[5];
} struct_0x60B0FE0;

static struct_0x60B0FE0 _0x60B0FE0[2][20];
static int16_t _0x60B13A0[2];
static bool _0x60B13A4;

void _0x6023788() {
	// Empty.
}

void _0x602378C() {
	// Empty.
}

void _0x6023790() {
	NextPauseMode = PAUSEMODE_NOPAUSE;

	MemSet(_0x607D218, 0, sizeof(_0x607D218));

	struct_0x607D218** var0 = _0x607CF10;
	struct_0x607D218* var1 = _0x607D218;
	for (size_t i = 0u; i < lengthof(_0x607CF10); i++) {
		*var0++ = var1++;
	}

	_0x6060068 = 0u;

	_0x607D210 = NULL;
	_0x607D214 = NULL;
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
			else if (_0x60AD218->_0x300[15]._0x0[2] != _0x6023788) {
				int16_t i = _0x60AD218->_0x18;
				void (**var1)() = &_0x60AD218->_0x300[1 - i + 14]._0x0[3];
				for (int16_t i = _0x60AD218->_0x18; i > 1; i--, var1 += 4u) {
					_0x6060064 = (uint16_t*)(var1 - 3); // TODO: struct_0x607D218_0x300::_0x0 might be a struct that has eight bytes of data, then two function pointers.
					_0x6060060 = var1;
					(*var1)();
					if (_0x60AD21E != 0u) {
						break;
					}
				}
				if (_0x60AD21E == 0) {
					_0x6060060 = &_0x60AD218->_0x300[15]._0x0[2];
					_0x60AD218->_0x300[15]._0x0[2]();
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

void _0x6023A0E(struct_0x607D218* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4) {
	// TODO
}

void* _0x6023A98(struct_0x607D218* arg0, void* arg1, void* arg2, void* arg3) {
	// TODO
	return NULL;
}

void _0x6023B76(struct_0x607D218* arg0) {
	// TODO
}

void _0x6023BC4(struct_0x607D218* arg0, void (*arg1)()) {
	// TODO
}

void _0x6023C3E(struct_0x607D218* arg0, void* arg1) {
	// TODO
}

void* _0x6023C8A(struct_0x607D218* arg0, void* arg1) {
	// TODO
	return NULL;
}

int32_t _0x6023CBC(void (*arg0)(), uint32_t arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7) {
	// TODO
	return 0u;
}

struct_0x607D218* _0x6023DAE(struct_0x607D218* arg0) {
	if (_0x6060068 >= lengthof(_0x607CF10) - 1) {
		return NULL;
	}

	struct_0x607D218* var0 = _0x607CF10[_0x6060068];
	MemSet(var0, sizeof(struct_0x607D218), 0u);

	if (arg0 != NULL) {
		var0->_0x0 = arg0;
		var0->_0xC = arg0->_0x4;
		if (arg0->_0x4 != NULL) {
			arg0->_0x4->_0x8 = var0;
		}
		arg0->_0x4 = var0;
	}

	if (_0x6060068 == 0) {
		var0->_0x10 = NULL;
		var0->_0x14 = NULL;
		_0x607D210 = var0;
	}
	else {
		var0->_0x10 = _0x607D214;
		var0->_0x14 = NULL;
		_0x607D214->_0x14 = var0;
	}

	if (_0x60AD220 == 0) {
		_0x60AD220 = 1;
	}

	_0x607D214 = var0;
	var0->_0x1A = _0x60AD220++;

	_0x6060068++;

	return var0;
}

void _0x6023E5A(struct_0x607D218* arg0) {
	struct_0x607D218* var0 = arg0->_0x0;
	struct_0x607D218* var1 = arg0->_0x4;
	struct_0x607D218* var2 = arg0->_0x8;
	struct_0x607D218* var3 = arg0->_0xC;

	if (var1 == NULL) {
		if (var2 == NULL) {
			if (var0 != NULL) {
				var0->_0x4 = var3;
			}
		}
		else {
			var2->_0xC = var3;
		}
		if (var3 != NULL) {
			var3->_0x8 = var2;
		}
	}
	else {
		if (var2 == NULL) {
			if (var0 != NULL) {
				var0->_0x4 = var1;
			}
		}
		else {
			var2->_0xC = var1;
		}
		var1->_0x8 = var2;
		for (var1->_0x0 = var0; var1->_0xC != NULL; var1->_0x0 = var0, var1 = var1->_0xC);
		if (var3 != NULL) {
			var1->_0xC = var3;
			var3->_0x8 = var1;
		}
	}
	if (arg0 == _0x607D210) {
		_0x607D210 = arg0->_0x14;
	}
	if (arg0 == _0x607D214) {
		_0x607D214 = arg0->_0x10;
	}
	arg0->_0x1A = 0u;
	if (arg0->_0x10 != NULL) {
		arg0->_0x10->_0x14 = arg0->_0x14;
	}
	if (arg0->_0x14 != NULL) {
		arg0->_0x14->_0x10 = arg0->_0x10;
	}
	_0x6060068--;
	_0x607CF10[_0x6060068] = arg0;
}

struct_0x607D218* _0x6023EFE(void (**arg0)(), struct_0x607D218* arg1) {
    struct_0x607D218* var1;
    
    struct_0x607D218* var0 = _0x606005C;
    if (arg1 == (struct_0x607D218 *)(intptr_t)~0) {
        arg1 = _0x606005C;
    }
    struct_0x607D218* var1 = _0x6023DAE(arg1);
    if (var1 == NULL) {
        var1 = NULL;
        var0 = _0x606005C;
    }
    else {
		void (*fun0)();
		void (*fun1)();
		void (*fun2)();

        if (arg0[3] == NULL) {
            fun0 = _0x6023788;
        }
        else {
            fun0 = arg0[3];
        }

        _0x606005C = var1;
        var1->_0x300[15]._0x0[3] = fun0;

        if (arg0[1] == NULL) {
            fun1 = _0x6023788;
        }
        else {
            fun1 = arg0[1];
        }

        var1->_0x300[15]._0x0[2] = fun1;

        if (arg0[0] == NULL) {
            fun2 = _0x6023788;
        }
        else {
            fun2 = arg0[0];
        }

        var1->_0x300[14]._0x0[3] = fun2;
        var1->_0x18 = 1;

        if (arg0[2] != NULL) {
            arg0[2]();
        }
    }
    _0x606005C = var0;
    return var1;
}

void* _0x6023FA4(void (**arg0)(), struct_0x607D218* arg1) {
	// TODO
	return NULL;
}

void _0x6024030(struct_0x607D218* arg0) {
	struct_0x607D218* var0 = _0x606005C;
	_0x606005C = arg0;
	arg0->_0x300[15]._0x0[3]();
	_0x6023E5A(arg0);
	if (arg0 == var0) {
		_0x60AD21E = 1u;
	}
	_0x606005C = var0;
}

void _0x602406E() {
	if (_0x6060068 != 0u) {
		for (struct_0x607D218* var0 = _0x607D210, * var1; var0 != NULL; var0 = var1) {
			var1 = var0->_0x14;
			_0x6024030(var0);
		}
	}
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

PauseMode GetNextPauseMode() {
	return NextPauseMode;
}

void AllocSpriteLayerNames(int16_t layer, int16_t num) {
	// Write sprite names beyond the first name to the data table.
	// The new names will be appended onto the bgEnd of the table.
	int16_t numNames = num - 1;
	int16_t i;
	for (i = 0; i < numNames; i++) {
		SpriteLayerNames[NumSprites + i] = NumSprites + i + 1;
	}

	// Modify the layer data so the bgEnd of the data newly written to the
	// layer data table leads to the list of names that were previously in
	// the layer.
	SpriteLayerNames[NumSprites + i] = SpriteLayers[layer];

	// Set the first sprite name of the selected layer in the layer table to
	// the bgStart of the new names.
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
	SetSprite(1, NULL);
	InitSpriteAlpha();
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x67u;
	for (int16_t i = 0; i < lengthof(_0x606006C); i++) {
		_0x606006C[i]._0x2B = 0u;
		_0x606006C[i]._0x2C = 0x8000u;
		_0x606106C[i] = i;
	}

	_0x6061932.tempSprite[6] = 0x003Fu;
	_0x6061932._0x12 = 0x7Du;
	_0x6061932._0x14 = 0x7Fu;
	_0x60AD224 = 0;
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
					if (var2->animFrame < var2->_0x28 - 1) {
						var3 = var2->animFrame + 1;
					}
					else {
						if (!(var2->_0x2C & 0x200)) {
							if (!(var2->_0x2C & 0x400)) {
								var2->animFrame = var2->_0x28 - 1;
								var2->_0x2B = 2u;
							}
							else {
								var2->animFrame--;
								var2->_0x2C |= 0x800;
							}
							break;
						}
						var3 = var2->_0x38;
					}
				}
				else {
					if (var2->_0x38 < var2->animFrame) {
						var3 = var2->animFrame;
					}
					else {
						if (!(var2->_0x2C & 0x200)) {
							if (!(var2->_0x2C & 0x400)) {
								var2->animFrame = var2->_0x38;
								var2->_0x2B = 2u;
							}
							else {
								var2->animFrame++;
								var2->_0x2C &= ~0x0800;
							}
							break;
						}
						var3 = var2->_0x28;
					}
					var3--;
				}
				var2->animFrame = var3;
				break;
			}

		case 2u:
			if (!(var2->_0x2C & 0x8000)) {
				if (ScreenTimeOdd) {
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
	int16_t numSprites;
	ObjectData* object;

	object = arg0->objectTable;
	for (int16_t frame = 0; frame < arg0->animFrame; frame++) {
		object += OBJECT_GETNUMSPRITES(object);
	}
	numSprites = OBJECT_GETNUMSPRITES(object);
	AllocSpriteLayerNames(arg0->layer, numSprites);

	if (arg0->_0x30 != 0) {
		arg0->_0x30--;
	}

	if (arg0->_0x32 < 1) {
		if (arg0->_0x32 < 0) {
			arg0->_0x32++;
		}
	}
	else {
		arg0->_0x32--;
	}

	bool var0 = false;
	bool var1 = false;
	if (!(arg0->_0x2C & 0x2000u)) {
		if (arg0->_0x2C & 0x1000u) {
			var0 = arg0->_0x32 == 0;
			if (var0) {
				arg0->_0x32 = 8;
			}
			arg0->_0x2C &= ~0x1000u;
		}
		else if (arg0->_0x32 < 0) {
			if (arg0->_0x32 % 2 != 0) {
				var0 = true;
			}
		}
		else if (CurrentPauseMode == PAUSEMODE_NOPAUSE && (arg0->_0x2C & 3u) != 0 && arg0->_0x30 == 0) {
			var1 = true;
			arg0->_0x30 = _0x60356C8[arg0->_0x2C & 3];
		}
	}
	object += numSprites - 1;
	for (int16_t i = 0; i < numSprites; i++, object--) {
		if (((*object)[0] & 0x8000u) == 0 || ((ScreenTimeOdd + 1) & arg0->_0x2F) == 0 ) {
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
			OBJECT_SETY(&_0x6061932.tempSprite, arg0->y + offsetY);
			OBJECT_SETX(&_0x6061932.tempSprite, arg0->x + offsetX);

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
			_0x6061932.tempSprite[2] = (((uint16_t)sprPriVertical) << 8) | arg0->scaleY;
			if (arg0->verticalHorizontal & 0x80) {
				if (horizontal & 0x80) {
					bgPriHorizontal &= 0x4Fu;
				}
				else {
					bgPriHorizontal |= 0x80u;
				}
			}
			_0x6061932.tempSprite[3] = (((uint16_t)bgPriHorizontal) << 8) | arg0->scaleX;
			if (var0) {
				if (OBJECT_GETSPRPRI(object) & 2) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, 16u);
				}
				else if (arg0->palNum == 0) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
				}
				else {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
				}
			}
			else if (var1) {
				if (OBJECT_GETSPRPRI(object) & 2) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, 32u);
				}
				else {
					if (arg0->palNum == 0) {
						OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
					}
					else {
						OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
					}
				}
			}
			else if (arg0->palNum == 0) {
				OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
			}
			else {
				OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
			}
			_0x6061932.tempSprite[4] = (_0x6061932.tempSprite[4] & 0xFF00u) | ((*object)[4] & 0x8Fu) | (arg0->bppAlphaTileTop & 0x70u);
			_0x6061932.tempSprite[5] = (*object)[5];
			for (int16_t i = 0; i < lengthof(_0x6061932.tempSprite); i++) {
				Sprites[NumSprites][i] = _0x6061932.tempSprite[i];
			}
			NumSprites++;
		}
	}
}

void _0x602471C(struct_0x606006C* arg0) {
	const ObjectData* object = arg0->objectTable;
	for (int16_t frame = 0; frame < arg0->animFrame; frame++) {
		object += OBJECT_GETNUMSPRITES(object);
	}
	int16_t numSprites = OBJECT_GETNUMSPRITES(object);
	AllocSpriteLayerNames(arg0->layer, numSprites);

	if (arg0->_0x30 != 0) {
		arg0->_0x30--;
	}
	if (arg0->_0x32 < 1) {
		if (arg0->_0x32 < 0) {
			arg0->_0x32++;
		}
	}
	else {
		arg0->_0x32--;
	}

	bool var0 = false;
	bool var1 = false;
	if (!(arg0->_0x2C & 0x2000u)) {
		if (arg0->_0x2C & 0x1000u) {
			var0 = arg0->_0x32 == 0;
			if (var0) {
				arg0->_0x32 = 8;
			}
			arg0->_0x2C &= ~0x1000u;
		}
		else if (arg0->_0x32 < 0) {
			if (arg0->_0x32 % 2) {
				var0 = true;
			}
		}
		else if (CurrentPauseMode == PAUSEMODE_NOPAUSE && (arg0->_0x2C & 3) && arg0->_0x30 == 0) {
			var1 = true;
			arg0->_0x30 = _0x60356C8[arg0->_0x2C & 3];
		}
	}

	object += numSprites - 1;
	for (int16_t i = 0; i < numSprites; i++, object--) {
		if (((*object)[0] & 0x8000u) == 0u || ((ScreenTimeOdd + 1) & arg0->_0x2F) == 0u) {
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
			OBJECT_SETY(&_0x6061932.tempSprite, arg0->y + offsetY);
			OBJECT_SETX(&_0x6061932.tempSprite, arg0->x + offsetX);
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
			_0x6061932.tempSprite[2] = ((uint16_t)sprPriVertical << 8) | arg0->scaleY;
			if (arg0->verticalHorizontal & 0x0080u) {
				if (horizontal & 0x80u) {
					bgPriHorizontal &= 0x4Fu;
				}
				else {
					bgPriHorizontal |= 0x80u;
				}
			}
			_0x6061932.tempSprite[3] = ((uint16_t)bgPriHorizontal << 8) | arg0->scaleX;
			if (var0) {
				if (OBJECT_GETSPRPRI(object) & 0x2u) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, 16u);
				}
				else if (arg0->palNum == 0) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
				}
				else {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
				}
			}
			else if (var1) {
				if (OBJECT_GETSPRPRI(object) & 2u) {
					OBJECT_SETPALNUM(&_0x6061932.tempSprite, 32u);
				}
				else {
					if (arg0->palNum == 0u) {
						OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
					}
					else {
						OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
					}
				}
			}
			else if (arg0->palNum == 0) {
				OBJECT_SETPALNUM(&_0x6061932.tempSprite, OBJECT_GETPALNUM(object));
			}
			else {
				OBJECT_SETPALNUM(&_0x6061932.tempSprite, arg0->palNum);
			}
			_0x6061932.tempSprite[4] = ((uint16_t)_0x6061932.tempSprite[4] & 0xFF00u) | ((*object)[4] & 0x008Fu) | (arg0->bppAlphaTileTop & 0x70u);
			_0x6061932.tempSprite[5] = (*object)[5];
			for (int16_t i = 0; i < lengthof(_0x6061932.tempSprite); i++) {
				Sprites[NumSprites][i] = _0x6061932.tempSprite[i];
			}
			NumSprites++;
		}
	}
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

int16_t _0x6024B0C() {
	int16_t var0 = _0x606106C[_0x60AD224];
	_0x60AD224++;
	_0x606006C[var0].flipXBgPri = 1u;
    _0x606006C[var0].verticalHorizontal = 0u;
    _0x606006C[var0].scaleX = UNSCALED;
    _0x606006C[var0].scaleY = UNSCALED;
    _0x606006C[var0].palNum = 0u;
    _0x606006C[var0].bppAlphaTileTop = 0u;
    _0x606006C[var0].layer = 16u;
    _0x606006C[var0]._0x2B = 1u;
    _0x606006C[var0]._0x2C = 0xA030u;
    _0x606006C[var0]._0x2E = 0u;
    _0x606006C[var0]._0x30 = 0;
    _0x606006C[var0]._0x32 = 0;
    _0x606006C[var0]._0x34 = 0;
    _0x606006C[var0].animFrame = 0;
    _0x606006C[var0]._0x36 = 0;
    _0x606006C[var0]._0x28 = 0;
    return var0;
}

void _0x6024B78(int16_t arg0) {
	if ((-1 < arg0) && (arg0 < 0x40)) {
		_0x606006C[arg0]._0x2B = 0u;
		_0x606006C[arg0]._0x2C = 0x8000u;
		int32_t var1 = 0;
		int16_t var0 = _0x606106C[0];
		while (var0 != arg0) {
			var1 += 1;
			var0 = _0x606106C[var1];
		}
		_0x60AD224 += -1;
		if (var1 != _0x60AD224) {
			int16_t* var2 = &_0x606106C[var1];
			// Swap *var2 and _0x606106C[_0x60AD224].
			_0x606106C[_0x60AD224] ^= *var2;
			*var2 ^= _0x606106C[_0x60AD224];
			_0x606106C[_0x60AD224] ^= *var2;
		}
	}
	return;
}

void _0x6024C3C(int16_t i, int16_t y, int16_t x, ObjectData* objectTable) {
	if (i >= 0 && i < 64) {
		_0x606006C[i].objectTable = objectTable;
		_0x606006C[i].y = y;
		_0x606006C[i].x = x;
		if (_0x606006C[i]._0x2B == 2u) {
			_0x606006C[i]._0x2B = 2u;
		}
		else {
			_0x606006C[i]._0x2B = 9u;
		}
		_0x606006C[i].animFrame = 0;
	}
}

// TODO: Could be InitBgIndex().
void _0x6024E5C(int16_t i) {
	Bgs[i]._0x2 = 0;
	Bgs[i]._0x4 = 1;
	Bgs[i]._0x6 = 0;
	Bgs[i]._0xA = 3;
	Bgs[i]._0xC = 0;
	Bgs[i].darkness = 0;
	Bgs[i]._0x10 = 0;
	Bgs[i]._0x14 = 0;
	Bgs[i]._0x30 = 0;
	Bgs[i]._0x34 = 0;
	Bgs[i]._0x8 = 0;
	Bgs[i]._0x40 = 0;
	Bgs[i]._0x44 = 0;
	Bgs[i]._0x4C = 0;

	for (size_t k = 0u; k < lengthoffield(Bg, _0x18); k++) {
		Bgs[i]._0x18[k] = -1;
	}

	for (size_t k = 0u; k < lengthoffield(Bg, _0x38); k++) {
		Bgs[i]._0x38[k] = 0;
	}
}

// TODO: Could be InitBg().
void _0x6024ED8() {
	BgMapBanks[0] = 10u;
	BgMapBanks[1] = 12u;
	BgMapBanks[2] = 14u;
	BgMapBanks[3] = 16u;

	BgMapSettings[0] = 0u;
	BgMapSettings[1] = 0u;

	_0x2405FFEB |= 0x40u;

	_0x60B13A4 = false;

	for (size_t i = 0u; i < lengthof(Bgs); i++) {
		Bgs[i]._0x0 = 0;
		_0x6024E5C(i);
	}

	for (size_t i = 0u; i < lengthof(_0x60AD228); i++) {
		_0x60AD228[i]._0x0 = 0u;
		for (size_t bank = 0u; bank < 2u; bank++) {
			// TODO: Create a RAM macro here, once this is better understood.
			_0x60AD228[i]._0x4[bank] = &GRAPHICSRAM[(0x5000u + i * 0x1000u + bank * 0x800u) / sizeof(uint32_t)];
			_0x60AD228[i]._0xC[bank] = bank + i * 2 + 10u;
		}
		_0x60AD228[i]._0x10 = NULL;
		_0x60AD228[i]._0x14 = 0;
		_0x60AD228[i]._0x60 = NULL;
		_0x60AD228[i]._0x64 = 0;
	}

	for (size_t i = 0u; i < lengthof(_0x60B0FE0); i++) {
		for (size_t k = 0u; k < lengthof(*_0x60B0FE0); k++) {
			MemSet(&_0x60B0FE0[i][k], 0, sizeof(struct_0x60B0FE0));
		}
	}
}

void _0x6025078() {
	if (CurrentPauseMode != PAUSEMODE_NOPAUSE) {
		return;
	}

	struct_0x60B0FE0* var0 = _0x60B0FE0[!_0x60B13A4];
	for (size_t i = 0u; i < lengthof(*_0x60B0FE0); i++) {
		if (var0[i].update != NULL) {
			var0[i].update(
				var0[i].args[0],
				var0[i].args[1],
				var0[i].args[2],
				var0[i].args[3],
				var0[i].args[4]);
			var0[i].update = NULL;
		}
	}
	_0x60B13A0[!_0x60B13A4] = 0;
}

void _0x60251B8(int16_t bgIndex, uint32_t* arg1, uint32_t* arg2) {
	for (size_t i = 0; i < 0xE0u; i += 8u) {
		for (size_t j = 0; j < 8u; j++) {
			arg1[i + j] = Bgs[bgIndex]._0x50[i + j + 0x00u];
		}
		for (size_t j = 0; j < 8u; j++) {
			arg2[i + j] = Bgs[bgIndex]._0x50[i + j + 0xE0u];
		}
	}
}

void _0x602523C() {
	if (CurrentPauseMode == PAUSEMODE_NOPAUSE) {
		_0x60B13A4 = !_0x60B13A4;
		VideoSetters[NumVideoSetters++].set = _0x602526A;
	}
}

void _0x602526A(void* unused0, void* unused1, void* unused2) {
	uint8_t bgMapBank0, bgMapBank1, bgMapBank2, bgMapBank3;
	uint8_t bgMapSettings01, bgMapSettings23;

#define _BGSET(bgIndex, tilemapBank, tilemapSettings) \
	do { \
		if (Bgs[bgIndex]._0x0 == 1 && Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6] >= 0) { \
			const int16_t var0 = Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6]; \
			const int16_t var1 = _0x60AD228[var0]._0x54; \
			const int16_t var2 = Bgs[bgIndex]._0x8; \
			uint16_t tilemapBankIndex; \
			if (_0x60AD228[var0]._0x56 == 0) { \
				tilemapBankIndex = (var1 + 1) % 2; \
			} \
			else { \
				_0x60AD228[var0]._0x54 = (var1 + 1) % 2; \
				_0x60AD228[var0]._0x56 = 0; \
				tilemapBankIndex = var1; \
			} \
			tilemapBank = (_0x60AD228[var2]._0xC[tilemapBankIndex] >> 8) & 0xFFu; \
			if (Bgs[bgIndex]._0x10 == 0) { \
				_0x60AD228[var2]._0x4[tilemapBankIndex][0xFCu + bgIndex] = \
					(_0x60AD228[var0]._0x18[var1] << 16) | \
					(_0x60AD228[var0]._0x20[var1] & 0x1FFu); \
				_0x60AD228[var2]._0x4[tilemapBankIndex][0x1FCu + bgIndex] = \
					((uint32_t)Bgs[bgIndex]._0xA << 24) | \
					_0x60AD228[var0]._0xC[tilemapBankIndex] | \
					(Bgs[bgIndex].darkness << 8) | \
					(Bgs[bgIndex]._0xC << 15); \
			} \
			else { \
				tilemapBank |= 0x80u; \
				uint32_t* var3 = _0x60AD228[var2]._0x4[tilemapBankIndex]; \
				_0x60251B8(1, var3, var3 + 0x100u); \
			} \
			if (!(_0x60AD228[var0]._0x10->_0x0->header.tileInfo & BGMAPTILEINFO_BIT21)) { \
				tilemapSettings |= 0x40u >> ((bgIndex % 2u) * 4u); \
			} \
			if (Bgs[bgIndex]._0x2 != 0) { \
				tilemapSettings |= 0x80u >> ((bgIndex % 2u) * 4u); \
			} \
		} \
		else { \
			tilemapBank = 10u; \
		} \
	} while (false)

	_BGSET(0u, bgMapBank0, bgMapSettings01);
	_BGSET(1u, bgMapBank1, bgMapSettings01);
	_BGSET(2u, bgMapBank2, bgMapSettings23);
	_BGSET(3u, bgMapBank3, bgMapSettings23);

#undef _BGSET

	BgMapBanks[0] = bgMapBank0;
	BgMapBanks[1] = bgMapBank1;
	BgMapBanks[2] = bgMapBank2;
	BgMapBanks[3] = bgMapBank3;
	BgMapSettings[0] = bgMapSettings01;
	BgMapSettings[1] = bgMapSettings23;
}

// TODO: Could be AllocBg(); returns the background index allocated or -1 on
// failure.
int32_t _0x60257EE() {
	int32_t i;

	for (i = 0; i < lengthof(Bgs); i++) {
		if (Bgs[i]._0x0 == 0) {
			break;
		}
	}

	if (i < 4) {
		_0x6024E5C(i);
		Bgs[i]._0x0 = 1;
	}
	else {
		i = -1;
	}

	return i;
}

int32_t _0x6025918() {
	int bgIndex;

	for (bgIndex = 1; bgIndex < 11; bgIndex++) {
		if (_0x60AD228[bgIndex]._0x0 == 0u) {
			break;
		}
	}

	if (bgIndex < 11) {
		_0x60AD228[bgIndex]._0x0 = 1;
		_0x60AD228[bgIndex]._0x10 = NULL;
		_0x60AD228[bgIndex]._0x60 = NULL;
		_0x60AD228[bgIndex]._0x54 = NULL;
		_0x60AD228[bgIndex]._0x56 = NULL;
		_0x60AD228[bgIndex]._0x4C = NULL;
		_0x60AD228[bgIndex]._0x50 = NULL;
		_0x60AD228[bgIndex]._0x58 = NULL;

		for (int16_t i = 0; i < 2; i++) {
			_0x60AD228[bgIndex]._0x18[i] = 0;
			_0x60AD228[bgIndex]._0x20[i] = 0;
			_0x60AD228[bgIndex]._0x28[i] = 0;
			_0x60AD228[bgIndex]._0x30[i] = 0;
			_0x60AD228[bgIndex]._0x34[i] = 0;
			_0x60AD228[bgIndex]._0x38[i] = 0;
			_0x60AD228[bgIndex]._0x3C[i] = 0;
			_0x60AD228[bgIndex]._0x40[i] = 0;
			_0x60AD228[bgIndex]._0x46[i] = 0;
		}
	}
	else {
		bgIndex = -1;
	}

	return bgIndex;
}

int32_t _0x6025AE4(int16_t bgIndex, GameBg* gameBg) {
	int16_t var0;
	if (Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6] < 0) {
		var0 = _0x6026918();
		Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6] = var0;
	}
	else {
		var0 = Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6];
	}
	_0x60AD228[var0]._0x10 = gameBg;
	_0x60AD228[bgIndex]._0x14 = 0;
	
	for (BgMap* bgMap = gameBg->_0x0; bgMap != NULL; bgMap = gameBg->_0x0) {
		_0x60AD228[var0]._0x14 += gameBg->_0x0->header._0x6 * gameBg->_0x4;
	}

	_0x60AD228[var0]._0x4C = 0;
	_0x60AD228[var0]._0x50 = 0;

	return var0;
}

void _0x6025B9A(int16_t bgIndex, GameBg* gameBg, int32_t arg2, int32_t arg3) {
	int32_t var0 = _0x6025AE4(bgIndex, gameBg);
	_0x60AD228[var0]._0x4C += arg2;
	_0x60AD228[var0]._0x50 += arg3;
}

void _0x60267E4(int16_t bgIndex) {
	const int16_t var0 = Bgs[bgIndex]._0x6;

	if (Bgs[bgIndex]._0x18[var0] < 0) {
		return;
	}

	const int16_t var2 = _0x60AD228[var2]._0x54;

	int16_t var4;
	int16_t var5;
	var4 = 0;
	if (Bgs[bgIndex]._0x18[var0] != -1) {
		const int16_t var1 = _0x60AD228[Bgs[bgIndex]._0x18[bgIndex]]._0x0;
		const int16_t var3 = Bgs[bgIndex]._0x0;
		const int32_t var13 = -(16 + _0x60AD228[var1]._0x18[var2] / 16);
		const int16_t var7 = -(16 + _0x60AD228[var1]._0x18[var0] / 16 + _0x60AD228[var1]._0x4C / 16);
		int16_t var16 = -(16 + _0x60AD228[var1]._0x28[var3] / 16 + _0x60AD228[var1]._0x50 / 16);
		var4 = 0;
		for (size_t i = 0u; i < 32u; i++, var16++) {
			uint32_t* const var10 = _0x60AD228[var1]._0x4[var2];
			GameBg* const var6 = _0x6026AAC(
				bgIndex,
				var16,
				&var4,
				&var5);
			if (var6->_0x0->header.tileInfo & BGMAPTILEINFO_PERTILEPAL) {
				ROMDATA BgMap* const bgMap = var6->_0x0;
				int16_t var8 = var7;
				int32_t var14 = var13;
				for (size_t j = 0u; j < 16u; j++, var8++, var14++) {
					int32_t var9 = var8 % var6->_0x0->header._0x4;
					if (var9 < 0) {
						var9 += var6->_0x0->header._0x4;
					}
					var10[(var14 % 16) * 32] = bgMap->names[var4 * bgMap->header._0x4 + var9];
				}
			}
			else {
				ROMDATA BgMap* const bgMap = var6->_0x0;
				int16_t var8 = var7;
				int32_t var14 = var13;
				for (size_t j = 0u; j < 16u; j++, var8++, var14++) {
					int32_t var9 = var8 % var6->_0x0->header._0x4;
					if (var9 < 0) {
						var9 += var6->_0x0->header._0x4;
					}
					var10[(var14 % 16) * 32] = (bgMap->names[var4 * bgMap->header._0x4 * 2 + var9] & 0xFFFFu) | var6->_0x0->header.tileInfo;
				}
			}
		}

		_0x60AD228[var2]._0x38[var1] %= 16;
		_0x60AD228[var2]._0x3C[var1] %= 16;
	}
}

GameBg* _0x6026AAC(int16_t bgIndex, int16_t arg1, int16_t* arg2, int16_t* arg3) {
	const int16_t var0 = Bgs[bgIndex]._0x18[Bgs[bgIndex]._0x6];
	GameBg* gameBg;
	if (var0 < 0) {
		gameBg = NULL;
	}
	else {
		gameBg = _0x60AD228[var0]._0x10;
		if (gameBg->_0x0 != NULL) {
			*arg2 = arg1;
			const int16_t var3 = *arg2;
			GameBg* var1 = gameBg;
			while (*arg2 < 0) {
				if (_0x60AD228[var3]._0x60 == NULL) {
					*arg2 += _0x60AD228[var3]._0x14;
				}
				else {
					*arg2 += _0x60AD228[var3]._0x64;
					gameBg = _0x60AD228[var3]._0x60;
					var1 = gameBg;
				}
			}
			while (*arg2 >= (int16_t)gameBg->_0x0->header._0x6 * gameBg->_0x4) {
				ROMDATA BgMap* var2 = gameBg->_0x8;
				*arg2 -= gameBg->_0x0->header._0x6 * gameBg->_0x4;
				if (var2->header.tileInfo == 0) {
					if (_0x60AD228[var3]._0x60 != NULL) {
						gameBg = _0x60AD228[var3]._0x60;
					}
					else {
						gameBg = var1;
					}
				}
			}
			*arg3 = gameBg->_0x0->header._0x6 - *arg2 - 1;
			*arg2 %= gameBg->_0x0->header._0x6;
		}
	}
	return gameBg;
}

void _0x6026FCA(int16_t bgIndex, int16_t arg1) {
	Bgs[bgIndex]._0x2 = arg1;
}

void _0x6026FDC(int16_t arg0, int16_t arg1) {
	Bgs[arg0]._0xA = arg1;
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

void _0x6029498(int16_t arg0) {
	VideoSetters[NumVideoSetters].set = _0x60294C0;
	VideoSetters[NumVideoSetters++].args[0] = (void*)(uintptr_t)arg0;
}

void _0x60294C0(uintptr_t arg0, void* unused1, void* unused2) {
	// TODO: Make a define for register 0x0B.
	VIDEOREGS[0x0B] = (VIDEOREGS[0x0B] & 0xF8u) | (arg0 & 0x07u);
}

void SetBackdropColor(Color color) {
	VideoSetters[NumVideoSetters].set = VideoSetBackdropColor;
	VideoSetters[NumVideoSetters].args[0] = color;
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

static void _0x602970C();
static void _0x602975E();
static const void (*_0x60356D0[4])() = {
	_0x602970C,
	_0x602970C,
	NULL,
	_0x602975E
};

void _0x6029546(int16_t arg0, int16_t arg1, int16_t arg2, int16_t arg3) {
	struct_0x607D218 *var0;
	uint32_t numVideoSetters;
	
	if (arg0 != 4) {
		if (_0x6064750 == NULL) {
			_0x6064750 = _0x6023EFE(_0x60356D0, NULL);
		}
		if (arg3 & 0x8000) {
			_0x6064750->_0x30 = 1u;
		}
		else {
			_0x6064750->_0x30 = 0u;
		}
		_0x6064750->_0x2C = F32(0, 0x0000u);
		F32I(_0x6064750->_0x2C) = 0x80;
		_0x6064750->_0x2C /= arg1;
		VideoSetters[NumVideoSetters++].set = VideoSetScanlinesBank;
		if ((arg3 & 7) != 0) {
			_0x6029498(arg3 & 7);
		}
		_0x6064750->_0x1A = arg0;
		if (arg0 == 0) {
			_0x6064750->backdropColor = 0xFFu;
			if (arg2 == 0) {
				_0x6064750->_0x24 = 0x80;
			}
			else {
				_0x6064750->_0x24 = arg2;
			}
			_0x6064750->_0x2C = -_0x6064750->_0x2C;
		}
		else {
			if (arg0 == 1) {
				_0x6064750->backdropColor = ~0u;
				if (arg2 == 0) {
					_0x6064750->_0x24 = 0x80;
				}
				else {
					_0x6064750->_0x24 = arg2;
				}
				_0x6064750->_0x2C = -_0x6064750->_0x2C;
			}
			else {
				if (arg0 == 2) {
					_0x6064750->backdropColor = COLOR(0, 0, 0, 0);
					if (arg2 == 0) {
						_0x6064750->_0x24 = 0;
					}
					else {
						_0x6064750->_0x24 = arg2;
					}
				}
				else {
					if (arg0 == 3) {
						_0x6064750->backdropColor = COLOR(0xFF, 0xFF, 0xFF, 0x00);
						if (arg2 == 0) {
							_0x6064750->_0x24 = 0;
						}
						else {
							_0x6064750->_0x24 = arg2;
						}
					}
				}
			}
		}
		_0x6064750->_0x28 = F32(0, 0x0000u);
		F32I(_0x6064750->_0x28) = _0x6064750->_0x24;
		SetBackdropColor(_0x6064750->backdropColor);
	}
	else if (_0x6064750 != NULL) {
		_0x6024030(_0x6064750);
		_0x6064750 = NULL;
	}
}

static void _0x602970C() {
	_0x6064750->_0x28 += _0x6064750->_0x2C;
	if (_0x6064750->_0x2C <= F32(0, 0x0000u)) {
		if (F32I(_0x6064750->_0x28) <= 0) {
			_0x6024030(_0x606005C);
			return;
		}
	}
	else if (F32I(_0x6064750->_0x28) >= 128) {
		_0x6024030(_0x606005C);
		return;
	}
	else {
		_0x6064750->backdropColor = (_0x6064750->backdropColor & ~0xFF) | F32I(_0x6064750->_0x28);
		SetBackdropColor(_0x6064750->backdropColor);
		return;
	}
}

static void _0x602975E() {
	if (F32I(_0x6064750->_0x2C) <= 0) {
		if (_0x6064750->_0x30 == 0u) {
			_0x6029498(0);
			_0x6064750->backdropColor = COLOR(0x11, 0x11, 0x11, 0xFF);
		}
		else {
			_0x6064750->backdropColor &= COLOR(0xFF, 0xFF, 0xFF, 0x00);
		}
	}
	else {
		_0x6064750->backdropColor |= COLOR(0x00, 0x00, 0x00, 0xFF);
	}
	SetBackdropColor(_0x6064750->backdropColor);
	_0x6064750 = NULL;
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

// TODO: Appears to fill background RAM.
void _0x6029814(uint32_t arg0, uint32_t arg1, uint8_t bgStart, uint8_t bgEnd) {
	// Check if the start and size need to be swapped.
	if (bgStart > bgEnd) {
		// Swap bgStart and bgEnd.
		bgEnd ^= bgStart;
		bgStart ^= bgEnd;
		bgEnd ^= bgStart;

		// Swap arg0 and arg1.
		arg1 ^= arg0;
		arg0 ^= arg1;
		arg1 ^= arg0;
	}
	uint32_t size = bgEnd - bgStart;

	// Write background data.
	uint32_t num3 = 0;
	uint32_t num2 = 0;
	uint32_t num1 = 0;
	for (uint32_t* bgData = &BGRAM[bgStart]; bgStart < bgEnd; bgStart++, bgData++) {
		// Bits [31, 24].
		num3 += ((arg1 >> 24) & 0xFFu) - ((arg0 >> 24) & 0xFFu);
		uint32_t offset3 = num3 / size;

		// Bits [23, 16].
		num1 += ((arg1 >> 16) & 0xFFu) - ((arg0 >> 16) & 0xFFu);
		uint32_t offset2 = num1 / size;

		// Bits [15, 8].
		num1 += ((arg1 >>  8) & 0xFFu) - ((arg0 >>  8) & 0xFFu);
		uint32_t offset1 = num1 / size;

		// Bits [7, 0] are set to zero.
		*bgData =
			(((((arg0 >>  8) & 0xFFu) + offset1) <<  8) & 0x0000FF00u) |
			(((((arg0 >> 16) & 0xFFu) + offset2) << 16) & 0x00FF0000u) |
			(((((arg0 >> 24) & 0xFFu) + offset3) << 24) & 0xFF000000u);
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
						PALRAM[cycle->palNum * NUMPALCOLORS_4BPP + j] = tempPal[j];
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

void NewPalCycle(uint8_t palNum, Color* pal0, Color* pal1, int16_t perPalDelay, PalCycleType type, int8_t stride, uint8_t endStep) {
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
		for (size_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
			PALRAM[palNum * NUMPALCOLORS_4BPP + j] = pal[j];
		}
	}
}

void _0x602AA16() {
	_0x60294C0(7u, NULL, NULL);
	VideoSetBackdropColor(0u, NULL, NULL);
}

void _0x602AA4C() {
	_0x60294C0(0x07u, NULL, NULL);
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

// TODO: Consider changing arg0 to a struct type. Elements [1, 3] of the array
// could be "int16_t argData[3]", and arg0[0] could be some sort of 16-bit type
// value.
void _0x602AC68(int16_t* arg0) {
	int16_t var0 = arg0[0];
	for (int16_t* var1 = arg0; var0 == 0x00A1; var1 += 4, var0 = var1[0]) {
		_0x602A96A(var1[1], ((int32_t)var1[2] << 16) | var1[3]);
	}
}
