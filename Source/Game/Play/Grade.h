#pragma once

#include "Game/Play/Player.h"
#include "Lib/Fixed.h"
#include "Input/InputTypes.h"

typedef enum GradeLevel {
	GRADELEVEL_9 = 0,
	GRADELEVEL_8 = 1,
	GRADELEVEL_7 = 2,
	GRADELEVEL_6 = 3,
	GRADELEVEL_5 = 4,
	GRADELEVEL_4 = 6,
	GRADELEVEL_3 = 8,
	GRADELEVEL_2 = 11,
	GRADELEVEL_1 = 14,
	GRADELEVEL_S1 = 17,
	GRADELEVEL_S2 = 18,
	GRADELEVEL_S3 = 19,
	GRADELEVEL_S4 = 22,
	GRADELEVEL_S5 = 24,
	GRADELEVEL_S6 = 26,
	GRADELEVEL_S7 = 28,
	GRADELEVEL_S8 = 30,
	GRADELEVEL_S9 = 32,
	GRADELEVEL_M = 35,
	GRADELEVEL_MAX = 51
} GradeLevel;
// Index with a player grade less than GM to get the GRS grade level.
extern const GradeLevel GradeLevels[NUMPLAYERGRADES - 1];

typedef struct Grade {
	Fixed16 currentGrade;
	Fixed16 bestGrade;
	Fixed16 sectionLevelUps[10];
	Fixed16 gradeLastSection;
	uint32_t sectionTimes[10];
	uint32_t timeLastSection;
	uint8_t decayFrames;
} Grade;
extern Grade Grades[NUMPLAYERS];

// Core GRS support.
void InitGrade(Player* player);
void UpdateGrade(Player* player, uint8_t numLogicalLines);
void CheckDecayGrade(Player* player); // Decay progress to the next grade while not in a combo.
void UpdatePlayerGrade(Player* player);
ModeFlag UpdateSectionGrade(Player* player);
void CheckNextSection(Player* player);

// Used to control awarding of recover medals. Hard-coded to be only for
// single-sized fields.
uint32_t NumFieldBlocks(Player* player);
