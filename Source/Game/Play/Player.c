// TODO: Change all-caps names to capitalized, e.g. "BG" becomes "Bg".
// TODO: Change "EEPROM[0] & REGION_SETTING" to macro "EEPROM_REGION", in the
// EEPROM header? Maybe even have the frontends define the Eeprom.h header, so
// that PsikyoSH uses macros directly accessing EEPROM address space, and "OS"
// frontends define EEPROM access using functions.
// TODO: Make all functions and data only used here private.
#include "Game/Play/Player.h"
#include "Game/Play/Grade.h"
#include "Game/Play/GravityTable.h"
#include "Game/Play/DelayTable.h"
#include "Game/Play/Ranking.h"
#include "Game/Play/Debug.h"
#include "Game/Play/Item/Item.h"
#include "Game/Play/Item/ShowItemEffect.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowEffect.h"
#include "Game/Graphics/ShowGameStatus.h"
#include "Game/Graphics/ShowNewChallenger.h"
#include "Game/Graphics/ShowStaff.h"
#include "Game/Graphics/ShowGameOverFade.h"
#include "Game/Graphics/DisplayObject.h"
#include "Game/Screen.h"
#include "Game/Save.h"
#include "Game/Temp.h"
#include "Input/Input.h"
#include "Input/Button.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "Sound/Sound.h"
#include "Eeprom/Setting.h"
#include "Main/Frame.h"
#include "Lib/Math.h"
#include "Lib/Fixed.h"
#include "Lib/Macros.h"
#include "PlatformTypes.h"

BlockDefSquare BlockDefs[9 * 4 * 4 * 4] = {
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,4,0, 0,0,0,0, 0,0,1,0,
	1,2,3,4, 0,0,3,0, 4,3,2,1, 0,0,2,0,
	0,0,0,0, 0,0,2,0, 0,0,0,0, 0,0,3,0,
	0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,4,0,

	0,0,0,0, 0,0,4,0, 0,0,0,0, 0,0,1,0,
	1,2,0,0, 0,2,3,0, 4,3,0,0, 0,3,2,0,
	0,3,4,0, 0,1,0,0, 0,2,1,0, 0,4,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 4,0,0,0, 0,0,0,0, 1,0,0,0,
	0,3,4,0, 3,2,0,0, 0,2,1,0, 2,3,0,0,
	1,2,0,0, 0,1,0,0, 4,3,0,0, 0,4,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,3,4,0, 0,0,0,0, 0,1,0,0,
	1,2,3,0, 0,2,0,0, 4,0,0,0, 0,2,0,0,
	0,0,4,0, 0,1,0,0, 3,2,1,0, 4,3,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,4,0,0, 0,0,0,0, 1,2,0,0,
	2,3,4,0, 0,3,0,0, 0,0,1,0, 0,3,0,0,
	1,0,0,0, 0,2,1,0, 4,3,2,0, 0,4,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,1,2,0, 0,2,4,0, 0,4,3,0, 0,4,1,0,
	0,3,4,0, 0,1,3,0, 0,2,1,0, 0,3,2,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,3,0,0, 0,0,0,0, 0,1,0,0,
	1,2,3,0, 0,2,4,0, 0,4,0,0, 4,2,0,0,
	0,4,0,0, 0,1,0,0, 3,2,1,0, 0,3,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

const uint16_t NextSectionLevels[10] = {100u, 200u, 300u, 400u, 500u, 600u, 700u, 800u, 900u, 999u};

ItemType ItemDescriptions[NUMPLAYERS];

bool Demo;

GameFlag GameFlags;

GameData Game;

Player Players[NUMPLAYERS];
//Player VersusPlayers[NUMPLAYERS];

static uint8_t NumFastDropRows[NUMPLAYERS];
static uint8_t NumInstantDropRows[NUMPLAYERS];

// Flag that controls manual lock protection. Used in high speed 20G contexts,
// such as T.A. death; in those contexts, it's used to protect against manual
// locking of a sequence of blocks, where manual locking is only enabled if
// down wasn't pressed at some point for the active block. Could be used for
// sub-20G speeds, but isn't.
static bool ManualLockUnprotected[NUMPLAYERS];

typedef struct EntryData {
	Block tempMatrix[MATRIX_HEIGHT * FIELD_SINGLEWIDTH * NUMPLAYERS];
	uint16_t numMatrixBlockings;
	uint16_t numPlayerBlockings;
	uint8_t itemWeights[NUMITEMTYPES];
} EntryData;

static bool Blocked(Player* player, int16_t col, int16_t row, Rotation rotation);
bool UpdateModeCodes(Player* player);
static void BackupMatrix(Player* player);
void CheckDisableItemDescription(Player* player);
void CheckSetItemMode(Player* player);
uint16_t GenNextInt(uint32_t* seed, bool update);
void GenNextItem(Player* player);
void NextPlayStart(Player* player);
uint8_t NumSecretGradeRows(Player* player);
static void RestoreMatrix(Player* player);
void SetFieldVisible(Player* player);
void ThrowOutActiveBlock(Player* player);
void UpdatePlayActive(Player* player);
void UpdatePlayBlockedEntry(Player* player);
void UpdatePlayClear(Player* player);
void UpdatePlayEntry(Player* player);
void UpdatePlayGameOver(Player* player);
void UpdatePlayGarbageCheck(Player* player);
void UpdatePlayGarbageEntry(Player* player);
void UpdatePlayLock(Player* player);
void UpdatePlayNext(Player* player);
void UpdatePlayStaffTransition(Player* player);
void UpdatePlayStart(Player* player);
void UpdatePlayVersusOver(Player* player);
void UpdateSiren(Player* player);
ButtonInput Select(Player* player);
Fixed32 CurrentGravity(Player* player);
uint32_t PointsBaseValue(Player* player, uint8_t numLines);

static MatrixBlock MatrixBlocks[NUMMATRIXBLOCKS_SHARED];

void InitPlayers() {
	for (PlayerNum playerNum = PLAYER1; playerNum <= NUMPLAYERS - 1; playerNum++) {
		Players[playerNum].num = playerNum;
		Players[playerNum].nowFlags = NOW_GAMEOVER | NOW_ABSENT;
		Players[playerNum].modeFlags = MODE_MASTER;
		InitPlayer(playerNum);
	}
}

const int16_t FieldPos[8] = {
	 58, 207, // Player 1 twin and doubles Y.
	218, 207, // Player 2 twin and doubles Y.
	 62, 207, // Player 1 versus.
	258, 207  // Player 2 versus.
};

void SetFieldPos(Player* player, bool versusPos) {
	if (GameFlags & GAME_DOUBLES) {
		player->screenPos[0] = 160;
		player->screenPos[1] = FieldPos[player->num * 2 + 2 * 0 + 1];
	}
	else {
		if ((GameFlags & GAME_VERSUS) && versusPos) {
			player->screenPos[0] = FieldPos[player->num * 2 + 2 * 2 + 0];
		}
		else {
			player->screenPos[0] = FieldPos[player->num * 2 + 2 * 0 + 0];
		}
		player->screenPos[1] = FieldPos[player->num * 2 + 2 * 0 + 1];
	}
}

void InitPlayer(PlayerNum playerNum) {
	// Player pointers.
	Player* player = &Players[playerNum];
	player->otherPlayer = &Players[playerNum == PLAYER1];
	PlayerNum itemPlayerNum;
	if (player->modeFlags & MODE_VERSUS) {
		if (playerNum == PLAYER1) {
			itemPlayerNum = PLAYER2;
		}
		else {
			itemPlayerNum = PLAYER1;
		}
	}
	else {
		if (playerNum == PLAYER1) {
			itemPlayerNum = PLAYER1;
		}
		else {
			itemPlayerNum = PLAYER2;
		}
	}
	player->itemPlayer = &Players[itemPlayerNum];

	// Additional states handled within play states.
	uint8_t* subState = player->subStates;
	for (uint8_t i = 0; i < lengthoffield(Player, subStates); i++, subState++) {
		*subState = 0;
	}

	// Temporaries.
	int16_t* value = player->values;
	for (uint8_t i = 0; i < lengthoffield(Player, values); i++, value++) {
		*value = 0;
	}

	// What's happening/showing now.
	player->nowFlags &= ~(NOW_STAFF | NOW_STOPPED | NOW_LOCKING | NOW_NAMEENTRY | NOW_SKILLCOMBO | NOW_NOGARBAGE);
	player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK | NOW_SHOWNEXTBLOCK);
	player->nowFlags |= NOW_BIT15 | NOW_SHOWFIELD;

	// Active block delays.
	player->lockFrames = 30;
	player->lockDelay = 30;
	player->autoshiftFrames = 0;

	// Time.
	if (GameFlags & GAME_VERSUS) {
		player->clearTime = (NextSectionLevels[Settings[SETTING_MAXVERSUSSECTION]] / 100) * TIME(1, 20, 0);
	}
	else {
		player->clearTime = 0u;
	}

	// Advancement.
	player->numGarbageRows = 0;
	player->lineFlags = LINEFLAG_NONE;
	player->numBlocks = 0;
	player->numLines = 0;
	player->level = 0u;
	player->gravity = F32(0, 0x0000);
	player->normalItemIndex = 0;
	player->combo = 1;
	player->tgmPlusGarbageIndex = 0;
	player->numTgmPlusBlocks = 0;

	// Versus.
	player->refusingChallenges = false;

	// M grade.
	player->numMGradeSkillClears = 0;
	player->numMGradeLines = 0u;
	player->gradeBeforeMGrade = PLAYERGRADE_S9;
	player->passedMGradeCheckpoint = false;

	// Gameplay state.
	player->play = (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_START };

	// Active block.
	player->activeBlock = (Block)BLOCKTYPE_I;
	player->activeRotation = ROTATION_ENTRY;
	player->activePos[1] = ENTRYPOS_Y;
	if (GameFlags & GAME_DOUBLES) {
		if (player->num == PLAYER1) {
			player->activePos[0] = F32(ENTRY_DOUBLES1PCOL, 0x0000);
		}
		else {
			player->activePos[0] = F32(ENTRY_DOUBLES2PCOL, 0x0000);
		}
	}
	else {
		player->activePos[0] = F32(ENTRY_SINGLECOL, 0x0000);
	}

	// Items.
	player->itemBag = ITEMBAG_FULL;
	player->normalItemBag = ITEMBAG_FULL;
	player->clearItemType = ITEMTYPE_NULL;
	player->itemMiscFlags = ITEMMISC_NONE;
	player->numItemBarBlocks = 0u;
	player->activeItemType = ITEMTYPE_NULL;
	player->activeBlockItemType = ITEMTYPE_NULL;
	player->nextBlockItemType = ITEMTYPE_NULL;
	ItemDescriptions[playerNum] = ITEMTYPE_NULL;

	// Player GRS data.
	player->section = 0u;
	player->gradeScale = 0x0000u;
	player->gradeBefore = PLAYERGRADE_9;
	player->grade = PLAYERGRADE_9;
	player->numComboClears = 0u;
	player->comboBefore = 0u;
	player->numRotateBlocks = 0u;
	player->numRotations = 0u;
	player->numActiveRotations = 0u;
	player->nextRotateMedalColor = MEDALCOLOR_NULL;
	player->numRecoveries = 0u;
	player->numSkillClears = 0u;
	player->numFieldBlocks = 0u;
	player->miscFlags = MISC_NONE;
	player->mGradeSectionTime = 0u;
	player->score = 0u;
	player->levelScaleV = F16(0, 0x0000);
	player->levelScale = F16(0, 0x0000);
	player->nextScaleA = F16(0, 0x0000);
	player->nextScaleV = F16(0, 0x0000);
	player->nextScale = F16(0, 0x0000);
	player->mGradeFlags = MGRADE_SECTIONTIMES | MGRADE_SKILLCLEARS;
	MedalColor* medal = player->medalColors;
	for (uint8_t i = 0u; i < NUMMEDALTYPES; i++, medal++) {
		*medal = MEDALCOLOR_NULL;
	}

	// Block randomizer.
	if ((GameFlags & GAME_DOUBLES) && !Demo) {
		InitSeed += Rand(1192u) + 1u;
	}
	if ((GameFlags & GAME_TWIN) && !Demo) {
		InitSeed += Rand(1192u) + 1u;
	}
	player->seed = InitSeed;
	uint8_t nextBlockNum = GenNextInt(&player->seed, true) % 7;
	if (nextBlockNum == TOBLOCKNUM(BLOCKTYPE_Z) || nextBlockNum == TOBLOCKNUM(BLOCKTYPE_S) || nextBlockNum == TOBLOCKNUM(BLOCKTYPE_O)) {
		bool rejected = true;
		while (rejected) {
			nextBlockNum = GenNextInt(&player->seed, true) % 7;
			if (nextBlockNum != TOBLOCKNUM(BLOCKTYPE_Z) && nextBlockNum != TOBLOCKNUM(BLOCKTYPE_S) && nextBlockNum != TOBLOCKNUM(BLOCKTYPE_O)) {
				rejected = false;
			}
		}
	}
	player->nextBlock = (Block)TOBLOCKTYPE(nextBlockNum);
	size_t historyIndex = 0u;
	for (; historyIndex < lengthoffield(Player, history) / 2; historyIndex++) {
		player->history[historyIndex] = TOBLOCKNUM(BLOCKTYPE_Z);
	}
	for (; historyIndex < lengthoffield(Player, history); historyIndex++) {
		player->history[historyIndex] = TOBLOCKNUM(BLOCKTYPE_S);
	}
	player->history[0] = TOBLOCKNUM(player->nextBlock & BLOCK_TYPE);

	// Field and garbage.
	if (GameFlags & GAME_DOUBLES) {
		player->matrix = MatrixBlocks;
		player->fieldHeight = FIELD_HEIGHT;
		player->fieldWidth = FIELD_DOUBLESWIDTH;
		player->matrixHeight = MATRIX_HEIGHT;
		player->matrixWidth = MATRIX_DOUBLESWIDTH;
	}
	else {
		player->matrix = &MatrixBlocks[playerNum * NUMMATRIXBLOCKS_SINGLE];
		player->fieldHeight = FIELD_HEIGHT;
		player->fieldWidth = FIELD_SINGLEWIDTH;
		player->matrixHeight = MATRIX_HEIGHT;
		player->matrixWidth = MATRIX_SINGLEWIDTH;
	}
	Block* rowBlocks = player->garbage;
	for (size_t row = 0u; row < GARBAGEHEIGHT; row++) {
		Block* block = rowBlocks;
		for (size_t col = 0u; col < MATRIX_SINGLEWIDTH; col++) {
			*block++ = NULLBLOCK;
		}
		rowBlocks += MATRIX_SINGLEWIDTH;
	}
	if (!(GameFlags & GAME_DOUBLES) || playerNum != PLAYER2) {
		for (size_t row = 0u; row < player->matrixHeight; row++) {
			for (size_t col = 0u; col < player->matrixWidth; col++) {
				if (row == 0u) {
					MATRIX(player, row, col).block = (Block)BLOCKTYPE_WALL;
				}
				else if (col == 0u || col == player->matrixWidth - 1u) {
					MATRIX(player, row, col).block = (Block)BLOCKTYPE_WALL;
				}
				else {
					MATRIX(player, row, col).block = NULLBLOCK;
				}
				MATRIX(player, row, col).itemType = ITEMTYPE_NULL;
			}
		}
	}
	SetFieldPos(player, true);
	SetFieldBorderColor(player, ITEMTYPE_NULL);

	// Mode codes.
	InitModeCodes(player);

	// GRS.
	InitGrade(player);
	InitMedals(player, 67, player->screenPos[0] + 49);

	ItemDescriptions[player->num] = ITEMTYPE_NULL;

	if (player->modeFlags & MODE_BIG) {
		player->nextBlock |= BLOCK_BIG;
	}
}

