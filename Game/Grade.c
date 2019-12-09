#include "Grade.h"
#include "Player.h"
#include "SoundEffect.h"
#include "Setting.h"
#include "Save.h"

GradeData Grades[NUMPLAYERS];

typedef struct GradeProgressData {
	uint8_t progress[4];
	uint8_t decayDuration;
} GradeProgressData;
// BUG: TAP might index this table with GRADELEVEL_MAX, rather than
// GRADELEVEL_MAX - 1, accessing data beyond the bounds of the intended array.
// The data that it accesses in that case has been added to the intended array,
// for accurate behavior, and to prevent memory access errors.
static GradeProgressData GradeProgressions[GRADELEVEL_MAX + 1] = {
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
	GradeData* grade = &Grades[player->num];

	grade->gradeLevel = F16(GRADELEVEL_9, 0u);
	grade->bestGradeLevel = F16(GRADELEVEL_9, 0u);
	grade->decayFrames = 0u;

	uint32_t* sectionTime = grade->sectionTimes;
	Fixed16* gradeLevel = grade->sectionGradeLevels;
	for (size_t i = 0u; i < 10u; i++) {
		*gradeLevel++ = F16(GRADELEVEL_9, 0u);
		*sectionTime++ = 0u;
	}
	grade->timeLastSection = 0u;
	grade->gradeLevelLastSection = F16(GRADELEVEL_9, 0u);
}

void UpdateGrade(Player* player, uint8_t numLogicalLines) {
	GradeData* grade = &Grades[player->num];

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

		GradeLevel level = F16I(grade->gradeLevel);
		if (level > GRADELEVEL_MAX) {
			level = GRADELEVEL_MAX;
		}
		uint32_t newProgress = (uint32_t)GradeProgressions[level].progress[(numLogicalLines - 1) % 4] * (uint32_t)GradeProgressScales[player->numComboClears][numLogicalLines];
		if (newProgress % 10u != 0u) {
			newProgress += 10u;
		}
		newProgress /= 10u;
		newProgress *= (player->level / 250u) + 1u;
		uint8_t progress = F16F(grade->gradeLevel) + newProgress;
		if (progress >= 100u) {
			F16I(grade->gradeLevel)++;
			F16F(grade->gradeLevel) = 0u;
			grade->decayFrames = 0u;
		}
		else {
			F16F(grade->gradeLevel) = progress;
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
		GradeData* grade = &Grades[player->num];
		GradeLevel level = F16I(grade->gradeLevel);

		if (level > GRADELEVEL_MAX) {
			level = GRADELEVEL_MAX;
		}

		if (F16F(grade->gradeLevel) != 0u && ++grade->decayFrames >= GradeProgressions[level].decayDuration) {
			grade->decayFrames = 0u;
			F16F(grade->gradeLevel)--;
		}
	}
}

void UpdatePlayerGrade(Player* player) {
	if (!(player->nowFlags & NOW_STAFF) && !(player->modeFlags & (MODE_TGMPLUS | MODE_TADEATH))) {
		while (F16I(Grades[player->num].gradeLevel) > GradeLevels[player->grade] && player->grade < PLAYERGRADE_S9) {
			player->grade++;
			player->masteringTime = player->clearTime;
			if (!(player->modeFlags & (MODE_NORMAL | MODE_DOUBLES | MODE_VERSUS))) {
				PlaySoundEffect(SOUNDEFFECT_LEVELUP);
			}
		}
	}
}

// Doesn't update anything and returns MODE_MASTER if the mode isn't master nor
// T.A. death. Otherwise, updates, then returns MODE_NONE.
ModeFlag UpdateSectionGrade(Player* player) {
	// BUG: The original probably was intended to return bool, with a false return
	// intended if it updated, but it didn't have a return statement if it doesn't
	// update. So this returns what the SH-2 code erroneously returns.
	if (player->modeFlags == MODE_MASTER || player->modeFlags == MODE_TADEATH) {
		GradeData* grade = &Grades[player->num];

		Fixed16 gradeLevel = grade->gradeLevel;
		if (gradeLevel < grade->gradeLevelLastSection) {
			grade->sectionGradeLevels[player->section] = F16(GRADELEVEL_9, 0u); // TODO: If renaming what is being set here, change the value being assigned to F16(0, 0u).
		}
		else {
			if (F16F(gradeLevel) < F16F(grade->gradeLevelLastSection)) {
				F16I(gradeLevel)--;
				F16F(gradeLevel) += 100u;
			}
			F16I(grade->sectionGradeLevels[player->section]) = F16I(gradeLevel) - F16I(grade->gradeLevelLastSection);
			F16F(grade->sectionGradeLevels[player->section]) = F16F(gradeLevel) - F16F(grade->gradeLevelLastSection);
		}
		grade->gradeLevelLastSection = grade->gradeLevel;

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
		uint16_t* maxVersusSection = &Settings[SETTING_MAXVERSUSSECTION];
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
					_0x6066188._0x10 |= player->num == PLAYER1 ? 0x02 : 0x04;
					if (player->modeFlags & MODE_VERSUS) {
						if (player->section >= *maxVersusSection + 1u) {
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

uint32_t NumFieldSquares(Player* player) {
	uint32_t numFieldSquares = 0u;
	for (int16_t col = 0; col < MATRIX_SINGLEWIDTH - 1; col++) {
		for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
			if (MATRIX(player, row, col).block & BLOCK_TYPE) {
				numFieldSquares++;
			}
		}
	}
	return numFieldSquares;
}
