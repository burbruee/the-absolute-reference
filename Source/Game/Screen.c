#include "Game/Screen.h"
#include "Eeprom/Eeprom.h"

ScreenState Screen;

const uint16_t DefaultSettings[NUMSETTINGS] = {
	SCREEN_MEMCHECK,
	1u,
	1u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	2u,
};

static const uint8_t SettingsMaxValues[NUMSETTINGS - 1u] = {
	SCREEN_COPYRIGHT,
	2u,
	9u,
	0u,
	COINSLOT_INDIVIDUAL,
	COINMODE_DOUBLE,
	2u,
	8u,
	8u,
	0u
};

bool SettingsValid() {
	bool valid = true;

	int32_t checksum = 0;
	for (size_t i = 0u; i < NUMSETTINGS - 1u; i++) {
		checksum += (int16_t)Settings[i];
	}

	if (checksum == (int16_t)Settings[SETTING_SETTINGSCHECKSUM]) {
		for (size_t i = 0u; i < NUMSETTINGS - 1u; i++) {
			if ((int16_t)Settings[i] > SettingsMaxValues[i]) {
				valid = false;
				break;
			}
		}
	}

	if (!valid) {
		InitSettings();
		SaveSettings();
	}
	return valid;
}

void InitSettings() {
	for (size_t i = 0u; i < NUMSETTINGS; i++) {
		Settings[i] = DefaultSettings[i];
	}
}

void SaveSettings() {
	uint16_t checksum = 0u;
	for (size_t i = 0u; i < NUMSETTINGS - 1u; i++) {
		checksum += Settings[i];
	}
	Settings[SETTING_SETTINGSCHECKSUM] = checksum;
	WriteSettings();
}
