#pragma once

#include <stddef.h>

#define NUMRANKINGCODEDIGITS 16
#define RANKINGCODEBASE 20

typedef struct Player Player;

void UNK_6023128(size_t arg0);
void NewRankingCode(Player *player);
void ShowRankingCode(Player* player);
