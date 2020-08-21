#include "Game/Play/Debug.h"
#include "Input/Button.h"
#include "Sound/Sound.h"
#include "Eeprom/Setting.h"
#include <stdint.h>

bool Debug;

static uint8_t NumDebugCodeInputs;

typedef struct DebugCodeInput {
	int8_t playerNum;
	ButtonInput button;
} DebugCodeInput;

static const DebugCodeInput DebugCode[17] = {
	{PLAYER1, BUTTON_3},
	{PLAYER1, BUTTON_3},
	{PLAYER1, BUTTON_1},
	{PLAYER1, BUTTON_3},
	{PLAYER2, BUTTON_1},
	{PLAYER2, BUTTON_1},
	{PLAYER2, BUTTON_3},
	{PLAYER2, BUTTON_1},
	{PLAYER1, BUTTON_2},
	{PLAYER1, BUTTON_2},
	{PLAYER2, BUTTON_2},
	{PLAYER2, BUTTON_2},
	{PLAYER2, BUTTON_3},
	{PLAYER1, BUTTON_1},
	{PLAYER1, BUTTON_3},
	{PLAYER2, BUTTON_3},
	{     -1, BUTTON_1},
};

void InitDebugCode() {
	NumDebugCodeInputs = 0u;
}

bool UpdateDebugCode() {
	const DebugCodeInput* debugCodeInput = &DebugCode[NumDebugCodeInputs];

	if (Settings[SETTING_COINMODE] == COINMODE_FREE && (GameButtonsNew[PLAYER1] || GameButtonsNew[PLAYER2])) {
		if (GameButtonsNew[debugCodeInput->playerNum] == debugCodeInput->button) {
			NumDebugCodeInputs++;
			debugCodeInput++;
		}
		else {
			NumDebugCodeInputs = 0u;
		}

		if (debugCodeInput->playerNum < 0) {
			NumDebugCodeInputs = 0u;
			Debug = true;
			PlaySoundEffect(SOUNDEFFECT_MEDALUP);
			return true;
		}
	} 

	return false;
}
