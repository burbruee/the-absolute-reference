#include "Frame.h"
#include "Warning.h"
#include "Button.h"
#include "Input.h"
#include "Video.h"
#include "Loop.h"

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
uint32_t NumVBlanks;
bool VBlankFinished;

bool UpdateFrame();

bool _0x602AECA();

bool UpdateGameFrame() {
	UpdateInputs();

	NumScreenFramesOdd = NumScreenFrames & 1;

	InitSpriteLayers();
	UpdateEntities();
	UpdatePlayers();
	ShowPlayers();
	_0x60237DE();
	_0x6024244();
	_0x6010666();
	UpdateBG();
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
		NumVBlanks = 0u;

		// TODO: Check when IRQCTRL[1] & 0x01 becomes false. I assume it's when
		// a frame starts. Once known, define and use relevant constants.
		// TODO: Add documentation for StepRandScaleWhile, and implement as a
		// simple increment macro for PsikyoSH.  For OS platforms, a small time
		// delay between increments of RandScale would reproduce the PsikyoSH
		// algorthm's properties, while keeping CPU usage down.  Analyze the
		// amount RandScale increases by per frame in TAP, then try and
		// reproduce that for OS platforms. And allow StepRandScaleWhile to
		// delay an arbitrary amount of time for OS platforms, so longer sleeps
		// can be used to keep CPU usage down.
		StepRandScaleWhile(IRQCTRL[1] & 0x01);
		VBlankFinished = false;
		StepRandScaleWhile(!VBlankFinished);
		VBlankFinished = false;

		if (SystemButtonsDown[PLAYER1] & BUTTON_START) {
			UpdatePlayers();
			break;
		}
	}

	_0x602BA70();
	RandScale = 0u;

	while (IRQCTRL[1] & 0x01) {
		StepRandScale();
	}
	while (!VBlankFinished) {
		StepRandScale();
	}

	UpdateVisuals();
	_0x602AA64();
	UpdatePalCycles();

	VBlankFinished |= true;

	if (CurrentMainLoopState != MAINLOOP_TEST && !TestModeDisabled && !(INPUTPORTS[INPUT_SERVICE] & SERVICE_TEST)) {
		// Enter test mode only if it's not currently test mode, entering test mode is enabled, and the test button was pressed.
		return true;
	}
	else {
		return false;
	}
}

bool UpdateAttractFrame(ButtonInput* p1ButtonsDown, ButtonInput* p2ButtonsDown);
