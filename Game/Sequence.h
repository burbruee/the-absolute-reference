#pragma once

#include "Object.h"
#include "PlatformTypes.h"
#include "Macros.h"

extern ROMDATA void* SequenceDataTable[17];

#define SEQUENCE_SYSTEMGRAPHIC 7u
typedef struct SystemGraphicData {
	ObjectData* objectTable;
	int16_t* _0x118;
} SystemGraphicData;
extern ROMDATA SystemGraphicData* SystemGraphicDataPtr;
