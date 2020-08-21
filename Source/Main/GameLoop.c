#include "Main/GameLoop.h"
#include "Main/DemoLoop.h"
#include "Main/Frame.h"
#include "Game/Play/Player.h"
#include "Game/Play/Ranking.h"
#include "Game/Play/Versus.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowText.h"
#include "Game/Save.h"
#include "Game/Screen.h"
#include "Game/PalNum.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Input/Button.h"
#include "Sound/Sound.h"
#include "Lib/Math.h"

ROMDATA Color PalCycleTextPal0[NUMPALCOLORS_4BPP];

// TODO: Some/all of these uint8_t's are probably actually GameMusic type.
static uint16_t UNK_6079290;
static uint16_t UNK_6079292;
static uint16_t UNK_6079294;
static uint8_t UNK_6079296;
static uint8_t UNK_6079297;
static GameMusic CurrentGameMusic;
static uint8_t UNK_6079299;
static uint8_t UNK_607929A;

typedef enum GameLoopState {
	GAMELOOP_RESTART = 0,
	GAMELOOP_STOP = 2,
	GAMELOOP_INIT = 5,
	GAMELOOP_TEST = 7,
	GAMELOOP_START = 8
} GameLoopState;
static GameLoopState GameLoop;

static const Color UNK_6032884[NUMPALCOLORS_4BPP] = {
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u,
	0x01010100u
};

static void UpdateGameMusic();
static GameLoopState StartGameLoop();

void InitGame() {
	UNK_602AA4C();
	UNK_6029814(0u, 0u, 0u, 0xFFu);
	
	if (UNK_6064750 != NULL) {
		UNK_6024030(UNK_6064750);
	}

	InitSpriteAdders();
	UNK_602AB9E();
	FreePalCycles(FREEALLPALCYCLES);

	Game.numVersusRounds = 0u;
	Game.numVersusRoundWins[PLAYER2] = 0u;
	Game.numVersusRoundWins[PLAYER1] = 0u;

	StopMusic();

	// TODO: These all control game music. Check UpdateGameMusic to figure out their names.
	UNK_6079296 = 0u;
	UNK_6079297 = 0u;
	UNK_607929A = 0u;
	CurrentGameMusic = GAMEMUSIC_0;
	UNK_6079299 = 0u;

	GameLoop = GAMELOOP_INIT;

	InitPlayers();
	InitItems();

	// Probably all sets of block palettes here.
	SetPal(148u, 1u, PALPTR(0x15A));
	SetPal(176u, 1u, PALPTR(0x15B));
	UNK_601886C();
	SetPal(48u, 10u, PALTABLE_PALNUM48BLOCK);
	SetPal(58u, 10u, PALTABLE_IBLOCK);
	SetPal(68u, 10u, PALTABLE_ZBLOCK);
	SetPal(78u, 10u, PALTABLE_SBLOCK);
	SetPal(88u, 10u, PALTABLE_JBLOCK);
	SetPal(98u, 10u, PALTABLE_LBLOCK);
	SetPal(108u, 10u, PALTABLE_OBLOCK);
	SetPal(118u, 10u, PALTABLE_TBLOCK);
	SetPal(128u, 10u, PALTABLE_GARBAGEBLOCK);
	SetPal(138u, 10u, PALTABLE_PALNUM138BLOCK);

	SetPal(220u, 1u, UNK_6032884);
	SetPal(221u, 1u, UNK_6032884);
	SetPal(222u, 1u, UNK_6032884);
	SetPal(223u, 1u, UNK_6032884);

	SetPal(203u, 5u, PALPTR(0x224));

	Alpha[0] = 0x00u;
	Alpha[4] = 0x05u;
	Alpha[1] = 0x80u;
	Alpha[2] = 0x00u;
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	UpdateFrame();
}

GameLoopState InitGameLoop();

MainLoopState RunGameLoop() {
	bool loop = true;
	InitGame();
	GameLoop = GAMELOOP_INIT;
	ScreenTime = 0u;

	while (loop) {
		switch (GameLoop) {
		case GAMELOOP_STOP:
			loop = false;
			break;

		case GAMELOOP_INIT:
			GameLoop = InitGameLoop();
			break;

		case GAMELOOP_TEST:
			return MAINLOOP_TEST;

		case GAMELOOP_START:
			GameLoop = StartGameLoop();
			break;

		default:
			GameLoop = GAMELOOP_START;
			break;
		}
	}

	Screen = SCREEN_NORMALRANKING;
	return MAINLOOP_DEMO;
}

