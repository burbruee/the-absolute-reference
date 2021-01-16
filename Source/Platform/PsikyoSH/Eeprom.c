#include "Eeprom/Eeprom.h"
#include "Eeprom/Setting.h"
#include "Lib/Macros.h"
#include "EepromAccess.h"
#include "HwData.h"
#include <stdbool.h>
#include <assert.h>

// NOTE: Don't remove the empty loops after EEP-ROM accesses in here; the
// PsikyoSH hardware probably requires some wait cycles between EEP-ROM
// accesses for the hardware to update.

static uint8_t ReadEepromByte(uint8_t offset);
static void WriteEepromByte(uint8_t offset, uint8_t data);

static void PushEepromBit(bool bit) {
	EEPROM[0] = ((uint8_t)bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = ((uint8_t)bit << 5) | 0xC2u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = ((uint8_t)bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM[0] = ((uint8_t)bit << 5) | 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);
}

static void StartEepromWrite() {
	EEPROM[0] = 0xA2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0xE2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x82u;
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(1);
	PushEepromBit(1);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(0);
	}

	EEPROM[0] = 0xC2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x02u;
	for (uint32_t i = 0u; i < 3u; i++);
}

static void FinishEepromWrite() {
	EEPROM[0] = 0xA2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0xE2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x82u;
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(0);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(0);
	}

	EEPROM[0] = 0xC2u;
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM[0] = 0x02u;
	for (uint32_t i = 0u; i < 3u; i++);
}

void ReadSettings() {
	for (uint8_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM[0] = 0xA2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0xE2u;
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM[0] = 0x82u;
		for (uint8_t j = 0u; j < 3u; j++);

		PushEepromBit(1);
		PushEepromBit(0);
		PushEepromBit(0);
		for (uint8_t j = 0u; j < 8u; j++) {
			if ((i >> (7u - j)) & 1u) {
				PushEepromBit(1);
			}
			else {
				PushEepromBit(0);
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

void UNK_602FA7E() {
	StartEepromWrite();

	EEPROM[0] = 0xA2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0xE2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(1);

	for (uint8_t i = 0u; i < 7u; i++) {
		PushEepromBit(0);
	}

	for (uint8_t i = 0u; i < 8u; i++) {
		if (i < 2u || i > 5u) {
			PushEepromBit(1);
		}
		else {
			PushEepromBit(0);
		}
	}

	EEPROM[0] = 0x42u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0x02u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0xC2u;
	while (!(EEPROM[0] & 0x10)) {
		EEPROM[0] |= 0x82u;
		for (uint8_t i = 0u; i < 3u; i++);
	}
	EEPROM[0] = 0x42u;

	FinishEepromWrite();
}

void WriteSettings() {
    StartEepromWrite();

	for (uint32_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM[0] = 0xA2u;
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM[0] = 0xE2u;
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM[0] = 0x82u;
		for (uint8_t i = 0u; i < 3u; i++);

        PushEepromBit(0);
        PushEepromBit(1);
        PushEepromBit(0);

		for (uint8_t j = 0u; j < 8u; j++) {
			if (i >> (7 - j)) {
				PushEepromBit(1);
			}
			else {
				PushEepromBit(0);
			}
		}

		for (uint8_t j = 0u; j < 8u; j++) {
			if (Settings[i] >> (7 - j)) {
				PushEepromBit(1);
			}
			else {
				PushEepromBit(0);
			}
		}

		EEPROM[0] = 0x42u;
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM[0] = 0x02u;
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM[0] = 0xC2u;
		while (!(EEPROM[0] & 0x10u));
		EEPROM[0] = 0x42u;
    }

    FinishEepromWrite();
}

uint8_t ReadEepromByte(uint8_t offset) {
	EEPROM[0] = 0xA2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0xE2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	PushEepromBit(1);
	PushEepromBit(0);
	PushEepromBit(0);

	for (uint8_t i = 0u; i < 8u; i++) {
		if (((offset >> (7 - i)) & 1)) {
			PushEepromBit(1);
		}
		else {
			PushEepromBit(0);
		}
	}

	EEPROM[0] = 0x82u;
	for (bool i = false; !i; i = true);
	EEPROM[0] = 0xC2u;
	for (bool i = false; !i; i = true);

	uint8_t byte = 0x00u;
	for (uint8_t i = 0u; i < 8u; i++) {
		if ((EEPROM[0] & 0x10u)) {
			byte |= 1u << (7u - i);
		}

		EEPROM[0] = 0x82u;
		for (bool j = false; !j; j = true);
		EEPROM[0] = 0xC2u;
		for (bool j = false; !j; j = true);
	}
	EEPROM[0] = 0x42u;

	return byte;
}

void EepromGet(uint8_t offset, void* destination, size_t size) {
	assert(size < EEPROM_SIZE);
    uint8_t* dstPtr = destination;
	for (uint8_t i = 0u; i < size; i++) {
        *dstPtr++ = ReadEepromByte(offset + i);
    }
}

void WriteEepromByte(uint8_t offset, uint8_t data) {
	StartEepromWrite();

	EEPROM[0] = 0xA2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0xE2u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0x82u;
	for (uint8_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(1);
	PushEepromBit(0);

	for (uint8_t i = 0u; i < 8u; i++) {
		if (((offset >> (7 - i)) & 1)) {
			PushEepromBit(1);
		}
		else {
			PushEepromBit(0);
		}
	}

	for (uint8_t i = 0u; i < 8u; i++) {
		if (((data >> (7 - i)) & 1)) {
			PushEepromBit(1);
		}
		else {
			PushEepromBit(0);
		}
	}

	EEPROM[0] = 0x42u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0x02u;
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM[0] = 0xC2u;
	while (!(EEPROM[0] & 0x10u));
	EEPROM[0] = 0x42u;

	FinishEepromWrite();
}

void EepromSet(uint8_t offset, void* source, size_t size) {
	assert(size < EEPROM_SIZE);
	uint8_t* srcPtr = source;
    for (uint8_t i = 0; i < size; i++) {
        WriteEepromByte(offset + i, *srcPtr++);
    }
}
