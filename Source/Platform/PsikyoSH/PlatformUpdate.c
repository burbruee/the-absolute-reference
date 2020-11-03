#include "PlatformUpdate.h"
#include "Video/InitScaleRam.h"
#include "HwData.h"
#include "Lib/Macros.h"
#include <stddef.h>

// These messages are just the bare minimum to get across what the original
// messages said. The original messages aren't copied here, as the rights
// holder of them might claim they're copyrighted. They're still all arrays of
// strings, though, matching the format of the original messages. The original
// messages just warned that the game should only be used in the region set in
// EEP-ROM.
//
// I assume these are just standard messages provided by Psikyo for all games
// on the PsikyoSH boards, and their presence doesn't necessarily imply Arika
// planned to release TAP in non-Japan regions. -Brandon McGriff
const char* RegionMessageJapan[1] = {
	"Japan region."
};
const char* RegionMessageUsaCanada[1] = {
	"USA and Canada region."
};
const char* RegionMessageKorea[1] = {
	"Korea region."
};
const char* RegionMessageHongKong[1] = {
	"Hong Kong region."
};
const char* RegionMessageTaiwan[1] = {
	"Taiwan region."
};

void InitVideo() {
	UNK_2405FFEA = 0x20u;
	UNK_2405FFEC = 0x0Au;
	UNK_2405FFED = 0x17u;
	VideoSettings[0] = 0x28u;
	VideoSettings[1] = 0xB8u;
	UNK_2405FFF0 = 0x00u;
	UNK_2405FFF1 = 0xAAu;
	VideoGraphicsCheckBank[0] = 0x30u;
	IRQCTRL[0] = 0u;
	IRQCTRL[0] |= 0x84u;
	IRQCTRL[1] |= 0x80u;
	UNK_2405FFF4 = 0x83u;
	UNK_2405FFF5 = 0xFFu;
	UNK_2405FFF7 = 0x0Eu;
	BgMapSettings[0] = 0x00u;
	BgMapSettings[1] = 0x00u;
	SpritePriority[0] = 0x35u;
	SpritePriority[1] = 0x67u;
	UNK_2405FFEB = 0x87u;
	ScanlinesBank[0] = 0x08u;

	for (size_t i = 0u; i < NUMALPHAS; i++) {
		AlphaValues[i] = 0x0u;
	}

	for (size_t i = 0u; i < lengthof(SCALERAM); i++) {
		SCALERAM[i] = InitScaleRam[i];
	}

	for (size_t i = 0u; i < 0x100u; i++) {
		for (size_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
			PALRAM[i * NUMPALCOLORS_4BPP + j] = 0xFFFFFFFFu;
		}
	}

	AddSprite(0u, NULL);
	SpriteNames[0] = 0u;
	SpriteNames[1] = SPRITENAME_TERMINATE;
}

void SetSystemGraphicDataPtr() {
	SystemGraphicDataPtr = (SystemGraphicData*)SequenceDataTablePtr[7];
}

// NOTE: Unused.
void UNK_6000AEC() {}
