#pragma once

#include <stdint.h>

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
	SETTING_SCREEN,
	SETTING_NUMVERSUSROUNDS,
	SETTING_MAXVERSUSSECTION,
	SETTING_INDEX3,
	SETTING_INDEX4,
	SETTING_COINSLOT,
	SETTING_COINMODE,
	SETTING_PRICE1P,
	SETTING_PRICE2P,
	SETTING_INDEX9,
	SETTING_INDEX10,
	NUMSETTINGS = 20
} Setting;
extern uint16_t Settings[NUMSETTINGS];