void InitState(Player* player) {
	uint8_t* subState = player->subStates;
	for (size_t i = 0; i < lengthoffield(Player, subStates); i++) {
		*subState = 0u;
	}

	int16_t* value = player->values;
	for (size_t i = 0; i < lengthoffield(Player, values); i++) {
		*value = 0;
	}
}

void StartPlayer(Player* player) {
	InitState(player);

	player->nowFlags |= NOW_BIT15 | NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;

	InitGrade(player);
}

void UpdatePlayerPlaying(Player* player) {
	if (player->nowFlags & NOW_NOUPDATE) {
		return;
	}

	if (!(GameButtonsDown[player->num] & BUTTON_DOWN)) {
		ManualLockUnprotected[player->num] = true;
	}

	if (
		player->play.state != PLAYSTATE_GAMEOVER &&
		player->play.state != PLAYSTATE_RANKING &&
		!(player->nowFlags & NOW_STAFF) &&
		!(player->modeFlags & MODE_VERSUS) &&
		(GameButtonsNew[player->num] & BUTTON_START)) {
		player->refusingChallenges = !player->refusingChallenges;
	}

	bool timeStopped = false;
	switch (player->play.state) {
		case PLAYSTATE_START:
			timeStopped = true;
			UpdatePlayStart(player);
			break;

		case PLAYSTATE_GARBAGECHECK:
			UpdatePlayGarbageCheck(player);
			break;

		case PLAYSTATE_NEXT:
			if (GameFlags & GAME_DOUBLES) {
				BackupMatrix(player);
				if (player->otherPlayer->play.state != PLAYSTATE_NEXT && player->otherPlayer->play.state != PLAYSTATE_BLOCKEDENTRY) {
					LockActiveBlock(player->otherPlayer, LOCKTYPE_BLOCKING);
				}
			}

			UpdatePlayNext(player);

			if ((GameFlags & GAME_DOUBLES) && !(player->nowFlags & NOW_STOPPED) && !(player->otherPlayer->nowFlags & NOW_STOPPED)) {
				RestoreMatrix(player);
			}
			break;

		case PLAYSTATE_ACTIVE:
			player->numActiveFrames++;

			if (GameFlags & GAME_DOUBLES) {
				BackupMatrix(player);
				if (player->otherPlayer->play.state != PLAYSTATE_NEXT && player->otherPlayer->play.state != PLAYSTATE_BLOCKEDENTRY) {
					LockActiveBlock(player->otherPlayer, LOCKTYPE_BLOCKING);
				}
			}

			UpdatePlayActive(player);

			if ((GameFlags & GAME_DOUBLES) && !(player->nowFlags & NOW_STOPPED) && !(player->otherPlayer->nowFlags & NOW_STOPPED)) {
				RestoreMatrix(player);
			}
			break;

		case PLAYSTATE_LOCK:
			if (GameFlags & GAME_VERSUS) {
				BackupMatrix(player);
			}

			player->nowFlags &= ~NOW_SHOWACTIVEBLOCK;
			LockActiveBlock(player, LOCKTYPE_NORMAL);

			UpdatePlayLock(player);
			break;

		case PLAYSTATE_CLEAR:
			UpdatePlayClear(player);
			break;

		case PLAYSTATE_ENTRY:
			UpdatePlayEntry(player);
			break;

		case PLAYSTATE_GARBAGEENTRY:
			UpdatePlayGarbageEntry(player);
			break;

		case PLAYSTATE_GAMEOVER:
			timeStopped = true;
			UpdatePlayGameOver(player);
			break;

		case PLAYSTATE_BLOCKEDENTRY:
			if (GameFlags & GAME_DOUBLES) {
				BackupMatrix(player);
				if (player->otherPlayer->play.state != PLAYSTATE_NEXT && player->otherPlayer->play.state != PLAYSTATE_BLOCKEDENTRY) {
					LockActiveBlock(player->otherPlayer, LOCKTYPE_BLOCKING);
				}
			}

			UpdatePlayBlockedEntry(player);

			if ((GameFlags & GAME_DOUBLES) && !(player->nowFlags & NOW_STOPPED) && !(player->otherPlayer->nowFlags & NOW_STOPPED)) {
				RestoreMatrix(player);
			}
			break;

		case PLAYSTATE_VERSUSOVER:
			timeStopped = true;
			UpdatePlayVersusOver(player);
			break;

		case PLAYSTATE_RANKING:
			timeStopped = true;
			UpdatePlayRanking(player);
			break;

		case PLAYSTATE_STAFFTRANSITION:
			timeStopped = true;
			UpdatePlayStaffTransition(player);
			break;

		default:
			break;
	}

	if (player->nowFlags & NOW_STOPPED) {
		timeStopped = true;
	}

	if (GameFlags & GAME_DOUBLES) {
		if ((player->nowFlags & NOW_STAFF) && (player->otherPlayer->nowFlags & NOW_STAFF)) {
			timeStopped = true;
		}
	}
	else if (player->nowFlags & NOW_STAFF) {
		timeStopped = true;
	}
	else if (player->level >= NextSectionLevels[9]) {
		timeStopped = true;
	}

	if (!timeStopped) {
		if (GameFlags & GAME_VERSUS) {
			if (player->clearTime != 0u) {
				player->clearTime--;
			}
		}
		else {
			player->clearTime++;
		}

		if (player->otherPlayer->activeItemType != ITEMTYPE_FLIPFIELD) {
			UpdateSiren(player);
		}

		if (player->clearTime > CLEARTIME_MAX) {
			player->clearTime = CLEARTIME_MAX;
		}

		if (player->clearTime > CLEARTIME_TIMEOUT) {
			player->modeFlags |= MODE_TIMEOUT;
		}

		player->gameTime++;
	}
}

void UpdatePlayerAbsent(Player* player) {
	if (player->otherPlayer->nowFlags & NOW_SELECTING) {
		// TODO: Detail how this works. It controls displaying the "PLEASE WAIT" message on and off.
		if (ScreenTime & 0x60) {
			DisplayObject(OBJECT_PLEASEWAIT, 120, player->screenPos[0], 0u, 110u);
		}
	}
	else {
		ShowStartRequirement(player);

		if (player->otherPlayer->refusingChallenges) {
			ShowText(player->screenPos[0] - TextWidth("NO MORE") / 2, 65, "NO MORE", 15u, false);
			ShowText(player->screenPos[0] - TextWidth("CHALLENGER") / 2, 77, "CHALLENGER", 15u, false);
		}

		if (CanStart(player->num, false)) {
			UpdateModeCodes(player);
		}
		else {
			// NOTE: If you want to enable support for the invisible and item
			// mode codes, you'll need to add clearing of those mode flags
			// here.
			player->modeFlags &= ~(MODE_20G | MODE_BIG | MODE_TLS);
		}

		if ((GameButtonsNew[player->num] & BUTTON_START) && CanStart(player->num, false)) {
			CheckBuyGame(player->num, false);
			player->nowFlags = NOW_SELECTING | NOW_STARTED;
			player->modeFlags &= ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS);
			player->modeFlags |= MODE_NORMAL;
			player->gameTime = 0u;
			InitPlayer(player->num);

			if (player->otherPlayer->nowFlags & NOW_GAMEOVER) {
				Game.versusWinner = player->num;
			}
			if (player->num == Game.versusWinner) {
				Game.numVersusWins = 0u;
			}
			Game.numVersusRoundWins[player->num] = 0u;
		}

		ShowModeCodes(player);
	}
}

void UpdatePlayerWaiting(Player* player) {
	if (++player->values[0] > TIME(1, 0, 0) || (GameFlags & GAME_STARTWAITINGPLAYER) || (player->otherPlayer->nowFlags & NOW_STOPPED)) {
		player->nowFlags = NOW_PLAYING | NOW_STARTED;
		InitPlayer(player->num);
		NextPlayStart(player);
	}

	if (Game.numVersusRounds == 0 && (!(player->nowFlags & NOW_WAITING) || !(player->otherPlayer->nowFlags & NOW_WAITING)) && (player->values[0] & 0x60)) {
		DisplayObject(OBJECT_PLEASEWAIT, 120, player->screenPos[0], 0u, 110u);
	}
}

int8_t SelectScales[NUMPLAYERS][4];
typedef enum ChallengeSelection {
	CHALLENGE_YES,
	CHALLENGE_NO,
	CHALLENGE_INVALID
} ChallengeSelection;

void StartChallenger(Player* player) {
	if (player->values[0] == CHALLENGE_YES) {
		GameFlags |= (player->num == PLAYER1) ? GAME_CHALLENGER1P : GAME_CHALLENGER2P;
		player->nowFlags = NOW_WAITING | NOW_STARTED;
		StartPlayer(player);
		PlaySoundEffect(SOUNDEFFECT_START);
		Game.versusWinner = player->otherPlayer->num;
	}
	else if (player->values[0] == CHALLENGE_NO) {
		player->subStates[0] = 10;
		player->values[0] = MODESELECTION_NORMAL;
		player->values[1] = TIME(0, 10, 59);
		for (int16_t i = 0; i < lengthof(*SelectScales); i++) {
			SelectScales[player->num][i] = i * -30;
		}
		Save.twinCount++;
		PlaySoundEffect(SOUNDEFFECT_START);
	}
	else if (player->values[0] == CHALLENGE_INVALID) {
		// NOTE: TAP's code is empty here.
	}
}

void SetMode(Player* player, ModeSelection modeSelection) {
	player->modeFlags &= ~(MODE_NORMAL | MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH | MODE_DOUBLES);
	switch (modeSelection) {
	case MODESELECTION_NORMAL : player->modeFlags |= MODE_NORMAL ; break;
	case MODESELECTION_MASTER : player->modeFlags |= MODE_MASTER ; break;
	case MODESELECTION_TGMPLUS: player->modeFlags |= MODE_TGMPLUS; break;
	case MODESELECTION_TADEATH: player->modeFlags |= MODE_TADEATH; break;
	case MODESELECTION_DOUBLES: player->modeFlags |= MODE_DOUBLES; break;
	default: break;
	}
}

void StartDoubles(Player* player) {
	Game.state = 0u;
	GameFlags |= GAME_STARTDOUBLES;

	Player* player1 = &Players[PLAYER1];
	Player* player2 = &Players[PLAYER2];
	player1->nextBlock &= BLOCK_TYPE;
	player2->nextBlock &= BLOCK_TYPE;
	player1->nowFlags = NOW_PLAYING | NOW_STARTED;
	player2->nowFlags = NOW_PLAYING | NOW_STARTED;

	player->nowFlags |= NOW_NOUPDATE;
	player->otherPlayer->nowFlags |= NOW_NOUPDATE;

	StartPlayer(player);
	StartPlayer(player->otherPlayer);
	Save.versusCount++;
	PlaySoundEffect(SOUNDEFFECT_START);
}

const ObjectData* ObjectTablesConfirm[2] = {
	OBJECTTABLE_YES, OBJECTTABLE_NO
};

void UpdatePlayerSelecting(Player* player) {
	uint16_t fieldBorderPalNum = (player->num == PLAYER1) ? 16u : 32u;
	SelectState state = player->subStates[SUBSTATE_SELECT];

	if (state != SELECT_CHALLENGE) {
		ShowModeCodes(player);
	}

	if (state == SELECT_START) {
		player->subStates[SUBSTATE_SELECT] = SELECT_MODE;
		player->values[0] = 0;
		player->values[1] = TIME(0, 11, 0) - 1;
		for (int16_t i = 0; i < lengthof(*SelectScales); i++) {
			SelectScales[player->num][i] = -30 * i;
		}
		player->values[0] = MODESELECTION_NORMAL;
		SetMode(player, player->values[0]);
		SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_NORMALFIELDBORDER);
		SetPal(15u, 1u, PAL_MODESELECTED);
		SetPal(14u, 1u, PAL_MODENOTSELECTED);
		if (!player->otherPlayer->refusingChallenges && (player->otherPlayer->nowFlags & NOW_STARTED)) {
			if (!(player->otherPlayer->nowFlags & NOW_SELECTING)) {
				if (player->otherPlayer->play.state != PLAYSTATE_GAMEOVER && player->otherPlayer->play.state != PLAYSTATE_RANKING && !(player->otherPlayer->nowFlags & NOW_STAFF)) {
					player->subStates[SUBSTATE_SELECT] = SELECT_CHALLENGE;
					player->values[0] = CHALLENGE_NO;
				}
				return;
			}
			else if (player->otherPlayer->subStates[SUBSTATE_SELECT] != SELECT_START) {
				if (player->otherPlayer->play.state != PLAYSTATE_GAMEOVER && player->otherPlayer->play.state != PLAYSTATE_RANKING && !(player->otherPlayer->nowFlags & NOW_STAFF)) {
					player->subStates[SUBSTATE_SELECT] = SELECT_CHALLENGE;
					player->values[0] = CHALLENGE_NO;
				}
				return;
			}
		}
	}
	else if (state == SELECT_MODE) {
		for (int16_t i = 0; i < lengthof(*SelectScales); i++) {
			if (SelectScales[player->num][i] < UNSCALED) {
				SelectScales[player->num][i] += 8;
			}
			else {
				SelectScales[player->num][i] = UNSCALED;
			}
		}
		DisplayObjectEx(OBJECT_SELECTMODE, 70, player->screenPos[0], 0u, 125u, UNSCALED, SelectScales[player->num][0], false);

		#define SHOWSELECTMODEOPTION(mode, y, modeSelection) ShowText(player->screenPos[0] - TextWidth((mode)) / 2, (y), (mode), player->values[0] == (modeSelection) ? 15u : 14u, false);
		SHOWSELECTMODEOPTION("NORMAL", 90, MODESELECTION_NORMAL);
		SHOWSELECTMODEOPTION("MASTER", 105, MODESELECTION_MASTER);
		SHOWSELECTMODEOPTION("TGM+", 122, MODESELECTION_TGMPLUS);
		SHOWSELECTMODEOPTION("T.A. DEATH", 137, MODESELECTION_TADEATH);
		if (player->otherPlayer->nowFlags & NOW_GAMEOVER) {
			SHOWSELECTMODEOPTION("DOUBLES", 154, MODESELECTION_DOUBLES);
		}
		ModeSelection modeSelectionOld = player->values[0];
		ButtonInput selectButton = Select(player);
		if (selectButton & BUTTON_UP) {
			if (player->values[0] > MODESELECTION_NORMAL) {
				player->values[0]--;
			}
		}
		else if (selectButton & BUTTON_DOWN) {
			if (player->otherPlayer->nowFlags & NOW_GAMEOVER) {
				if (player->values[0] < MODESELECTION_DOUBLES) {
					player->values[0]++;
				}
			}
			else if (player->values[0] < MODESELECTION_TADEATH) {
				player->values[0]++;
			}
		}
		if (player->otherPlayer->nowFlags & NOW_GAMEOVER) {
			player->values[0] %= NUMMODESELECTIONS;
		}
		else {
			player->values[0] %= NUMSINGLEMODESELECTIONS;
		}

		if (player->values[0] != modeSelectionOld) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
		}
		SetMode(player, player->values[0]);

		switch (player->values[0]) {
		case MODESELECTION_NORMAL: SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_NORMALFIELDBORDER); break;
		case MODESELECTION_MASTER: SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_WHITEFIELDBORDER); break;
		case MODESELECTION_TGMPLUS: SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_BLUEFIELDBORDER); break;
		case MODESELECTION_TADEATH: SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_REDFIELDBORDER); break;
		case MODESELECTION_DOUBLES: SetPal((uint8_t)fieldBorderPalNum, 16u, PAL_WHITEFIELDBORDER); break;
		default: break;
		}

		if (!(GameButtonsNew[player->num] & (BUTTON_3 | BUTTON_2 | BUTTON_1))) {
			if (--player->values[1] == 0) {
				if (player->modeFlags & MODE_DOUBLES) {
					StartDoubles(player);
				}
				else {
					player->subStates[SUBSTATE_SELECT] = SELECT_SINGLE;
					PlaySoundEffect(SOUNDEFFECT_START);
				}
			}
		}
		else {
			if (player->modeFlags & MODE_DOUBLES) {
				StartDoubles(player);
			}
			else {
				player->subStates[SUBSTATE_SELECT] = SELECT_SINGLE;
				PlaySoundEffect(SOUNDEFFECT_START);
				if (!(player->modeFlags & (MODE_NORMAL | MODE_DOUBLES | MODE_TADEATH))) {
					CheckSetItemMode(player);
				}
			}
		}
	}
	else if (state != SELECT_20 && state != SELECT_21) {
		if (state == SELECT_CHALLENGE) {
			for (int16_t i = 0; i < lengthof(*SelectScales); i++) {
				if (SelectScales[player->num][i] < UNSCALED) {
					SelectScales[player->num][i] += 8;
				}
				else {
					SelectScales[player->num][i] = UNSCALED;
				}
			}
			DisplayObjectEx(OBJECT_CHALLENGE, 70, player->screenPos[0], 0u, 125u, UNSCALED, SelectScales[player->num][0], false);
			int16_t y = 100;
			for (ChallengeSelection challenge = CHALLENGE_YES; challenge < CHALLENGE_INVALID; challenge++, y += 20) {
				if (SelectScales[player->num][challenge + 1] > 0) {
					DisplayObjectEx(&ObjectTablesConfirm[challenge][player->values[0] == challenge ? CHALLENGE_YES : CHALLENGE_NO], y, player->screenPos[0], 0u, 125u, UNSCALED, SelectScales[player->num][challenge + 1], false);
				}
			}

			ChallengeSelection challengeOld = player->values[0];
			ButtonInput selectButton = Select(player);
			if (selectButton & BUTTON_UP) {
				player->values[0] -= player->values[0] > CHALLENGE_YES;
			}
			else if (selectButton & BUTTON_DOWN) {
				player->values[0] += player->values[0] < CHALLENGE_NO;
			}

			if (player->values[0] == CHALLENGE_YES) {
				ShowChallengerMode(player);
			}
			if (player->values[0] != challengeOld) {
				PlaySoundEffect(SOUNDEFFECT_SELECT);
			}
			if (!((player->otherPlayer->nowFlags & NOW_GAMEOVER) || player->otherPlayer->play.state == PLAYSTATE_GAMEOVER || player->otherPlayer->play.state == PLAYSTATE_RANKING) && !(player->otherPlayer->nowFlags & NOW_STAFF)) {
				if (GameButtonsNew[player->num] & (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
					StartChallenger(player);
				}
				else if (--player->values[1] == 0) {
					StartChallenger(player);
				}
			}
			else {
				GameFlags &= ~GAME_BIT12;
				player->subStates[SUBSTATE_SELECT] = SELECT_MODE;
				player->values[0] = MODESELECTION_NORMAL;
				player->values[1] = TIME(0, 11, 0) - 1u;
			}
		}
		else if (player->subStates[SUBSTATE_SELECT] == SELECT_SINGLE) {
			if (player->modeFlags & MODE_TGMPLUS) {
				player->modeFlags &= MODE_20G | MODE_TGMPLUS | MODE_ITEM | MODE_TLS;
				player->nextBlock &= BLOCK_TYPE;
			}
			if (player->modeFlags & MODE_TADEATH) {
				player->modeFlags &= MODE_TADEATH;
				player->nextBlock &= BLOCK_TYPE;
			}
			player->nowFlags = NOW_PLAYING | NOW_STARTED;
			StartPlayer(player);
			NextPlayStart(player);
			
			CurrentGameBg.UNK_10 |= player->num == PLAYER1 ? 2u : 4u;
		}
	}
}

