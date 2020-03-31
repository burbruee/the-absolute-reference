#pragma once

#include "Player.h"
#include "Block.h"
#include "Matrix.h"
#include "Medal.h"
#include "Item.h"
#include "RankingCode.h"
#include "HwInput.h"
#include "Macros.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum LineFlag {
	LINEFLAG_NONE = 0,
	LINEFLAG_HARDCLEAR = 1 << 31
} LineFlag;

typedef enum SubState {
	SUBSTATE_SELECT = 0,
	SUBSTATE_READYGO = 0,
	SUBSTATE_GARBAGE = 2
} SubState;

typedef enum SelectState {
	SELECT_START = 0,
	SELECT_MODE = 10,
	// NOTE: If the select state is 20 or 21, then no updating of selection is done.
	SELECT_20 = 20,
	SELECT_21 = 21,
	SELECT_CHALLENGE = 30,
	SELECT_SINGLE = 0xFF
} SelectState;

typedef enum GarbageState {
	GARBAGE_ENTRY,
	GARBAGE_DELAY,
	GARBAGE_END
} GarbageState;
#define GARBAGEHEIGHT 8

/*
// TODO: Remove this, and define index constants where Player::values is used.
// Do it in the style established with items and entitites, such as:
// Definition:
// #define menuIndex values[0]
// Usage:
// player->menuIndex
typedef enum PlayerValue {
	PLAYERVALUE_MENU = 0,
	PLAYERVALUE_NUMSELECTFRAMES = 1,
	PLAYERVALUE_READYGOHEIGHT = 0,
	PLAYERVALUE_READYGOY = 1,
	PLAYERVALUE_ROTATENEXT = 0, // TODO: Versus IRS?
	PLAYERVALUE_NUMENTRYFRAMES = 0,
	PLAYERVALUE_NUMCLEARFRAMES = 0,
	// TODO: Some game over related index of 2; other names may make sense
	// for index 2 as well.
	PLAYERVALUE_NUMSECRETGRADEROWS = 3
} PlayerValue;
*/

#define ROTATENEXT_DISABLED 80 // TODO: Versus IRS?

typedef enum NowFlag {
	NOW_NULL = 0, // TODO: Might not need this one.
	NOW_SELECTING = 1 << 0,
	NOW_WAITING = 1 << 1,
	NOW_PLAYING = 1 << 2,
	NOW_ABSENT = 1 << 4,
	NOW_STAFF = 1 << 5,
	NOW_STOPPED = 1 << 6,
	NOW_LOCKING = 1 << 7,
	NOW_NAMEENTRY = 1 << 8,
	NOW_BIT10 = 1 << 10,
	NOW_BIT11 = 1 << 11,
	NOW_STARTED = 1 << 12,
	NOW_INIT = 1 << 13,
	NOW_SHOWTLSBLOCK = 1 << 14,
	NOW_BIT15 = 1 << 15, // TODO: Probably a "NOW_SHOW*" constant. Purpose not known yet.
	NOW_SHOWFIELD = 1 << 16,
	NOW_SHOWACTIVEBLOCK = 1 << 17,
	NOW_SHOWNEXTBLOCK = 1 << 18,
	NOW_SHOWRANKINGCODE = 1 << 19,
	NOW_SKILLCOMBO = 1 << 20,
	NOW_NOINVISIBLE = 1 << 21,
	NOW_GAMEOVER = 1 << 28,
	NOW_NOGARBAGE = 1 << 30,
	NOW_NOUPDATE = 1 << 31
} NowFlag;

typedef enum ModeFlag {
	MODE_NONE = 0,
	MODE_NORMAL = 1 << 0,
	MODE_MASTER = 1 << 1,
	MODE_DOUBLES = 1 << 2,
	MODE_VERSUS = 1 << 3,
	MODE_INVISIBLE = 1 << 4,
	MODE_20G = 1 << 5,
	MODE_BIG = 1 << 6,
	MODE_TGMPLUS = 1 << 7,
	MODE_CEMENT = 1 << 8,
	MODE_ITEM = 1 << 9,
	MODE_TLS = 1 << 10,
	MODE_NOITEM = 1 << 11,
	MODE_TADEATH = 1 << 12,
	MODE_TIMEOUT = 1 << 13,
	MODE_SELECTIONS = MODE_NORMAL | MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH | MODE_DOUBLES | MODE_VERSUS
} ModeFlag;

