#include "Main/GameLoop.h"
#include "Main/DemoLoop.h"
#include "Main/Frame.h"
#include "Game/Play/Player.h"
#include "Game/Play/Ranking.h"
#include "Game/Graphics/ShowText.h"
#include "Game/Graphics/ShowVersusReadyGo.h"
#include "Game/Graphics/Entity.h"
#include "Game/Save.h"
#include "Game/Screen.h"
#include "Game/PalNum.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Input/Button.h"
#include "Sound/Sound.h"
#include "Lib/Math.h"

ROMDATA Color UNK_6032884[NUMPALCOLORS_4BPP];
ROMDATA Color UNK_60328C4[NUMPALCOLORS_4BPP];
ROMDATA Color PalCycleTextPal0[NUMPALCOLORS_4BPP];

// Used in CheckSetNewChallenger.
static uint16_t NumVersusNoItemFrames = 0u;
static uint16_t NumVersusCementFrames = 0u;

// Used in StartGameLoop.
static uint16_t UNK_6079294 = 0u;

// These are used in UpdateGameMusic.
static uint8_t UNK_6079296 = 0u;
static uint8_t UNK_6079297 = 0u;
static GameMusic CurrentGameMusic = GAMEMUSIC_0;
GameMusic NextGameMusic = GAMEMUSIC_0;
static uint8_t UNK_607929A = 0u; // TODO: FSM state. In range [0, 2].

typedef enum GameLoopState {
	GAMELOOP_RESTART = 0,
	GAMELOOP_STOP = 2,
	GAMELOOP_INIT = 5,
	GAMELOOP_TEST = 7,
	GAMELOOP_START = 8
} GameLoopState;
static GameLoopState GameLoop;

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
	NextGameMusic = 0u;

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

	   NumVersusNoItemFrames = 0;
	   NumVersusCementFrames = 0;
}