ButtonInput Select(Player* player) {
	ButtonInput selectButton = BUTTON_NONE;

	if (GameButtonsDown[player->num] & BUTTON_UP) {
		if (GameButtonsNew[player->num] & BUTTON_UP) {
			player->autoshiftFrames = 0u;
			selectButton = BUTTON_UP;
		}
		else if (player->autoshiftFrames < 14u) {
			player->autoshiftFrames++;
		}
		else {
			player->autoshiftFrames = 0u;
			selectButton = BUTTON_UP;
		}
	}

	if (GameButtonsDown[player->num] & BUTTON_DOWN) {
		if (GameButtonsNew[player->num] & BUTTON_DOWN) {
			player->autoshiftFrames = 0u;
			selectButton = BUTTON_DOWN;
		}
		else if (player->autoshiftFrames < 14u) {
			player->autoshiftFrames++;
		}
		else {
			player->autoshiftFrames = 0u;
			selectButton = BUTTON_DOWN;
		}
	}

	return selectButton;
}

void UpdatePlayers() {
	for (PlayerNum playerNum = PLAYER1; playerNum <= PLAYER2; playerNum++) {
		Player* player = &Players[playerNum];

		if (player->nowFlags & NOW_INIT) {
			InitPlayer(playerNum);
			player->nowFlags &= ~NOW_INIT;
			player->nowFlags |= NOW_SHOWNEXTBLOCK;
			if (player->nowFlags & NOW_STARTED) {
				NextPlayStart(player);
			}
		}

		if (
			!(player->nowFlags & (NOW_SELECTING | NOW_WAITING | NOW_STAFF | NOW_STOPPED)) &&
			(GameFlags & (GAME_NEWCHALLENGER | GAME_NEWVERSUSROUND))) {
			player->nowFlags |= NOW_WAITING;
			player->nowFlags &= ~NOW_PLAYING;
			player->values[0] = 0;
			if (Game.numVersusRounds == 0u) {
				GameFlags |= GAME_CHALLENGEDELAY;
				ShowNewChallenger(player);
			}
		}

		if (CurrentPauseMode < PAUSEMODE_GAME) {
			player->subStates[1] = player->nowFlags;
			if (player->nowFlags & NOW_PLAYING) {
				UpdatePlayerPlaying(player);
			}
			else if (player->nowFlags & NOW_SELECTING) {
				UpdatePlayerSelecting(player);
			}
			else if (player->nowFlags & NOW_ABSENT) {
				UpdatePlayerAbsent(player);
			}
			else if (player->nowFlags & NOW_WAITING) {
				UpdatePlayerWaiting(player);
			}
		}
	}

	if (GameFlags & GAME_DOUBLES) {
		if (Players[PLAYER1].clearTime < Players[PLAYER2].clearTime) {
			Players[PLAYER1].clearTime = Players[PLAYER2].clearTime;
		}
		else {
			Players[PLAYER2].clearTime = Players[PLAYER1].clearTime;
		}
	}

	UpdateItems();

	ShowCredits();
}

void ShowPlayers() {
	for (PlayerNum playerNum = PLAYER1; playerNum <= PLAYER2; playerNum++) {
		Player* player = &Players[playerNum];

		ShowBlock(player, SHOWBLOCKTYPE_ACTIVE, player->nowFlags & NOW_SHOWACTIVEBLOCK);
		ShowBlock(player, SHOWBLOCKTYPE_NEXT, player->nowFlags & NOW_SHOWNEXTBLOCK);
		if ((player->modeFlags & (MODE_TLS | MODE_VERSUS)) || player->level < 100u) {
			ShowBlock(player, SHOWBLOCKTYPE_TLS, player->nowFlags & NOW_SHOWTLSBLOCK);
		}

		if (GameFlags & GAME_DOUBLES) {
			if (playerNum == PLAYER2 && (player->nowFlags & NOW_SHOWFIELD)) {
				ShowFieldPlus(player);
			}
		}
		else if (player->nowFlags & NOW_SHOWFIELD) {
			ShowFieldPlus(player);
		}
	}
}

static const int16_t ModeCodeBig[] = {
	BUTTON_LEFT, BUTTON_LEFT, BUTTON_LEFT, BUTTON_LEFT,
	BUTTON_DOWN,
	BUTTON_3, BUTTON_2, BUTTON_1,
	-1
};
static const int16_t ModeCode20G[] = {
	BUTTON_DOWN, BUTTON_DOWN, BUTTON_DOWN, BUTTON_DOWN,
	BUTTON_DOWN, BUTTON_DOWN, BUTTON_DOWN, BUTTON_DOWN,
	BUTTON_3, BUTTON_2, BUTTON_1,
	-1
};
static const int16_t ModeCodeTls[] = {
	BUTTON_1, BUTTON_2, BUTTON_3,
	BUTTON_3, BUTTON_2, BUTTON_1,
	BUTTON_1, BUTTON_3, BUTTON_2,
	-1
};
static const int16_t ModeCodeItem[] = {
	BUTTON_RIGHT, BUTTON_RIGHT, BUTTON_RIGHT, BUTTON_RIGHT,
	BUTTON_UP,
	BUTTON_3, BUTTON_2, BUTTON_1,
	-1
};
static const int16_t ModeCodeInvisible[] = {
	BUTTON_UP, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_LEFT,
	BUTTON_UP, BUTTON_LEFT, BUTTON_DOWN, BUTTON_RIGHT,
	BUTTON_3, BUTTON_2, BUTTON_1,
	-1
};

static const int16_t* ModeCodes[NUMMODECODES] = {
	ModeCodeBig,
	ModeCode20G,
	ModeCodeTls,
	ModeCodeItem,
	ModeCodeInvisible
};

static const ModeFlag ModeCodeFlagsEnable[NUMMODECODES] = {
	MODE_BIG, MODE_20G, MODE_TLS, MODE_ITEM, MODE_INVISIBLE
};

static uint8_t NumModeCodeInputs[NUMPLAYERS][NUMENABLEDMODECODES];

void InitModeCodes(Player* player) {
	for (int16_t i = 0; i < NUMENABLEDMODECODES; i++) {
		NumModeCodeInputs[player->num][i] = 0u;
	}
}

bool UpdateModeCodes(Player* player) {
	bool enteredCode = false;

	// Check whether the button pressed this frame matches any of the enabled
	// mode codes' input sequences.
	for (int16_t checkIndex = 0; checkIndex < NUMENABLEDMODECODES; checkIndex++) {
		// Skip over mode codes that have been completed.
		//
		// This is never needed, because this function ensures the input
		// sequence index is never at the end of a mode code after returning.
		if (ModeCodes[checkIndex][NumModeCodeInputs[player->num][checkIndex]] == -1) {
			continue;
		}
		// Increment number of matched inputs, if the current button matches a
		// mode code; else, reset the number of matched inputs. A single button
		// has to be pressed per frame, else the mode code input sequence is
		// invalid.
		else if (GameButtonsNew[player->num]) {
			if (ModeCodes[checkIndex][NumModeCodeInputs[player->num][checkIndex]] == GameButtonsNew[player->num]) {
				NumModeCodeInputs[player->num][checkIndex]++;
			}
			else {
				NumModeCodeInputs[player->num][checkIndex] = 0u;
			}
		}

		// Add the mode flag for the current mode code, if the input sequence
		// was completed. And reset the number of matched inputs, so another
		// code can be entered. But a code can't be disabled.
		if (ModeCodes[checkIndex][NumModeCodeInputs[player->num][checkIndex]] == -1) {
			enteredCode = true;
			player->modeFlags |= ModeCodeFlagsEnable[checkIndex];
			for (int16_t resetIndex = 0; resetIndex < NUMENABLEDMODECODES; resetIndex++) {
				if (checkIndex == resetIndex) {
					NumModeCodeInputs[player->num][resetIndex] = 0u;
				}
			}
		}
	}

	return enteredCode;
}

static const SoundEffect SirenSoundEffects[] = {
	SOUNDEFFECT_SIREN1,
	SOUNDEFFECT_SIREN2,
	SOUNDEFFECT_SIREN3,
	SOUNDEFFECT_SIREN4,
	SOUNDEFFECT_SIREN5
};

void UpdateSiren(Player* player) {
	int16_t numBlocks = 0;

	// Count the number of squares in the top five rows of the field, below the
	// vanish row.
	for (int16_t row = player->matrixHeight - 6; row < player->matrixHeight - 1; row++) {
		for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
			if (MATRIX(player, row, col).block != NULLBLOCK) {
				numBlocks++;
			}
		}
	}

	// Play the siren while there's more than ten squares in those top five
	// rows.
	if (numBlocks > 10 && ScreenTime % 20u == 0u) {
		int16_t sirenIndex = numBlocks / 10;
		if (sirenIndex > lengthof(SirenSoundEffects) - 1) {
			sirenIndex = 4;
		}
		PlaySoundEffect(SirenSoundEffects[sirenIndex]);
	}
}

void CheckSetItemMode(Player* player) {
	if (GameButtonsDown[player->num] == (BUTTON_START | BUTTON_3 | BUTTON_2)) {
		player->modeFlags |= MODE_ITEM;
	}
}

#define NEXTBLOCKINT_SCALE 0x41C64E6D
#define NEXTBLOCKINT_STEP 12345
uint16_t GenNextInt(uint32_t* seed, bool update) {
	const uint32_t oldSeed = *seed;

	if (update) {
		*seed = NEXTBLOCKINT_SCALE * oldSeed + NEXTBLOCKINT_STEP;
	}

	return ((NEXTBLOCKINT_SCALE * oldSeed + NEXTBLOCKINT_STEP) >> 10) & 0x7FFF;
}

void NextPlay(Player* player, PlayData play) {
	player->play.flags |= play.flags;
	player->play.state = play.state;
}

void NextPlayStart(Player* player) {
	player->nowFlags |= NOW_SHOWNEXTBLOCK;
	player->play.state = PLAYSTATE_START;
	player->values[0] = ROTATENEXT_DISABLED;
}

void NextPlayGarbageCheck(Player* player) {
	player->play.state = PLAYSTATE_GARBAGECHECK;
	player->play.flags &= ~PLAYFLAG_FORCEENTRY;
}

void NextPlayActive(Player* player) {
	player->play.state = PLAYSTATE_ACTIVE;

	player->nowFlags |= NOW_SHOWACTIVEBLOCK;

	if (GameFlags & GAME_DOUBLES) {
		player->nowFlags &= ~NOW_SHOWTLSBLOCK;
	}
	else {
		player->nowFlags |= NOW_SHOWTLSBLOCK;
	}

	NumInstantDropRows[player->num] = 0u;
	NumFastDropRows[player->num] = 0u;
	player->numActiveFrames = 0u;

	if (!(player->modeFlags & MODE_TGMPLUS)) {
		if (player->modeFlags & MODE_TADEATH) {
			player->lockDelay = (int8_t)LockDelayTaDeath[player->level > 500u ? 500u : player->level];
		}
		else {
			if (player->level > 999u) {
				player->level = 999u;
			}

			if (player->level >= 500u) {
				player->lockDelay = (int8_t)LockDelay[player->level - 500u];
			}
		}
	}

	if (player->modeFlags & MODE_TIMEOUT) {
		player->lockDelay = (int8_t)LockDelay[499u];
	}

	player->lockFrames = player->lockDelay;
}

void NextPlayLock(Player* player) {
	player->play.state = PLAYSTATE_LOCK;

	player->lockFrames = 0u;
	ManualLockUnprotected[player->num] = false;
	player->nowFlags &= ~NOW_SHOWTLSBLOCK;
	player->nowFlags |= NOW_LOCKING;
}

void NextPlayClear(Player* player) {
	player->play.state = PLAYSTATE_CLEAR;

	if (player->modeFlags & MODE_TADEATH) {
		player->values[0] = ClearDelayTaDeath[player->level > 500u ? 500u : player->level];
	}
	else if (player->modeFlags & MODE_TGMPLUS) {
		player->values[0] = 40;
	}
	else {
		if (player->level >= 999u) {
			player->level = 999u;
		}

		if (player->level >= 500u) {
			player->values[0] = ClearDelay[player->level - 500u];
		}
		else {
			player->values[0] = 40;
		}
	}

	if (player->modeFlags & MODE_TIMEOUT) {
		player->values[0] = ClearDelay[499u];
	}

	if (GameFlags & GAME_DOUBLES) {
		player->values[0] = 1;
	}
}