// Selectable modes. Versus isn't considered a mode on its own the way normal,
// master, etc. are.
typedef enum ModeSelection {
	MODESELECTION_NORMAL,
	MODESELECTION_MASTER,
	MODESELECTION_TGMPLUS,
	MODESELECTION_TADEATH,
	MODESELECTION_DOUBLES,
	NUMMODESELECTIONS,
	NUMSINGLEMODESELECTIONS = NUMMODESELECTIONS - 1
} ModeSelection;

typedef enum PlayerGrade {
	PLAYERGRADE_9,
	PLAYERGRADE_8,
	PLAYERGRADE_7,
	PLAYERGRADE_6,
	PLAYERGRADE_5,
	PLAYERGRADE_4,
	PLAYERGRADE_3,
	PLAYERGRADE_2,
	PLAYERGRADE_1,
	PLAYERGRADE_S1,
	PLAYERGRADE_S2,
	PLAYERGRADE_S3,
	PLAYERGRADE_S4,
	PLAYERGRADE_S5,
	PLAYERGRADE_S6,
	PLAYERGRADE_S7,
	PLAYERGRADE_S8,
	PLAYERGRADE_S9,
	PLAYERGRADE_M,
	PLAYERGRADE_GM,
	NUMPLAYERGRADES
} PlayerGrade;

typedef enum MGradeFlag {
	MGRADE_CHECKPOINT1 = 1 << 0,
	MGRADE_CHECKPOINT2 = 1 << 1,
	MGRADE_CHECKPOINT3 = 1 << 2,
	MGRADE_SECTION9TIME = 1 << 3,
	MGRADE_SECTIONTIMES = 1 << 4,
	MGRADE_SKILLCLEARS = 1 << 5,
	MGRADE_S9GRADE = 1 << 6,
	// The qualifications here are not in error; TAP doesn't require all
	// checked conditions.
	MGRADE_QUALIFIED = MGRADE_CHECKPOINT1 | MGRADE_CHECKPOINT3 | MGRADE_SECTIONTIMES | MGRADE_SKILLCLEARS | MGRADE_S9GRADE
} MGradeFlag;

typedef enum MiscFlag{
	MISC_NONE = 0,
	MISC_SKILLCLEARS1 = 1 << 0,
	MISC_SKILLCLEARS2 = 1 << 1,
	MISC_SKILLCLEARS3  = MISC_SKILLCLEARS1 | MISC_SKILLCLEARS2,
	MISC_NUMSKILLCLEARS = MISC_SKILLCLEARS3,
	MISC_RECOVERING = 1 << 2,
	MISC_FORCELOWG = 1 << 3,
	MISC_ITEMHEADSUP = 1 << 4,
	MISC_ORANGELINE = 1 << 5
} MiscFlag;

typedef enum PlayFlag {
	PLAYFLAG_NONE,
	PLAYFLAG_FORCEENTRY = 1 << 7
} PlayFlag;

typedef enum PlayState {
	PLAYSTATE_NULL,
	PLAYSTATE_NEXT,
	PLAYSTATE_ACTIVE,
	PLAYSTATE_LOCK,
	PLAYSTATE_CLEAR,
	PLAYSTATE_ENTRY,
	PLAYSTATE_GARBAGEENTRY,
	PLAYSTATE_GAMEOVER,
	PLAYSTATE_BLOCKEDENTRY,
	PLAYSTATE_VERSUSOVER,
	PLAYSTATE_START,
	PLAYSTATE_RANKING,
	PLAYSTATE_GARBAGECHECK,
	PLAYSTATE_STAFFTRANSITION
} PlayState;

typedef struct PlayData {
	PlayFlag flags;
	PlayState state;
} PlayData;

typedef enum Rotation {
	ROTATION_DOWN,
	ROTATION_RIGHT,
	ROTATION_UP,
	ROTATION_LEFT,
	ROTATION_ENTRY = ROTATION_DOWN
} Rotation;
#define ROTATE_LEFT(r) (((r) + 1) % 4)
#define ROTATE_RIGHT(r) (((r) + 3) % 4)
#define ROTATED_ANY(buttons) ((buttons) & (BUTTON_1 | BUTTON_2 | BUTTON_3))
#define ROTATED_LEFT(buttons) ((buttons) & (BUTTON_1 | BUTTON_3))
#define ROTATED_RIGHT(buttons) ((buttons) & BUTTON_2)

