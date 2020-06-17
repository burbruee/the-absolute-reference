#pragma once

#include <stdint.h>

typedef struct ChecksumData {
	char header[9];
	uint8_t data[4 * 20];
} ChecksumData;

extern const ChecksumData Checksum;