void NextPlayEntry(Player* player, bool afterClear) {
	player->nowFlags &= ~NOW_LOCKING;
	player->play.state = PLAYSTATE_ENTRY;

	if (afterClear) {
		if (player->modeFlags & MODE_TADEATH) {
			player->values[0] = ClearDelayTaDeath[player->level > 500u ? 500u : player->level];
		}
		else if (player->modeFlags & MODE_TGMPLUS) {
			player->values[0] = 25u;
		}
		else {
			if (player->level > 999u) {
				player->level = 999u;
			}

			if (player->level < 500) {
				player->values[0] = 25;
			}
			else {
				player->values[0] = ClearDelay[player->level - 500u];
			}
		}
	}
	else if (player->modeFlags & MODE_TADEATH) {
		player->values[0] = EntryDelayTaDeath[player->level > 500u ? 500u : player->level];
	}
	else if (player->modeFlags & MODE_TGMPLUS) {
		player->values[0] = 25u;
	}
	else if (player->level < 500) {
		player->values[0] = 25u;
	}
	else {
		player->values[0] = EntryDelay[player->level - 500u];
	}

	if (player->modeFlags & MODE_TIMEOUT) {
		player->values[0] = ClearDelay[499u];
	}
}

void NextPlayGarbageEntry(Player* player) {
	player->play.state = PLAYSTATE_GARBAGEENTRY;
	player->subStates[SUBSTATE_GARBAGE] = GARBAGE_ENTRY;
}

static int16_t SecretGradeScales[NUMPLAYERS];

#define showGameOver values[2]
#define numSecretGradeRows values[3]

void NextPlayGameOver(Player* player) {
	NewRankingData* newRanking = &NewRankings[player->num];

	player->values[0] = TIME(0, 6, 0);
	player->values[1] = 1;
	player->numSecretGradeRows = NumSecretGradeRows(player);
	SecretGradeScales[player->num] = SPRITESCALE(33);
	player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
	player->nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;
	if (
			(player->nowFlags & NOW_STAFF) &&
			(player->modeFlags & (MODE_NORMAL | MODE_DOUBLES | MODE_TGMPLUS | MODE_TADEATH)) &&
			(player->nowFlags & NOW_STOPPED)) {
		player->values[0] = TIME(0, 3, 0);
	}
	else {
		player->showGameOver = false;
		ShowGameOverFade(player);
	}
	player->nowFlags &= ~NOW_STAFF;
	player->nowFlags |= NOW_STOPPED;
	player->play.state = PLAYSTATE_GAMEOVER;
	if (GameFlags & GAME_DOUBLES) {
		player->otherPlayer->nowFlags |= NOW_STOPPED | NOW_NOUPDATE;
		player->otherPlayer->nowFlags &= ~NOW_SHOWACTIVEBLOCK;
	}

	if (GameFlags & GAME_VERSUS) {
		SetFieldPos(player, false);
		player->level = 0u;
		player->grade = PLAYERGRADE_9;
		player->clearTime = 0u;
		MedalColor* medal = player->medalColors;
		for (size_t i = 0; i < NUMMEDALTYPES; i++) {
			*medal = MEDALCOLOR_NULL;
		}
		InitGrade(player);
		InitMedals(player, 67, player->screenPos[0] + 49);
	}

	SetFieldBorderColor(player, ITEMTYPE_NULL);

	newRanking->flags = 0u;
	if (!(player->modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE)) && !(player->modeFlags & MODE_VERSUS)) {
		NextPlayRanking(player);
	}
	if (player->modeFlags == MODE_TADEATH || player->modeFlags == MODE_TGMPLUS) {
		NextPlayRanking(player);
		player->masteringTime = player->clearTime;
	}

	SetFieldVisible(player);

	player->refusingChallenges = false;
}

void NextPlayVersusOver(Player* player) {
	player->play.state = PLAYSTATE_VERSUSOVER;
	player->nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;

	GameFlag winnerFlag;
	if (player->num == PLAYER1) {
		winnerFlag = GAME_WINNER1P;
	}
	else {
		winnerFlag = GAME_WINNER2P;
	}

	if (GameFlags & GAME_BIT10) {
		player->values[1] = 0;
	}
	else if (GameFlags & winnerFlag) {
		player->values[1] = SPRITESCALE(65); // TODO: Replace with a named constant?
	}
	else {
		player->values[1] = SPRITESCALE(0);
	}
}

void NextPlayStaffTransition(Player* player) {
	player->play.state = PLAYSTATE_STAFFTRANSITION;
	player->values[0] = 0;
	player->values[1] = 1;
	ItemDescriptions[player->num] = ITEMTYPE_NULL;
}

void UpdateAutoshift(Player* player) {
	if (GameButtonsDown[player->num] & (BUTTON_LEFT | BUTTON_RIGHT)) {
		player->autoshiftFrames++;
	}
	else {
		player->autoshiftFrames = 0u;
	}
}

// - Commentary on how this code checks if the active block is blocked -
//
// Row 0, column 0, and column player->matrixWidth - 1 are filled with wall
// squares. ARS has no vertical kicks, and the extent of Sega rotations,
// shifts, and ARS kicks never exceed 1 column/row, so 1-square walls with no
// ceiling are sufficient for blocking checks; this means 3x3 blocks will never
// have block squares beyond the boundaries of the matrix.
//
// Though a rotated I-block may have a block square outside the bounds of the
// matrix, it's never checked, so no out-of-matrix access occurs. Given the
// check algorithm used, this assumption only works as long as all blocks are
// polyominoes of some type; if a block were composed of multiple disconnected
// pieces, that block could be judged as successfully rotated, in the case
// where one of the squares is outside the matrix, and that rotated block isn't
// blocked by any matrix squares.

static inline void CountBlockings(const Player* const player, const int16_t col, const int16_t row, const Rotation rotation, const int16_t size, EntryData* const entry) {
	BlockDefSquare* blockDef = BLOCKDEF(player->activeBlock & BLOCK_TYPE);
	for (int16_t blockRow = 0; blockRow < size; blockRow++) {
		int16_t matrixRow = row - blockRow;
		if (matrixRow < player->matrixHeight) {
			MatrixBlock* matrixBlock = &MATRIX(player, matrixRow, col);
			BlockDefSquare* blockDefRow = BLOCKDEFROW(blockDef, rotation, blockRow / (size / 4));
			for (int16_t blockCol = 0; blockCol < size; blockCol++, matrixBlock++) { 
				if (col + blockCol >= 0 && blockDefRow[blockCol / (size / 4)] != DEFBLOCK_EMPTY && (matrixBlock->block & ~BLOCK_INVISIBLE)) {
					if (matrixBlock->block & BLOCK_BLOCKING) {
						entry->numPlayerBlockings++;
					}
					else {
						entry->numMatrixBlockings++;
					}
				}
			}
		}
	}
}

// Use to check if the active block is blocked when not rotating. For checking
// if a rotation is possible, use RotationBlockedCheckKick.
static bool Blocked(Player* player, int16_t col, int16_t row, Rotation rotation) {
	TEMPPTR(EntryData, entry);

	entry->numPlayerBlockings = 0u;
	entry->numMatrixBlockings = 0u;

	// TODO: Document how a big/normal active block's position works.
	if (player->activeBlock & BLOCK_BIG) {
		CountBlockings(player, col - 2, row + 1, rotation, 8, entry);
	}
	else {
		CountBlockings(player, col, row, rotation, 4, entry);
	}

	return (bool)(entry->numPlayerBlockings + entry->numMatrixBlockings);
}

// In the rotation requested, this returns false if rotation is successful. It
// may apply an ARS kick, by modifying the player's active block position, if
// the kicked position isn't blocked. It returns true if the requested rotation
// attempt failed.
//
// The player's active block's rotation isn't modified by this function; the
// player's active block rotation has to be updated after calling this, if this
// returns false.
//
// The kicked position is based on the active block position of the player,
// rather than using the col/row arguments. So be sure col/row are
// F32I(player->activePos[]), or the kicks won't be what's expected. But if
// you're forking the code, a sensible change would be to remove col/row, and
// use F32I(player->activePos[]) for matrix square checking instead. And maybe
// make this function update the player's active block rotation too, rather
// than requiring it be updated after calling this.
bool RotationBlockedCheckKick(Player* player, int16_t col, int16_t row, Rotation rotation) {
	if (player->activeBlock & BLOCK_BIG) {
		col -= 2;
		for (int16_t defRow = 0; defRow < 8; defRow++) {
			if ((row + 1) - defRow < player->matrixHeight) {
				for (int16_t defCol = 0; defCol < 8; defCol++) {
					if (
						col + defCol >= 0 &&
						col + defCol <= (int8_t)player->matrixWidth - 1 &&
						DEFBLOCKBIG(player->activeBlock & BLOCK_TYPE, rotation, defRow, defCol) != DEFBLOCK_EMPTY &&
						(MATRIX(player, (row + 1) - defRow, col + defCol).block & ~BLOCK_INVISIBLE) != NULLBLOCK) {
						// The active block is blocked in the current
						// position/rotation.

						// I blocks never kick.
						if ((player->activeBlock & BLOCK_TYPE) == BLOCKTYPE_I) {
							return true;
						}

						// Reject rotations blocked in the middle column of 3x3 blocks.
						if (defCol == 2 || defCol == 3) {
							return true;
						} 

						// Kick right by default.
						for (int16_t kick = 1; kick < 2; kick++) {
							if (!Blocked(player, col + kick * 2 + 2, row, rotation)) {
								player->activePos[0].integer += kick * 2;
								return false;
							}
						}
						// Failing that, kick left.
						for (int16_t kick = 1; kick < 2; kick++) {
							if (!Blocked(player, col - kick * 2 + 2, row, rotation)) {
								player->activePos[0].integer -= kick * 2;
								return false;
							}
						}
						return true;
					}
				}
			}
		}
	}
	else {
		for (int16_t defRow = 0; defRow < 4; defRow++) {
			if (row - defRow < player->matrixHeight) {
				for (int16_t defCol = 0; defCol < 4; defCol++) {
					if (
						col + defCol >= 0 &&
						col + defCol <= player->matrixWidth - 1 &&
						DEFBLOCK(player->activeBlock & BLOCK_TYPE, rotation, defRow, defCol) != DEFBLOCK_EMPTY &&
						(MATRIX(player, row - defRow, col + defCol).block & ~BLOCK_INVISIBLE) != NULLBLOCK) {
						// The active block is blocked in the current
						// position/rotation.

						// I blocks never kick.
						if ((player->activeBlock & BLOCK_TYPE) == BLOCKTYPE_I) {
							return true;
						}

						// Reject rotations blocked in the middle column of 3x3 blocks.
						if (defCol == 1) {
							return true;
						}

						// Kick right by default.
						if (!Blocked(player, col + 1, row, rotation)) {
							player->activePos[0].integer++;
							return false;
						}
						// Failing that, kick left.
						else if (!Blocked(player, col - 1, row, rotation)) {
							player->activePos[0].integer--;
							return false;
						}
						else {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void CheckShiftActiveBlock(Player* player) {
	int16_t shiftCol = player->activePos[0].integer;
	int16_t row = player->activePos[1].integer;

	uint8_t shiftDelay;
	if (player->modeFlags & MODE_TGMPLUS) {
		shiftDelay = 14u;
	}
	else if (player->modeFlags & MODE_TADEATH) {
		if (player->level >= 400u) {
			shiftDelay = 6u;
		}
		else if (player->level >= 300) {
			shiftDelay = 8u;
		}
		else if (player->level >= 200) {
			shiftDelay = 9u;
		}
		else {
			shiftDelay = 10u;
		}
	}
	else if (player->level >= 900) {
		shiftDelay = 6u;
	}
	else if (player->level >= 500) {
		shiftDelay = 8u;
	}
	else {
		shiftDelay = 14u;
	}

	ButtonInput shiftDirection = BUTTON_NONE;
	if (GameButtonsDown[player->num] & BUTTON_LEFT) {
		if (!(GameButtonsNew[player->num] & BUTTON_LEFT)) {
			if (player->autoshiftFrames < shiftDelay) {
				player->autoshiftFrames++;
			}
			else {
				shiftDirection = BUTTON_LEFT;
			}
		}
		else {
			player->autoshiftFrames = 0u;
			shiftDirection = BUTTON_LEFT;
		}
	}
	if (GameButtonsDown[player->num] & BUTTON_RIGHT) {
		if (!(GameButtonsNew[player->num] & BUTTON_RIGHT)) {
			if (player->autoshiftFrames < shiftDelay) {
				player->autoshiftFrames++;
			}
			else {
				shiftDirection = BUTTON_RIGHT;
			}
		}
		else {
			player->autoshiftFrames = 0u;
			shiftDirection = BUTTON_RIGHT;
		}
	}

	if (shiftDirection & BUTTON_LEFT) {
		if (player->modeFlags & MODE_BIG) {
			if (!Blocked(player, shiftCol - 2, row, player->activeRotation)) {
				shiftCol -= 2;
			}
		}
		else if (!Blocked(player, shiftCol - 1, row, player->activeRotation)) {
			shiftCol--;
		}
	}
	if (shiftDirection & BUTTON_RIGHT) {
		if (player->modeFlags & MODE_BIG) {
			if (!Blocked(player, shiftCol + 2, row, player->activeRotation)) {
				shiftCol += 2;
			}
		}
		else if (!Blocked(player, shiftCol + 1, row, player->activeRotation)) {
			shiftCol++;
		}
	}

	player->activePos[0].integer = shiftCol;
}

void LandActiveBlock(Player* player, Fixed32 gravityStep) {
	Fixed32 landingRow = StepGravity(player, gravityStep);
	if (!Blocked(player, player->activePos[0].integer, landingRow.integer, player->activeRotation)) {
		if (landingRow.integer < player->activePos[1].integer ) {
			player->lockFrames = player->lockDelay;
		}
		player->activePos[1] = landingRow;
	}
 
	TEMPPTR(EntryData, entry);
	if (Blocked(player, player->activePos[0].integer, player->activePos[1].integer - 1, player->activeRotation)) {
		if (!(player->modeFlags & MODE_DOUBLES) || entry->numMatrixBlockings != 0) {
			if (player->lockFrames == player->lockDelay) {
				PlaySoundEffect(SOUNDEFFECT_LAND);
			}

			if (player->lockFrames != 0) {
				player->lockFrames--;
			}

			player->activePos[1].fraction = 0xFFFFu;
			if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_DOWN) {
				if (player->modeFlags & MODE_TADEATH) {
					if (player->level >= 0u) {
						if (ManualLockUnprotected[player->num]) {
							player->lockFrames = 0;
						}
					}
					else {
						player->lockFrames = 0;
					}
				}
				else if (player->level >= 900u) {
					if (ManualLockUnprotected[player->num]) {
						player->lockFrames = 0;
					}
				}
				else {
					player->lockFrames = 0;
				}
			}
		}
	}

	if (player->lockFrames <= 0) {
		NextPlayLock(player);
		if (GameFlags & GAME_DOUBLES) {
			player->otherPlayer->nowFlags |= NOW_NOUPDATE;
		}
	}
}

// If the gravity step has a fractional amount, the player may be blocked at
// the new position.
//
// If the gravity step is only whole rows, the player won't be blocked at the
// new position.
Fixed32 StepGravity(Player* player, Fixed32 gravity) {
	int16_t rows = gravity.integer;
	Fixed32 posY;

	if (rows < 1) {
		// Step for sub-1G gravity.
		posY.value = player->activePos[1].value - gravity.value;
	}
	else {
		posY.value = player->activePos[1].value - gravity.fraction;
		if (!Blocked(player, player->activePos[0].integer, posY.integer, player->activeRotation)) {
			// Stepping by the fractional amount succeeded.

			// Step by whole rows, until a step is blocked.
			for (; rows > 0; rows--) {
				if (Blocked(player, player->activePos[0].integer, posY.integer - 1, player->activeRotation)) {
					rows = 0;
				}
				else {
					posY.integer--;
				}
			}
		}
	}

	return posY;
}

void UpdatePlayActive(Player* player) {
	if (player->play.flags & PLAYFLAG_FORCEENTRY) {
		ThrowOutActiveBlock(player);
		NextPlayEntry(player, false);
		return;
	}

	CheckDecayGrade(player);

	if (Debug && (GameButtonsNew[player->num] & BUTTON_START)) {
		// Advance a section's worth of levels.
		if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_RIGHT) {
			player->level += 100;
		}

		// Test staff roll; GM can't be awarded.
		if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_LEFT) {
			player->section = 8u;
			player->level = 899u;
			player->mGradeSectionTime = TIME(1, 0, 0);
			player->miscFlags |= MISC_SKILLCLEARS3;
			Grades[player->num].currentGrade.integer = GradeLevels[PLAYERGRADE_S8];
			player->mGradeFlags |= MGRADE_CHECKPOINT1 | MGRADE_CHECKPOINT2 | MGRADE_SECTIONTIMES | MGRADE_SKILLCLEARS;
		}

		// Test staff roll; GM can be awarded.
		if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_UP) {
			player->section = 8u;
			player->level = 899u;
			player->mGradeSectionTime = TIME(1, 0, 0);
			player->miscFlags |= MISC_SKILLCLEARS3;
			Grades[player->num].currentGrade.integer = GradeLevels[PLAYERGRADE_S9];
			player->mGradeFlags |= MGRADE_CHECKPOINT1 | MGRADE_CHECKPOINT2 | MGRADE_SECTIONTIMES | MGRADE_SKILLCLEARS | MGRADE_S9GRADE;
		}

		if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_DOWN) {
			player->miscFlags |= MISC_FORCELOWG;
		}

		if (player->nowFlags & NOW_NOINVISIBLE) {
			player->nowFlags &= ~NOW_NOINVISIBLE;
		}
		else {
			player->nowFlags |= NOW_NOINVISIBLE;
			SetFieldVisible(player);
		}
	}

	if (
			(ROTATED_LEFT(GameButtonsNew[player->num]) || (player->itemMiscFlags & ITEMMISC_ROTATE)) &&
			!RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, ROTATE_LEFT(player->activeRotation))) {
		if (player->activeBlock & BLOCK_TRANSFORM) {
			Block oldActiveBlock = player->activeBlock;
			player->activeBlock &= ~BLOCK_TYPE;
			player->activeBlock |= TOBLOCKTYPE(Rand(7u));
			if (RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, player->activeRotation)) {
				player->activeBlock = oldActiveBlock;
			}

			Rotation newRotation = ROTATE_LEFT(player->activeRotation);
			if (!RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, newRotation)) {
				player->activeRotation = newRotation;
			}
		}
		else {
			player->activeRotation = ROTATE_LEFT(player->activeRotation);
		}
		player->numActiveRotations++;
	}
	if (
			ROTATED_RIGHT(GameButtonsNew[player->num]) &&
			!RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, ROTATE_RIGHT(player->activeRotation))) {
		if (player->activeBlock & BLOCK_TRANSFORM) {
			Block oldActiveBlock = player->activeBlock;
			player->activeBlock &= ~BLOCK_TYPE;
			player->activeBlock |= TOBLOCKTYPE(Rand(7u));
			if (RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, player->activeRotation)) {
				player->activeBlock = oldActiveBlock;
			}

			Rotation newRotation = ROTATE_RIGHT(player->activeRotation);
			if (!RotationBlockedCheckKick(player, player->activePos[0].integer, player->activePos[1].integer, newRotation)) {
				player->activeRotation = newRotation;
			}
		}
		else {
			player->activeRotation = ROTATE_RIGHT(player->activeRotation);
		}
		player->numActiveRotations++;
	}
	if (player->itemMiscFlags & ITEMMISC_ROTATE) {
		player->itemMiscFlags &= ~ITEMMISC_ROTATE;
	}

	CheckShiftActiveBlock(player);

	Fixed32 gravity = CurrentGravity(player);
	player->gravity = gravity;

	if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_DOWN) {
		if (gravity.integer < 1u) {
			gravity = F32(1, 0x0000);
		}
		NumFastDropRows[player->num]++;
	}
	else if ((GameButtonsDown[player->num] & BUTTON_ALLDIRECTIONS) == BUTTON_UP) {
		gravity = F32(20, 0x0000);
		int16_t droppedRows = player->activePos[1].integer - StepGravity(player, gravity).integer;
		if (droppedRows < 0) {
			droppedRows = 0;
		}
		if (droppedRows > NumInstantDropRows[player->num]) {
			NumInstantDropRows[player->num] = (uint8_t)droppedRows;
		}
	}

	if ((player->modeFlags & MODE_20G) || ((player->nowFlags & NOW_STAFF) && (player->modeFlags & MODE_NORMAL))) {
		gravity = F32(20, 0x0000);
	}

	LandActiveBlock(player, gravity);
}

