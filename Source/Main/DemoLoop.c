#include "Main/DemoLoop.h"
#include "Main/GameLoop.h"
#include "Main/Frame.h"
#include "Main/DemoReplayInput.h"
#include "Game/Play/Player.h"
#include "Game/Play/Versus.h"
#include "Game/Play/Debug.h"
#include "Game/Save.h"
#include "Game/Graphics/ShowGameStatus.h"
#include "Game/Graphics/ShowText.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowObject.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Input/Coin.h"
#include "Input/Input.h"
#include "Input/Button.h"
#include "Sound/Sound.h"
#include "Lib/Math.h"

ROMDATA Color UNK_6033790[NUMPALCOLORS_4BPP];

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
	return SCREEN_TESTMODE;
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
			SetPal(160u, 16u, PALPTR(0x205));

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
	int16_t sVar3 = 300;
	bool bVar2 = false;

	UNK_602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}

	UNK_602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	UNK_6029814(0u, 0u, 0u, 0xFFu);
	do {
		if (UNK_6064750 == NULL) {
			InitVideoSetters();
			UNK_602406E();
			InitEntities();
			UNK_6026FCA(CurrentGameBg.bgIndex, 0);
			SetPal(160u, 16u, PALPTR(0x1F4));
			for (int16_t i = 0; i < 3; i++) {
				if (UpdateFrame()) {
					return SCREEN_TESTMODE;
				}
			}

			SpritePriority[0] = 0x13;
			SpritePriority[1] = 0x66;
			UNK_6029498(6);
			SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
			Alpha[4] = 0x1Fu;
			UNK_6029546(0, 20, 0, 6);

			while (true) {
				if (UpdateFrame()) {
					UNK_602406E();
					return SCREEN_TESTMODE;
				}
				if (NextScreenVersionTitle()) break;

				ShowObject(OBJECTPTR(0x638) + (ScreenTime & 0xFu), 0, 0, 160u, 40u);
				if (sVar3 == 0) {
					bVar2 = true;
				}
				else {
					sVar3--;
				}
				if (bVar2) {
					do {
						if (UNK_6064750 == NULL) {
							UpdateFrame();
							UNK_602406E();
							UNK_6029546(2, 10, 0, 6);
							for (int16_t i = 0; i < 10; i++) {
								if (UpdateFrame()) {
									UNK_602406E();
									return SCREEN_TESTMODE;
								}
							}

							return SCREEN_DEVELOPER;
						}
					} while (!UpdateFrame());

					return SCREEN_TESTMODE;
				}
			}

			UNK_602406E();
			return SCREEN_VERSIONTITLE;
		}

		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	} while (!NextScreenVersionTitle());

	return SCREEN_VERSIONTITLE;
}

static ScreenState StartDeveloperScreen() {
	bool var0 = false;
	int16_t var1 = 300;

	UNK_602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}

	UNK_602AB9E();
	FreePalCycles(FREEALLPALCYCLES);
	UNK_6029814(0u, 0u, 0u, 0xFFu);
	do {
		if (UNK_6064750 == NULL) {
			InitVideoSetters();
			UNK_602406E();
			InitEntities();
			UNK_6026FCA(CurrentGameBg.bgIndex, 0);
			SetPal(160u, 16u, PALPTR(0x214));
			for (int16_t i = 0; i < 3; i++) {
				if (UpdateFrame()) {
					return SCREEN_TESTMODE;
				}
			}

			SpritePriority[0] = 0x13;
			SpritePriority[1] = 0x66;
			UNK_6029498(6);
			SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
			Alpha[4] = 0x1Fu;
			UNK_6029546(0, 20, 0, 6);

			while (true) {
				if (UpdateFrame()) {
					UNK_602406E();
					return SCREEN_TESTMODE;
				}
				if (NextScreenVersionTitle()) break;

				ShowObject(OBJECTPTR(0x6A4), 0, 0, 160u, 40u);
				if (var1 == 0) {
					var0 = true;
				}
				else {
					var1--;
				}
				if (var0) {
					do {
						if (UNK_6064750 == NULL) {
							UpdateFrame();
							UNK_602406E();
							UNK_6029546(2, 10, 0, 6);

							for (int16_t i = 0; i < 10; i++) {
								if (UpdateFrame()) {
									UNK_602406E();
									return SCREEN_TESTMODE;
								}
							}

							return SCREEN_VERSIONTITLE;
						}
					} while (!UpdateFrame());

					return SCREEN_TESTMODE;
				}
			}

			UNK_602406E();
			return SCREEN_VERSIONTITLE;
		}

		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	} while (!NextScreenVersionTitle());

	return SCREEN_VERSIONTITLE;
}

