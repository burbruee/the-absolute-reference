#include "Frame.h"
#include "Player.h"
#include "ShowGameStatus.h"
#include "Entity.h"
#include "Bg.h"
#include "Warning.h"
#include "Button.h"
#include "Input.h"
#include "Video.h"
#include "Loop.h"
#include "Sound.h"
#include "UnknownSprite.h"
#include "Vblank.h"

PauseMode CurrentPauseMode = PAUSEMODE_NOPAUSE;

bool NumScreenFramesOdd;

uint32_t NumScreenFrames;

uint32_t RandScale;

// TODO: Based on the fact there's a long gap of unused bytes between RandScale
// and TestModeDisabled, data after RandScale might be more appropriate in
// another .c/.h.

bool TestModeDisabled;

uint16_t _0x6060024;
uint32_t _0x6060028;
// TODO: Replace all vblank data with macros, so atomic types can be used.
volatile uint32_t NumVblanks;
volatile bool VblankFinished;

uint32_t _0x6065644;
uint32_t _0x6065648;

bool UpdateFrame() {
	NumScreenFramesOdd = NumScreenFrames % 2u;
	UpdateInputs();
	_0x60237DE();
	_0x6024244();
	UpdateEntities();
	_0x6025078();
	_0x602523C();
	UpdateBg();
	_0x602C5C2();
	WriteSpriteLayers();
	_0x602DAD4();
	NumSprites = SPRITE_FIRST;

	ADVANCE_FRAME();

	bool startTestMode;
	if (CurrentMainLoopState == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		startTestMode = false;
	}
	else {
		startTestMode = true;
	}

	NumScreenFrames++;
	_0x6065644++;
	_0x6065648++;

	return startTestMode;
}

bool _0x602AECA() {
	NumScreenFramesOdd = NumScreenFrames % 2u;
	UpdateInputs();
	InitSpriteLayers();
	UpdatePlayers();
	_0x6025078();
	_0x602523C();
	_0x602C5C2();
	WriteSpriteLayers();
	_0x602DAD4();
	NumSprites = SPRITE_FIRST;

	ADVANCE_FRAME();

	bool startTestMode;
	if (CurrentMainLoopState == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		startTestMode = false;
	}
	else {
		startTestMode = true;
	}

	NumScreenFrames++;
	_0x6065644++;
	_0x6065648++;

	return startTestMode;
}

bool UpdateGame() {
	UpdateInputs();
	NumScreenFramesOdd = NumScreenFrames % 2u;
	InitSpriteLayers();
	UpdateEntities();
	UpdatePlayers();
	ShowPlayers();
	_0x60237DE();
	_0x6024244();
	ShowPlayersStatus();
	UpdateBg();
	_0x602C5C2();
	WriteSpriteLayers();
	_0x6025078();
	NumSprites = SPRITE_FIRST;
	NumScreenFrames++;
	_0x6065644++;
	_0x6065648++;
	_0x602523C();
	_0x602DAD4();

	ADVANCE_FRAME();

	if (CurrentMainLoopState == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}

static ButtonInput AttractButtonsOld[NUMPLAYERS] = { BUTTON_NONE, BUTTON_NONE };

bool UpdateAttract(ButtonInput* buttonsDown1p, ButtonInput* buttonsDown2p) {
	UpdateInputs();
	ButtonInput systemButtons2p = SystemButtonsDown[PLAYER2];
	ButtonInput systemButtons1p = SystemButtonsDown[PLAYER1];
	NumScreenFramesOdd = NumScreenFrames % 2u;
	SystemButtonsDown[PLAYER1] = *buttonsDown1p;
	SystemButtonsDown[PLAYER2] = *buttonsDown2p;
	GameButtonsDown[PLAYER1] = *buttonsDown1p;
	GameButtonsDown[PLAYER2] = *buttonsDown2p;
	GameButtonsNew[PLAYER1] = ~AttractButtonsOld[PLAYER1] & GameButtonsDown[PLAYER1];
	GameButtonsNew[PLAYER2] = ~AttractButtonsOld[PLAYER2] & GameButtonsDown[PLAYER2];
	AttractButtonsOld[PLAYER1] = GameButtonsDown[PLAYER1];
	AttractButtonsOld[PLAYER2] = GameButtonsDown[PLAYER2];

	InitSpriteLayers();
	_0x60237DE();
	UpdatePlayers();
	GameButtonsDown[PLAYER1] = systemButtons1p;
	GameButtonsDown[PLAYER2] = systemButtons2p;
	SystemButtonsDown[PLAYER1] = systemButtons1p;
	SystemButtonsDown[PLAYER2] = systemButtons2p;
	UpdateEntities();
	ShowPlayers();
	_0x6024244();
	ShowPlayersStatus();
	_0x6025078();
	NumSprites = SPRITE_FIRST;
	NumScreenFrames++;
	_0x6065644++;
	_0x6065648++;
	_0x602523C();
	_0x602DAD4();

	ADVANCE_FRAME();

	if (CurrentMainLoopState == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}
