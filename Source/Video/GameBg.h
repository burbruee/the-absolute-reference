#pragma once

#include "Video/BgMap.h"
#include "BuildData/BuildData.h"
#include "PlatformTypes.h"
#include <stdint.h>

// SH-2: sizeof(STRUCT_GameBg_0) == 0x8
typedef struct STRUCT_GameBg_0 {
	const BgMap* UNK_0;
	int32_t UNK_4;
} STRUCT_GameBg_0;

// SH-2: sizeof(GameBg) == 0x20
typedef struct GameBg GameBg;
struct GameBg {
	STRUCT_GameBg_0 UNK_0[2];
	uint8_t UNK_10;
	uint8_t index;
	uint8_t UNK_12;
	uint8_t UNK_13;
	uint8_t frame;
	int16_t UNK_16[4];
	int16_t UNK_1E;
};

extern GameBg CurrentGameBg;

void UNK_60169DC();

void UNK_6016A30(uint8_t arg0);

void UNK_6016B40();

void UNK_6016BDE();

void UNK_6016C14();

void UpdateGameBg();
