#include "DemoLoop.h"
#include "Coin.h"
#include "Input.h"
#include "Button.h"
#include "Video.h"
#include "Player.h"
#include "Sound.h"
#include "Frame.h"
#include "Pal.h"
#include "ShowObject.h"
#include "Entity.h"

bool NextScreenVersionTitle() {
	return
		AddCoin[PLAYER1] || AddCoin[PLAYER2] ||
		AddService ||
		(CanStart(PLAYER1, false) && (ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) ||
		(CanStart(PLAYER2, false) && (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)));
}

static void InitDemoLoop() {
	_0x6029814(0u, 0u, 0u, 0xFFu);
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	_0x602AA4C();
	if (_0x6064750 != NULL) {
		_0x6024030(_0x6064750);
	}
	_0x6023790();
	_0x602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	_0x602419C();
	_0x60169DC();
	_0x602E72A(0u);
	InitPlayers();
	InitModeCodes(&Players[PLAYER1]);
	InitModeCodes(&Players[PLAYER2]);
	_0x6064750 = NULL;
	Demo = false;
	ScreenTime = 0u;
	DemoScreen = DEMOSCREEN_START;
	DemoWaitTime = 0u;
	GameStartPlayer = STARTPLAYER_NONE;
	UpdateFrame();
	_0x602AA16();
}

static ScreenState StartRankingScreen();
static ScreenState StartCopyrightScreen();
static ScreenState StartTitleScreen();
static ScreenState StartDeveloperScreen();
static ScreenState StartVersionTitleScreen();
static ScreenState StartDemoScreen();

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

static ScreenState StartRankingScreen() {
	// TODO
}

static ScreenState StartCopyrightScreen() {
	_0x602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}

	_0x602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	_0x6029814(0u, 0u, 0x00u, 0xFFu);

	for (bool nextScreenVersionTitle = false; !nextScreenVersionTitle; nextScreenVersionTitle = NextScreenVersionTitle()) {
		if (_0x6064750 == NULL) {
			ResetVideoSetters();
			_0x602406E();
			InitEntities();
			_0x6026FCA(CurrentGameBg._0x1E, 0u);
			SetPal(160u, NUMPALCOLORS_4BPP, PALPTR(0x205));

			for (int16_t frames = 0; frames < 3; frames++) {
				if (UpdateFrame()) {
					return SCREEN_TESTMODE;
				}
			}

			SpritePriority[0] = 0x13u;
			SpritePriority[1] = 0x66u;
			_0x6029498(6); // TODO
			SetBackdropColor(COLOR(0u, 0u, 0u, 0u));
			AlphaValues[4] = 0x1Fu;
			_0x6029546(0, 0x14, 0, 6);

			for (int16_t frames = 300; frames != 0; frames--) {
				if (UpdateFrame()) {
					_0x602406E();
					return SCREEN_TESTMODE;
				}
				if (NextScreenVersionTitle()) {
					_0x602406E();
					return SCREEN_VERSIONTITLE;
				}
				ShowObject(OBJECTPTR(0x658), 0, 0, 160u, 40u); // TODO: Define a constant for the object pointer.
			}

			for (bool stopDemoLoop = false; !stopDemoLoop; stopDemoLoop = UpdateFrame()) {
				if (_0x6064750 == NULL) {
					UpdateFrame();
					_0x602406E();
					_0x6029546(2, 10, 0, 6); // TODO
					for (int16_t frames = 0; frames < 10; frames++) {
						if (UpdateFrame()) {
							_0x602406E();
							return SCREEN_TESTMODE;
						}
					}
					return SCREEN_DEVELOPER;
				}
			}
			return SCREEN_TESTMODE;
		}

		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	}

	return SCREEN_VERSIONTITLE;
}

static ScreenState StartTitleScreen() {
	// TODO
}

static ScreenState StartDeveloperScreen() {
	// TODO
}

static ScreenState StartVersionTitleScreen() {
	// TODO
}

static ScreenState StartDemoScreen() {
	// TODO
}