GameLoopState InitGameLoop() {
	GameFlags = GAME_TWIN;
	InitPlayers();
	Demo = false;
	Game.numVersusWins = 0u;
	Game.versusWinner = PLAYER1;
	SetPal(226u, 10u, PALPTR(0x1C2));
	SetPal(236u, 10u, PALPTR(0x1CC));
	SetPal(246u, 10u, PALPTR(0x1D6));
	UNK_602E56A();
	SettingsValid();
	return GAMELOOP_START;
}

void CheckSetNewChallenger(Player *player) {
	GameFlags &= ~(GAME_STARTWAITINGPLAYER | GAME_NEWCHALLENGER);
	if (player->otherPlayer->nowFlags & NOW_STARTED) {
		if (player->otherPlayer->nowFlags & (NOW_STAFF | NOW_STOPPED)) {
			GameFlags |= GAME_STARTWAITINGPLAYER;
		}
		else {
			GameFlags |= GAME_NEWCHALLENGER;
		}
	}
	else {
		GameFlags |= GAME_STARTWAITINGPLAYER;
	}

	UNK_6079290 = 0;
	UNK_6079292 = 0;
}

GameLoopState GameStartVersus() {
	ModeFlag modeFlags;
	
	if ((Players[PLAYER1].nowFlags & NOW_WAITING) && (Players[PLAYER2].nowFlags & NOW_WAITING)) {
		if (GameFlags & GAME_CHALLENGEDELAY) {
			UNK_6079299 = 2u;
			Players[PLAYER1].nowFlags |= NOW_NOUPDATE;
			Players[PLAYER2].nowFlags |= NOW_NOUPDATE;
			if (GameButtonsDown[PLAYER1] == BUTTON_START && GameButtonsDown[PLAYER2] == BUTTON_START) {
				UNK_6079290++;
			}
			if (
				GameButtonsDown[PLAYER1] == (BUTTON_START | BUTTON_2 | BUTTON_1) &&
				GameButtonsDown[PLAYER2] == (BUTTON_START | BUTTON_2 | BUTTON_1)) {
				UNK_6079292++;
			}
		}
		else {
			SetPal(PALNUM_15, 1u, PALPTR(0xF9));
			SetPal(PALNUM_14, 1u, PALPTR(0xF8));
			UNK_6079299 = 10u;
			Game.state = 0u;
			GameFlags &= ~(GAME_TWIN | GAME_NEWCHALLENGER | GAME_WINNER1P | GAME_WINNER2P | GAME_BIT10) | GAME_VERSUS | GAME_BIT11;
			Game.modeFlags[PLAYER1] = Players[PLAYER1].modeFlags;
			Game.modeFlags[PLAYER2] = Players[PLAYER2].modeFlags;
			modeFlags = Game.modeFlags[Game.versusWinner] & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE | MODE_ITEM);
			Players[PLAYER1].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT;
			Players[PLAYER2].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT;
			Players[PLAYER1].modeFlags = (Players[PLAYER1].modeFlags & (MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE)) | modeFlags | MODE_VERSUS;
			Players[PLAYER2].modeFlags = (Players[PLAYER2].modeFlags & (MODE_NORMAL|MODE_MASTER|MODE_DOUBLES|MODE_VERSUS|MODE_INVISIBLE)) | modeFlags | MODE_VERSUS;
			// TODO: These could be signalling to start a fading background
			// transition (|= 1u) and transition to the versus background (=
			// 10u).
			CurrentGameBg.UNK_12 = 10u;
			CurrentGameBg.UNK_10 |= 1u;
			Game.numVersusRoundWins[PLAYER2] = 0u;
			Game.numVersusRoundWins[PLAYER1] = 0u;
			Game.numVersusRounds = 0u;
			InitItems();
			UNK_601FAD0(); // TODO
			if (UNK_6079292 >= TIME(0, 1, 0)) {
				Players[PLAYER1].modeFlags |= MODE_CEMENT;
				Players[PLAYER2].modeFlags |= MODE_CEMENT;
			}
			else if (UNK_6079290 >= TIME(0, 1, 0)) {
				Players[PLAYER1].modeFlags |= MODE_NOITEM;
				Players[PLAYER2].modeFlags |= MODE_NOITEM;
			}

			while (UNK_6064750 != NULL) {
				if (UpdateFrame()) {
					return GAMELOOP_TEST;
				}
			}
			UNK_6029546(0, 20, 0, 6);
		}
	}
	return GAMELOOP_RESTART;
}

void GameStartVersusRound() {
	if ((Players[PLAYER1].nowFlags & NOW_WAITING) && (Players[1].nowFlags & NOW_WAITING)) {
		Game.state = 0u;
		GameFlags =
			(GameFlags | GAME_VERSUS | GAME_BIT11) &
			~(GAME_CHALLENGER1P | GAME_CHALLENGER2P | GAME_STARTWAITINGPLAYER);
		Players[PLAYER1].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT | NOW_SHOWFIELD;
		Players[PLAYER2].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT | NOW_SHOWFIELD;
		InitItems();
		UNK_601FAD0(); // TODO
	}
}