GameLoopState GameStartVersus() {
	ModeFlag modeFlags;
	
	if ((Players[PLAYER1].nowFlags & NOW_WAITING) && (Players[PLAYER2].nowFlags & NOW_WAITING)) {
		if (GameFlags & GAME_CHALLENGEDELAY) {
			NextGameMusic = 2u;
			Players[PLAYER1].nowFlags |= NOW_NOUPDATE;
			Players[PLAYER2].nowFlags |= NOW_NOUPDATE;
			if (GameButtonsDown[PLAYER1] == BUTTON_START && GameButtonsDown[PLAYER2] == BUTTON_START) {
				            NumVersusNoItemFrames++;
			}
			if (
				GameButtonsDown[PLAYER1] == (BUTTON_START | BUTTON_2 | BUTTON_1) &&
				GameButtonsDown[PLAYER2] == (BUTTON_START | BUTTON_2 | BUTTON_1)) {
				            NumVersusCementFrames++;
			}
		}
		else {
			SetPal(PALNUM_15, 1u, PALPTR(0xF9));
			SetPal(PALNUM_14, 1u, PALPTR(0xF8));
			NextGameMusic = 10u;
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
			CurrentGameBg.nextBgMapIndex = BGMAP_VERSUS;
			CurrentGameBg.UNK_10 |= 1u;
			Game.numVersusRoundWins[PLAYER2] = 0u;
			Game.numVersusRoundWins[PLAYER1] = 0u;
			Game.numVersusRounds = 0u;
			InitItems();
			ShowVersusReadyGo();
			if (NumVersusCementFrames >= TIME(0, 1, 0)) {
				Players[PLAYER1].modeFlags |= MODE_CEMENT;
				Players[PLAYER2].modeFlags |= MODE_CEMENT;
			}
			else if (NumVersusNoItemFrames >= TIME(0, 1, 0)) {
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
		ShowVersusReadyGo();
	}
}

static uint8_t NumVersusRounds[3] = { 1u, 2u, 3u };

uint8_t NumVersusRoundsSetting() {
	return NumVersusRounds[Settings[SETTING_NUMVERSUSROUNDS]];
}

GameLoopState UNK_6008406(PlayerNum playerNum) {
	NextPlayGameOver(&Players[playerNum]);

	Players[playerNum].modeFlags &= MODE_NORMAL | MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH;
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

	SetPal(PALNUM_15, 1u, PAL_MODESELECTED);
	SetPal(PALNUM_14, 1u, PAL_MODENOTSELECTED);

	do {
		if (UNK_6064750 == NULL) {
			return GAMELOOP_RESTART;
		}
	} while (!UpdateFrame());

	return GAMELOOP_TEST;
}

GameLoopState UNK_6008516() {
	GameFlag newGameFlags;
	PlayerNum playerNum = Game.versusWinner;
	PlayerNum otherPlayerNum = Game.versusWinner == PLAYER1;
	GameLoopState nextState = GAMELOOP_RESTART;

	const uint16_t maxLevel = NextSectionLevels[Settings[SETTING_MAXVERSUSSECTION]];
	switch (Game.state) {
	case 0u:
		if (!(GameFlags & (GAME_WINNER1P | GAME_WINNER2P))) {
			GameFlags &= ~(GAME_BIT18 | GAME_BIT19);
			if (Players[PLAYER1].level < maxLevel && Players[PLAYER2].level < maxLevel) {
				if (Players[PLAYER1].clearTime == 0u) {
					const uint16_t playerLevel = Players[playerNum].level;
					const uint16_t otherPlayerLevel = Players[otherPlayerNum].level;
					if (playerLevel < otherPlayerLevel) {
						if (otherPlayerNum == PLAYER2) {
							newGameFlags = GAME_WINNER2P;
						}
						else {
							newGameFlags = GAME_WINNER1P;
						}
					}
					else {
						if (playerNum == PLAYER1) {
							newGameFlags = GAME_WINNER1P;
						}
						else {
							newGameFlags = GAME_WINNER2P;
						}
					}
					GameFlags |= newGameFlags;
					if (playerLevel == otherPlayerLevel) {
						GameFlags |= GAME_BIT19;
					}
					else {
						GameFlags |= GAME_BIT18;
					}
				}
			}
			else {
				if (maxLevel <= Players[PLAYER1].level) {
					Players[PLAYER1].level = maxLevel;
				}
				if (maxLevel <= Players[PLAYER2].level) {
					Players[PLAYER2].level = maxLevel;
				}
				if (Players[playerNum].level < Players[otherPlayerNum].level) {
					if (otherPlayerNum == PLAYER2) {
						newGameFlags = GAME_WINNER2P;
					}
					else {
						newGameFlags = GAME_WINNER1P;
					}
					GameFlags |= newGameFlags;
				}
				else {
					if (Game.versusWinner == PLAYER1) {
						newGameFlags = GAME_WINNER1P;
					}
					else {
						newGameFlags = GAME_WINNER2P;
					}
					GameFlags |= newGameFlags;
				}
			}
		}
		if (GameFlags & (GAME_WINNER1P | GAME_WINNER2P)) {
			Game.state = 1u;
			Game.UNK_2 = 300;
			if (GameFlags & GAME_WINNER1P) {
				Game.numVersusRoundWins[PLAYER1] += 1u;
			}
			if (GameFlags & GAME_WINNER2P) {
				Game.numVersusRoundWins[PLAYER2] += 1u;
			}
			NextPlayVersusOver(&Players[PLAYER1]);
			NextPlayVersusOver(&Players[PLAYER2]);
			InitItems();
			if ((GameFlags & GAME_WINNER1P) && (GameFlags & GAME_WINNER2P)) {
				Game.UNK_2 = 120;
				GameFlags = (GameFlags & ~(GAME_WINNER1P | GAME_WINNER2P | GAME_BIT18 | GAME_BIT19)) | GAME_BIT10;
			}
		}
		break;

	case 1u:
		if (((GameButtonsNew[PLAYER1] & BUTTON_START) || (GameButtonsNew[PLAYER2] & BUTTON_START)) && Game.UNK_2 > 10) {
			Game.UNK_2 = 10;
		}
		Game.UNK_2--;
		if (Game.UNK_2 == 0) {
			const bool playerCompletedVersus = Game.numVersusRoundWins[playerNum] >= NumVersusRounds[Settings[SETTING_NUMVERSUSROUNDS]];
			const bool otherPlayerCompletedVersus = Game.numVersusRoundWins[otherPlayerNum] >= NumVersusRounds[Settings[SETTING_NUMVERSUSROUNDS]];
			if (!(GameFlags & GAME_BIT10)) {
				GameFlags |= GAME_BIT11;

				uint32_t* sectionTime = BestMasterSectionTimes;
				for (int16_t section = 0; section < 10; section++, sectionTime++) {
					*sectionTime = Save->rankings[section].data;
				}
				if (playerCompletedVersus || otherPlayerCompletedVersus) {
					PlayerNum versusLoser;
					if (playerCompletedVersus) {
						if (Game.numVersusWins < 99u) {
							Game.numVersusWins++;
						}
						else {
							Game.numVersusWins = 99u;
						}
						Players[playerNum].modeFlags = Game.modeFlags[playerNum];
						versusLoser = otherPlayerNum;
					}
					else {
						Game.numVersusWins = 1u;
						Game.versusWinner = otherPlayerNum;
						Players[otherPlayerNum].modeFlags = Game.modeFlags[playerNum];
						versusLoser = playerNum;
					}
					nextState = UNK_6008406(versusLoser);
				}
				else {
					GameFlags |= GAME_BIT11 | GAME_BIT13;
				}
			}
			else {
				if (playerCompletedVersus || otherPlayerCompletedVersus) {
					if (playerCompletedVersus) {
						if (Game.versusWinner == PLAYER1) {
							newGameFlags = GAME_WINNER1P;
						}
						else {
							newGameFlags = GAME_WINNER2P;
						}
					}
					else {
						if (Game.versusWinner == PLAYER1) {
							newGameFlags = GAME_WINNER2P;
						}
						else {
							newGameFlags = GAME_WINNER1P;
						}
					}
					GameFlags = (GameFlags & ~GAME_BIT10) | newGameFlags;
					Game.UNK_2 = 300;
				}
				else {
					GameFlags |= GAME_BIT11 | GAME_BIT13;
				}
			}
		}
		break;

	default:
		break;
	}

	return nextState;
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

StartPlayerFlag StartPlayers;
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
		if (StartPlayers & (1 << playerNum)) {
			player->nowFlags |= NOW_STARTED | NOW_WAITING;
			InitPlayer(playerNum);
		}
	}

	Game.versusWinner = (Players[PLAYER1].nowFlags & NOW_STARTED) ? PLAYER1 : PLAYER2;

	while (UNK_6064750 != NULL) {
		if (UpdateFrame()) return GAMELOOP_TEST;
	}

	UNK_60169DC();
	UNK_6026FCA(CurrentGameBg.bgIndex, 1);
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
			if ((downNextPalCycle = !downNextPalCycle)) {
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
				state = UNK_6008516();
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
				NextGameMusic = 2;
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

GameMusic UNK_6008E38(uint16_t level) {
	if (level < 495u) {
		return GAMEMUSIC_1;
	}
	else if (level >= 495u && level < 500u)  {
		return GAMEMUSIC_2;
	}
	else if (level >= 500u && level < 695u) {
		return GAMEMUSIC_3;
	}
	else if (level >= 695u && level < 700u) {
		return GAMEMUSIC_4;
	}
	else if (level >= 700u && level < 880u) {
		return GAMEMUSIC_5;
	}
	else if (level >= 880u && level < 900u) {
		return GAMEMUSIC_6;
	}
	else if (level >= 900u) {
		return GAMEMUSIC_7;
	}
	return GAMEMUSIC_7;
}

GameMusic UNK_6008EC8(uint16_t level) {
	if (level < 280u) {
		return GAMEMUSIC_1;
	}
	else if (level >= 280u && level < 300u) {
		return GAMEMUSIC_2;
	}
	else if (level >= 300u) {
		return GAMEMUSIC_3;
	}
	return GAMEMUSIC_3;
}

GameMusic UNK_6008EEC(uint16_t level) {
	if (level < 280u) {
		return GAMEMUSIC_3;
	}
	else if (level >= 280u && level < 300u) {
		return GAMEMUSIC_2;
	}
	else if (level >= 300u && level < 480u) {
		return GAMEMUSIC_5;
	}
	else if (level >= 480u && level < 500u) {
		return GAMEMUSIC_2;
	}
	else if (level >= 500u) {
		return GAMEMUSIC_7;
	}
	return GAMEMUSIC_7;
}

static const uint8_t UNK_3A8CF[11][2] = {
	{ 1u, 5u },
	{ 0u, 0u },
	{ 8u, 1u },
	{ 1u, 5u },
	{ 8u, 1u },
	{ 1u, 5u },
	{ 8u, 1u },
	{ 1u, 5u },
	{ 4u, 3u },
	{ 1u, 1u },
	{ 4u, 3u }
};

void UpdateGameMusic() {
	GameMusic gameMusic1p, gameMusic2p;
	Music music;

	if (!(Players[PLAYER1].nowFlags & NOW_STARTED) || (Players[PLAYER1].modeFlags & MODE_VERSUS)) {
		gameMusic1p = GAMEMUSIC_NEGATIVE;
	}
	else if (!(Players[PLAYER1].nowFlags & NOW_SELECTING) || ((Players[PLAYER2].nowFlags & NOW_PLAYING) && !(Players[PLAYER2].nowFlags & NOW_STOPPED))) {
		if ((Players[PLAYER1].nowFlags & NOW_SELECTING) && (Players[PLAYER2].nowFlags & NOW_PLAYING) && !(Players[PLAYER2].nowFlags & NOW_STOPPED)) {
			gameMusic1p = GAMEMUSIC_NEGATIVE;
		}
		else if (!(Players[PLAYER1].nowFlags & NOW_NAMEENTRY)) {
			if (!(Players[PLAYER1].nowFlags & NOW_STAFF) || !(Players[PLAYER1].modeFlags & MODE_MASTER) || !(GameFlags & GAME_TWIN)) {
				if (!(Players[PLAYER1].nowFlags & NOW_STAFF) || !(Players[PLAYER1].modeFlags & MODE_NORMAL) || !(GameFlags & GAME_TWIN)) {
					if (!(Players[PLAYER1].nowFlags & NOW_STAFF) || !(Players[PLAYER1].modeFlags & (MODE_TGMPLUS | MODE_TADEATH)) || !(GameFlags & GAME_TWIN)) {
						if (Players[PLAYER1].nowFlags & NOW_WAITING) {
							gameMusic1p = GAMEMUSIC_NEGATIVE;
						}
						if (Players[PLAYER1].play.state != PLAYSTATE_START || (Players[PLAYER2].nowFlags & NOW_PLAYING)) {
							if (!(Players[PLAYER1].nowFlags & NOW_STOPPED)) {
								if (!(GameFlags & GAME_TWIN)) {
									if (!(Players[PLAYER1].modeFlags & MODE_DOUBLES)) {
										gameMusic1p = UNK_6008E38(Players[PLAYER1].level);
									}
								}
								else if (Players[PLAYER1].modeFlags & MODE_TADEATH) {
									uint16_t level;
									if (Players[PLAYER1].level > 500u) {
										level = 500u;
									}
									else {
										level = Players[PLAYER1].level;
									}
									gameMusic1p = UNK_6008EEC(level);
								}
								else if (!(GameFlags & GAME_TWIN) || !(Players[PLAYER1].modeFlags & MODE_NORMAL)) {
									if (!(Players[PLAYER1].modeFlags & MODE_DOUBLES)) {
										gameMusic1p = UNK_6008E38(Players[PLAYER1].level);
									}
									else {
										gameMusic1p = UNK_6008EC8(Players[PLAYER1].level);
									}
								}
								else {
									gameMusic1p = UNK_6008EC8(Players[PLAYER1].level);
								}
							}
							else if (Players[PLAYER1].values[2] != 0) {
								gameMusic1p = GAMEMUSIC_NEGATIVE;
							}
						}
						else {
							gameMusic1p = GAMEMUSIC_2;
						}
					}
					else {
						gameMusic1p = GAMEMUSIC_7;
					}
				}
				else {
					gameMusic1p = GAMEMUSIC_3;
				}
			}
			else {
				gameMusic1p = GAMEMUSIC_9;
			}
		}
		else {
			gameMusic1p = GAMEMUSIC_8;
		}
	}
	else {
		gameMusic1p = GAMEMUSIC_0;
	}

	if (!(Players[PLAYER2].nowFlags & NOW_STARTED) || (Players[PLAYER2].modeFlags & MODE_VERSUS)) {
		gameMusic2p = GAMEMUSIC_NEGATIVE;
	}
	else if (!(Players[PLAYER2].nowFlags & NOW_SELECTING) || ((Players[PLAYER1].nowFlags & NOW_PLAYING) && !(Players[PLAYER1].nowFlags & NOW_STOPPED))) {
		if ((Players[PLAYER2].nowFlags & NOW_SELECTING) && (Players[PLAYER1].nowFlags & NOW_PLAYING) && !(Players[PLAYER1].nowFlags & NOW_STOPPED)) {
			gameMusic2p = GAMEMUSIC_NEGATIVE;
		}
		if (!(Players[PLAYER2].nowFlags & NOW_NAMEENTRY)) {
			if (!(Players[PLAYER2].nowFlags & NOW_STAFF) || !(Players[PLAYER2].modeFlags & MODE_MASTER) || !(GameFlags & GAME_TWIN)) {
				if (!(Players[PLAYER2].nowFlags & NOW_STAFF) || !(Players[PLAYER2].modeFlags & MODE_NORMAL) || !(GameFlags & GAME_TWIN)) {
					if (!(Players[PLAYER2].nowFlags & NOW_STAFF) || !(Players[PLAYER2].modeFlags & (MODE_TGMPLUS | MODE_TADEATH)) || !(GameFlags & GAME_TWIN)) {
						if (Players[PLAYER2].nowFlags & NOW_WAITING) {
							gameMusic2p = GAMEMUSIC_NEGATIVE;
						}
						else if (Players[PLAYER2].play.state != PLAYSTATE_START || (Players[PLAYER1].nowFlags & NOW_PLAYING)) {
							if (!(Players[PLAYER2].nowFlags & NOW_STOPPED)) {
								if (!(GameFlags & GAME_TWIN)) {
									if (!(Players[PLAYER2].modeFlags & MODE_DOUBLES)) {
										gameMusic2p = UNK_6008E38(Players[PLAYER2].level);
									}
								}
								else if (Players[PLAYER2].modeFlags & MODE_TADEATH) {
									uint16_t level;
									if (Players[PLAYER2].level > 500u) {
										level = 500u;
									}
									else {
										level = Players[PLAYER2].level;
									}
									gameMusic2p = UNK_6008EEC(level);
								}
								else if (!(GameFlags & GAME_TWIN) || !(Players[PLAYER2].modeFlags & MODE_NORMAL)) {
									if (!(Players[PLAYER2].modeFlags & MODE_DOUBLES)) {
										gameMusic2p = UNK_6008E38(Players[PLAYER2].level);
									}
									else {
										gameMusic2p = UNK_6008EC8(Players[PLAYER2].level);
									}
								}
								else {
									gameMusic2p = UNK_6008EC8(Players[PLAYER2].level);
								}
							}
							else if (Players[PLAYER2].values[2] != 0) {
								gameMusic2p = GAMEMUSIC_NEGATIVE;
							}
						}
						else {
							gameMusic2p = GAMEMUSIC_2;
						}
					}
					else {
						gameMusic2p = GAMEMUSIC_7;
					}
				}
				else {
					gameMusic2p = GAMEMUSIC_3;
				}
			}
			else {
				gameMusic2p = GAMEMUSIC_9;
			}
		}
		else {
			gameMusic2p = GAMEMUSIC_8;
		}
	}
	else {
		gameMusic2p = GAMEMUSIC_0;
	}

	if (gameMusic1p > GAMEMUSIC_NEGATIVE && gameMusic2p > GAMEMUSIC_NEGATIVE) {
		GameMusic gameMusic;
		if (gameMusic1p < gameMusic2p) {
			gameMusic = gameMusic2p;
		}
		else {
			gameMusic = gameMusic1p;
		}

		if (Game.music != gameMusic) {
			CurrentGameMusic = gameMusic;
		}
	}
	else if (gameMusic1p > GAMEMUSIC_NEGATIVE || gameMusic2p > GAMEMUSIC_NEGATIVE) {
		GameMusic gameMusic;
		if (gameMusic1p == GAMEMUSIC_NEGATIVE) {
			gameMusic = gameMusic2p;
		}
		else {
			gameMusic = gameMusic1p;
		}

		if (Game.music != gameMusic) {
			CurrentGameMusic = gameMusic;
		}
	}

	if (NextGameMusic > GAMEMUSIC_NEGATIVE) {
		CurrentGameMusic = NextGameMusic;
		NextGameMusic = GAMEMUSIC_NEGATIVE;
	}

	switch (UNK_607929A) {
	case 0u:
		if (CurrentGameMusic > GAMEMUSIC_NEGATIVE) {
			if (CurrentGameMusic == Game.music) {
				CurrentGameMusic = GAMEMUSIC_NEGATIVE;
			}
			else {
				UNK_607929A = 1u;
				UNK_6079297 = 0u;
			}
		}
		break;

	case 1u:
		if (UNK_6079297 < UNK_6079296) {
			if (UNK_3A8CF[CurrentGameMusic][0] == 0u) {
				UNK_6079296 = UNK_6079297;
			}
			else {
				if (ScreenTime % UNK_3A8CF[CurrentGameMusic][0] == 0u) {
					UNK_6079296 -= 1u;
				}
			}

			UNK_602E72A(UNK_6079296);
		}
		else {
			UNK_607929A = 2u;
			UNK_6079297 = 5u;

			switch (CurrentGameMusic) {
			case GAMEMUSIC_0:
				music = MUSIC_7;
				break;

			default:
				music = MUSIC_0;
				break;

			case GAMEMUSIC_2:
			case GAMEMUSIC_4:
			case GAMEMUSIC_6:
				music = MUSIC_STOP;
				break;

			case GAMEMUSIC_3:
				music = MUSIC_1;
				break;

			case GAMEMUSIC_5:
				music = MUSIC_2;
				break;

			case GAMEMUSIC_7:
				music = MUSIC_3;
				break;

			case GAMEMUSIC_8:
				music = MUSIC_6;
				break;

			case GAMEMUSIC_9:
				music = MUSIC_5;
				break;

			case GAMEMUSIC_10:
				music = MUSIC_4;
				break;
			}

			if (music <= MUSIC_STOP) {
				StopMusic();
			}
			else {
				PlayMusic(music);
			}

			Game.music = CurrentGameMusic;
			UNK_602E72A(UNK_6079296);
		}
		break;

	case 2u:
		if (UNK_6079297 > UNK_6079296) {
			if (UNK_3A8CF[CurrentGameMusic][1] == 0u) {
				UNK_6079296 = UNK_6079297;
			}
			else {
				if (ScreenTime % UNK_3A8CF[CurrentGameMusic][1] == 0u) {
					UNK_6079296 += 1u;
				}
			}
		}
		else {
			CurrentGameMusic = GAMEMUSIC_NEGATIVE;
			UNK_607929A = 0u;
		}

		UNK_602E72A(UNK_6079296);
		break;

	default:
		break;
	}
}
