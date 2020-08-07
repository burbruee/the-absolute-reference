#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum ChecksumFlag {
	CHECKSUM_VALUE = 0x0020,
	CHECKSUM_LAST = 0x8000,
	CHECKSUM_END = 0x0000
} ChecksumFlag;

typedef struct Checksum {
	uint16_t checksumFlags;
	uint16_t data[3];
} Checksum;

typedef struct ChecksumData {
	char header[8];
	Checksum checksums[10];
} ChecksumData;

extern const ChecksumData BuildChecksum;

bool ChecksumPass();
bool ShowChecksumPass();