static uint8_t NumVersusRounds[3] = { 1u, 2u, 3u };

uint8_t NumVersusRoundsSetting() {
	return NumVersusRounds[Settings[SETTING_NUMVERSUSROUNDS]];
}

GameLoopState UNK_6008406(PlayerNum playerNum) {
	bool startTestMode;
	
	NextPlayGameOver(&Players[playerNum]);

	Players[playerNum].modeFlags &= 0x1083;
	Players[PLAYER1].modeFlags &= ~MODE_VERSUS;
	Players[PLAYER2].modeFlags &= ~MODE_VERSUS;

	if (!(Players[PLAYER1].nowFlags & NOW_STOPPED)) {
		Players[PLAYER1].nowFlags |= NOW_INIT;
	}
	if (!(Players[PLAYER2].nowFlags & NOW_STOPPED)) {
		Players[PLAYER2].nowFlags |= NOW_INIT;
	}

	GameFlags =
		(GameFlags | GAME_TWIN) &
		~(GAME_VERSUS | GAME_DOUBLES | GAME_STARTWAITINGPLAYER | GAME_NEWCHALLENGER);
	Game.numVersusRounds = 0u;

	SetPal(PALNUM_15,1, PAL_MODESELECTED);
	SetPal(PALNUM_14,1, PAL_MODENOTSELECTED);

	do {
		if (UNK_6064750 == NULL) {
			return GAMELOOP_RESTART;
		}
	} while (!UpdateFrame());

	return GAMELOOP_TEST;
}

GameLoopState UNK_6008516() {
	GameLoopState state = GAMELOOP_RESTART;
	// TODO
	return state;
}

void UNK_60088FC() {
	GameFlags = (GameFlags & ~GAME_BIT13) | GAME_NEWVERSUSROUND;
	Game.numVersusRounds++;
}

static GameLoopState GameStartDoubles() {
	switch (Game.state) {
	case 0u:
		Game.state = 1u;
		UNK_6029546(2, 20, 0, 6);
		break;

	case 1u:
		while (UNK_6064750 != NULL) {
			if (UpdateGame()) {
				return GAMELOOP_TEST;
			}
		}
		Game.state++;
		UNK_6029546(0, 10, 0, 6);
		break;

	case 2u:
		break;

	default:
		return GAMELOOP_RESTART;
	}

	Players[PLAYER1].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT;
	Players[PLAYER2].nowFlags = NOW_PLAYING | NOW_STARTED | NOW_INIT;
	GameFlags = GAME_DOUBLES;
	Game.state = 0u;
	Players[PLAYER1].modeFlags = MODE_DOUBLES;
	Players[PLAYER2].modeFlags = MODE_DOUBLES;
	return GAMELOOP_RESTART;
}

// TODO: Init from ROM data.
ROMDATA Color UNK_60328C4[NUMPALCOLORS_4BPP];

