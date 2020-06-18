#include "Loop.h"
#include "Player.h"
#include "Item.h"
#include "ShowText.h"
#include "Frame.h"
#include "Entity.h"
#include "Math.h"
#include "Save.h"
#include "Video.h"
#include "Pal.h"
#include "PalNum.h"
#include "Sound.h"
#include "Setting.h"
#include "Config.h"
#include "PlatformTypes.h"

typedef enum GameLoopState {
	GAMELOOP_CONTINUE = 0,
	GAMELOOP_STOP = 2,
	GAMELOOP_START = 5,
	GAMELOOP_TEST = 7,
	GAMELOOP_GAME = 8
} GameLoopState;
static uint32_t CurrentGameLoopState;

// TODO: Some/all of these uint8_t's are probably actually GameMusic type.
static uint8_t _0x6079296;
static uint8_t _0x6079297;
static GameMusic CurrentGameMusic;
static uint8_t _0x6079299;
static uint8_t _0x607929A;

static const Color _0x6032884[NUMPALCOLORS_4BPP] = {
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

void InitGame() {
	_0x602AA4C();
	_0x6029814(0u, 0u, 0u, 0xFFu);
	
	if (_0x6064750 != NULL) {
		_0x6024030(_0x6064750);
	}

	_0x602419C();
	_0x602AB9E();
	FreePalCycles(500u);

	Game.numVersusRounds = 0u;
	Game.numVersusRoundWins[PLAYER2] = 0u;
	Game.numVersusRoundWins[PLAYER1] = 0u;

	StopMusic();

	// TODO: These all control game music. Check UpdateGameMusic to figure out their names.
	_0x6079296 = 0u;
	_0x6079297 = 0u;
	_0x607929A = 0u;
	CurrentGameMusic = GAMEMUSIC_0;
	_0x6079299 = 0u;

	CurrentGameLoopState = GAMELOOP_START;

	InitPlayers();
	InitItems();

	// Probably all sets of block palettes here.
	SetPal(148u, 1u, PALPTR(0x15A));
	SetPal(176u, 1u, PALPTR(0x15B));
	_0x601886C();
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

	SetPal(220u, 1u, _0x6032884);
	SetPal(221u, 1u, _0x6032884);
	SetPal(222u, 1u, _0x6032884);
	SetPal(223u, 1u, _0x6032884);

	SetPal(203u, 5u, PALPTR(0x224));

	AlphaValues[0] = 0x00u;
	AlphaValues[4] = 0x05u;
	AlphaValues[1] = 0x80u;
	AlphaValues[2] = 0x00u;
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	UpdateFrame(); // TODO
}

MainLoopState RunGameLoop() {
	bool loop = true;
	CurrentGameLoopState = 5;
	ScreenTime = 0u;

	while (loop) {
		switch (CurrentGameLoopState) {
		case GAMELOOP_STOP:
			loop = false;
			break;

		case GAMELOOP_START:
			CurrentGameLoopState = StartGameLoop();
			break;

		case GAMELOOP_TEST:
			return MAINLOOP_TEST;

		case GAMELOOP_GAME:
			CurrentGameLoopState = GameLoop();
			break;

		default:
			CurrentGameLoopState = GAMELOOP_GAME;
			break;
		}
	}

	Screen = SCREEN_NORMALRANKING;
	return MAINLOOP_DEMO;
}

// TODO

static uint8_t NumVersusRounds[3] = { 1u, 2u, 3u };

uint8_t NumVersusRoundsSetting() {
	return NumVersusRounds[Settings[SETTING_NUMVERSUSROUNDS]];
}

// TODO: ...

uint16_t GameStartPlayer;
GameLoopState GameLoop() {
	_0x602AA4C();

	if (UpdateFrame()) return GAMELOOP_TEST;

	_0x6029546(4, 0, 0, 0);

	while (_0x6064750) {
		if (UpdateFrame()) return GAMELOOP_TEST;
	}

	ResetVideoSetters();
	_0x602406E();

	// Sound.
	_0x602EB4C();
	_0x602EC5C();
	_0x602E6B8();
	_0x602E586();
	StopMusic();
	_0x602E72A(5);
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

	while (_0x6064750 != NULL) {
		if (UpdateFrame()) return 7;
	}

	_0x60169DC();
	_0x6026FCA(_0x6066188._0x1E, 1);
	SetPal(202u, 1u, _0x60328C4);
	NewPalCycle(159u, _0x6033748, _0x67910, 1, PALCYCLETYPE_UPSTOP, 1u, 63u);
	bool downNextPalCycle = false;
	uint32_t numPalCycleFrames = 0u;
	_0x6029546(0u, 10, 0, 6);
	Player *player1 = &Players[PLAYER1];
	Player *player2 = &Players[PLAYER2];

	GameLoopState state = GAMELOOP_CONTINUE;
	while (state == 0) {
		if (UpdateGame()) {
			_0x602406E();
			return GAMELOOP_TEST;
		}

		_0x6008F2C();
		ShowPalCycleText(218 + 67, 218, VERSION_NAME, true);
		if ((++numPalCycleFrames % 64u) == 0u) {
			if (downNextPalCycle = !downNextPalCycle) {
				NewPalCycle(PALNUM_PALCYCLETEXT, _0x6033748, _0x67910, 1, PALCYCLETYPE_DOWNSTOP, 1u, 63u);
			}
			else {
				NewPalCycle(PALNUM_PALCYCLETEXT, _0x6033748, _0x67910, 1, PALCYCLETYPE_UPSTOP, 1u, 63u);
			}
		}

		InitSeed = Rand(894u);

		if (GameFlags & GAME_DOUBLESSTART) {
			state = _0x6008918();
		}
		else {
			if (GameFlags & GAME_CHALLENGER1P) {
				_0x6008134(player1);
			}
			else {
				_0x6008134(player2);
			}

			GameFlags &= ~(GAME_CHALLENGER1P | GAME_CHALLENGER2P);
			if (GameFlags & GAME_NEWCHALLENGER) {
				state = _0x600817E();
			}

			if (GameFlags & GAME_NEWVERSUSROUND) {
				_0x60083A0();
			}

			if ((GameFlags & GAME_VERSUS) && !(GameFlags & GAME_BIT11)) {
				state = _0x6008516();
			}

			if ((GameFlags & GAME_VERSUS) && (GameFlags & GAME_BIT13)) {
				_0x60088FC();
			}

			if (!(GameFlags & GAME_VERSUS) && (GameFlags & GAME_DOUBLES) && (!(player1->nowFlags & NOW_STARTED) || !(player2->nowFlags & NOW_STARTED))) {
				// BUG: The SH-2 code calculates this expression, but never assigns it to anything.
				// There's probably an oversight here, where the "&" was supposed to be "&=".
				// There's no problem though, because the following line resets GameFlags.
				GameFlags & ~(GAME_VERSUS | GAME_DOUBLES | GAME_BIT6 | GAME_NEWCHALLENGER);

				GameFlags = GAME_TWIN;
				player1->nowFlags |= NOW_INIT;
				player2->nowFlags |= NOW_INIT;
				_0x6029546(0u, 10, 0, 6);
			}

			if ((player1->nowFlags & NOW_GAMEOVER) && (player2->nowFlags & NOW_GAMEOVER)) {
				_0x6079299 = 2;
				if (++_0x6079294 < 180u) {
					state = 2;
				}

				if (_0x6014234()) {
					_0x6079294 = 0;
				}
			}
			else {
				_0x6079294 = 0;
			}
		}
	}

	InitSeed = Rand(1192u) + 1u;

	if (state == 2) {
		_0x6029546(2, 30, 72, 6);
	}

	while (_0x6064750) {
		if (UpdateFrame()) {
			_0x602406E();
			return GAMELOOP_TEST;
		}
	}

	BackupRankings();
	_0x602406E();
	_0x602E586();

	// BUG: This is very odd, but correct, code.
	//
	// The SH-2 code checks if state is not equal to GAMELOOP_STOP, but in both
	// cases the function returns GAMELOOP_STOP.
	if (state != GAMELOOP_STOP) {
		return GAMELOOP_STOP;
	}
	else {
		return GAMELOOP_STOP;
	}
}

// TODO: ...

int16_t _0x6008E38(int16_t level) {
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

int16_t _0x6008EC8(int16_t level) {
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

int16_t _0x6008EEC(int16_t level) {
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

MainLoopState CurrentMainLoopState;
void RunMainLoop() {
	while (true) {
		UpdateFrame();

		InitSeed += Rand(1192u) + 1u;

		switch (CurrentMainLoopState) {
			case MAINLOOP_DEMO:
				CurrentMainLoopState = RunAttractLoop();
				continue;

			case MAINLOOP_GAME:
				CurrentMainLoopState = RunGameLoop();
				continue;

			case MAINLOOP_TEST:
				CurrentMainLoopState = RunTestLoop();
				break;

			default:
				break;
		}

		CurrentMainLoopState = MAINLOOP_DEMO;
	}
}