static inline void WriteBlockToMatrix(Player* const player, const LockType lockType, const BlockType lockBlockType, const int16_t lockCol, const int16_t lockRow, const Rotation lockRotation, const int16_t lockBlockSize, const int16_t visibleFrames) {
	BlockDefSquare* blockDef = BLOCKDEF(lockBlockType);
	for (int16_t blockRow = 0; blockRow < lockBlockSize; blockRow++) {
		int16_t matrixRow = lockRow - blockRow;
		if (matrixRow < player->matrixHeight) {
			BlockDefSquare* blockDefRow = BLOCKDEFROW(blockDef, lockRotation, blockRow / (lockBlockSize / 4u));
			for (int16_t blockCol = 0; blockCol < lockBlockSize; blockCol++) {
				if (BLOCKDEFCOL(blockDefRow, blockCol / (lockBlockSize / 4u)) != DEFBLOCK_EMPTY) {
					if (lockType != LOCKTYPE_GAMEOVER) {
						MATRIX(player, matrixRow, lockCol + blockCol).block = player->activeBlock | BLOCK_FLASH | TOBLOCKFLASHFRAMES(2u);
					}
					else {
						MATRIX(player, matrixRow, lockCol + blockCol).block = player->activeBlock;
					}

					if (player->activeBlock & BLOCK_ITEM) {
						MATRIX(player, matrixRow, lockCol + blockCol).block |= BLOCK_ITEM;
						MATRIX(player, matrixRow, lockCol + blockCol).itemType = player->activeBlockItemType;
					}

					if ((player->modeFlags & MODE_INVISIBLE) && lockType != LOCKTYPE_GAMEOVER && visibleFrames != 0) {
						MATRIX(player, matrixRow, lockCol + blockCol).block |= BLOCK_FADING;
						MATRIX(player, matrixRow, lockCol + blockCol).visibleFrames = visibleFrames;
					}

					if (lockType == LOCKTYPE_BLOCKING) {
						MATRIX(player, matrixRow, lockCol + blockCol).block |= BLOCK_BLOCKING;
					}
				}
			}
		}
	}
}

void LockActiveBlock(Player* player, LockType lockType) {
	if (player->play.flags & PLAYFLAG_FORCEENTRY) {
		NextPlayEntry(player, false);
		return;
	}

	const int16_t activeCol = player->activePos[0].integer;
	const int16_t activeRow = player->activePos[1].integer;
	const Rotation activeRotation = player->activeRotation;
	const BlockType activeBlockType = player->activeBlock & BLOCK_TYPE;

	int16_t visibleFrames;
	if ((player->mGradeFlags & MGRADE_QUALIFIED) == MGRADE_QUALIFIED) {
		visibleFrames = 3;
	}
	else {
		visibleFrames = 300;
	}

	if (Debug && (player->nowFlags & NOW_NOINVISIBLE)) {
		visibleFrames = 0;
	}

	if (player->activeBlock & BLOCK_BIG) {
		WriteBlockToMatrix(player, lockType, activeBlockType, activeCol - 2, activeRow + 1, activeRotation, 8, visibleFrames);
	}
	else {
		WriteBlockToMatrix(player, lockType, activeBlockType, activeCol, activeRow, activeRotation, 4, visibleFrames);
	}

	if (lockType == LOCKTYPE_NORMAL) {
		PlaySoundEffect(SOUNDEFFECT_LOCK);
	}
}

static void BackupMatrix(Player* player) {
	TEMPPTR(EntryData, entry);
	for (int16_t row = 0; row < player->matrixHeight; row++) {
		for (int16_t col = 0; col < player->matrixWidth - 1; col++) {
			entry->tempMatrix[row * FIELD_SINGLEWIDTH * NUMPLAYERS + col] = MATRIX(player, row, col).block & ~BLOCK_INVISIBLE;
		}
	}
}

static void RestoreMatrix(Player* player) {
	TEMPPTR(EntryData, entry);
	for (int16_t row = 0; row < player->matrixHeight; row++) {
		for (int16_t col = 0; col < player->matrixWidth - 1; col++) {
			MATRIX(player, row, col).block = entry->tempMatrix[row * FIELD_SINGLEWIDTH * NUMPLAYERS + col];
		}
	}
}

static bool AllClear(Player* player) {
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
			if ((MATRIX(player, row, col).block & ~BLOCK_INVISIBLE) != NULLBLOCK) {
				return false;
			}
		}
	}
	return true;
}

static bool Line(Player* player, int16_t row) {
	for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
		if ((MATRIX(player, row, col).block & ~BLOCK_INVISIBLE) == NULLBLOCK) {
			return false;
		}
	}
	return true;
}

static LineFlag StartClear(Player* player, LineFlag lineFlags) {
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		if ((1 << row) & lineFlags) {
			ShowLineClear(player, row);
			MatrixBlock* matrixBlock = &MATRIX(player, row, 1);
			for (int16_t col = 1; col < player->matrixWidth - 1; col++, matrixBlock++) {
				if (matrixBlock->block & BLOCK_HARD) {
					lineFlags |= LINEFLAG_HARDCLEAR;
					lineFlags &= ~(1 << row);
					matrixBlock->block &= ~(BLOCK_TRANSFORM | BLOCK_BIG | BLOCK_HARD | BLOCK_ROLLROLL);
				}
				else {
					if (matrixBlock->block & BLOCK_ITEM) {
						player->clearItemType = matrixBlock->itemType;
						player->itemEffectPos[0] = (uint8_t)col;
						player->itemEffectPos[1] = (uint8_t)row;
					}
					matrixBlock->block = NULLBLOCK;
					matrixBlock->itemType = ITEMTYPE_NULL;
				}
			}
		}
	}
	return lineFlags;
}

static void SendGarbageRow(Player* player, int16_t fieldRow, int16_t garbageRow) {
	TEMPPTR(EntryData, entry);
	Player* const otherPlayer = player->otherPlayer;

	const Block* garbageBlock = &entry->tempMatrix[fieldRow * FIELD_SINGLEWIDTH * NUMPLAYERS + 1u];
	size_t i = 0u;
	for (size_t col = 1u; col < MATRIX_SINGLEWIDTH - 1; col++, i++, garbageBlock++) {
		otherPlayer->garbage[((size_t)otherPlayer->numGarbageRows + (size_t)garbageRow) * MATRIX_SINGLEWIDTH + i] = *garbageBlock;
	}
}

static uint8_t AllClearGarbage(Player* player, uint8_t numLines) {
	TEMPPTR(EntryData, entry);
	Player* const otherPlayer = player->otherPlayer;

	for (size_t row = 0u; row < numLines; row++) {
		size_t i = 0u;
		Block* garbageBlock = &entry->tempMatrix[row * FIELD_SINGLEWIDTH * NUMPLAYERS + 1u];
		for (size_t col = 1u; col < MATRIX_SINGLEWIDTH - 1; col++, i++, garbageBlock++) {
			*garbageBlock = otherPlayer->garbage[(otherPlayer->numGarbageRows + row) * MATRIX_SINGLEWIDTH + i];
		}
	}

	for (size_t row = 0u; row < numLines; row++) {
		size_t i = 0u;
		const Block* garbageBlock = &entry->tempMatrix[row * FIELD_SINGLEWIDTH * NUMPLAYERS + 1u];
		for (size_t col = 1u; col < MATRIX_SINGLEWIDTH - 1; col++, i++, garbageBlock++) {
			if (otherPlayer->numGarbageRows + row < GARBAGEHEIGHT) {
				otherPlayer->garbage[(otherPlayer->numGarbageRows + row) * MATRIX_SINGLEWIDTH + i] = *garbageBlock;
			}
		}
	}

	for (size_t row = 0u; row < numLines; row++) {
		size_t i = 0u;
		const Block* garbageBlock = &entry->tempMatrix[row * FIELD_SINGLEWIDTH * NUMPLAYERS + 1u];
		for (size_t col = 1u; col < MATRIX_SINGLEWIDTH - 1; col++, i++, garbageBlock++) {
			if (otherPlayer->numGarbageRows + row + numLines < GARBAGEHEIGHT) {
				otherPlayer->garbage[(player->otherPlayer->numGarbageRows + row + numLines) * MATRIX_SINGLEWIDTH + i] = *garbageBlock;
			}
		}
	}

	uint8_t numSentGarbageRows = numLines * 2u;
	if (otherPlayer->numGarbageRows + numLines * 2u > GARBAGEHEIGHT) {
		numSentGarbageRows = GARBAGEHEIGHT - otherPlayer->numGarbageRows;
	}
	return numSentGarbageRows;
}

uint8_t NumLines(Player* player, LineFlag lineFlags) {
	uint8_t numLines = 0u;
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		if ((1 << row) & lineFlags) {
			numLines++;
		}
	}
	return numLines;
}

void UpdateScore(Player* player, uint32_t pointsBaseValue, uint32_t allClearScale) {
	uint32_t points = pointsBaseValue * allClearScale;
	if (player->modeFlags & MODE_NORMAL) {
		points *= 9u;
	}
	player->score += points;
	if (player->score > 999999u) {
		player->score = 999999u;
	}
}

