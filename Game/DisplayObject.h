#pragma once

#include "HwInput.h"
#include <stdbool.h>

// The layout of the original game's object data matches the layout of PS6406B
// sprites where they share data in common.
//
// Object data only contains the following sprite fields:
// - Pixel Y
// - Pixel X
// - Vertical flip
// - Sprite priority
// - Height
// - Horizontal flip
// - Background priority
// - Width
// - Palette number*
// - Bits per pixel.
// - Tile number
//
// * The palette number field is only used by DisplayObjectEx. The value of the
// palette number field in the first element of the object data array is used
// for all sprites when passing zero as the palette number argument to
// DisplayObjectEx.
//
// Use SPRITE_GET* macros to access sprite data in objects. Never use
// SPRITE_SET* macros on objects, nor define objects outside the object data
// table, because the frontends can optimize under the assumption that only
// objects declared in Object.h are used.
// TODO: I think layer number can be defined in an object. Maybe other fields,
// too.
typedef uint16_t ObjectData[6];

// The sprite count in the first object data of an object is the only one used.
// Objects can have up to 63 sprites.
#define OBJECT_GETNUMSPRITES(object) ((uint8_t)(((*(object))[1] >> 10) & 0x3F))
#define OBJECT_SETNUMSPRITES(object, numSprites) ((*(object))[1] = ((*(object))[1] & 0xFC00) | (((uint16_t)(numSprites) & 0x3F) << 10))

// Unscaled. Some objects have hard-coded alpha values.
void DisplayObject(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);

// Full sprite feature support. No hard-coded alpha selection.
void DisplayObjectEx(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, uint8_t scaleY, uint8_t scaleX, bool alpha);
