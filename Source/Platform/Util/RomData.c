#include "Video/Object.h"
#include "Video/Pal.h"
#include "Video/BgMap.h"
#include "Main/DemoReplayInput.h"
#include "PlatformTypes.h"

ROMDATA ObjectDataTable Objects;

ROMDATA PalData Pal;

#define HEADER16 { \
	0xA0000000u | 0x10000u, \
	0x78u, \
	0x50u \
}

#define HEADER32 { \
	0xA0000000u | BGMAPTILEINFO_PERTILEPAL | 0x10000u, \
	0x78u, \
	0x50u \
}

// TODO: Actually initialize these from the ROMs. For now, the headers are initialized correctly so the code can at least run.
ROMDATA BgMap16 BgMapSection0 = { HEADER16, {} };
ROMDATA BgMap16 BgMapSection1 = { HEADER16, {} };
ROMDATA BgMap32 BgMapSection2 = { HEADER32, {} };
ROMDATA BgMap16 BgMapSection3 = { HEADER16, {} };
ROMDATA BgMap32 BgMapSection4 = { HEADER32, {} };
ROMDATA BgMap16 BgMapSection5 = { HEADER16, {} };
ROMDATA BgMap16 BgMapSection6 = { HEADER16, {} };
ROMDATA BgMap16 BgMapSection7 = { HEADER16, {} };
ROMDATA BgMap16 BgMapSection8 = { HEADER16, {} };
ROMDATA BgMap16 BgMapSection9 = { HEADER16, {} };
ROMDATA BgMap16 BgMapVersus = { HEADER16, {} };

ROMDATA ButtonInput DemoReplayInputTwin[DEMOREPLAY_LENGTH * NUMPLAYERS];
ROMDATA ButtonInput DemoReplayInputDoubles[DEMOREPLAY_LENGTH * NUMPLAYERS];
ROMDATA ButtonInput DemoReplayInputVersus[DEMOREPLAY_LENGTH * NUMPLAYERS];
