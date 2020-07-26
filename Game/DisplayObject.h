#pragma once

#include "Object.h"
#include "HwInput.h"
#include "PlatformTypes.h"
#include <stdbool.h>

// Unscaled. Some objects have hard-coded alpha values.
void DisplayObject(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);

// Full sprite feature support. No hard-coded alpha selection.
void DisplayObjectEx(const ObjectData *object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, SpriteScale scaleY, SpriteScale scaleX, bool alpha);
