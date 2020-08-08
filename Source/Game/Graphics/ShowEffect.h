#pragma once

#include "Game/Play/Player.h"
#include <stdint.h>

void ShowFieldBlockExplosion(Player *player, int16_t row, int16_t col);

void ShowFireworks(Player *player, int16_t row, int16_t col, uint32_t seed);

void ShowAllClear(Player *player, int16_t row, int16_t col);

void UNK_60173B4(Player* player, int16_t row, int16_t col);

void ShowLineClear(Player *player, int16_t row);

void ShowStaffClear(Player* player, int16_t row);

void ShowThrownOutActiveBlock(Player *player);
