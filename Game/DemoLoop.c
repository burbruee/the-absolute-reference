#include "DemoLoop.h"
#include "Coin.h"
#include "Input.h"
#include "Button.h"
#include "Video.h"
#include "Player.h"
#include "Sound.h"
#include "Frame.h"

// TODO: Just a guess, but this probably returns whether the demo loop should
// show the version title screen.
bool _0x06014234() {
	return
		AddCoin[PLAYER1] || AddCoin[PLAYER2] ||
		AddService ||
		(CanStart(PLAYER1, false) && (ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) ||
		(CanStart(PLAYER2, false) && (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)));
}

void InitDemoLoop() {
	_0x6029814(0u, 0u, 0u, 0xFFu);
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	_0x602AA4C();
	if (_0x6064750 != NULL) {
		_0x6024030(_0x6064750);
	}
	_0x6023790();
	_0x602AB9E();
	FreePalCycles(500u);
	_0x602419C();
	_0x60169DC();
	_0x602E72A(0u);
	InitPlayers();
	InitModeCodes(Players);
	InitModeCodes(Players + 1);
	_0x6064750 = NULL;
	Demo = false;
	ScreenTime = 0u;
	DemoScreen = DEMOSCREEN_START;
	DemoWaitTime = 0u;
	GameStartPlayer = STARTPLAYER_NONE;
	UpdateFrame();
	_0x602AA16();
}

MainLoopState RunDemoLoop() {
	InitDemoLoop();
	DisablePause();

	for (bool runDemoLoop = true, stopDemoLoop = UpdateFrame(); !stopDemoLoop; stopDemoLoop = UpdateFrame()) {
		switch (Screen) {
		case SCREEN_COPYRIGHT:
			Screen = StartCopyrightScreen();
			break;

		case SCREEN_TITLE:
			Screen = StartTitleScreen();
			break;

		case SCREEN_DEVELOPER:
			Screen = StartDeveloperScreen();
			if (Screen == SCREEN_GAME) {
				runDemoLoop = false;
			}
			break;

		case SCREEN_TWINDEMO:
		case SCREEN_VERSUSDEMO:
		case SCREEN_DOUBLESDEMO:
			Screen = StartDemoScreen();
			break;

		case SCREEN_NORMALRANKING:
		case SCREEN_MASTERRANKING:
		case SCREEN_MASTERSECTIONTIMERANKING:
		case SCREEN_DOUBLESRANKING:
			Screen = StartRankingScreen();
			break;

		case SCREEN_TESTMODE:
			return MAINLOOP_TEST;

		case SCREEN_VERSIONTITLE:
			Screen = StartVersionTitleScreen();
			if (Screen == SCREEN_GAME) {
				runDemoLoop = false;
			}
			break;

		case SCREEN_GAME:
			return MAINLOOP_GAME;

		default:
			Screen = SCREEN_COPYRIGHT;
			break;
		}

		if (!runDemoLoop) {
			return MAINLOOP_GAME;
		}
	}

	return MAINLOOP_TEST;
}

ScreenState StartRankingScreen();
ScreenState StartCopyrightScreen();
ScreenState StartTitleScreen();
ScreenState StartDeveloperScreen();
ScreenState StartVersionTitleScreen();
ScreenState StartDemoScreen();
