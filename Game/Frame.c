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
#include "Unknown.h"
#include "UnknownSprite.h"

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
volatile uint32_t NumVblanks;
volatile bool VblankFinished;

uint32_t _0x6065644;
uint32_t _0x6065648;

bool UpdateFrame() {
	NumScreenFramesOdd = !!(NumScreenFrames % 2);
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

	// TODO: Refactor into a PAUSE() macro, and use in UpdateFrame, _0x602AECA,
	// UpdateGame, and UpdateAttract?
	while (PauseAllowed) {
		UpdateInputs();

		RandScale = 0u;
		NumVblanks = 0u;
		while (IRQCTRL[1] & 1) {
			RandScale++;
		}
		VblankFinished = false;
		while (!VblankFinished) {
			RandScale++;
		}
		VblankFinished = false;

		if (SystemButtonsDown[PLAYER1] & BUTTON_START) {
			UpdatePlayers();
			break;
		}
	}

	_0x602BA70();
	RandScale = 0u;
	while (IRQCTRL[1] & 1) {
		RandScale++;
	}
	VblankFinished = false;
	while (!VblankFinished) {
		RandScale++;
	}

	SetVideo();
	_0x602AA64();
	UpdatePalCycles();

	VblankFinished = false;
	IRQCTRL[1] |= 1;

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

bool _0x602AECA();

bool UpdateGame() {
	UpdateInputs();
	NumScreenFramesOdd = NumScreenFrames & 1;
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

	while (PauseAllowed) {
		UpdateInputs();

		RandScale = 0u;
		NumVblanks = 0u;
		while (IRQCTRL[1] & 1) {
			RandScale++;
		}
		VblankFinished = false;
		while (!VblankFinished) {
			RandScale++;
		}
		VblankFinished = false;

		if (SystemButtonsDown[PLAYER1] & BUTTON_START) {
			UpdatePlayers();
			break;
		}
	}

	_0x602BA70();
	RandScale = 0u;
	while (IRQCTRL[1] & 0x01) {
		RandScale++;
	}
	while (!VblankFinished) {
		RandScale++;
	}

	SetVideo();
	_0x602AA64();
	UpdatePalCycles();

	VblankFinished = false;
	IRQCTRL[1] |= 1;

	if (CurrentMainLoopState == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}

bool UpdateAttract(ButtonInput* buttonsDown1p, ButtonInput* buttonsDown2p);
