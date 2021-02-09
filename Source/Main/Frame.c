#include "Main/Frame.h"
#include "Main/MainLoop.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowGameStatus.h"
#include "Game/Graphics/Entity.h"
#include "Video/GameBg.h"
#include "Game/RegionWarning.h"
#include "Input/Button.h"
#include "Input/Input.h"
#include "Video/Video.h"
#include "Video/UnknownSprite.h"
#include "Sound/Sound.h"
#include "PlatformUpdate.h"

PauseMode CurrentPauseMode = PAUSEMODE_NOPAUSE;

bool ScreenTimeOdd = false;
uint32_t ScreenTime = 0u;
uint32_t Uptime = 0u;
uint32_t DemoWaitTime = 0u;
uint16_t UNK_606564C = 0u;

uint32_t RandScale;

// TODO: Based on the fact there's a long gap of unused bytes between RandScale
// and TestModeDisabled, data after RandScale might be more appropriate in
// another .c/.h.

bool TestModeDisabled = false;

uint16_t UNK_6060024 = 0u;
uint32_t UNK_6060028 = 0u;
uint32_t NumVblanks = 0u;
bool VblankFinished = false;

void UNK_602ACB0() {
	UNK_FFFFFE10 = 1u;
	UNK_FFFFFE12 = 0u;
	UNK_FFFFFE13 = 0u;
	UNK_FFFFFE11 = 1u;
	UNK_FFFFFE16 = 2u;
	UNK_FFFFFE14 = 0xFFu;
	UNK_FFFFFE15 = 0xFFu;
	UNK_6060024 = 0xFFFFu;
	UNK_6060028 = 0u;
}

void UNK_602ACE0(uint16_t arg0) {
	UNK_FFFFFE10 = 1u;
	UNK_FFFFFE12 = 1u;
	UNK_FFFFFE13 = 0u;
	UNK_FFFFFE14 = (uint8_t)(arg0 >> 8);
	UNK_FFFFFE15 = (uint8_t)(arg0 >> 0);
	UNK_FFFFFE10 = 0u;
	UNK_6060024 = arg0;
	UNK_6060028 = 0u;
}

bool UpdateFrame() {
	ScreenTimeOdd = ScreenTime % 2u;
	UpdateInputs();
	InitSpriteLayers();
	UNK_60237DE();
	UNK_6024244();
	UpdateEntities();
	UNK_6025078();
	UNK_602523C();
	UpdateGameBg();
	UNK_602C5C2();
	WriteSpriteLayers();
	UNK_602DAD4();
	NumSprites = SPRITE_FIRST;

	do {
		if (!PauseAllowed) {
			goto noPause;
		}
		RandScale = 0u;
		NumVblanks = 0u;
		PLATFORM_FRAME();
	} while (!(SystemButtonsDown[PLAYER1] & BUTTON_START));
	UpdatePlayers();

	noPause:
	UNK_602BA70();
	RandScale = 0u;
	PLATFORM_FRAME();

	SetVideo();
	UNK_602AA64();
	UpdatePalCycles();
	PLATFORM_FINISHUPDATE();

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

bool UNK_602AECA() {
	ScreenTimeOdd = ScreenTime % 2u;
	UpdateInputs();
	InitSpriteLayers();
	UpdatePlayers();
	UNK_6025078();
	UNK_602523C();
	UNK_602C5C2();
	WriteSpriteLayers();
	UNK_602DAD4();
	NumSprites = SPRITE_FIRST;

	do {
		if (!PauseAllowed) {
			goto noPause;
		}
		UpdateInputs();
		RandScale = 0u;
		NumVblanks = 0u;
		PLATFORM_FRAME();
	} while (!(SystemButtonsDown[PLAYER1] & BUTTON_START));
	UpdatePlayers();

	noPause:
	UNK_602BA70();
	RandScale = 0u;
	PLATFORM_FRAME();

	SetVideo();
	UNK_602AA64();
	UpdatePalCycles();
	PLATFORM_FINISHUPDATE();

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
	UNK_60237DE();
	UNK_6024244();
	ShowPlayersStatus();
	UpdateGameBg();
	UNK_602C5C2();
	WriteSpriteLayers();
	UNK_6025078();
	NumSprites = SPRITE_FIRST;
	ScreenTime++;
	Uptime++;
	DemoWaitTime++;
	UNK_602523C();
	UNK_602DAD4();

	do {
		if (!PauseAllowed) {
			goto noPause;
		}
		UpdateInputs();
		RandScale = 0u;
		NumVblanks = 0u;
		PLATFORM_FRAME();
	} while (!(SystemButtonsDown[PLAYER1] & BUTTON_START));
	UpdatePlayers();

	noPause:
	UNK_602BA70();
	RandScale = 0u;
	PLATFORM_FRAME();

	SetVideo();
	UNK_602AA64();
	UpdatePalCycles();
	PLATFORM_FINISHUPDATE();

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
	UNK_60237DE();
	UpdatePlayers();

	GameButtonsDown[PLAYER1] = systemButtons1p;
	GameButtonsDown[PLAYER2] = systemButtons2p;
	SystemButtonsDown[PLAYER1] = systemButtons1p;
	SystemButtonsDown[PLAYER2] = systemButtons2p;
	UpdateEntities();
	ShowPlayers();
	UNK_6024244();
	ShowPlayersStatus();
	UpdateGameBg();
	UNK_602C5C2();
	WriteSpriteLayers();
	UNK_6025078();
	NumSprites = SPRITE_FIRST;
	ScreenTime++;
	Uptime++;
	DemoWaitTime++;
	UNK_602523C();
	UNK_602DAD4();

	do {
		if (!PauseAllowed) {
			goto noPause;
		}
		UpdateInputs();
		RandScale = 0u;
		NumVblanks = 0u;
		PLATFORM_FRAME();
	} while (!(SystemButtonsDown[PLAYER1] & BUTTON_START));
	UpdatePlayers();

	noPause:
	UNK_602BA70();
	RandScale = 0u;
	PLATFORM_FRAME();

	SetVideo();
	UNK_602AA64();
	UpdatePalCycles();
	PLATFORM_FINISHUPDATE();

	if (MainLoop == MAINLOOP_TEST || TestModeDisabled || (INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
		return false;
	}
	else {
		return true;
	}
}