uint16_t GameStartPlayer;
static GameLoopState StartGameLoop() {
	UNK_602AA4C();

	if (UpdateFrame()) return GAMELOOP_TEST;

	UNK_6029546(4, 0, 0, 0);

	while (UNK_6064750) {
		if (UpdateFrame()) return GAMELOOP_TEST;
	}

	InitVideoSetters();
	UNK_602406E();

	// Sound.
	UNK_602EB4C();
	UNK_602EC5C();
	UNK_602E6B8();
	UNK_602E586();
	StopMusic();
	UNK_602E72A(5);
	Game.music = -1;

	InitEntities();

	// Init players.
	for (PlayerNum playerNum = PLAYER1; playerNum <= NUMPLAYERS - 1; playerNum++) {
		Player *player = &Players[playerNum];
		player->modeFlags |= Game.modeFlags[playerNum] & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE);
		if (GameStartPlayer & (1 << playerNum)) {
			player->nowFlags |= NOW_STARTED | NOW_WAITING;
			InitPlayer(playerNum);
		}
	}

	Game.versusWinner = (Players[PLAYER1].nowFlags & NOW_STARTED) ? PLAYER1 : PLAYER2;

	while (UNK_6064750 != NULL) {
		if (UpdateFrame()) return GAMELOOP_TEST;
	}

	UNK_60169DC();
	UNK_6026FCA(CurrentGameBg.UNK_1E, 1);
	SetPal(202u, 1u, UNK_60328C4);
	NewPalCycle(159u, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, PALCYCLETYPE_UPSTOP, 1u, 63u);
	bool downNextPalCycle = false;
	uint32_t numPalCycleFrames = 0u;
	UNK_6029546(0u, 10, 0, 6);
	Player *player1 = &Players[PLAYER1];
	Player *player2 = &Players[PLAYER2];

	GameLoopState state = GAMELOOP_RESTART;
	while (state == GAMELOOP_RESTART) {
		if (UpdateGame()) {
			UNK_602406E();
			return GAMELOOP_TEST;
		}

		UpdateGameMusic();
		ShowPalCycleText(218 + 67, 218, VERSION_NAME, true);
		if ((++numPalCycleFrames % 64u) == 0u) {
			if (downNextPalCycle = !downNextPalCycle) {
				NewPalCycle(PALNUM_PALCYCLETEXT, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, PALCYCLETYPE_DOWNSTOP, 1u, 63u);
			}
			else {
				NewPalCycle(PALNUM_PALCYCLETEXT, PalCycleTextPal0, PAL_SYSTEMTEXT, 1, PALCYCLETYPE_UPSTOP, 1u, 63u);
			}
		}

		InitSeed = Rand(894u);

		if (GameFlags & GAME_STARTDOUBLES) {
			state = GameStartDoubles();
		}
		else {
			if (GameFlags & GAME_CHALLENGER1P) {
				CheckSetNewChallenger(player1);
			}
			else if (GameFlags & GAME_CHALLENGER2P) {
				CheckSetNewChallenger(player2);
			}

			GameFlags &= ~(GAME_CHALLENGER1P | GAME_CHALLENGER2P);
			if (GameFlags & GAME_NEWCHALLENGER) {
				state = GameStartVersus();
			}

			if (GameFlags & GAME_NEWVERSUSROUND) {
				GameStartVersusRound();
			}

			if ((GameFlags & GAME_VERSUS) && !(GameFlags & GAME_BIT11)) {
				state = UNK_6008516(); // TODO
			}

			if ((GameFlags & GAME_VERSUS) && (GameFlags & GAME_BIT13)) {
				UNK_60088FC();
			}

			if (!(GameFlags & GAME_VERSUS) && (GameFlags & GAME_DOUBLES) && (!(player1->nowFlags & NOW_STARTED) || !(player2->nowFlags & NOW_STARTED))) {
				// BUG: The SH-2 code calculates this expression, but never assigns it to anything.
				// There's probably an oversight here, where the "&" was supposed to be "&=".
				// There's no problem though, because the following line resets GameFlags.
				GameFlags & ~(GAME_VERSUS | GAME_DOUBLES | GAME_STARTWAITINGPLAYER | GAME_NEWCHALLENGER);

				GameFlags = GAME_TWIN;
				player1->nowFlags |= NOW_INIT;
				player2->nowFlags |= NOW_INIT;
				UNK_6029546(0, 10, 0, 6);
			}

			if ((player1->nowFlags & NOW_GAMEOVER) && (player2->nowFlags & NOW_GAMEOVER)) {
				UNK_6079299 = 2;
				if (++UNK_6079294 < 180u) {
					state = GAMELOOP_STOP;
				}

				if (NextScreenVersionTitle()) {
					UNK_6079294 = 0;
				}
			}
			else {
				UNK_6079294 = 0;
			}
		}
	}

	InitSeed += Rand(1192u) + 1u;

	if (state == GAMELOOP_STOP) {
		UNK_6029546(2, 30, 72, 6);
	}

	do {
		if (UNK_6064750 == NULL) {
			CheckSaveRankings();
			UNK_602406E();
			UNK_602E586();
			return GAMELOOP_STOP;
		}
	} while (!UpdateFrame());
	UNK_602406E();

	return GAMELOOP_TEST;
}

// TODO: ...

int16_t UNK_6008E38(int16_t level) {
	if (level < 495) {
		return 1;
	}
	else if (level >= 495 && level < 500)  {
		return 2;
	}
	else if (level >= 500 && level < 695) {
		return 3;
	}
	else if (level >= 695 && level < 700) {
		return 4;
	}
	else if (level >= 700 && level < 880) {
		return 5;
	}
	else if (level >= 880 && level < 900) {
		return 6;
	}
	else if (level >= 900) {
		return 7;
	}
}

int16_t UNK_6008EC8(int16_t level) {
	if (level < 280) {
		return 1;
	}
	else if (level >= 280 && level < 300) {
		return 2;
	}
	else if (level >= 300) {
		return 3;
	}
}

int16_t UNK_6008EEC(int16_t level) {
	if (level < 280) {
		return 3;
	}
	else if (level >= 280 && level < 300) {
		return 2;
	}
	else if (level >= 300 && level < 480) {
		return 5;
	}
	else if (level >= 480 && level < 500) {
		return 2;
	}
	else if (level >= 500) {
		return 7;
	}
}

static void UpdateGameMusic() {
	// TODO
}