#define MATRIX(player, row, col) ((player)->matrix[(row) * (player)->matrixWidth + (col)])

#define CLEARTIME_TIMEOUT TIME(15, 0, 0)
#define CLEARTIME_MAX TIME(100, 39, 59)

// TODO: Reorder. The SH-2 compiler reordered the fields to pack more tightly.
// Look at init functions for ordering and grouping.
typedef struct Player Player;
struct Player {
	MatrixBlock *matrix;
	Block garbage[MATRIX_SINGLEWIDTH * GARBAGEHEIGHT];
	uint8_t fieldHeight;
	uint8_t fieldWidth;
	uint8_t matrixHeight;
	uint8_t matrixWidth;
	int16_t screenOffset[2];
	//int16_t versusItemType; // Only used in UpdateVersusItems(), which is an unused function. That function could be a relic of TGM1's code, or was for an older version of TGM2.
	Player *otherPlayer;
	Player *itemPlayer;
	uint8_t subStates[4];
	int16_t values[4];
	NowFlag nowFlags;
	ModeFlag modeFlags;
	PlayerNum num;
	uint32_t seed;
	int16_t screenPos[2];
	Fixed16 nextScale;
	Fixed16 nextScaleV;
	Fixed16 nextScaleA;
	Fixed16 levelScale;
	Fixed16 levelScaleV;
	uint16_t level;
	uint16_t numLines;
	uint16_t numBlocks;
	int16_t combo;
	int16_t comboBefore;
	uint32_t score;
	uint32_t mGradeSectionTime;
	MGradeFlag mGradeFlags;
	MiscFlag miscFlags;
	MedalColor medalColors[NUMMEDALTYPES];
	uint8_t numSkillClears;
	uint8_t numRecoveries;
	MedalColor nextRotateMedalColor;
	uint8_t numComboClears;
	uint16_t numFieldBlocks;
	uint8_t normalItemIndex;
	int8_t lockFrames;
	int8_t lockDelay;
	uint8_t autoshiftFrames;
	uint32_t gameTime; // Complete time duration of a game; includes ready-go time, for example. Also, Save->gameTime is a total of this field from all games.
	uint32_t clearTime; // Time spent playing; doesn't include ready-go time, for example. It's also the time displayed by the clock.
	uint32_t masteringTime; // Time until the last player grade change.
	Fixed32 gravity;
	PlayData play;
	Block activeBlock;
	Block nextBlock;
	Rotation activeRotation;
	Fixed32 activePos[2];
	uint8_t history[4];
	ItemBagFlag itemBagFlags;
	ItemBagFlag normalItemBagFlags;
	ItemType clearItemType;
	ItemType activeItemType;
	uint8_t itemEffectPos[2];
	ItemMiscFlag itemMiscFlags;
	uint8_t numItemBarBlocks;
	ItemType activeBlockItemType;
	ItemType nextBlockItemType;
	LineFlag lineFlags;
	uint8_t numGarbageRows;
	PlayerGrade grade;
	uint16_t section;
	PlayerGrade gradeBefore;
	Fixed16 gradeScale;
	uint16_t numRotations;
	uint16_t numRotateBlocks;
	uint16_t numActiveRotations;
	uint8_t numTgmPlusBlocks;
	uint8_t tgmPlusGarbageIndex;
	bool refusingChallenges;
	int16_t numActiveFrames;
	uint8_t numMGradeLines;
	uint8_t numMGradeSkillClears;
	uint8_t gradeBeforeMGrade;
	bool passedMGradeCheckpoint;
	char rankingCode[NUMRANKINGCODEDIGITS];
};

extern bool Attract;

