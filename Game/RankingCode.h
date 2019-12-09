#pragma once

#include <stddef.h>

#define NUMRANKINGCODEDIGITS 16

extern const uint8_t RankingCodeEncryptionKey[0x100];
#define RANKINGCODEBASE 20
extern const char RankingCodeChars[RANKINGCODEBASE];

typedef struct Player Player;

void _0x6023128(size_t arg0);
void NewRankingCode(Player *player);
// ShowRankingCode
