#pragma once

#include <stdint.h>

// TODO: Add support for region setting. PsikyoSH main() has some hints as to
// how it works.

// Supported players.
// Always use NUMPLAYERS in code handling all players, so more players can be
// added here. But if you add players here, you'll break compatibility with the
// PS5V2.
typedef enum PlayerNum {
	PLAYER1,
	PLAYER2,
	NUMPLAYERS
} PlayerNum;

// Input categories. Adding categories will break PS5V2 compatibility.
typedef enum Input {
	INPUT_1PBUTTONS,
	INPUT_2PBUTTONS,
	INPUT_UNUSED,
	INPUT_SERVICE,
	NUMINPUTS
} Input;

// Player buttons. Adding buttons will break PS5V2 compatibility.
typedef enum ButtonInput {
	BUTTON_NONE = 0,
	BUTTON_START = 1 << 0,
	BUTTON_3 = 1 << 1,
	BUTTON_2 = 1 << 2,
	BUTTON_1 = 1 << 3,
	BUTTON_LEFT = 1 << 4,
	BUTTON_RIGHT = 1 << 5,
	BUTTON_DOWN = 1 << 6,
	BUTTON_UP = 1 << 7
} ButtonInput;

// Extra inputs that aren't player buttons.
// Only one more service input can be added, unless the type of the Inputs[]
// array is changed to a type larger than uint8_t; the change might break
// compatibility with the PS5V2, unless the extra input can be wired onto the
// PCB, but no more inputs could be added.
typedef enum ServiceInput {
	SERVICE_NONE = 0,
	SERVICE_COIN1 = PLAYER1 + 1,
	SERVICE_COIN2 = PLAYER2 + 1,
	SERVICE_ADDSERVICE = 1 << (NUMPLAYERS + 0),
	SERVICE_TEST = 1 << (NUMPLAYERS + 3),
	SERVICE_TILT = 1 << (NUMPLAYERS + 4) // TODO: Consider renaming to SERVICE_PAUSE.
} ServiceInput;
