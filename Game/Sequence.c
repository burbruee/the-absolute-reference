#include "Sequence.h"

typedef struct SequenceData {
	uint32_t header;
	char name[16];
	DATA(data, 1);
} SequenceData;

typedef struct SequenceSystemGraphic {
	uint32_t header;
	char name[16];
	union {
		DATA(data, 1);
		SystemGraphicData graphicData;
	};
} SequenceSystemGraphic;
static ROMDATA SequenceSystemGraphic SystemGraphic;

// TODO: Define all the sequences and put pointers to their data here.
#define SEQUENCEDATA(sequenceData) ((SequenceData*)&(sequenceData).data)
ROMDATA void* SequenceDataTable[17] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SEQUENCEDATA(SystemGraphic),
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

ROMDATA SystemGraphicData* SystemGraphicDataPtr;