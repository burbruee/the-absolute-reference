#pragma once

#include "DisplayObject.h"

void ShowObject(ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);
void ShowObjectEx(ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, SpriteScale scaleY, SpriteScale scaleX, bool alpha);