static ScreenState StartVersionTitleScreen() {
	bool bVar3;
	bool bVar4;
	uint32_t uVar1;
	int16_t sVar7;
	int16_t sStack60;
	uint16_t uStack56;

	int16_t sVar6 = 600;
	bool bVar5 = false;
	Demo = false;

	UNK_602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}
	UNK_6029546(4, 0, 0, 0);
	while (UNK_6064750 != NULL) {
		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	}

	InitVideoSetters();
	UNK_602406E();
	InitEntities();
	GameStartPlayer = PLAYER1;
	InitPlayers();
	SetPal(160u, 16u, PALPTR(0x1F4));
	InitSystemTextPal();
	SetPal(202u, 1u, PalCycleTextPal0);
	NewPalCycle(159u, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, PALCYCLETYPE_UPSTOP, 1, 63u);
	bVar3 = false;
	uStack56 = 0;
	UNK_6029814(0u, 0u, 0u, 0xFFu);
	while (UNK_6064750 != NULL) {
		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	}
	UNK_6026FCA(CurrentGameBg.bgIndex, 0);
	for (int16_t i = 0; i < 3; i++) {
		bVar4 = UpdateFrame();
		if (bVar4 != false) {
			return SCREEN_TESTMODE;
		}
	}
	UNK_602E56A();
	EnableSound();
	UNK_602AA16();
	UNK_6029498(6);
	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	UNK_6029546(0, 20, 0, 6);
	sStack60 = 0;
	InitDebugCode();
	while (true) {
		bVar4 = UpdateFrame();
		if (UpdateFrame()) {
			UNK_602406E();
			return SCREEN_TESTMODE;
		}
		if ((ScreenTime & 1u) == 0u) {
			sStack60 += 1;
		}
		sStack60 &= 0xF;
		ShowObject(OBJECTPTR(0x638) + sStack60 * 2u, 0, 0, 160u, 16u);
		UNK_600FC50();
		UpdateDebugCode();
		if (CanStart(PLAYER1, false) && UpdateModeCodes(&Players[PLAYER1])) {
			sVar6 += 300;
		}
		if (CanStart(PLAYER2, false) && UpdateModeCodes(&Players[PLAYER2])) {
			sVar6 += 300;
		}
		ShowModeCodes(&Players[PLAYER1]);
		ShowModeCodes(&Players[PLAYER2]);
		if ((!(GameButtonsNew[PLAYER1] & BUTTON_START) || (GameStartPlayer & STARTPLAYER_1P)) || (GameStartPlayer & STARTPLAYER_2P)) {
			if (((GameButtonsNew[PLAYER2] & BUTTON_START) && !(GameStartPlayer & STARTPLAYER_2P)) && !(GameStartPlayer & STARTPLAYER_1P) && CanStart(PLAYER2, false)) {
				CheckBuyGame(PLAYER2, false);
				GameStartPlayer |= STARTPLAYER_2P;
				Game.modeFlags[PLAYER1] = Players[PLAYER1].modeFlags;
				Game.modeFlags[PLAYER2] = Players[PLAYER2].modeFlags;
				if (5 < sVar6) {
					sVar6 = 5;
				}
			}
		}
		else {
			if (CanStart(PLAYER1, false)) {
				CheckBuyGame(PLAYER1, false);
				GameStartPlayer |= STARTPLAYER_1P;
				Game.modeFlags[PLAYER1] = Players[PLAYER1].modeFlags;
				Game.modeFlags[PLAYER2] = Players[PLAYER2].modeFlags;
				if (5 < sVar6) {
					sVar6 = 5;
				}
			}
		}
		if (sVar6 == 0) {
			bVar5 = true;
		}
		else {
			sVar6--;
		}
		ShowPalCycleText(256, 160, "PLUS", false);
		if (++uStack56 % 64u == 0u) {
			bVar3 = !bVar3;
			NewPalCycle(159u, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, bVar3 ? PALCYCLETYPE_DOWNSTOP : PALCYCLETYPE_UPSTOP, 1, 63u);
		}
		if (bVar5) break;
		ShowCredits();
	}
	while (UNK_6064750 != NULL) {
		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	}
	UpdateFrame();
	UNK_602406E();
	if (GameStartPlayer != STARTPLAYER_NONE) {
		UNK_6029546(2, 10, 0, 6);
		for (int16_t i = 0; i < 10; i++) {
			if (UpdateFrame()) {
				UNK_602406E();
				return SCREEN_TESTMODE;
			}
		}
		if (Save->demoWaitTime < DemoWaitTime) {
			Save->demoWaitTime = DemoWaitTime;
		}
		return SCREEN_GAME;
	}

	// BUG: Should be >, not >=. Results in the DOUBLES3 case never being hit. Left in, for same-as-original behavior. Maybe it was intended, but the switch below wasn't changed?
	if (++DemoScreen >= DEMOSCREEN_DOUBLES3) {
		DemoScreen = DEMOSCREEN_TWIN0;
	}

	switch (DemoScreen) {
	case DEMOSCREEN_TWIN0:
	default:
		return SCREEN_TWINDEMO;

	case DEMOSCREEN_VERSUS1:
		return SCREEN_VERSUSDEMO;

	case DEMOSCREEN_DOUBLES1:
		return SCREEN_DOUBLESDEMO;

	case DEMOSCREEN_TWIN1:
		return SCREEN_TWINDEMO;

	case DEMOSCREEN_VERSUS2:
		return SCREEN_VERSUSDEMO;

	case DEMOSCREEN_DOUBLES2:
		return SCREEN_DOUBLESDEMO;

	case DEMOSCREEN_DOUBLES3:
		return SCREEN_DOUBLESDEMO;
	}
}

