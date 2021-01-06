#include "Game/Play/Grade.h"
#include "Game/Play/Player.h"
#include "Game/Save.h"
#include "Video/GameBg.h"
#include "Sound/Sound.h"
#include "Sound/SoundEffect.h"
#include "Eeprom/Setting.h"
#include <assert.h>

Grade Grades[NUMPLAYERS];

typedef struct GradeProgress {
	uint8_t progress[4];
	uint8_t decayDuration;
} GradeProgress;
// BUG: TAP might index this table with GRADELEVEL_MAX, rather than
// GRADELEVEL_MAX - 1, accessing data beyond the bounds of the intended array.
// The data that it accesses in that case has been added to the intended array,
// for accurate behavior, and to prevent memory access errors.
static const GradeProgress GradeProgressions[GRADELEVEL_MAX + 1] = {
	{ { 10u, 20u, 40u, 50u }, 125u },
	{ { 10u, 20u, 30u, 40u },  80u },
	{ { 10u, 20u, 30u, 40u },  80u },
	{ { 10u, 15u, 30u, 40u },  50u },
	{ { 10u, 15u, 20u, 40u },  45u },
	{ {  5u, 15u, 20u, 30u },  45u },
	{ {  5u, 10u, 20u, 30u },  45u },
	{ {  5u, 10u, 15u, 30u },  40u },
	{ {  5u, 10u, 15u, 30u },  40u },
	{ {  5u, 10u, 15u, 30u },  40u },
	{ {  2u, 12u, 13u, 30u },  40u },
	{ {  2u, 12u, 13u, 30u },  40u },
	{ {  2u, 12u, 13u, 30u },  30u },
	{ {  2u, 12u, 13u, 30u },  30u },
	{ {  2u, 12u, 13u, 30u },  30u },
	{ {  2u, 12u, 13u, 30u },  20u },
	{ {  2u, 12u, 13u, 30u },  20u },
	{ {  2u, 12u, 13u, 30u },  20u },
	{ {  2u, 12u, 13u, 30u },  20u },
	{ {  2u, 12u, 13u, 30u },  20u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  15u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ {  2u, 12u, 13u, 30u },  10u },
	{ { 10u, 10u, 10u, 10u },  10u }
};

static const uint8_t GradeProgressScales[10][4] = {
	{ 10, 10, 10, 10 },
	{ 10, 12, 14, 15 },
	{ 10, 12, 15, 18 },
	{ 10, 14, 16, 20 },
	{ 10, 14, 17, 22 },
	{ 10, 14, 18, 23 },
	{ 10, 14, 19, 24 },
	{ 10, 15, 20, 25 },
	{ 10, 15, 21, 26 },
	{ 10, 20, 25, 30 }
};

const GradeLevel GradeLevels[NUMPLAYERGRADES - 1] = {
	GRADELEVEL_9,
	GRADELEVEL_8,
	GRADELEVEL_7,
	GRADELEVEL_6,
	GRADELEVEL_5,
	GRADELEVEL_4,
	GRADELEVEL_3,
	GRADELEVEL_2,
	GRADELEVEL_1,
	GRADELEVEL_S1,
	GRADELEVEL_S2,
	GRADELEVEL_S3,
	GRADELEVEL_S4,
	GRADELEVEL_S5,
	GRADELEVEL_S6,
	GRADELEVEL_S7,
	GRADELEVEL_S8,
	GRADELEVEL_S9,
	GRADELEVEL_M
};

void InitGrade(Player* player) {
	Grade* grade = &Grades[player->num];

	grade->currentGrade = F16(GRADELEVEL_9, 0u);
	grade->bestGrade = F16(GRADELEVEL_9, 0u);
	grade->decayFrames = 0u;

	uint32_t* sectionTime = grade->sectionTimes;
	Fixed16* currentGrade = grade->sectionLevelUps;
	for (size_t i = 0u; i < 10u; i++) {
		*currentGrade++ = F16(0u, 0u);
		*sectionTime++ = 0u;
	}
	grade->timeLastSection = 0u;
	grade->gradeLastSection = F16(GRADELEVEL_9, 0u);
}

