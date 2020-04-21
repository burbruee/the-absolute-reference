#pragma once

#include "BuildData.h"
#include <stdint.h>

// SH-2: sizeof(GameBg) == 0x20
typedef struct GameBg GameBg;
struct GameBg {
	ROMDATA BgMap* _0x0;
	int32_t _0x4;
	ROMDATA BgMap* _0x8;
	uint32_t _0xC;
	uint8_t _0x10;
	uint8_t index;
	uint8_t _0x12;
	uint8_t _0x13;
	uint8_t frame;
	int16_t _0x16[4];
	int16_t _0x1E;
};

extern GameBg CurrentGameBg;

void _0x60169DC();

void _0x6016A30(uint8_t arg0);

void _0x6016B40();

void _0x6016BDE();

void _0x6016C14();

void UpdateGameBg();