void UpdatePlayLock(Player* player) {
	uint8_t numSkillClearLines = (player->modeFlags & MODE_BIG) ? 8u : 4u;

	bool versusGarbage = GameFlags & GAME_VERSUS;
	if (player->modeFlags & MODE_CEMENT) {
		versusGarbage = false;
	}

	LineFlag lineFlags = LINEFLAG_NONE;
	uint8_t numLines = 0u;
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		if (Line(player, row)) {
			lineFlags |= 1 << row;
			if (versusGarbage && player->otherPlayer->numGarbageRows + numLines < GARBAGEHEIGHT) {
				SendGarbageRow(player, row, numLines);
			}
			numLines++;
		}
	}

	uint32_t pointsBaseValue = 0u; // BUG: Not initialized, though it never ends up being used uninitialized. Initialized to zero here, just in case.
	if (GameFlags & (GAME_TWIN | GAME_VERSUS | GAME_DOUBLES)) {
		if ((player->modeFlags & MODE_INVISIBLE) && (player->mGradeFlags & MGRADE_QUALIFIED) == MGRADE_QUALIFIED) {
			if (numLines > 3u) {
				player->numMGradeSkillClears++;
			}
			player->numMGradeLines += numLines;
		}

		if (!(player->nowFlags & NOW_STAFF)) {
			pointsBaseValue = PointsBaseValue(player, numLines);
		}
	}

	if (numLines != 0u) {
		lineFlags = StartClear(player, lineFlags);
	}

	if (player->clearItemType != ITEMTYPE_NULL) {
		pointsBaseValue = 0u;
	}

	uint8_t flaggedLines = NumLines(player, lineFlags);
	if (flaggedLines != 0u) {
		lineFlags &= ~LINEFLAG_HARDCLEAR;
	}

	if (!(player->nowFlags & NOW_STAFF)) {
		bool allClear = false;
		if (player->clearItemType == ITEMTYPE_NULL) {
			allClear = AllClear(player);
		}
		if (allClear) {
			if (player->modeFlags & MODE_DOUBLES) {
				ShowAllClear(player, 10, 6, false);
			}
			else {
				ShowAllClear(player, 10, 4, false);
			}
		}

		if (versusGarbage) {
			if (flaggedLines > 1u && allClear) {
				flaggedLines = AllClearGarbage(player, flaggedLines);
			}
		}
		else {
			UpdateScore(player, pointsBaseValue, allClear ? 4u : 1u);
		}
	}

	if (player->nowFlags & NOW_SKILLCOMBO) {
		if (flaggedLines == 0u) {
			player->nowFlags &= ~NOW_SKILLCOMBO;
		}
		else {
			PlaySoundEffect(flaggedLines >= numSkillClearLines ? SOUNDEFFECT_SURPRISE : SOUNDEFFECT_CHEER);
		}
	}
	else if (flaggedLines >= numSkillClearLines) {
		PlaySoundEffect(SOUNDEFFECT_APPLAUSE);
		player->nowFlags |= NOW_SKILLCOMBO;
	}

	if (versusGarbage) {
		if (player->otherPlayer->numGarbageRows + flaggedLines > GARBAGEHEIGHT) {
			flaggedLines = GARBAGEHEIGHT - player->numGarbageRows;
		}

		if (player->clearItemType == ITEMTYPE_NULL && flaggedLines > 1) {
			player->otherPlayer->numGarbageRows += flaggedLines;
		}
	}
	if (lineFlags != LINEFLAG_NONE) {
		player->lineFlags |= lineFlags;
		NextPlayClear(player);
		PlaySoundEffect(SOUNDEFFECT_CLEAR);
	}
	else {
		if (player->modeFlags & MODE_TGMPLUS) {
			player->numTgmPlusBlocks++;
		}
		NextPlayEntry(player, false);
		if (GameFlags & GAME_DOUBLES) {
			player->otherPlayer->nowFlags &= ~NOW_NOUPDATE;
		}
	}
	player->numFieldBlocks = NumFieldBlocks(player);
	if (player->clearItemType != ITEMTYPE_NULL) {
		ShowItemEffect(player, player->itemEffectPos[1], player->itemEffectPos[0]);
		if (
			player->clearItemType == ITEMTYPE_NEGAFIELD ||
			player->clearItemType == ITEMTYPE_TOPDELFIELD ||
			player->clearItemType == ITEMTYPE_BOTTOMDELFIELD ||
			player->clearItemType == ITEMTYPE_DELEVEN ||
			player->clearItemType == ITEMTYPE_SHOTGUN ||
			player->clearItemType == ITEMTYPE_LASER ||
			player->clearItemType == ITEMTYPE_FREEFALL) {
			player->miscFlags &= ~MISC_RECOVERING;
		}
	}

	if (!(player->nowFlags & NOW_STAFF)) {
		if (player->numFieldBlocks == 0u && (GameFlags & GAME_TWIN) && (player->modeFlags & (MODE_MASTER | MODE_TADEATH))) {
			UpdateAllClearMedal(player);
		}
		if ((GameFlags & GAME_TWIN) && (player->modeFlags & (MODE_MASTER | MODE_TADEATH))) {
			UpdateRecoverMedal(player);
		}
		if ((GameFlags & GAME_TWIN) && (player->modeFlags & (MODE_MASTER | MODE_TADEATH))) {
			UpdateRotateMedal(player);
		}
	}
}

void UpdatePlayClear(Player* player) {
	if (--player->values[0] == 0) {
		// Collapse lines.
		LineFlag lineFlags = player->lineFlags;
		if (lineFlags & LINEFLAG_HARDCLEAR) {
			lineFlags &= ~LINEFLAG_HARDCLEAR;
		}
		else {
			PlaySoundEffect(SOUNDEFFECT_COLLAPSE);
		}
		int16_t collapseRow = 1;
		while (lineFlags != LINEFLAG_NONE) {
			if ((lineFlags & (1 << collapseRow)) == LINEFLAG_NONE) {
				collapseRow++;
			}
			else {
				// Collapse a line.
				for (int16_t row = collapseRow; row < player->matrixHeight - 1; row++) {
					for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
						MATRIX(player, row, col) = MATRIX(player, row + 1, col);
						MATRIX(player, row, col).block &= ~BLOCK_FLASH;
					}
				}
				// Set vanish row empty.
				for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
					MATRIX(player, player->matrixHeight - 1, col).block = NULLBLOCK;
					MATRIX(player, player->matrixHeight - 1, col).itemType = ITEMTYPE_NULL;
				}
				// Other player's active block is forced down if the field is
				// blocking it after collapsing a row.
				if (
					(GameFlags & GAME_DOUBLES) &&
					player->otherPlayer->play.state == PLAYSTATE_ACTIVE &&
					Blocked(player->otherPlayer, player->otherPlayer->activePos[0].integer, player->otherPlayer->activePos[1].integer, player->otherPlayer->activeRotation)) {
					player->otherPlayer->activePos[1].value -= F32(1, 0x0000).value;
				}
				// Remove collapsed row from the line flags then shift the flags a row down.
				lineFlags = (lineFlags & ~(1 << collapseRow)) >> 1;
				player->numLines++;
			}
		}
		player->lineFlags = LINEFLAG_NONE;

		if (GameFlags & GAME_DOUBLES) {
			player->otherPlayer->nowFlags &= ~NOW_NOUPDATE;
		}

		NextPlayEntry(player, true);

		// Check if starting staff roll.
		if (!(player->nowFlags & NOW_STAFF)) {
			if ((player->modeFlags & MODE_NORMAL) && (GameFlags & GAME_TWIN)) {
				if (player->level >= NextSectionLevels[2]) {
					player->level = NextSectionLevels[2];
					player->nowFlags |= NOW_STAFF;
					ShowStaff(player);
					if (player->clearTime <= TIME(5, 0, 0)) {
						uint32_t numBonusSeconds = (TIME(5, 0, 0) - player->clearTime) / TIME(0, 1, 0);
						if ((TIME(5, 0, 0) - player->clearTime) % TIME(0, 1, 0) != 0u) {
							numBonusSeconds++;
						}
						player->score += numBonusSeconds * 1253u;
						if (player->score > 999999u) {
							player->score = 999999u;
						}
					}
				}
			}
			else if ((player->modeFlags & MODE_MASTER) && (GameFlags & GAME_TWIN)) {
				if (player->level >= NextSectionLevels[9]) {
					player->level = NextSectionLevels[9];
					PlaySoundEffect(SOUNDEFFECT_GRANDMASTER);
					NextPlayStaffTransition(player);
					player->numMGradeSkillClears = 0u;
					player->numMGradeLines = 0u;
				}
			}
			else if ((player->modeFlags & MODE_TGMPLUS) && (GameFlags & GAME_TWIN)) {
				if (player->level >= NextSectionLevels[9]) {
					player->level = NextSectionLevels[9];
					player->nowFlags |= NOW_STAFF;
					ShowStaff(player);
				}
			}
			else if ((player->modeFlags & MODE_TADEATH) && (GameFlags & GAME_TWIN)) {
				if (player->passedMGradeCheckpoint) {
					if (player->level >= NextSectionLevels[9]) {
						player->level = NextSectionLevels[9];
						player->grade = PLAYERGRADE_GM;
						player->nowFlags |= NOW_STAFF;
						ShowStaff(player);
					}
				}
				else if (player->level >= NextSectionLevels[4]) {
					player->level = NextSectionLevels[4];
					player->nowFlags |= NOW_STAFF;
					ShowStaff(player);
				}
			}
			else if (player->modeFlags & MODE_DOUBLES) {
				if (player->level >= NextSectionLevels[2]) {
					player->level = NextSectionLevels[2];
					player->nowFlags |= NOW_STAFF;
					if ((player->otherPlayer->nowFlags & NOW_STAFF) && !(player->otherPlayer->nowFlags & NOW_STOPPED)) {
						Players[PLAYER1].nowFlags |= NOW_STAFF;
						ShowStaff(&Players[PLAYER1]);
					}
				}
			}
		}

		if (player->clearItemType != ITEMTYPE_NULL) {
			Item* item = AllocItem();
			if (item != NULL) {
				item->status = ITEMSTATUS_ACTIVE;
				item->type = player->clearItemType;
				item->activatingPlayer = player;
			}
			player->clearItemType = ITEMTYPE_NULL;
		}
	}
}

uint8_t GenNextBlockNum(Player* player) {
	uint8_t blockNum;
	if (GameFlags & GAME_DOUBLES) {
		uint32_t* seed;
		if (player->num == PLAYER2) {
			seed = &player->otherPlayer->seed;
		}
		else {
			seed = &player->seed;
		}
		blockNum = GenNextInt(seed, true) % 7;
	}
	else {
		blockNum = GenNextInt(&player->seed, true) % 7;
	}

	if (!Demo) {
		blockNum %= 7;
	}
	return blockNum;
}

void NextBagItem(Player* player, uint8_t itemNum) {
	player->itemBag &= ~(1 << itemNum);
	player->nextBlockItemType = TOITEMTYPE(itemNum);
	player->nextBlock |= BLOCK_ITEM;
}

const uint8_t NormalItemNums[3] = {TOITEMNUM(ITEMTYPE_FREEFALL), TOITEMNUM(ITEMTYPE_DELEVEN), TOITEMNUM(ITEMTYPE_DELEVEN)};

// TODO: Verify the refactored version is correct, then delete this.
#if 1
void NextItem(Player* player) {
	if ((player->modeFlags & MODE_NORMAL) && !(player->modeFlags & MODE_VERSUS) && !(player->nowFlags & NOW_STAFF)) {
	       if (player->level >= ((player->normalItemIndex + 1) * 100)) {
		       const uint8_t itemNum = NormalItemNums[player->normalItemIndex];
		       const uint32_t itemBagFlag = 1 << itemNum;
		       if (player->normalItemBag & itemBagFlag) {
			       player->normalItemBag &= ~itemBagFlag;
			       NextBagItem(player, itemNum);
			       player->normalItemIndex++;
		       }
	       }
	       else if ((player->modeFlags & (MODE_VERSUS | MODE_ITEM)) && player->numItemBarBlocks >= MAXITEMBARBLOCKS && player->itemBag != ITEMBAG_NONE) {
		       GenNextItem(player);
	       }
	}
	else if ((player->modeFlags & (MODE_VERSUS | MODE_ITEM)) && player->numItemBarBlocks >= MAXITEMBARBLOCKS && player->itemBag != ITEMBAG_NONE) {
		GenNextItem(player);
	}
}
#else
void NextItem(Player* player) {
	if (
		(player->modeFlags & MODE_NORMAL) &&
		!(player->modeFlags & MODE_VERSUS) &&
		!(player->nowFlags & NOW_STAFF) &&
		player->level >= ((player->normalItemIndex + 1) * 100)) {
		const uint8_t itemNum = NormalItemNums[player->normalItemIndex];
		const uint32_t itemBagFlag = 1 << itemNum;
		if (player->normalItemBag & itemBagFlag) {
			player->normalItemBag &= ~itemBagFlag;
			NextBagItem(player, itemNum);
			player->normalItemIndex++;
		}
	}
	else if (
		(player->modeFlags & (MODE_VERSUS | MODE_ITEM)) &&
		player->numItemBarBlocks >= MAXITEMBARBLOCKS &&
		player->itemBag != ITEMBAG_NONE) {
		GenNextItem(player);
	}
}
#endif

static const SoundEffect NextSoundEffects[7] = {
	SOUNDEFFECT_IBLOCK,
	SOUNDEFFECT_ZBLOCK,
	SOUNDEFFECT_SBLOCK,
	SOUNDEFFECT_JBLOCK,
	SOUNDEFFECT_LBLOCK,
	SOUNDEFFECT_OBLOCK,
	SOUNDEFFECT_TBLOCK
};

void UpdatePlayNext(Player* player) {
	TEMPPTR(EntryData, entry);

	// Level at most is 999.
	if (player->level > NextSectionLevels[9]) {
		player->level = NextSectionLevels[9];
	}

	// Advance blocks tracking for items.
	player->numBlocks++;
	if (player->numItemBarBlocks < 20u && player->activeItemType == ITEMTYPE_NULL) {
		player->numItemBarBlocks++;
	}
	if (player->nextBlock & BLOCK_ITEM) {
		player->numItemBarBlocks = 0u;
	}

	// Generate next block.
	player->activeBlock = player->nextBlock;
	player->activeBlockItemType = player->nextBlockItemType;
	player->nextBlockItemType = ITEMTYPE_NULL;
	uint8_t nextBlockNum;
	for (uint8_t numRetries = Demo ? 6u : 5u; numRetries != 0u;) {
		numRetries--;
		nextBlockNum = GenNextBlockNum(player);
		if (RANDOMIZER_USEHISTORY) {
			int16_t numRejects = 0;
			uint8_t* historyEntry = player->history;
			for (int16_t historyIndex = 0; historyIndex < lengthoffield(Player, history); historyIndex++) {
				if (nextBlockNum == *historyEntry++) {
					numRejects++;
				}
			}

			if (numRejects > 0) {
				nextBlockNum = GenNextBlockNum(player);
			}
			else {
				numRetries = 0u;
			}
		}
	}
	for (int16_t historyIndex = lengthoffield(Player, history) - 1; historyIndex > 0; historyIndex--) {
		player->history[historyIndex] = player->history[historyIndex - 1];
	}
	player->history[0] = nextBlockNum;
	player->nextBlock = (Block)TOBLOCKTYPE(nextBlockNum);

	// Item/big mode.
	if ((player->modeFlags & MODE_BIG) || (player->itemMiscFlags & ITEMMISC_DEATHBLOCK)) {
		player->nextBlock |= BLOCK_BIG;
	}
	if ((player->modeFlags & MODE_ITEM) && player->itemBag == ITEMBAG_NONE) {
		player->itemBag = ITEMBAG_FULL;
	}
	if (!(player->modeFlags & (MODE_NOITEM | MODE_CEMENT))) {
		NextItem(player);
	}

	// Entry position.
	player->activeRotation = ROTATION_DOWN;
	Fixed32 activeCol;
	if (GameFlags & GAME_DOUBLES) {
		if (player->num == PLAYER1) {
			activeCol = F32(ENTRY_DOUBLES1PCOL, 0x0000);
		}
		else {
			activeCol = F32(ENTRY_DOUBLES2PCOL, 0x0000);
		}
	}
	else {
		activeCol = F32(ENTRY_SINGLECOL, 0x0000);
	}
	player->activePos[0] = activeCol;
	if (player->modeFlags & MODE_BIG) {
		player->activePos[0].integer++;
	}
	player->activePos[1] = ENTRYPOS_Y;

	// IRS.
	if (ROTATED_ANY(GameButtonsDown[player->num])) {
		Rotation initialRotation;
		if (ROTATED_LEFT(GameButtonsDown[player->num])) {
			initialRotation = ROTATE_LEFT(player->activeRotation);
		}
		else {
			initialRotation = ROTATE_RIGHT(player->activeRotation);
		}
		player->numActiveRotations++;

		if (!Blocked(player, player->activePos[0].integer, player->activePos[1].integer, initialRotation)) {
			player->activeRotation = initialRotation;
			PlaySoundEffect(SOUNDEFFECT_IRS);
		}
	}

	// Entry of next block.
	if (Blocked(player, player->activePos[0].integer, player->activePos[1].integer, player->activeRotation)) {
		// Entering block is blocked. Game over, unless only blocked by other player's active block in doubles.
		if (GameFlags & GAME_DOUBLES) {
			if (entry->numMatrixBlockings == 0) {
				// Entry is blocked, without matrix blockings; only other player's block is blocking.
				NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, PLAYSTATE_BLOCKEDENTRY });
			}
			else {
				// Entry is blocked by the matrix, so game over.
				if ((player->nowFlags & NOW_STAFF) && (player->otherPlayer->nowFlags & NOW_STAFF)) {
					Players[PLAYER1].values[1] = 1;
					Players[PLAYER1].showGameOver = false;

					ShowGameOverFade(&Players[PLAYER1]);
					LockActiveBlock(&Players[PLAYER1], LOCKTYPE_GAMEOVER);
					LockActiveBlock(&Players[PLAYER2], LOCKTYPE_GAMEOVER);

					Players[PLAYER1].nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
					Players[PLAYER1].nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;
					Players[PLAYER1].nowFlags |= NOW_STOPPED | NOW_NOUPDATE;

					Players[PLAYER2].nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
					Players[PLAYER2].nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;
					Players[PLAYER2].nowFlags |= NOW_NOUPDATE;
				}
				else {
					LockActiveBlock(player, LOCKTYPE_NORMAL);
					NextPlayGameOver(player);
				}
			}
		}
		else if (GameFlags & GAME_VERSUS) {
			LockActiveBlock(player, LOCKTYPE_NORMAL);

			player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);

			if (player->num == PLAYER1) {
				GameFlags |= GAME_WINNER2P;
			}
			else {
				GameFlags |= GAME_WINNER1P;
			}
		}
		else {
			if ((player->nowFlags & NOW_STAFF) && (player->modeFlags & (MODE_NORMAL | MODE_TGMPLUS | MODE_TADEATH))) {
				player->values[1] = 1;
				player->values[2] = 0;

				ShowGameOverFade(player);
				LockActiveBlock(player, LOCKTYPE_NORMAL);

				player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
				player->nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;
				player->nowFlags |= NOW_STOPPED | NOW_NOUPDATE;
			}
			else {
				LockActiveBlock(player, LOCKTYPE_NORMAL);

				player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);

				NextPlayGameOver(player);
			}
		}
	}
	else {
		// Apply initial gravity, then enter the next block.

		// Gravity for the current mode.
		Fixed32 gravity = CurrentGravity(player);

		// Force 20G.
		if (player->modeFlags & MODE_20G || ((player->nowFlags & NOW_STAFF) && (player->modeFlags & MODE_NORMAL))) {
			gravity = F32(20, 0x0000);
		}

		// Apply initial drop, if 1G or higher.
		if (gravity.integer >= 1) {
			player->activePos[1] = StepGravity(player, gravity);
		}

		NextPlayActive(player);
	}

	if (player->play.state != PLAYSTATE_BLOCKEDENTRY) {
		SoundEffect soundEffect;
		if (player->nextBlock & BLOCK_ITEM) {
			soundEffect = SOUNDEFFECT_ITEM;
		}
		else {
			soundEffect = NextSoundEffects[TOBLOCKNUM(player->nextBlock & BLOCK_TYPE)];
		}
		PlaySoundEffect(soundEffect);
	}
}

