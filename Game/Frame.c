#include "Frame.h"
#include "Player.h"
#include "ShowGameStatus.h"
#include "Entity.h"
#include "GameBg.h"
#include "Warning.h"
#include "Button.h"
#include "Input.h"
#include "Video.h"
#include "Loop.h"
#include "Sound.h"
#include "UnknownSprite.h"
#include "Vblank.h"

PauseMode CurrentPauseMode = PAUSEMODE_NOPAUSE;

bool ScreenTimeOdd;
uint32_t ScreenTime;
uint32_t Uptime;
uint32_t DemoWaitTime;
uint16_t _0x606564C;

uint32_t RandScale;

// TODO: Based on the fact there's a long gap of unused bytes between RandScale
// and TestModeDisabled, data after RandScale might be more appropriate in
// another .c/.h.

bool TestModeDisabled;

uint16_t _0x6060024;
uint32_t _0x6060028;
uint32_t NumVblanks;
bool VblankFinished;

void _0x602ACB0() {
	_0x6060024 = 0xFFFFu;
	_0x6060028 = 0u;
	_0xFFFFFE10 = 1u;
	_0xFFFFFE11 = 1u;
	_0xFFFFFE12 = 0u;
	_0xFFFFFE13 = 0u;
	_0xFFFFFE14 = 0xFFu;
	_0xFFFFFE15 = 0xFFu;
	_0xFFFFFE16 = 2u;
}

bool UpdateFrame() {
	ScreenTimeOdd = ScreenTime % 2u;
	UpdateInputs();
	InitSpriteLayers();
	_0x60237DE();
	_0x6024244();
	UpdateEntities();
	_0x6025078();
	_0x602523C();
	UpdateGameBg();
	_0x602C5C2(); // TODO
	WriteSpriteLayers();
	_0x602DAD4(); // TODO
	NumSprites = SPRITE_FIRST;

	ADVANCE_FRAME();

	bool startTestMode;
	if (MainLoop == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		startTestMode = false;
	}
	else {
		startTestMode = true;
	}

	ScreenTime++;
	Uptime++;
	DemoWaitTime++;

	return startTestMode;
}

bool _0x602AECA() {
	ScreenTimeOdd = ScreenTime % 2u;
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
	if (MainLoop == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		startTestMode = false;
	}
	else {
		startTestMode = true;
	}

	ScreenTime++;
	Uptime++;
	DemoWaitTime++;

	return startTestMode;
}

bool UpdateGame() {
	UpdateInputs();
	ScreenTimeOdd = ScreenTime % 2u;
	InitSpriteLayers();
	UpdateEntities();
	UpdatePlayers();
	ShowPlayers();
	_0x60237DE();
	_0x6024244();
	ShowPlayersStatus();
	UpdateGameBg();
	_0x602C5C2();
	WriteSpriteLayers();
	_0x6025078();
	NumSprites = SPRITE_FIRST;
	ScreenTime++;
	Uptime++;
	DemoWaitTime++;
	_0x602523C();
	_0x602DAD4();

	ADVANCE_FRAME();

	if (MainLoop == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}

static ButtonInput DemoButtonsOld[NUMPLAYERS] = { BUTTON_NONE, BUTTON_NONE };

bool UpdateDemo(ButtonInput* buttonsDown1p, ButtonInput* buttonsDown2p) {
	UpdateInputs();
	ButtonInput systemButtons2p = SystemButtonsDown[PLAYER2];
	ButtonInput systemButtons1p = SystemButtonsDown[PLAYER1];
	ScreenTimeOdd = ScreenTime % 2u;
	SystemButtonsDown[PLAYER1] = *buttonsDown1p;
	SystemButtonsDown[PLAYER2] = *buttonsDown2p;
	GameButtonsDown[PLAYER1] = *buttonsDown1p;
	GameButtonsDown[PLAYER2] = *buttonsDown2p;
	GameButtonsNew[PLAYER1] = ~DemoButtonsOld[PLAYER1] & GameButtonsDown[PLAYER1];
	GameButtonsNew[PLAYER2] = ~DemoButtonsOld[PLAYER2] & GameButtonsDown[PLAYER2];
	DemoButtonsOld[PLAYER1] = GameButtonsDown[PLAYER1];
	DemoButtonsOld[PLAYER2] = GameButtonsDown[PLAYER2];

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
	UpdateGameBg();
	_0x602C5C2();
	WriteSpriteLayers();
	_0x6025078();
	NumSprites = SPRITE_FIRST;
	ScreenTime++;
	Uptime++;
	DemoWaitTime++;
	_0x602523C();
	_0x602DAD4();

	ADVANCE_FRAME();

	if (MainLoop == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}
