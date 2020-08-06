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
	void* _0x4[69];
	int16_t* _0x118;
	uint8_t _0x11C[92];
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
