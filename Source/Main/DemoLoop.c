#include "Main/DemoLoop.h"
#include "Main/Frame.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowObject.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Input/Coin.h"
#include "Input/Input.h"
#include "Input/Button.h"
#include "Sound/Sound.h"

DemoScreenState DemoScreen;

bool NextScreenVersionTitle() {
	return
		AddCoin[PLAYER1] || AddCoin[PLAYER2] ||
		AddService ||
		(CanStart(PLAYER1, false) && (ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) ||
		(CanStart(PLAYER2, false) && (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)));
}

static void InitDemoLoop() {
	UNK_6029814(0u, 0u, 0u, 0xFFu);
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	UNK_602AA4C();
	if (UNK_6064750 != NULL) {
		UNK_6024030(UNK_6064750);
	}
	UNK_6023790();
	UNK_602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	InitSpriteAdders();
	UNK_60169DC();
	UNK_602E72A(0u);
	InitPlayers();
	InitModeCodes(&Players[PLAYER1]);
	InitModeCodes(&Players[PLAYER2]);
	UNK_6064750 = NULL;
	Demo = false;
	ScreenTime = 0u;
	DemoScreen = DEMOSCREEN_START;
	DemoWaitTime = 0u;
	GameStartPlayer = STARTPLAYER_NONE;
	UpdateFrame();
	UNK_602AA16();
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
	UNK_602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}

	UNK_602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	UNK_6029814(0u, 0u, 0x00u, 0xFFu);

	for (bool nextScreenVersionTitle = false; !nextScreenVersionTitle; nextScreenVersionTitle = NextScreenVersionTitle()) {
		if (UNK_6064750 == NULL) {
			InitVideoSetters();
			UNK_602406E();
			InitEntities();
			UNK_6026FCA(CurrentGameBg.bgIndex, 0u);
			SetPal(160u, NUMPALCOLORS_4BPP, PALPTR(0x205));

			for (int16_t frames = 0; frames < 3; frames++) {
				if (UpdateFrame()) {
					return SCREEN_TESTMODE;
				}
			}

			SpritePriority[0] = 0x13u;
			SpritePriority[1] = 0x66u;
			UNK_6029498(6);
			SetBackdropColor(COLOR(0u, 0u, 0u, 0u));
			Alpha[4] = 0x1Fu;
			UNK_6029546(0, 0x14, 0, 6);

			for (int16_t frames = 300; frames != 0; frames--) {
				if (UpdateFrame()) {
					UNK_602406E();
					return SCREEN_TESTMODE;
				}
				if (NextScreenVersionTitle()) {
					UNK_602406E();
					return SCREEN_VERSIONTITLE;
				}
				ShowObject(OBJECTPTR(0x658), 0, 0, 160u, 40u); // TODO: Define a constant for the object pointer.
			}

			for (bool stopDemoLoop = false; !stopDemoLoop; stopDemoLoop = UpdateFrame()) {
				if (UNK_6064750 == NULL) {
					UpdateFrame();
					UNK_602406E();
					UNK_6029546(2, 10, 0, 6);
					for (int16_t frames = 0; frames < 10; frames++) {
						if (UpdateFrame()) {
							UNK_602406E();
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
