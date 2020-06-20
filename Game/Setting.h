#pragma once

#include <stdint.h>

typedef enum ScreenMode {
	SCREENMODE_FLIP = 1
} ScreenMode;

typedef enum CoinSlot {
	COINSLOT_SAME,
	COINSLOT_INDIVIDUAL
} CoinSlot;

typedef enum CoinMode {
	COINMODE_NORMAL,
	// NOTE: Unavailable in the setting screen. Makes games cost two credits,
	// instead of one. One service can still be used to start a game, though.
	// The CheckBuyGame function has an argument that can force normal coin
	// mode if it's currently double coin mode.
	COINMODE_DOUBLE,
	COINMODE_FREE
} CoinMode;

typedef enum Setting {
	SETTING_SCREENMODE,
	SETTING_NUMVERSUSROUNDS,
	SETTING_MAXVERSUSSECTION,
	SETTING_INDEX3,
	SETTING_INDEX4,
	SETTING_COINSLOT,
	SETTING_COINMODE,
	SETTING_PRICE1P,
	SETTING_PRICE2P,
	SETTING_INDEX9,
	SETTING_SETTINGSCHECKSUM,

	// The number of actually-used settings. The number of available settings
	// is fixed at 16, due to the limited space in EEP-ROM.
	NUMSETTINGS
} Setting;
extern uint16_t Settings[16];
