#include "Video/BgMap.h"

ROMDATA BgMap* BgMapTable[11] = {
	(ROMDATA BgMap*)&BgMapSection2,
	(ROMDATA BgMap*)&BgMapSection5,
	(ROMDATA BgMap*)&BgMapSection8,
	(ROMDATA BgMap*)&BgMapSection3,
	(ROMDATA BgMap*)&BgMapSection1,
	(ROMDATA BgMap*)&BgMapSection0,
	(ROMDATA BgMap*)&BgMapSection4,
	(ROMDATA BgMap*)&BgMapSection6,
	(ROMDATA BgMap*)&BgMapSection7,
	(ROMDATA BgMap*)&BgMapSection9,
	(ROMDATA BgMap*)&BgMapVersus
};

ROMDATA BgMap16 BgMapSection0;
ROMDATA BgMap16 BgMapSection1;
ROMDATA BgMap32 BgMapSection2;
ROMDATA BgMap16 BgMapSection3;
ROMDATA BgMap32 BgMapSection4;
ROMDATA BgMap16 BgMapSection5;
ROMDATA BgMap16 BgMapSection6;
ROMDATA BgMap16 BgMapSection7;
ROMDATA BgMap16 BgMapSection8;
ROMDATA BgMap16 BgMapSection9;
ROMDATA BgMap16 BgMapVersus;