void UpdateGrade(Player* player, uint8_t numLogicalLines) {
	assert(numLogicalLines <= 4u);
	Grade* grade = &Grades[player->num];

	if (numLogicalLines == 0u) {
		player->numComboClears = 0u;
		player->comboBefore = 0;
	}
	else {
		if (numLogicalLines == 4u) {
			if ((GameFlags & GAME_TWIN) && (player->modeFlags & (MODE_MASTER | MODE_TADEATH))) {
				UpdateSkillMedal(player);
			}
			uint8_t numSkillClears = player->miscFlags & MISC_NUMSKILLCLEARS;
			if (numSkillClears < 3) {
				numSkillClears++;
			}
			player->miscFlags |= numSkillClears;
		}

		GradeLevel level = grade->currentGrade.integer;
		if (level > GRADELEVEL_MAX) {
			level = GRADELEVEL_MAX;
		}

		// BUG: GradeProgressScales[player->numComboClears][numLogicalLines] can be an out-of-bounds access. This implements the behavior of the original code, in the case of out-of-bounds array access.
		uint32_t newProgress;
		// Well-defined case. Still bugged. This is what the original code was.
		if (player->numComboClears < lengthof(GradeProgressScales) && numLogicalLines < lengthof(*GradeProgressScales)) {
			newProgress = (uint32_t)GradeProgressions[level].progress[((int32_t)numLogicalLines - 1) % lengthoffield(GradeProgress, progress)] * (uint32_t)GradeProgressScales[player->numComboClears][numLogicalLines];
		}
		// One or both of the indices into GradeProgressScales is out-of-bounds, so calculate the correct index here. This case is for when the calculated index is still in the bounds of the entire GradeProgressScales array.
		else if (player->numComboClears * lengthof(*GradeProgressScales) + numLogicalLines < lengthof(GradeProgressScales) * lengthof(*GradeProgressScales)) {
			size_t i = player->numComboClears * lengthof(*GradeProgressScales) + numLogicalLines;
			newProgress = (uint32_t)GradeProgressions[level].progress[((int32_t)numLogicalLines - 1) % lengthoffield(GradeProgress, progress)] * (uint32_t)GradeProgressScales[i / lengthof(*GradeProgressScales)][i % lengthof(*GradeProgressScales)];
		}
		// Access is beyond the bounds of the GradeProgressScales array. The original code ends up accessing values in the GradeLevels array, due to that array being right after the GradeProgressScales array in memory.
		// Just for the sake of keeping this code relatively simple, but still having it behave as the original code would, only accesses within the GradeLevels array are assumed to ever occur.
		// But, if it's found the assertion here ever fails, then more data beyond GradeLevels will have to be used here.
		else {
			size_t i = player->numComboClears * lengthof(*GradeProgressScales) + numLogicalLines - lengthof(GradeProgressScales) * lengthof(*GradeProgressScales);
			assert(i < lengthof(GradeLevels));
			newProgress = GradeLevels[i];
		}

		if (newProgress % 10u != 0u) {
			newProgress += 10u;
		}
		newProgress /= 10u;
		newProgress *= (player->level / 250u) + 1u;
		newProgress += grade->currentGrade.fraction;
		if (newProgress >= 100u) {
			grade->currentGrade.integer++;
			grade->currentGrade.fraction = 0u;
			grade->decayFrames = 0u;
		}
		else {
			grade->currentGrade.fraction = newProgress;
		}

		if (player->combo > player->comboBefore && numLogicalLines > 1u) {
			player->numComboClears++;
			if ((GameFlags & GAME_TWIN) && (player->modeFlags & (MODE_MASTER | MODE_TADEATH))) {
				UpdateComboMedal(player);
			}
		}
		player->comboBefore = player->combo;
	}
}

void CheckDecayGrade(Player* player) {
	if (player->combo <= 1u) {
		Grade* grade = &Grades[player->num];
		GradeLevel level = grade->currentGrade.integer;

		if (level > GRADELEVEL_MAX) {
			level = GRADELEVEL_MAX;
		}

		if (grade->currentGrade.fraction != 0u && ++grade->decayFrames >= GradeProgressions[level].decayDuration) {
			grade->decayFrames = 0u;
			grade->currentGrade.fraction--;
		}
	}
}

void UpdatePlayerGrade(Player* player) {
	if (!(player->nowFlags & NOW_STAFF) && !(player->modeFlags & (MODE_TGMPLUS | MODE_TADEATH))) {
		while (Grades[player->num].currentGrade.integer > GradeLevels[player->grade] && player->grade < PLAYERGRADE_S9) {
			player->grade++;
			player->masteringTime = player->clearTime;
			if (!(player->modeFlags & (MODE_NORMAL | MODE_DOUBLES | MODE_VERSUS))) {
				PlaySoundEffect(SOUNDEFFECT_LEVELUP);
			}
		}
	}
}

