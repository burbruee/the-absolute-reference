#pragma once

#include "Lib/Macros.h"
#include "Video/Object.h"
#include "PlatformTypes.h"

extern ROMDATA void* SequenceDataTable[17];

typedef enum SequenceType {
	SEQUENCE_SYSTEMGRAPHIC = 7u
} SequenceType;

typedef struct SequenceData {
	uint32_t header;
	char name[16];
	DATA(data, 1);
} SequenceData;

typedef struct SystemGraphicData {
	ObjectData* objectTable;
	void* UNK_4[69];
	int16_t* UNK_118;
	uint8_t UNK_11C[92];
} SystemGraphicData;
typedef struct SequenceSystemGraphic {
	uint32_t header;
	char name[16];
	union {
		DATA(data, 1);
		SystemGraphicData graphicData;
	};
} SequenceSystemGraphic;
extern ROMDATA SequenceSystemGraphic SystemGraphic;
extern ROMDATA SystemGraphicData* SystemGraphicDataPtr;
