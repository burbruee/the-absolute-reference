// TODO: Change how EEP-ROM access is done in the code and move this to the PsikyoSH platform.
#include "Eeprom/Eeprom.h"
#include "Eeprom/Setting.h"
#include "Lib/Macros.h"
#include "EepromAccess.h"
#include "HwData.h"
#include <stdbool.h>

// NOTE: Don't remove the empty loops after EEP-ROM accesses in here; the
// PsikyoSH hardware probably requires some wait cycles between EEP-ROM
// accesses for the hardware to update.

// TODO: Remove the return statements at the start of these functions and get EEP-ROM support to work.

void PushEepromBit(bool bit) {
	return;
	EEPROM_WRITE_ACCESS(0u, ((uint8_t)bit << 5) | 0x82u);
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, ((uint8_t)bit << 5) | 0xC2u);
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, ((uint8_t)bit << 5) | 0x82u);
	for (uint8_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, ((uint8_t)bit << 5) | 0x82u);
	for (uint8_t i = 0u; i < 3u; i++);
}

void UNK_602F704() {
	return;
	EEPROM_WRITE_ACCESS(0u, 0xA2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0xE2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0x82u);
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(1);
	PushEepromBit(1);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(false);
	}

	EEPROM_WRITE_ACCESS(0u, 0xC2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0x02u);
	for (uint32_t i = 0u; i < 3u; i++);
}

void UNK_602F7DC() {
	return;
	EEPROM_WRITE_ACCESS(0u, 0xA2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0xE2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0x82u);
	for (uint32_t i = 0u; i < 3u; i++);

	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(0);
	PushEepromBit(0);

	for (uint32_t i = 0u; i < 6u; i++) {
		PushEepromBit(0);
	}

	EEPROM_WRITE_ACCESS(0u, 0xC2u);
	for (uint32_t i = 0u; i < 3u; i++);

	EEPROM_WRITE_ACCESS(0u, 0x02u);
	for (uint32_t i = 0u; i < 3u; i++);
}

void UNK_602F8BC() {
	return;
	for (uint8_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM_WRITE_ACCESS(0u, 0xA2u);
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM_WRITE_ACCESS(0u, 0xE2u);
		for (uint8_t j = 0u; j < 3u; j++);
		EEPROM_WRITE_ACCESS(0u, 0x82u);
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

		EEPROM_WRITE_ACCESS(0u, 0x82u);
		for (bool j = false; !j; j = true);
		EEPROM_WRITE_ACCESS(0u, 0xC2u);
		for (bool j = false; !j; j = true);

		for (uint8_t j = 0u; j < 8u; j++) {
			if (EEPROM_READ_ACCESS(0u) & 0x10u) {
				Settings[i] |= 1 << (7u - j);
			}

			EEPROM_WRITE_ACCESS(0u, 0x82u);
			for (bool k = false; !k; k = true);
			EEPROM_WRITE_ACCESS(0u, 0xC2u);
			for (bool k = false; !k; k = true);
		}
		EEPROM_WRITE_ACCESS(0u, 0x42u);
	}
}

void UNK_602FA7E() {
	return;
	UNK_602F704();

	EEPROM_WRITE_ACCESS(0u, 0xA2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0xE2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0x82u);
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

	EEPROM_WRITE_ACCESS(0u, 0x42u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0x02u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0xC2u);
	while (!(EEPROM_READ_ACCESS(0u) & 0x10)) {
		EEPROM_WRITE_ACCESS(0u, EEPROM_READ_ACCESS(0u) | 0x82u);
		for (uint8_t i = 0u; i < 3u; i++);
	}
	EEPROM_WRITE_ACCESS(0u, 0x42u);

	UNK_602F7DC();
}

void WriteSettings() {
	return;
    UNK_602F704();

	for (uint32_t i = 0u; i < lengthof(Settings); i++) {
		EEPROM_WRITE_ACCESS(0u, 0xA2u);
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM_WRITE_ACCESS(0u, 0xE2u);
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM_WRITE_ACCESS(0u, 0x82u);
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

		EEPROM_WRITE_ACCESS(0u, 0x42u);
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM_WRITE_ACCESS(0u, 0x02u);
		for (uint8_t i = 0u; i < 3u; i++);
		EEPROM_WRITE_ACCESS(0u, 0xC2u);
		while (!(EEPROM_READ_ACCESS(0u) & 0x10u));
		EEPROM_WRITE_ACCESS(0u, 0x42u);
    }

    UNK_602F7DC();
}

uint8_t ReadEepromByte(uint8_t offset) {
	return 0u;
	EEPROM_WRITE_ACCESS(0u, 0xA2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0xE2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0x82u);
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

	EEPROM_WRITE_ACCESS(0u, 0x82u);
	for (bool i = false; !i; i = true);
	EEPROM_WRITE_ACCESS(0u, 0xC2u);
	for (bool i = false; !i; i = true);

	uint8_t byte = 0x00u;
	for (uint8_t i = 0u; i < 8u; i++) {
		if ((EEPROM_READ_ACCESS(0u) & 0x10u)) {
			byte |= 1u << (7u - i);
		}

		EEPROM_WRITE_ACCESS(0u, 0x82u);
		for (bool j = false; !j; j = true);
		EEPROM_WRITE_ACCESS(0u, 0xC2u);
		for (bool j = false; !j; j = true);
	}
	EEPROM_WRITE_ACCESS(0u, 0x42u);

	return byte;
}

void ReadEeprom(uint8_t offset, void* destination, size_t size) {
	return;
    uint8_t* dstPtr = destination;
	for (size_t i = 0u; i < size; i++) {
        *dstPtr++ = ReadEepromByte(offset + i);
    }
}

void WriteEepromByte(uint8_t offset, uint8_t data) {
	return;
	UNK_602F704();

	EEPROM_WRITE_ACCESS(0u, 0xA2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0xE2u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0x82u);
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

	EEPROM_WRITE_ACCESS(0u, 0x42u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0x02u);
	for (uint8_t i = 0u; i < 3u; i++);
	EEPROM_WRITE_ACCESS(0u, 0xC2u);
	while (!(EEPROM_READ_ACCESS(0u) & 0x10u));
	EEPROM_WRITE_ACCESS(0u, 0x42u);

	UNK_602F7DC();
}

void WriteEeprom(uint8_t offset, void* source, size_t size) {
	return;
	uint8_t* srcPtr = source;
    for (size_t i = 0; i < size; i++) {
        WriteEepromByte(offset + i, *srcPtr++);
    }
}