// Doesn't update anything and returns MODE_MASTER if the mode isn't master nor
// T.A. death. Otherwise, updates, then returns false.
ModeFlag UpdateSectionGrade(Player* player) {
	// BUG: The original probably was intended to return bool, with a false return
	// intended if it updated, but it didn't have a return statement if it doesn't
	// update. So this returns what the SH-2 code erroneously returns.
	if (player->modeFlags == MODE_MASTER || player->modeFlags == MODE_TADEATH) {
		Grade* grade = &Grades[player->num];

		Fixed16 currentGrade = grade->currentGrade;
		if (currentGrade.value < grade->gradeLastSection.value) {
			grade->sectionLevelUps[player->section] = F16(0u, 0u);
		}
		else {
			if (currentGrade.fraction < grade->gradeLastSection.fraction) {
				currentGrade.integer--;
				currentGrade.fraction += 100u;
			}
			grade->sectionLevelUps[player->section].integer = currentGrade.integer - grade->gradeLastSection.integer;
			grade->sectionLevelUps[player->section].fraction = currentGrade.fraction - grade->gradeLastSection.fraction;
		}
		grade->gradeLastSection = grade->currentGrade;
		grade->sectionTimes[player->section] = player->clearTime - grade->timeLastSection;
		grade->timeLastSection = player->clearTime;

		uint8_t numSkillClears = player->miscFlags & MISC_NUMSKILLCLEARS;
		switch (player->section) {
		case 0u:
		case 1u:
		case 2u:
		case 3u:
		case 4u:
			if (numSkillClears < 2u) {
				player->mGradeFlags &= ~MGRADE_SKILLCLEARS;
			}
			break;
		case 5u:
		case 6u:
		case 7u:
		case 8u:
			if (numSkillClears < 1u) {
				player->mGradeFlags &= ~MGRADE_SKILLCLEARS;
			}
			break;
		default:
			break;
		}
		player->miscFlags &= ~MISC_NUMSKILLCLEARS;

		if (player->section == 0u) {
			if (player->clearTime <= TIME(1, 5, 0)) {
				player->mGradeFlags |= MGRADE_CHECKPOINT1;
			}
		}
		else if (player->section == 4u) {
			if (player->clearTime <= TIME(6, 0, 0)) {
				player->mGradeFlags |= MGRADE_CHECKPOINT2;
			}
			player->mGradeSectionTime = player->clearTime / 5u;
		}
		else if (player->section == 9u) {
			if (player->clearTime <= TIME(8, 45, 0)) {
				player->mGradeFlags |= MGRADE_CHECKPOINT3;
			}
			if (grade->sectionTimes[player->section] <= TIME(0, 45, 0)) {
				player->mGradeFlags |= MGRADE_SECTION9TIME;
			}
			if (player->grade >= PLAYERGRADE_S9) {
				player->mGradeFlags |= MGRADE_S9GRADE;
			}
			player->masteringTime = player->clearTime;
		}

		switch (player->section) {
		case 0u:
		case 1u:
		case 2u:
		case 3u:
		case 4u:
			if (grade->sectionTimes[player->section] > TIME(1, 5, 0)) {
				player->mGradeFlags &= ~MGRADE_SECTIONTIMES;
			}
			break;
		case 5u:
		case 6u:
		case 7u:
		case 8u:
		case 9u:
			if (grade->sectionTimes[player->section] > player->mGradeSectionTime + TIME(0, 2, 0)) {
				player->mGradeFlags &= ~MGRADE_SECTIONTIMES;
			}
			player->mGradeSectionTime = grade->sectionTimes[player->section];
			break;
		default:
			break;
		}
		return false;
	}
	return MODE_MASTER; // BUG
}

void CheckNextSection(Player* player) {
	if (!(player->nowFlags & NOW_STAFF) && player->section < 10u) {
		bool sectionUp = false;
		const uint16_t* nextSectionLevel = &NextSectionLevels[player->section];
		for (uint16_t section = player->section; section < 10u; section++, nextSectionLevel++) {
			if (player->level >= *nextSectionLevel) {
				UpdateSectionGrade(player);

				if (GameFlags & GAME_TWIN) {
					if (player->modeFlags & MODE_MASTER) {
						UpdateSectionTimeMedal(player, Grades[player->num].sectionTimes[player->section], BestMasterSectionTimes[player->section]);
					}
					if (player->modeFlags & MODE_TADEATH) {
						UpdateSectionTimeMedal(player, Grades[player->num].sectionTimes[player->section], BestTaDeathSectionTimes[player->section]);
					}
				}

				if (player->section < 9u) {
					if (player->modeFlags & MODE_TADEATH) {
						if (player->passedMGradeCheckpoint) {
							player->section++;
						}
						else if (player->section < 4u) {
							player->section++;
						}
					}
					else {
						player->section++;
					}
					CurrentGameBg.UNK_10 |= player->num == PLAYER1 ? (PLAYER1 + 1) << 1 : (PLAYER2 + 1) << 1;
					if (player->modeFlags & MODE_VERSUS) {
						if (player->section >= Settings[SETTING_MAXVERSUSSECTION] + 1u) {
							sectionUp = true;
						}
					}
					else if (player->modeFlags & (MODE_NORMAL | MODE_DOUBLES)) {
						if (player->section == 5u) {
							sectionUp = true;
						}
					}
					else {
						sectionUp = true;
					}
					if (sectionUp) {
						PlaySoundEffect(SOUNDEFFECT_SECTIONUP);
					}
				}
			}
		}
	}
}

uint32_t NumFieldBlocks(Player* player) {
	uint32_t numFieldBlocks = 0u;
	for (int16_t col = 0; col < MATRIX_SINGLEWIDTH - 1; col++) {
		for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
			if (MATRIX(player, row, col).block & BLOCK_TYPE) {
				numFieldBlocks++;
			}
		}
	}
	return numFieldBlocks;
}
