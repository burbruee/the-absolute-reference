#pragma once

// Functions to access EEP-ROM. Definitions provided by frontends.
//
// A frontend has to do conversion of EEP-ROM data to guarantee the save data
// format matches the PS5V2 version. The code directly stores structs with
// bitfields in EEP-ROM, so layout can't be guaranteed to match the original's.
// The bitfields might mismatch the original's, even on big endian
// architectures. Though testing of a frontend on a big endian archicture could
// be done, and if the format ends up matching the original's, the conversion
// could be omitted.
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_SIZE 0x100

typedef enum Region {
	REGION_JAPAN = 0,
	REGION_USACANADA = 1 << 0,
	REGION_KOREA = 1 << 1,
	REGION_HONGKONG = 1 << 2,
	REGION_TAIWAN = 1 << 3,
	REGION_SETTING = 0xF
} Region;

void ReadSettings();
void UNK_602FA7E();
void WriteSettings();
void ReadEeprom(uint8_t offset, void* destination, size_t size);
void WriteEeprom(uint8_t offset, void* source, size_t size);
