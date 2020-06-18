#pragma once

#include <stdint.h>

typedef struct ChecksumData {
	char header[9];
	uint8_t data[8 * 10];
} ChecksumData;

extern const ChecksumData Checksum;