void UpdatePlayBlockedEntry(Player* player) {
	TEMPPTR(EntryData, entry);
	if (Blocked(player, player->activePos[0].integer, player->activePos[1].integer, player->activeRotation)) {
		if (entry->numMatrixBlockings == 0) {
			// Blocked by other player's active block and no matrix squares.
			// So wait until the other player is out of the way.
			return;
		}
		else {
			LockActiveBlock(player, LOCKTYPE_NORMAL);
			NextPlayGameOver(player);
		}
	}
	else {
		// Not blocked, so enter the active block.

		SoundEffect nextBlockSoundEffect;
		if (player->nextBlock & BLOCK_ITEM) {
			nextBlockSoundEffect = SOUNDEFFECT_ITEM;
		}
		else {
			nextBlockSoundEffect = NextSoundEffects[TOBLOCKNUM(player->nextBlock)];
		}
		PlaySoundEffect(nextBlockSoundEffect);

		if (ROTATED_ANY(GameButtonsDown[player->num])) {
			Rotation newRotation;
			if (ROTATED_LEFT(GameButtonsDown[player->num])) {
				newRotation = ROTATE_LEFT(player->activeRotation);
			}
			else {
				newRotation = ROTATE_RIGHT(player->activeRotation);
			}
			player->numActiveRotations++;
			if (!Blocked(player, player->activePos[0].integer, player->activePos[1].integer, newRotation)) {
				player->activeRotation = newRotation;
				PlaySoundEffect(SOUNDEFFECT_IRS);
			}
		}

		Fixed32 gravity = CurrentGravity(player);
		if ((player->modeFlags & MODE_20G) || ((player->nowFlags & NOW_STAFF) && (player->modeFlags & MODE_NORMAL))) {
			gravity = F32(20, 0x0000);
		}
		if (gravity.integer >= 1) {
			player->activePos[1] = StepGravity(player, gravity);
		}
		NextPlayActive(player);
	}
}

void UpdatePlayGarbageEntry(Player* player) {
	UpdateAutoshift(player);

	switch (player->subStates[SUBSTATE_GARBAGE]) {
	case GARBAGE_ENTRY:
		for (int16_t row = MATRIX_HEIGHT - 1; row > 1; row--) {
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(player, row, col) = MATRIX(player, row - 1, col);
			}
		}
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
			if ((player->modeFlags & MODE_MASTER) && (player->modeFlags & MODE_INVISIBLE)) {
				if (player->garbage[col - 1] != BLOCKTYPE_EMPTY) {
					MATRIX(player, 1, col).block = BLOCK_FADING | (player->garbage[col - 1] & BLOCK_TYPE);
				}
				else {
					MATRIX(player, 1, col).block = player->garbage[col - 1] & BLOCK_TYPE;
				}
				MATRIX(player, 1, col).brightness = 0;
			}
			else {
				MATRIX(player, 1, col).block = player->garbage[col - 1] & BLOCK_TYPE;
				MATRIX(player, 1, col).itemType = ITEMTYPE_NULL;
				MATRIX(player, 1, col).brightness = 0;
			}
		}

		for (int16_t row = 0; row < GARBAGEHEIGHT - 1; row++) {
			for (int16_t col = 0; col < FIELD_SINGLEWIDTH; col++) {
				player->garbage[row * MATRIX_SINGLEWIDTH + col] = player->garbage[(row + 1) * MATRIX_SINGLEWIDTH + col];
			}
		}
		Block* block = &player->garbage[(GARBAGEHEIGHT - 1) * MATRIX_SINGLEWIDTH];
		for (int16_t col = 0; col < FIELD_SINGLEWIDTH; col++) {
			*block = NULLBLOCK;
		}

		player->numGarbageRows--;
		player->subStates[SUBSTATE_GARBAGE] = GARBAGE_DELAY;
		player->values[0] = 4;
		PlaySoundEffect(SOUNDEFFECT_IRS);
		break;

	case GARBAGE_DELAY:
		if (--player->values[0] == 0) {
			if (player->numGarbageRows != 0) {
				player->subStates[SUBSTATE_GARBAGE] = GARBAGE_ENTRY;
			}
			else {
				player->subStates[SUBSTATE_GARBAGE] = GARBAGE_END;
				player->values[0] = 15;
				CheckDisableItemDescription(player);
			}
		}
		break;

	case GARBAGE_END:
		if (--player->values[0] == 0) {
			NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_GARBAGECHECK });
			player->play.flags &= ~PLAYFLAG_FORCEENTRY;
		}
		break;

	default:
		break;
	}
}

static uint8_t TgmPlusGarbage[] = {
	0,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,0,

	0,0,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1,0,0,
	1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,0,

	1,1,0,1,1,1,1,1,1,1,
	1,0,0,1,1,1,1,1,1,1,
	1,0,1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,0,1,1,
	1,1,1,1,1,1,1,0,0,1,
	1,1,1,1,1,1,1,1,0,1,

	1,1,1,1,0,0,1,1,1,1,
	1,1,1,1,0,0,1,1,1,1,

	1,1,1,1,0,1,1,1,1,1,
	1,1,1,0,0,0,1,1,1,1
};

void UpdateTgmPlusGarbage(Player* player) {
	uint8_t minTgmPlusBlocks = 13u;
	if (player->level >= 100u) {
		minTgmPlusBlocks = 12u;
	}
	if (player->level >= 200u) {
		minTgmPlusBlocks = 11u;
	}
	if (player->level >= 300u) {
		minTgmPlusBlocks = 10u;
	}
	if (player->level >= 400u) {
		minTgmPlusBlocks = 9u;
	}
	if (player->level >= 500u) {
		minTgmPlusBlocks = 8u;
	}
	if (player->level >= 600u) {
		minTgmPlusBlocks = 7u;
	}
	if (player->level >= 700u) {
		minTgmPlusBlocks = 6u;
	}
	if (player->level >= 800u) {
		minTgmPlusBlocks = 5u;
	}
	if (player->level >= 900u) {
		minTgmPlusBlocks = 4u;
	}
	// The above is equivalent to this:
	// minTgmPlusBlocks = (player->level >= 900u) ? 4u : 13u - player->level / 100u;

	if (player->numTgmPlusBlocks >= minTgmPlusBlocks) {
		player->numTgmPlusBlocks = 0u;
		// Shift field up a row, to make room for the garbage row entering at the bottom.
		for (int16_t row = MATRIX_HEIGHT - 1; row > 1; row--) {
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(player, row, col) = MATRIX(player, row - 1, col);
			}
		}

		// Copy a TGM+ garbage sequence row into the entering garbage row.
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
			MATRIX(player, 1, col).block = TgmPlusGarbage[player->tgmPlusGarbageIndex * FIELD_SINGLEWIDTH + col - 1] ? (Block)BLOCKTYPE_GARBAGE : (Block)BLOCKTYPE_EMPTY;
			MATRIX(player, 1, col).itemType = ITEMTYPE_NULL;
			MATRIX(player, 1, col).brightness = 0u;
		}

		player->tgmPlusGarbageIndex = ((size_t)player->tgmPlusGarbageIndex + 1u) % (lengthof(TgmPlusGarbage) / FIELD_SINGLEWIDTH);
		PlaySoundEffect(SOUNDEFFECT_IRS);
	}
}

void UpdatePlayEntry(Player* player) {
	if (--player->values[0] != 0) {
		UpdateAutoshift(player);
		if (player->modeFlags & MODE_TGMPLUS) {
			UpdateTgmPlusGarbage(player);
		}
	}
	else {
		NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_GARBAGECHECK });
		player->play.flags &= ~PLAYFLAG_FORCEENTRY;

		int16_t maxLevel;
		if (player->modeFlags & MODE_VERSUS) {
			maxLevel = NextSectionLevels[Settings[SETTING_MAXVERSUSSECTION]];
		}
		else if (player->modeFlags & MODE_DOUBLES) {
			maxLevel = NextSectionLevels[2];
		}
		else if (player->modeFlags & MODE_NORMAL) {
			maxLevel = NextSectionLevels[2];
		}
		else {
			maxLevel = NextSectionLevels[player->section];
		}
		maxLevel--;

		if (!(player->nowFlags & NOW_STAFF)) {
			if (++player->level > maxLevel) {
				player->level = maxLevel;
			}
		}
	}
}

void UpdatePlayGameOver(Player* player) {
	if ((GameFlags & GAME_TWIN) && (player->nowFlags & NOW_SHOWRANKINGCODE)) {
		ShowRankingCode(player);
	}
	else if (!(GameFlags & GAME_DOUBLES) && player->numSecretGradeRows >= 5) {
		DisplayObject(OBJECT_SECRETGRADE, 158, player->screenPos[0] - 35, 2u, 110u);
		int32_t offsetX = ((int32_t)(SecretGradeScales[player->num] - UNSCALED) * -0x4000) / UNSCALED;
		int32_t offsetY = ((int32_t)(SecretGradeScales[player->num] - UNSCALED) * -0x4000) / UNSCALED;
		DisplayObjectEx(
			ObjectTableGrades[player->numSecretGradeRows - 1],
			170 + (offsetY >> 10),
			player->screenPos[0] - 13 + (offsetX >> 10),
			1u,
			110u,
			(SpriteScale)SecretGradeScales[player->num],
			(SpriteScale)SecretGradeScales[player->num],
			false
		);

		SecretGradeScales[player->num] -= 2;
		if (SecretGradeScales[player->num] < UNSCALED) {
			SecretGradeScales[player->num] = UNSCALED;
		}
	}

	if (player->values[1]) {
		player->values[1] = false;
		PlaySoundEffect(SOUNDEFFECT_GAMEOVER);
	}

	if (player->showGameOver) {
		DisplayObject(OBJECT_A6F14, 100, player->screenPos[0], 0u, 125u);
		if (player->otherPlayer->refusingChallenges) {
			ShowText(player->screenPos[0] - TextWidth("NO MORE") / 2, 65, "NO MORE", 15u, false);
			ShowText(player->screenPos[0] - TextWidth("CHALLENGER") / 2, 77, "CHALLENGER", 15u, false);
		}
	}

	if (--player->values[0] > 0) {
		if ((GameButtonsDown[player->num] & BUTTON_START) && player->values[0] > 10) {
			player->values[0] = 10;
		}
		if ((GameFlags & GAME_DOUBLES) && (GameButtonsDown[player->otherPlayer->num] & BUTTON_START) && player->values[0] > 10) {
			player->values[0] = 10;
		}
		if ((GameFlags & GAME_DOUBLES) && player->values[0] == 10) {
			UNK_6029546(2u, 10, 0, 7);
		}

		player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
		player->nowFlags |= NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK;
	}
	else {
		player->nextScaleA = F16(0, 0x0000);
		player->nextScaleV = F16(0, 0x0000);
		player->nextScale = F16(0, 0x0000);

		player->numItemBarBlocks = 0u;

		player->nowFlags = NOW_ABSENT | NOW_SHOWFIELD | NOW_SHOWNEXTBLOCK | NOW_GAMEOVER;

		player->modeFlags &= MODE_SELECTIONS;
		player->modeFlags &= ~MODE_DOUBLES;

		AddGameTime(player->gameTime);

		if (GameFlags & GAME_DOUBLES) {
			player->otherPlayer->nowFlags = NOW_ABSENT | NOW_GAMEOVER;
			player->otherPlayer->modeFlags &= ~MODE_DOUBLES;
		}
	}
}

#undef numSecretGradeRows

uint32_t PointsBaseValue(Player* player, uint8_t numLines) {
	uint8_t numLogicalLines;
	if (player->modeFlags & MODE_BIG) {
		numLogicalLines = numLines / 2;
		if (numLines % 2 != 0) {
			numLines++;
		}
	}
	else {
		numLogicalLines = numLines;
	}

	if (player->modeFlags & MODE_BIG) {
		if (numLines >= 8u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 4u;
		}
		else if (numLines > 4u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 3u;
		}
		else if (numLines > 2u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 2u;
		}
		else if (numLines == 0u) {
			player->combo = 1;
		}
		else {
			player->level++;
		}
	}
	else {
		if (numLines >= 4u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 4u;
		}
		else if (numLines >= 3u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 3u;
		}
		else if (numLines >= 2u) {
			player->combo += (numLogicalLines - 1) * 2;
			player->level += 2u;
		}
		else if (numLines == 1u) {
			player->level++;
		}
		else {
			player->combo = 1;
		}
	}

	UpdateGrade(player, numLogicalLines);
	UpdatePlayerGrade(player);

	if (numLines != 0u) {
		if (
			(player->modeFlags & MODE_TADEATH) &&
			(GameFlags & GAME_TWIN) &&
			player->level >= NextSectionLevels[4] &&
			!player->passedMGradeCheckpoint &&
			player->clearTime <= TIME(3, 25, 0)) {
			player->passedMGradeCheckpoint = true;
			player->grade = PLAYERGRADE_M;
			PlaySoundEffect(SOUNDEFFECT_GRANDMASTER);
		}
		CheckNextSection(player);
	}

	uint32_t pointsBaseValue = player->level / 4;
	if (player->level % 4 != 0) {
		pointsBaseValue++;
	}

	if ((player->modeFlags & MODE_NORMAL) && (GameFlags & GAME_TWIN)) {
		if (player->level >= NextSectionLevels[2]) {
			player->level = NextSectionLevels[2];
		}
	}
	else if ((player->modeFlags & (MODE_MASTER | MODE_TGMPLUS)) && (GameFlags & GAME_TWIN)) {
		if (player->level >= NextSectionLevels[9]) {
			player->level = NextSectionLevels[9];
		}
	}
	else if ((player->modeFlags & MODE_TADEATH) && (GameFlags & GAME_TWIN)) {
		if (player->passedMGradeCheckpoint) {
			if (player->level >= NextSectionLevels[9]) {
				player->level = NextSectionLevels[9];
			}
		}
		else if (player->level >= NextSectionLevels[4]) {
			player->level = NextSectionLevels[4];
		}
	}
	else if ((player->modeFlags & MODE_DOUBLES) && player->level >= NextSectionLevels[2]) {
		player->level = NextSectionLevels[2];
	}

	pointsBaseValue += NumFastDropRows[player->num] + NumInstantDropRows[player->num] * 2;
	pointsBaseValue *= numLines * player->combo;
	if (pointsBaseValue != 0) {
		pointsBaseValue += player->level / 2 + player->level % 2;
	}

	if (player->lockDelay - player->numActiveFrames > 0 && pointsBaseValue != 0) {
		pointsBaseValue += (player->lockDelay - player->numActiveFrames) * 7;
	}

	return pointsBaseValue;
}

