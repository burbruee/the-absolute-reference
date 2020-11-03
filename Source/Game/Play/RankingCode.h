#pragma once

#include <stddef.h>
struct Player;

#define NUMRANKINGCODEDIGITS 16
#define RANKINGCODEBASE 20

//typedef struct Player Player;

void UNK_6023128(size_t arg0);
void NewRankingCode(struct Player *player);
void ShowRankingCode(struct Player* player);
