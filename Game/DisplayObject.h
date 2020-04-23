#pragma once

#include "Object.h"
#include "HwInput.h"
#include <stdbool.h>

// The DisplayObject* functions accept ObjectData arrays, not SpriteData
// arrays, and each ObjectData contains only the following SpriteData fields,
// in the same layout as SpriteData:
//
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
// When dynamically generating objects, only ever use the object tile numbers
// that are in Objects::data; the game follows this convention all throughout
// the code, and this allows renderers to optimize around that assumption.
//
// TODO: There appears to be more object data beyond what DisplayObject*
// functions support, and some of that data appears to be at indices 6 and 7 of
// SpriteData.
typedef uint16_t ObjectData[6];

// Unscaled. Some objects have hard-coded alpha values.
void DisplayObject(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);

// Full sprite feature support. No hard-coded alpha selection.
void DisplayObjectEx(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, SpriteScale scaleY, SpriteScale scaleX, bool alpha);
