#pragma once

#include <stddef.h>

#define NUMRANKINGCODEDIGITS 16

#define RANKINGCODEBASE 20
extern const char RankingCodeChars[RANKINGCODEBASE];

typedef struct Player Player;

void _0x6023128(size_t arg0);
void NewRankingCode(Player *player);
void ShowRankingCode(Player* player);
