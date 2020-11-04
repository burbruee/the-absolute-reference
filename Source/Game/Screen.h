#pragma once

#include "Eeprom/Setting.h"
#include "Lib/Macros.h"
#include <stdbool.h>

typedef enum ScreenState {
	SCREEN_MEMCHECK,
	SCREEN_COPYRIGHT,
	SCREEN_TITLE,
	SCREEN_DEVELOPER,
	SCREEN_TWINDEMO,
	SCREEN_VERSUSDEMO,
	SCREEN_DOUBLESDEMO,
	SCREEN_NORMALRANKING,
	SCREEN_MASTERRANKING,
	SCREEN_MASTERSECTIONTIMERANKING,
	SCREEN_DOUBLESRANKING,
	SCREEN_TESTMODE,
	SCREEN_VERSIONTITLE,
	SCREEN_GAME,
	SCREEN_NULL = 0
} ScreenState;
extern ScreenState Screen;

extern const uint16_t DefaultSettings[NUMSETTINGS];

// TODO
bool SettingsValid();
void InitSettings();
void SaveSettings();
// TODO
