#pragma once

#include "Player.h"
#include <stdint.h>

void ShowFieldBlockExplosion(Player *player, int16_t row, int16_t col);

void ShowFireworks(Player *player, int16_t row, int16_t col, uint32_t seed);

void ShowAllClear(Player *player, int16_t row, int16_t col);

//_0x60173B4; unused.

void ShowClear(Player *player, int16_t row);

void ShowSingleRowClear(Player* player, int16_t row);

void ShowThrownOutActiveBlock(Player *player);
