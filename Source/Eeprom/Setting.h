#pragma once

#include <stdint.h>

typedef enum ScreenMode {
	SCREENMODE_NORMAL,
	SCREENMODE_FLIP
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
	SETTING_ENABLESOUND,
	SETTING_DEMOSOUND,
	SETTING_COINSLOT,
	SETTING_COINMODE,
	SETTING_PRICE1P,
	SETTING_PRICE2P,
	SETTING_INDEX9,
	SETTING_SETTINGSCHECKSUM,

	// The number of actually-used settings. The number of saved settings is
	// fixed at 16, due to the limited space in EEP-ROM, though 20 settings are
	// used by the code in some spots.
	NUMSETTINGS
} Setting;

// Although this is an array of uint16_t, WriteSettings() only writes the low
// eight bits of each index to EEP-ROM. But feel free to use the extra eight
// high bits for settings data that aren't saved.
extern uint16_t Settings[20];
