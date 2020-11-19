#include "Game/Play/RankingCode.h"
#include "Game/Play/Player.h"
#include "Game/Play/Ranking.h"
#include "Game/Graphics/ShowText.h"
#include "Game/Temp.h"
#include "Lib/Math.h"
#include "Lib/Macros.h"
#include <assert.h>

typedef struct RankingCodeNode RankingCodeNode;
struct RankingCodeNode {
	uint8_t digit;
	RankingCodeNode *previous;
	RankingCodeNode *next;
};

typedef enum RankingLine {
	RANKINGLINE_NULL,
	RANKINGLINE_GREEN,
	RANKINGLINE_ORANGE
} RankingLine;

static const uint8_t RankingCodeEncryptionKey[0x100] = {
	 3u, 10u, 18u,  1u, 16u, 14u,  8u,  6u, 14u,  1u, 15u, 14u,  2u,  3u,  2u,  8u,
	 5u, 16u,  5u,  7u,  1u, 11u, 18u, 17u, 15u, 14u,  9u, 15u, 10u,  2u,  5u, 11u,
	18u, 15u,  7u,  5u, 11u,  9u,  6u, 15u, 12u, 16u,  4u,  3u,  5u, 10u, 14u,  2u,
	 1u, 16u, 14u, 19u,  7u,  0u,  5u,  2u, 18u,  9u, 17u, 12u,  6u, 16u, 17u,  4u,
	18u,  0u,  9u, 18u,  8u,  9u, 17u,  5u,  8u,  9u, 19u,  2u, 13u, 10u,  0u, 13u,
	18u,  0u, 12u, 18u,  8u, 11u, 10u, 19u,  1u,  6u,  9u, 13u, 10u,  2u,  0u, 14u,
	 3u, 17u,  4u, 19u,  6u,  5u, 11u,  7u,  0u,  5u,  1u,  5u, 10u,  2u, 19u, 14u,
	15u,  3u,  6u,  1u, 10u,  0u, 11u,  0u,  2u,  1u, 11u, 12u,  7u, 13u,  3u,  9u,
	 8u, 12u, 17u,  4u, 10u, 18u,  6u,  7u,  1u,  7u, 16u, 18u, 16u, 14u,  6u, 16u,
	12u, 13u, 12u,  8u, 12u, 10u, 16u, 17u, 16u,  4u,  2u,  3u, 11u, 13u, 12u, 19u,
	 7u, 13u,  4u,  5u, 15u, 12u, 15u,  6u, 18u, 11u, 13u,  6u, 19u,  8u, 13u, 15u,
	17u,  3u,  0u, 14u, 17u, 10u,  1u,  7u, 12u,  6u,  9u,  4u,  1u, 17u,  7u,  4u,
	 3u,  8u, 16u, 15u,  0u,  9u,  4u, 13u, 17u, 18u, 19u,  3u,  5u, 14u,  2u, 11u,
	19u, 13u,  4u,  2u,  8u, 12u,  7u,  8u, 15u,  3u,  8u,  4u, 14u, 11u, 19u,  5u,
	 8u,  1u,  7u, 10u,  4u,  2u,  0u, 12u, 19u, 14u,  3u, 19u,  9u, 11u,  9u, 11u,
	 0u, 17u, 15u,  7u,  9u,  4u,  0u,  6u, 13u, 15u,  1u,  6u, 16u, 13u, 10u,  3u
};

static const char RankingCodeChars[RANKINGCODEBASE] = {
	'3', '4', '6', '7', '9',
	'C', 'D', 'E', 'F', 'G',
	'H', 'J', 'K', 'M', 'P',
	'R', 'T', 'W', 'X', 'Y'
};

// code[13], NULL
// code[arg0 - 1], code[14]
// code[arg0], code[15]
// Unused. Not sure what it was intended for.
void UNK_6023128(size_t arg0) {
	TEMPPTR(RankingCodeNode, code);

	arg0--;

	code[13].next = NULL;

	code[arg0].next = &code[14];
	code[14].previous = &code[arg0];

	code[15].next = &code[arg0 + 1];
	code[arg0 + 1].previous = &code[15];
}

