#pragma once

// Functions to access EEPROM. Definitions provided by frontends.
//
// A frontend has to do conversion of EEPROM data to guarantee the save data
// format matches the PS5V2 version. The code directly stores structs with
// bitfields in EEPROM, so layout can't be guaranteed to match the original's.
// The bitfields might mismatch the original's, even on big endian
// architectures. Though testing of a frontend on a big endian archicture could
// be done, and if the format ends up matching the original's, the conversion
// could be omitted.
#include <stddef.h>
#include <stdint.h>

uint8_t ReadEeprom(uint8_t offset, void* destination, size_t size);
void WriteEeprom(uint8_t offset, void* source, size_t size);
