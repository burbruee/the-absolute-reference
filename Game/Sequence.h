#pragma once

#include "Object.h"
#include "PlatformTypes.h"
#include "Macros.h"

extern ROMDATA void* const SequenceDataTable[17];

typedef struct SequenceData {
	uint32_t header;
	char name[16];
	DATA(data, 1);
} SequenceData;

#define SEQUENCE_SYSTEMGRAPHIC 7u
typedef struct SequenceSystemGraphic {
	uint32_t header;
	char name[16];
	union {
		DATA(data, 1);
		SystemGraphicData graphicData;
	};
} SequenceSystemGraphic;
extern ROMDATA SequenceSystemGraphic SystemGraphic;
typedef struct SystemGraphicData {
	ObjectData* objectTable;
	int16_t* _0x118;
} SystemGraphicData;
extern ROMDATA SystemGraphicData* SystemGraphicDataPtr;