void NewRankingCode(Player *player) {
	// Init digit nodes.
	TEMPPTR(RankingCodeNode, code);
	code[0].previous = &code[NUMRANKINGCODEDIGITS - 1];
	for (size_t i = 0; i < NUMRANKINGCODEDIGITS - 1; i++) {
		code[i].digit = 0u;
		code[i].next = &code[i + 1];
		code[i + 1].previous = &code[i];
	}

	// Generate ranking code.

	char* name = NewRankings[player->num].nameEntries->name;
	uint8_t nameCharSum = 0u;
	nameCharSum += name[0];
	nameCharSum += name[1];
	nameCharSum += name[2];
	nameCharSum += name[3];

	uint8_t keyIndex = Rand(32u);
	code[15].digit = keyIndex & 0xF;
	code[14].digit = (keyIndex >> 4) & 1;

	code[14].digit |=  (player->medalColors[MEDALTYPE_CO] & 3) << 1;
	code[14].digit |= ((player->medalColors[MEDALTYPE_RO] & 3) & 1) << 3;
	code[11].digit  = ((player->medalColors[MEDALTYPE_RO] & 3) >> 1) & 1;
	code[11].digit |=  (player->medalColors[MEDALTYPE_RE] & 3) << 1;
	code[11].digit |= ((player->medalColors[MEDALTYPE_SK] & 3) & 1) << 3;
	code[10].digit  = ((player->medalColors[MEDALTYPE_SK] & 3) >> 1) & 1;
	code[10].digit |=  (player->medalColors[MEDALTYPE_ST] & 3) << 1;
	code[10].digit |= ((player->medalColors[MEDALTYPE_AC] & 3) & 1) << 3;
	code[ 9].digit |= ((player->medalColors[MEDALTYPE_AC] & 3) >> 1) & 1;

	uint32_t masteringValue;
	if (player->modeFlags & MODE_NORMAL) {
		masteringValue = player->score;
	}
	else {
		masteringValue = player->masteringTime;
		if (masteringValue > TIME(15, 0, 0)) {
			masteringValue = TIME(15, 0, 1);
		}
	}
	code[9].digit |= (masteringValue & 7) << 1;
	code[8].digit  = (masteringValue >> 3) & 0xF;
	code[7].digit  = (masteringValue >> 7) & 0xF;
	code[6].digit  = (masteringValue >> 11) & 0xF;
	code[5].digit  = (masteringValue >> 15) & 0xF;
	code[4].digit  = (masteringValue >> 19) & 1;

	RankingLine line = RANKINGLINE_NULL;
	if (player->modeFlags & MODE_TADEATH) {
		if (player->passedMGradeCheckpoint) {
		       if (player->level == 999u) {
			       if (player->miscFlags & MISC_ORANGELINE) {
				       line = RANKINGLINE_ORANGE;
			       }
			       else {
				       line = RANKINGLINE_GREEN;
			       }
		       }
		}
		else if (player->level == 500u) {
			if (player->miscFlags & MISC_ORANGELINE) {
				line = RANKINGLINE_ORANGE;
			}
			else {
				line = RANKINGLINE_GREEN;
			}
		}
	}
	else if (player->modeFlags & MODE_NORMAL) {
		if (player->level == 300u) {
			if (player->miscFlags & MISC_ORANGELINE) {
				line = RANKINGLINE_ORANGE;
			}
			else {
				line = RANKINGLINE_GREEN;
			}
		}
	}
	else {
		if (player->level == 999u) {
			if (player->miscFlags & MISC_ORANGELINE) {
				line = RANKINGLINE_ORANGE;
			}
			else {
				line = RANKINGLINE_GREEN;
			}
		}
	}
	code[4].digit |= line << 1;

	code[4].digit |= (player->level & 1) << 3;
	code[3].digit  = (player->level >> 1) & 0xF;
	code[2].digit  = (player->level >> 5) & 0xF;
	code[1].digit  = (player->level >> 9) & 1;

	code[1].digit |= (player->grade & 7) << 1;
	code[0].digit  = (player->grade >> 3) & 3;

	// BUG: This was uninitialized originally, but is initialized here to
	// silence compiler warnings. The behavior of the code guarantees a mode
	// will be selected, though.
	ModeSelection selection = MODESELECTION_NORMAL;
	if (player->modeFlags & MODE_TADEATH) {
		selection = MODESELECTION_TADEATH;
	}
	if (player->modeFlags & MODE_TGMPLUS) {
		selection = MODESELECTION_TGMPLUS;
	}
	if (player->modeFlags & MODE_MASTER) {
		selection = MODESELECTION_MASTER;
	}
	if (player->modeFlags & MODE_NORMAL) {
		selection = MODESELECTION_NORMAL;
	}
	code[0].digit |= selection << 2;

	uint32_t codeChecksum = 0u;
	for (size_t i = 0; i < 12; i++) {
		codeChecksum += code[i].digit;
	}
	codeChecksum += code[15].digit;
	codeChecksum += code[14].digit;
	codeChecksum += nameCharSum;
	code[13].digit = codeChecksum & 0xF;
	code[12].digit = ((codeChecksum & 0xFF) >> 4) & 0xF;

	// Encrypt the ranking code.
	for (size_t i = 0; i < 12; i++) {
		code[i].digit += RankingCodeEncryptionKey[keyIndex++];
		code[i].digit %= RANKINGCODEBASE;
		keyIndex %= lengthof(RankingCodeEncryptionKey);
	}

	// Write ranking code to the player data.
	RankingCodeNode *node = &code[nameCharSum & 0xF];
	char *rankingCodeChar = player->rankingCode;
	for (size_t i = 0; i < NUMRANKINGCODEDIGITS; i++) {
		assert(rankingCodeChar != NULL && node != NULL && node->digit % RANKINGCODEBASE < lengthof(RankingCodeChars));
		*rankingCodeChar++ = RankingCodeChars[node->digit % RANKINGCODEBASE];
		assert(node->next != NULL);
		node = node->next;
	}
}

void ShowRankingCode(Player* player) {
	char charOut[2];
	for (size_t row = 0u; row < 2u; row++) {
		int16_t offsetX = 0;
		char* codeChar = &player->rankingCode[(NUMRANKINGCODEDIGITS / 2) * row];
		for (size_t col = 0u; col < NUMRANKINGCODEDIGITS / 2; col++, offsetX += 10) {
			charOut[0] = *codeChar++;
			charOut[1] = '\0';
			ShowPalCycleText(player->screenPos[0] + offsetX - 40, (int16_t)(150 + row * 12), charOut, true);
		}
	}
}