static ScreenState StartDemoScreen() {
	ScreenState nextScreen = SCREEN_NULL;

	Demo = true;
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x64u;
	UNK_602AA4C();
	if (UpdateFrame()) {
		return SCREEN_TESTMODE;
	}
	SavePlayStatus();
	UNK_6029546(4, 0, 0, 0);
	do {
		if (UNK_6064750 == NULL) {
			InitVideoSetters();
			UNK_602E586();
			StopMusic();
			UNK_602E72A(7u);
			UNK_602E560();
			if (!Settings[SETTING_DEMOSOUND]) {
				DisableSound();
			}
			UNK_602EB4C();
			UNK_602EC5C();
			UNK_602E6B8();
			InitEntities();
			InitGame();
			Game.numVersusWins = 0u;
			Game.versusWinner = PLAYER1;
			SetPal(226u, 10u, PALPTR(0x1C2));
			SetPal(236u, 10u, PALPTR(0x1CC));
			SetPal(246u, 10u, PALPTR(0x1D6));
			SetPal(157u, 1u, PAL_TEXTOVERLAYBG);
			UNK_60169DC();
			Players[PLAYER1].nowFlags = NOW_BIT3 | NOW_STARTED;
			Players[PLAYER2].nowFlags = NOW_BIT3 | NOW_STARTED;
			Game.music = -1;
			int16_t initSeed1p = 0, initSeed2p = 0; // BUG: Original didn't initialize these. Not really required, but it silences a compiler warning.
			ButtonInput* demoReplayInput = NULL;
			switch (Screen) {
			case SCREEN_TWINDEMO:
				demoReplayInput = DemoReplayInputTwin;
				initSeed1p = 7;
				initSeed2p = 615;
				GameFlags = GAME_TWIN;
				Players[PLAYER1].modeFlags = (Players[PLAYER1].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | MODE_MASTER;
				Players[PLAYER2].modeFlags = (Players[PLAYER2].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | MODE_NORMAL;
				NextGameMusic = GAMEMUSIC_1;
				break;

			case SCREEN_VERSUSDEMO:
				demoReplayInput = DemoReplayInputVersus;
				initSeed1p = 555;
				initSeed2p = 555;
				GameFlags = GAME_VERSUS;
				Players[PLAYER1].modeFlags = (Players[PLAYER1].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | (MODE_MASTER | MODE_VERSUS);
				Players[PLAYER2].modeFlags = (Players[PLAYER2].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | (MODE_NORMAL | MODE_VERSUS);
				NextGameMusic = GAMEMUSIC_10;
				SetPal(15u, 1u, PALPTR(0xF9));
				SetPal(14u, 1u, PALPTR(0xF8));
				UNK_601FAD0();
				break;

			case SCREEN_DOUBLESDEMO:
				demoReplayInput = DemoReplayInputDoubles;
				initSeed1p = 7;
				initSeed2p = 615;
				GameFlags = GAME_DOUBLES;
				Players[PLAYER1].modeFlags = (Players[PLAYER1].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | MODE_DOUBLES;
				Players[PLAYER2].modeFlags = (Players[PLAYER2].modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS)) | MODE_DOUBLES;
				NextGameMusic = GAMEMUSIC_1;
				break;

			default:
				break;
			}

			uint32_t backupSeed = InitSeed;
			InitSeed = (int32_t)initSeed1p;
			InitPlayer(PLAYER1);
			InitSeed = (int32_t)initSeed2p;
			InitPlayer(PLAYER2);
			InitSeed = backupSeed;

			NextPlayStart(&Players[PLAYER1]);
			NextPlayStart(&Players[PLAYER2]);

			SetPal(179u, 1u, PAL_SYSTEMTEXT);
			SetPal(202u, 1u, UNK_6033790);
			NewPalCycle(159u, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, PALCYCLETYPE_UPSTOP, 1, 63u);
			UNK_6026FCA(CurrentGameBg.bgIndex,1);
			UNK_6029498(6);
			UNK_6029546(0, 20, 0, 6);
			ScreenTime = 0u;
			int32_t demoReplayFrames = 0;
			for (int16_t i = 0; i < 10; i++) {
				if (UpdateDemo( demoReplayInput + ScreenTime, demoReplayInput + ScreenTime + DEMOREPLAY_LENGTH)) {
					demoReplayFrames = DEMOREPLAY_FRAMES;
					nextScreen = SCREEN_TESTMODE;
				}
				if (NextScreenVersionTitle()) {
					UNK_602AA4C();
					InitPlayers();
					Demo = false;
					StopMusic();
					UNK_602E586();
					return SCREEN_VERSIONTITLE;
				}
				if (UNK_6064750 == NULL) break;
			}
			InitVideoSetters();
			ShowObject(OBJECTPTR(0x636), 108, 164, 157u, 115u);
			int16_t textOffsetX = 0;
			bool var0 = false;
			bool palCycleDown = false;
			uint16_t palCycleFrames = 0u;
			while (true) {
				if (UpdateDemo(&demoReplayInput[ScreenTime], &demoReplayInput[ScreenTime + DEMOREPLAY_LENGTH])) {
					demoReplayFrames = DEMOREPLAY_FRAMES;
					nextScreen = SCREEN_TESTMODE;
				}
				demoReplayFrames++;
				UpdateGameMusic();
				ShowPalCycleText(285, 218, "PLUS", true);
				if (++palCycleFrames % 64u == 0u) {
					palCycleDown = !palCycleDown;
					NewPalCycle(159u, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, palCycleDown ? PALCYCLETYPE_DOWNSTOP : PALCYCLETYPE_UPSTOP, 1, 63u);
				}
				if (NextScreenVersionTitle()) {
					nextScreen = SCREEN_VERSIONTITLE;
					demoReplayFrames = DEMOREPLAY_FRAMES;
				}
				InitSeed += Rand(357u) + 1u;
				UNK_600FC50();
				int16_t textX;
				if (var0) {
					ShowObject(OBJECTPTR(0x636), 108, 164 - textOffsetX, 202u, 115u);
					ShowObject(OBJECTPTR(0x636), 108, 484 - textOffsetX, 157u, 115u);
					textX = 160;
				}
				else {
					ShowObject(OBJECTPTR(0x636), 108, 164 - textOffsetX, 157u, 115u);
					ShowObject(OBJECTPTR(0x636), 108, 484 - textOffsetX, 202u, 115u);
					textX = 480;
				}
				ShowText(textX - textOffsetX - TextWidth("http://www.arika.co.jp") / 2, 112, "http://www.arika.co.jp", 179u, false);
				int16_t nextTextOffsetX;
				if (textOffsetX + 1 > VIDEO_WIDTH) {
					var0 = !var0;
					nextTextOffsetX = textOffsetX - VIDEO_WIDTH - 1;
				}
				else {
					nextTextOffsetX = textOffsetX + 1;
				}
				textOffsetX = nextTextOffsetX;

				if (demoReplayFrames < DEMOREPLAY_FRAMES) {
					continue;
				}

				Players[PLAYER1].nowFlags |= NOW_NOUPDATE;
				Players[PLAYER2].nowFlags |= NOW_NOUPDATE;
				if (nextScreen != SCREEN_NULL) {
					InitSeed += Rand(1192u) + 1u;
					UNK_602AA4C();
					InitPlayers();
					Demo = false;
					StopMusic();
					UNK_602E586();
					return nextScreen;
				}
				InitSeed += Rand(1192u) + 1u;
				while (UNK_6064750 != NULL) {
					if (UpdateDemo(&demoReplayInput[ScreenTime], &demoReplayInput[ScreenTime + DEMOREPLAY_LENGTH])) {
						UNK_602AA4C();
						InitPlayers();
						Demo = false;
						StopMusic();
						UNK_602E586();
						return SCREEN_TESTMODE;
					}
				}
				UNK_602406E();
				UNK_6029546(2, 10, 0, 6);
				while (UNK_6064750 != NULL) {
					if (UpdateDemo(&demoReplayInput[ScreenTime], &demoReplayInput[ScreenTime + DEMOREPLAY_LENGTH])) {
						UNK_602AA4C();
						InitPlayers();
						Demo = false;
						StopMusic();
						UNK_602E586();
						return SCREEN_TESTMODE;
					}
				}
				UNK_602AA4C();
				InitPlayers();
				Demo = false;
				StopMusic();
				UNK_602E586();

				switch (Screen) {
				default:
				case SCREEN_TWINDEMO:
					break;

				case SCREEN_VERSUSDEMO:
					return SCREEN_VERSIONTITLE;

				case SCREEN_DOUBLESDEMO:
					DemoScreen = DEMOSCREEN_START;
					return SCREEN_DOUBLESRANKING;
				}
			}

			return SCREEN_NORMALRANKING;
		}
		if (UpdateFrame()) {
			return SCREEN_TESTMODE;
		}
	} while (NextScreenVersionTitle());
	Demo = false;
	return SCREEN_VERSIONTITLE;
}
