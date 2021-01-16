#pragma once

#include "HwData.h"
#include <stdint.h>

#define SOUNDCTRL_STARTINIT() \
do { \
	int16_t delay0; \
	int32_t delay1; \
	for (delay0 = 0, delay1 = 0; delay1 < 1000; delay0++, delay1++); \
	EEPROM[0] = 0x02u; \
	UNK_60B181C = 14; \
	UNK_60B186A = 0; \
	for (delay1 = 0; delay1 < delay0; delay1++); \
} while (0)

#define SOUNDCTRL_STARTWAIT() for (int32_t delay = 0; delay < 64; delay++)
	
#define SOUNDCTRL_WAIT(bit) while (SOUNDCTRL[0] & (uint8_t)(bit))

#define SOUNDCTRL_WRITE(i, value) (SOUNDCTRL[(uint8_t)(i)] = (uint8_t)(value))