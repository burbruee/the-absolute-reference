#pragma once

#include <stdint.h>

uint8_t EepromReadAccess(uint8_t i);
#define EEPROM_READ_ACCESS(i) EepromReadAccess(i)

void EepromWriteAccess(uint8_t i, uint8_t value);
#define EEPROM_WRITE_ACCESS(i, value) EepromWriteAccess(i, value)