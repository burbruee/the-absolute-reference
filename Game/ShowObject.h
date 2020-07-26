#pragma once

#include "DisplayObject.h"
#include "PlatformTypes.h"

void ShowObject(const ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);
void ShowObjectEx(const ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, SpriteScale scaleY, SpriteScale scaleX, bool alpha);
