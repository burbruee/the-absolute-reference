#pragma once

#include "ShowText.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum ChecksumFlag {
	CHECKSUM_NORMAL = 0x0020,
	CHECKSUM_LAST = 0x8000,
	CHECKSUM_END = 0x0000
} ChecksumFlag;

typedef struct ChecksumData {
	char header[8];
	// TODO: Probably change this to an array of BuildChecksum structs; each struct
	// would contain 4 uint16_t, the first would be "uint16_t type" and the
	// remaining three "uint16_t data[3]".
	uint16_t data[4 * 10];
} ChecksumData;

extern const ChecksumData BuildChecksum;

bool _0x6030100();
bool _0x60302C4();
void ShowHexNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool displayZeroes, NumAlign numAlign, int16_t palNum, bool alpha);