static const ObjectData* UNK_3A8C0[3] = {
	OBJECTPTR(0x170),
	OBJECTPTR(0x16E),
	OBJECTPTR(0x16F)
};

void UpdatePlayVersusOver(Player* player) {
	GameFlag winnerFlag;
	if (player->num == PLAYER1) {
		winnerFlag = GAME_WINNER1P;
	}
	else {
		winnerFlag = GAME_WINNER2P;
	}

	size_t objectIndex;
	uint8_t palNum;
	uint32_t var1;
	if (GameFlags & GAME_BIT10) {
		objectIndex = 0u;
		var1 = 0x30;
		palNum = 0u;
		if (player->values[1] >= UNSCALED) {
			player->values[1] = UNSCALED;
		}
		else {
			player->values[1] += 4;
		}
	}
	else if (GameFlags & winnerFlag) {
		objectIndex = 1u;
		if (ScreenTime % 4) {
			palNum = 8u;
		}
		else {
			palNum = 0u;
		}
		var1 = 0x20;
		if (player->values[1] > UNSCALED) {
			player->values[1] -= 4;
		}
		else {
			player->values[1] = UNSCALED;
		}
		if (GameButtonsNew[player->num] & BUTTON_START) {
			player->values[0]++;
		}
		if (Game.UNK_2 > 10 && (player->values[3]-- == 0 || player->values[0] % 5 == 0)) {
			player->values[3] = 20;
			player->values[0] = 1;
			player->showGameOver = (player->showGameOver + 1) % 2;
			PlaySoundEffect(SOUNDEFFECT_SHOTGUN);
			// BUG: I would assume the original code had these Rand() calls as
			// direct arguments to ShowFireworks. Per standard C, it's
			// undefined what order function calls are evaluated when used as
			// function call arguments. So, since correct randomization is
			// desired, the calls are moved out, so their call order is
			// guaranteed. But one of the Rand() calls is a ShowFireworks()
			// function call argument, since that call is guaranteed to be
			// after these two.
			uint32_t arg3 = Rand(10); // TODO
			uint32_t arg2 = Rand(10); // TODO
			ShowFireworks(player, Rand(13) + 5, arg2, arg3);
		}
		if (GameFlags & GAME_BIT19) {
			DisplayObject(OBJECT_AAEB0, 140, player->screenPos[0], 0u, 110u);
		}
		else if (GameFlags & GAME_BIT18) {
			DisplayObject(OBJECT_AAEBC, 140, player->screenPos[0] - 35, 0u, 110u);
			DisplayObject(OBJECT_AAEC8, 153, player->screenPos[0] - 8, 0u, 110u);
			ShowStatusNumEx(player->level, 150, player->screenPos[0] - 36, 0u, 110u, 3, false, NUMALIGN_RIGHT);
			ShowStatusNumEx(player->otherPlayer->level, 150, player->screenPos[0] + 10, 0u, 110u, 3, false, NUMALIGN_RIGHT);
		}
	}
	else {
		objectIndex = 2u;
		palNum = 0u;
		var1 = 0x30;
		if (player->values[1] < UNSCALED) {
			player->values[1] += 4;
		}
		else {
			player->values[1] = UNSCALED;
		}
	}
	// TODO: Rename var0/var1 and document how the pixel position calculation
	// here relates to the scale value. I believe it adjusts the position of
	// the object to be centered as it scales at a screen position.
	int32_t var0 = ((player->values[1] - UNSCALED) << 10) / UNSCALED;
	DisplayObjectEx(UNK_3A8C0[objectIndex], 120 + (-(var0 << 3) >> 10), player->screenPos[0] + (-(int32_t)(var1 * var0) >> 10), palNum, 125u, (SpriteScale)player->values[1], (SpriteScale)player->values[1], false);
}

#undef showGameOver

enum ReadyGoState {
	READYGOSTATE_INIT,
	READYGOSTATE_READYENTRY,
	READYGOSTATE_READYDELAY,
	READYGOSTATE_READYEXIT,
	READYGOSTATE_GOENTRY,
	READYGOSTATE_GODELAY,
	READYGOSTATE_GOEXIT
};

#define READYGO_FRAMES 0
#define READYGO_Y 1

void UpdatePlayStart(Player* player) {
	if (player->modeFlags & MODE_VERSUS) {
		if (!(GameFlags & GAME_VERSUSREADYGO)) {
			NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_NEXT });
			GameFlags &= ~GAME_BIT11;
		}
		goto skipReadyGo;
	}
	else if ((player->num != Game.versusWinner || Game.numVersusWins == 0) && !(player->modeFlags & (MODE_NORMAL | MODE_DOUBLES | MODE_TADEATH))) {
		CheckSetItemMode(player);
	}

	switch (player->subStates[SUBSTATE_READYGO]) {
		case READYGOSTATE_INIT:
			player->subStates[SUBSTATE_READYGO]++;
			player->values[READYGO_FRAMES] = 10;
			player->values[READYGO_Y] = 70;

		case READYGOSTATE_READYENTRY:
			DisplayObjectEx(OBJECT_READY, player->values[READYGO_Y], player->screenPos[0], 0, 110, -5 * player->values[READYGO_FRAMES] + UNSCALED, UNSCALED, false);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->subStates[SUBSTATE_READYGO]++;
				player->values[READYGO_FRAMES] = 30;
				PlaySoundEffect(SOUNDEFFECT_READY);
			}
			else {
				player->values[READYGO_Y] += 5;
			}
			break;

		case READYGOSTATE_READYDELAY:
			DisplayObject(OBJECT_READY, player->values[READYGO_Y], player->screenPos[0], 0, 110);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->subStates[SUBSTATE_READYGO]++;
				player->values[READYGO_FRAMES] = 10;
			}
			break;

		case READYGOSTATE_READYEXIT:
			DisplayObjectEx(OBJECT_READY, player->values[READYGO_Y], player->screenPos[0], 0, 110, 5 * player->values[READYGO_FRAMES] + 0x0D, UNSCALED, false);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->subStates[SUBSTATE_READYGO]++;
				player->values[READYGO_FRAMES] = 10;
				player->values[READYGO_Y] = 70;
			}
			else {
				player->values[READYGO_Y] += 5;
			}
			break;

		case READYGOSTATE_GOENTRY:
			DisplayObjectEx(OBJECT_GO, player->values[READYGO_Y], player->screenPos[0], 0, 110, -5 * player->values[READYGO_FRAMES] + UNSCALED, UNSCALED, false);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->subStates[SUBSTATE_READYGO]++;
				player->values[READYGO_FRAMES] = 30;
				PlaySoundEffect(SOUNDEFFECT_GO);
			}
			else {
				player->values[READYGO_Y] += 5;
			}
			break;

		case READYGOSTATE_GODELAY:
			DisplayObject(OBJECT_GO, player->values[READYGO_Y], player->screenPos[0], 0, 110);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->subStates[SUBSTATE_READYGO]++;
				player->values[READYGO_FRAMES] = 10;
			}
			break;

		case READYGOSTATE_GOEXIT:
			DisplayObjectEx(OBJECT_GO, player->values[READYGO_Y], player->screenPos[0], 0, 110, 5 * player->values[READYGO_FRAMES] + 0x0D, UNSCALED, false);
			if (--player->values[READYGO_FRAMES] == 0) {
				player->numGarbageRows = 0u;
				NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_NEXT });
				GameFlags &= ~GAME_BIT11;
			}
			else {
				player->values[READYGO_Y] += 5;
			}
			break;

		default:
			break;
	}

skipReadyGo:
	UpdateAutoshift(player);

	if ((GameFlags & GAME_VERSUS) && player->values[READYGO_FRAMES] < 70 && ROTATED_ANY(GameButtonsDown[player->num])) {
		Rotation newRotation;
		if (ROTATED_LEFT(GameButtonsDown[player->num])) {
			newRotation = ROTATE_LEFT(player->activeRotation);
		}
		else {
			newRotation = ROTATE_RIGHT(player->activeRotation);
		}
		player->activeRotation = newRotation;
	}
}

void UpdatePlayGarbageCheck(Player* player) {
	if (!(player->nowFlags & NOW_NOGARBAGE)) {
		if (player->numGarbageRows != 0u && player->itemPlayer->activeItemType != ITEMTYPE_MIRRORBLOCK && player->itemPlayer->activeItemType != ITEMTYPE_DARKBLOCK) {
			NextPlayGarbageEntry(player);
		}
		else {
			NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_NEXT });
		}
	}

	UpdateAutoshift(player);
}

static const uint8_t InitItemWeights[NUMITEMTYPES] = { 50u, 1u, 250u, 250u, 100u, 50u, 3u, 150u, 100u, 5u, 50u, 250u, 150u, 250u, 3u, 150u, 3u, 150u, 5u };

void GenNextItem(Player* player) {
	TEMPPTR(EntryData, entry);
	for (uint16_t itemNum = 0u; itemNum < NUMITEMTYPES; itemNum++) {
		entry->itemWeights[itemNum] = InitItemWeights[itemNum];
	}

	for (uint16_t itemNum = 0u; itemNum < NUMITEMTYPES; itemNum++) {
		if (!(player->itemBag & (1 << itemNum))) {
			entry->itemWeights[itemNum] = 0u;
		}
	}

	uint16_t seedSum = 0;
	for (uint16_t itemNum = 0; itemNum < NUMITEMTYPES; itemNum++) {
		seedSum += entry->itemWeights[itemNum];
	}

	if (seedSum > 0) {
		bool retry = true;
		while (retry) {
			uint16_t sumMax;
			if (!Demo || Screen != SCREEN_VERSUSDEMO) {
				sumMax = (ScreenTime + Rand(4750u)) % seedSum + 1u;
			}
			else if (player->num == PLAYER1) {
				sumMax = 0u;
			}
			else {
				sumMax = 50u;
			}

			uint16_t itemNum = 0u;
			for (uint16_t weightSum = 0u; itemNum < NUMITEMTYPES; itemNum++) {
				weightSum += entry->itemWeights[itemNum];
				if (weightSum >= sumMax) {
					break;
				}
			}
			if (itemNum == NUMITEMTYPES) {
				break;
			}
			if (player->itemBag & (1 << itemNum)) {
				NextBagItem(player, (uint8_t)itemNum);
				retry = false;
			}
		}
	}
}

void Staff(Player* player) {
	player->nowFlags |= NOW_STAFF;
	ShowStaff(player);
}

void ThrowOutActiveBlock(Player* player) {
	player->nowFlags &= ~(NOW_SHOWTLSBLOCK | NOW_SHOWACTIVEBLOCK);
	ShowThrownOutActiveBlock(player);
	if (player->activeBlock & BLOCK_ITEM) {
		ItemDescriptions[player->num] = ITEMTYPE_NULL;
	}
}

// If you want this to properly adapt to resizing the matrix, replace
// FIELD_HEIGHT constants here with player->fieldHeight. You'd need to change
// the secret grade ranking code too, though.
uint8_t NumSecretGradeRows(Player* player) {
	uint8_t numSecretGradeRows = 0u;
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		int16_t emptyBlocks = 0;
		for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
			if (MATRIX(player, row, col).block == NULLBLOCK) {
				emptyBlocks++;
			}
		}
		if (emptyBlocks == 1) {
			if (row < FIELD_HEIGHT / 2) {
				// Check lower half of pattern.
				if (MATRIX(player, row, row).block == NULLBLOCK) {
					if (MATRIX(player, row + 1, row).block != NULLBLOCK) {
						// Row matches the pattern plus there's a square above the hole in the current row.
						numSecretGradeRows++;
					}
				}
				else {
					row = player->matrixHeight;
				}
			}
			else {
				// Check upper half of pattern.
				if (MATRIX(player, row, FIELD_HEIGHT - row).block == NULLBLOCK) {
					if (MATRIX(player, row + 1, FIELD_HEIGHT - row).block != NULLBLOCK) {
						// Row matches the pattern plus there's a square above the hole in the current row.
						numSecretGradeRows++;
					}
				}
				else {
					row = player->matrixHeight;
				}
			}
		}
		else {
			row = player->matrixHeight;
		}
	}
	if (numSecretGradeRows >= FIELD_HEIGHT - 1) {
		numSecretGradeRows = FIELD_HEIGHT;
	}
	return numSecretGradeRows;
}

void SetFieldVisible(Player* player) {
	for (int16_t row = 1; row < player->matrixHeight - 1; row++) {
		MatrixBlock* matrixBlock = &MATRIX(player, row, 1);
		for (int16_t col = 1; col < player->matrixWidth - 1; col++, matrixBlock++) {
			matrixBlock->block &= ~(BLOCK_FLASH | BLOCK_FADING | BLOCK_INVISIBLE);
		}
	}
}

Fixed32 CurrentGravity(Player* player) {
	Fixed32 gravity;
	if (player->modeFlags & MODE_DOUBLES) {
		gravity = GravityDoubles[player->level];
	}
	else if (player->modeFlags & (MODE_MASTER | MODE_VERSUS)) {
		gravity = Gravity[player->level];
	}
	else {
		gravity = GravityNormal[player->level];
	}

	if (player->modeFlags & MODE_TGMPLUS) {
		gravity = GravityTgmPlus[player->level];
	}
	else if (player->modeFlags & MODE_TADEATH) {
		gravity = Gravity[player->level + 700u < 1000u ? player->level + 700u : 999u];
	}

	if (player->modeFlags & MODE_TIMEOUT) {
		gravity = Gravity[999u];
	}

	if (player->miscFlags & MISC_FORCELOWG) {
		gravity = F32(0, 0x4000);
	}

	return gravity;
}

void UpdatePlayStaffTransition(Player* player) {
	player->miscFlags &= ~MISC_RECOVERING;
	if ((++player->values[0] % 6) == 0) {
		ShowStaffClear(player, player->values[1]);
		for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
			MATRIX(player, player->values[1], col).block = NULLBLOCK;
		}
		if (++player->values[1] == player->matrixHeight - 1) {
			player->nowFlags |= NOW_STAFF;
			ShowStaff(player);
			player->modeFlags |= MODE_INVISIBLE;
			NextPlayEntry(player, true);
		}
	}
}

void CheckDisableItemDescription(Player* player) {
	for (int16_t row = 1; row < player->matrixHeight; row++) {
		MatrixBlock* square = &MATRIX(player, row, 1);
		for (int16_t col = 1; col < player->matrixWidth - 1; col++, square++) {
			if ((square->block & BLOCK_ITEM) && square->itemType == ItemDescriptions[player->num]) {
				return;
			}
		}
	}

	ItemDescriptions[player->num] = ITEMTYPE_NULL;
	player->numTgmPlusBlocks++;
}
