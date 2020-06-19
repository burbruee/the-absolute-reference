#pragma once

#include <stdint.h>

#define MEMCHECKSTATUS_OKAY 0x35530F0F
#define MEMCHECKSTATUS_NOGOOD 0x00000000
typedef enum MemCheck {
	MEMCHECK_WORKRAM,
	MEMCHECK_GRAPHICSRAM,
	MEMCHECK_PALETTERAM,
	MEMCHECK_SCALERAM,
	MEMCHECK_EEPROM, // EEP-ROM checking appears to have been removed from the code.
	MEMCHECK_PROGRAMCHECKSUM,
	NUMMEMCHECKS
} MemCheck;
extern uint32_t MemCheckData[NUMMEMCHECKS];
