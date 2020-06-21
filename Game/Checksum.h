#pragma once

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
	// remaining three "uint16_t sum[3]".
	uint16_t data[8 * 5];
} ChecksumData;

extern const ChecksumData BuildChecksum;

bool _0x60302C4();
