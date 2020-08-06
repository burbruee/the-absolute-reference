#include "Game/Sequence.h"
#include <stddef.h>

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

ROMDATA SequenceSystemGraphic SystemGraphic;
ROMDATA SystemGraphicData* SystemGraphicDataPtr;