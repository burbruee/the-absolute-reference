#include "Screen.h"
#include "Setting.h"
#include "Eeprom.h"

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
