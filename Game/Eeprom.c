// TODO: Change how EEP-ROM access is done in the code and move this to the PsikyoSH platform.
#include "Eeprom.h"
#include "Setting.h"
#include "HwData.h"
#include <stdbool.h>

// NOTE: Don't remove the empty loops after EEP-ROM accesses in here; the
// PsikyoSH hardware probably requires some wait cycles between EEP-ROM
// accesses for the hardware to update.

void PushEepromBit(bool bit) {
	EEPROM[0] = (bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (bit << 5) | 0xC2u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = (bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);
}

void _0x602F704() {
	EEPROM[0] = 0xA2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0xE2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x82;
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(false);
	PushEepromBit(false);
	PushEepromBit(true);
	PushEepromBit(true);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(false);
	}

	EEPROM[0] = 0xC2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x02;
	for (uint32_t i = 0u; i < 3u; i++);
}

void _0x602F7DC() {
	EEPROM[0] = 0xA2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0xE2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x82;
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(false);
	PushEepromBit(false);
	PushEepromBit(false);
	PushEepromBit(false);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(false);
	}

	EEPROM[0] = 0xC2;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x02;
	for (uint32_t i = 0u; i < 3u; i++);
}

void _0x602F8BC() {
	for (uint8_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM[0] = 0xA2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0xE2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0x82u;
		for (uint8_t j = 0u; j < 3u; j++);

		PushEepromBit(1u);
		PushEepromBit(0u);
		PushEepromBit(0u);
		for (uint8_t j = 0u; j < 8u; j++) {
			if ((i >> (7u - j)) & 1u) {
				PushEepromBit(1u);
			}
			else {
				PushEepromBit(0u);
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

void _0x602FA7E() {
	// TODO
}

void WriteSettings() {
	// TODO
}

void ReadEepromByte(uint8_t offset) {
	// TODO
}

uint8_t ReadEeprom(uint8_t offset, void* destination, size_t size) {
	// TODO
	return 0u;
}

void WriteEepromByte(uint8_t offset, uint8_t data) {
	// TODO
}

void WriteEeprom(uint8_t offset, void* source, size_t size) {
	// TODO
}
