#pragma once

#include "ShowText.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum ChecksumFlag {
	CHECKSUM_NORMAL = 0x0020,
	CHECKSUM_LAST = 0x8000,
	CHECKSUM_END = 0x0000
} ChecksumFlag;

typedef struct BuildChecksumData {
	const uint16_t checksumFlags;
	const uint16_t data[3];
} BuildChecksumData;

typedef struct BuildChecksumData {
	const char header[8];
	const BuildChecksumData checksums[10];
} BuildChecksumData;

extern const BuildChecksumData BuildChecksum;

bool _0x6030100();
bool _0x60302C4();
void ShowHexNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool displayZeroes, NumAlign numAlign, int16_t palNum, bool alpha);