// TODO: Rename the BIT constants once they're understood. Also, remove the
// entirely unused BIT constants, once all game code is written.
typedef enum GameFlag {
	GAME_TWIN = 1 << 0,
	GAME_VERSUS = 1 << 1,
	GAME_DOUBLES = 1 << 2,
	GAME_BIT3 = 1 << 3,
	GAME_CHALLENGER1P = 1 << 4,
	GAME_CHALLENGER2P = 1 << 5,
	GAME_BIT6 = 1 << 6,
	GAME_NEWCHALLENGER = 1 << 7,
	GAME_WINNER1P = 1 << 8,
	GAME_WINNER2P = 1 << 9,
	GAME_BIT10 = 1 << 10,
	GAME_BIT11 = 1 << 11, // TODO: GAME_PLAYERSABSENT? See UpdatePlayerStart (other locations?).
	GAME_BIT12 = 1 << 12,
	GAME_BIT13 = 1 << 13,
	GAME_NEWVERSUSROUND = 1 << 14,
	GAME_VERSUSREADYGO = 1 << 15,
	GAME_CHALLENGEDELAY = 1 << 16, // TODO: GAME_NEWCHALLENGER? And rename the other one?
	GAME_DOUBLESSTART = 1 << 17, // TODO: Set when starting doubles. GAME_STARTDOUBLES?
	GAME_BIT18 = 1 << 18,
	GAME_BIT19 = 1 << 19,
	// ...
	GAME_ITEMSNOUSE = 1 << 31
} GameFlag;
extern GameFlag GameFlags;

typedef enum GameMusic {
	GAMEMUSIC_0,
	GAMEMUSIC_1,
	GAMEMUSIC_2,
	GAMEMUSIC_3,
	GAMEMUSIC_4,
	GAMEMUSIC_5,
	GAMEMUSIC_6,
	GAMEMUSIC_7,
	GAMEMUSIC_8,
	GAMEMUSIC_9,
	GAMEMUSIC_10
} GameMusic;

typedef struct GameData {
	uint8_t state; // TODO: Change to enum?
	int16_t _0x2; // TODO: Used in versus, in the function at 0x60071D6.
	uint8_t numVersusRoundWins[2];
	uint8_t numVersusRounds;
	uint8_t numVersusWins;
	PlayerNum versusWinner;
	uint8_t _0x9; // TODO: Padding?
	ModeFlag modeFlags[NUMPLAYERS];
	int16_t music; // Set to GameMusic enum values. Has to be a signed int type, so check-if-negative is guaranteed to work.
	uint16_t _0x10; // TODO: Padding? Unused?
	uint16_t _0x12; // TODO: Padding? Unused?
} GameData;
extern GameData Game;

extern Player Players[NUMPLAYERS];
// Used in UpdateVersusItems.
//extern Player VersusPlayers[NUMPLAYERS];

void InitPlayers();

extern const int16_t FieldPos[8];
void SetFieldPos(Player *player, bool useVersusX);

extern const int16_t NextSectionLevels[10];
extern ItemType ItemDescriptions[NUMPLAYERS];
void InitPlayer(PlayerNum playerNum);

void UpdatePlayers();
void ShowPlayers();

// Nine block types, four rotations, each block is 4x4.
//
// The first two block definitions, for empty and wall squares, are unused, and
// only present so BLOCKTYPE_* constants can be used as indices.
typedef const uint8_t BlockDefSquare;
extern BlockDefSquare BlockDefs[9 * 4 * 4 * 4];
#define BLOCKDEF(type) (&BlockDefs[(type) * 4 * 4 * 4])
#define BLOCKDEFROW(blockDef, rotation, row) (&((blockDef)[(rotation) * 4 + (row) * 4 * 4]))
#define BLOCKDEFROWBIG(blockDef, rotation, row) (&(blockDef)[(rotation) * 4 + (row)])
#define BLOCKDEFSQUARE(blockDefRow, col) ((blockDefRow)[(col)])
#define BLOCKDEFSQUAREBIG(blockDefRow, col) ((blockDefRow)[(col) / 2])
#define BLOCKDEFSQUARE_EMPTY 0u

void NextPlayLock(Player* player);

void NextPlayGameOver(Player* player);

Fixed32 StepGravity(Player* player, Fixed32 gravity);

typedef enum LockType {
	LOCKTYPE_NORMAL,
	LOCKTYPE_BLOCKING,
	LOCKTYPE_GAMEOVER
} LockType;
void LockActiveBlock(Player *player, LockType lockType);

#define NUMMODECODES 5

// Item and invisible mode codes are disabled.
// They can't be reenabled by just changing this; code supporting those modes
// must be added.
#define NUMENABLEDMODECODES (NUMMODECODES - 2)

#define RANDOMIZER_USEHISTORY true
