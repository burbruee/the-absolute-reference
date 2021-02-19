#include "Main/InitVideo.h"
#include "Video/Video.h"
#include "Video/SpriteInit.h"
#include "Video/InitScaleRam.h"
#include "Game/Sequence.h"
#include "HwData.h"

void InitVideo() {
	UNK_2405FFEA = 0x20u;
	UNK_2405FFEC = 0x0Au;
	UNK_2405FFED = 0x17u;
	VideoSetting[0] = 0x28u;
	VideoSetting[1] = 0xB8u;
	UNK_2405FFF0 = 0x00u;
	UNK_2405FFF1 = 0xAAu;
	GRAPHICSCHECK_SETBANK(0, 0x30);
	IRQCTRL_WRITE(0, 0);
	IRQCTRL_WRITE(0, IRQCTRL_READ(0) | 0x84);
	IRQCTRL_WRITE(1, IRQCTRL_READ(1) | 0x80);
	UNK_2405FFF4 = 0x83u;
	UNK_2405FFF5 = 0xFFu;
	UNK_2405FFF7 = 0x0Eu;
	BgMapSettings[0] = 0x00u;
	BgMapSettings[1] = 0x00u;
	SpritePriority[0] = 0x35u;
	SpritePriority[1] = 0x67u;
	UNK_2405FFEB = 0x87u;
	RastersBank = 0x08u;

	for (size_t i = 0u; i < NUMALPHAS; i++) {
		Alpha[i] = 0x0u;
	}

	for (size_t i = 0u; i < lengthof(SCALERAM); i++) {
		SCALERAM[i] = InitScaleRam[i];
	}

	for (size_t i = 0u; i < NUMPALS; i++) {
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
