#include "Eeprom.h"
#include "Setting.h"
#include "HwData.h"
#include <stdbool.h>

void _0x602F658(uint8_t arg0) {
	EEPROM[0] = (arg0 << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (arg0 << 5) | 0xC2u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (arg0 << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (arg0 << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);
}

void _0x602F8BC() {
	for (uint8_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM[0] = 0xA2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0xE2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0x82u;
		for (uint8_t j = 0u; j < 3u; j++);

		_0x602F658(1u);
		_0x602F658(0u);
		_0x602F658(0u);
		for (uint8_t j = 0u; j < 8u; j++) {
			if ((i >> (7u - j)) & 1u) {
				_0x602F658(1u);
			}
			else {
				_0x602F658(0u);
			}
		}
		Settings[i] = 0u;

		EEPROM[0] = 0x82u;
		for (bool j = false; !j; j = true);
		EEPROM[0] = 0xC2u;
		for (bool j = false; !j; j = true);

		for (uint8_t j = 0u; j < 8u; j++) {
			if (EEPROM[0] & 0x10u) {
				Settings[i] |= 1 << (7u - j);
			}

			EEPROM[0] = 0x82u;
			for (bool k = false; !k; k = true);
			EEPROM[0] = 0xC2u;
			for (bool k = false; !k; k = true);
		}
		EEPROM[0] = 0x42u;
	}
}
