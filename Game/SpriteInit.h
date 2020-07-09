// TODO: Maybe move this to HwSprite.h/c.
#pragma once

#include "Video.h"
#include <stdint.h>

// Sets one sprite using data from arg1 based on the selected type of setting.
// NOTE: Sprite setting types 0 and 1 initialize the first two sprites with
// some as-of-yet undocumented settings; the first two sprites are not actually
// shown sprites.
// TODO: Change the type argument to an enum, SetSpriteType, once all the
// setting types are understood.

typedef enum AddSpriteType {
	ADDSPRITE_0,
	ADDSPRITE_1,
	ADDSPRITE_2,
	ADDSPRITE_3,
	ADDSPRITE_4,
	ADDSPRITE_5,
	ADDSPRITE_6,
	ADDSPRITE_7,
	ADDSPRITE_8,
	ADDSPRITE_9,
	ADDSPRITE_10,
	ADDSPRITE_11
} AddSpriteType;

void AddSprite(AddSpriteType type, AddSpriteData* data);
