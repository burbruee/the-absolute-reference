// TODO: Move the VIDEOREGS defines to more specific headers, like HwSprite.h.
#pragma once

#include "PlatformTypes.h"
#include "HwSprite.h"
#include "HwInput.h"
#include "VideoDefs.h"
#include "Macros.h"
#include <stdint.h>

// NOTE: Don't cast and access these with other types. TAP's SH-2 code accesses
// these using specifically sized types, and that might be required on
// PsikyoSH. The only exceptions to that rule would be accessing ROM and
// WORKRAM, which can be accessed mostly freely.

extern ROMDATA void* ROM[0x100000 / sizeof(void*)];

extern RAMDATA void* WORKRAM[0x100000 / sizeof(void*)];

extern RAMDATA uint8_t INPUTS[NUMINPUTS];

extern RAMDATA uint8_t EEPROM[4];

extern RAMDATA uint8_t SOUNDCTRL[8];

extern RAMDATA void* GRAPHICSRAM[0x10000 / sizeof(void*)];
#define SPRITERAM ((RAMDATA uint16_t*)&GRAPHICSRAM[0x00000 / sizeof(void*)])
#define BGRAM (&GRAPHICSRAM[0x004000 / sizeof(void*)])
#define BACKDROPRAM ((RAMDATA Color*)(&BGRAM[0x100 / sizeof(void*)]))
#define NUMBACKDROPLINES 0x100u

#define NUMPALS 0x400u
extern RAMDATA uint8_t PALRAM[NUMPALS * NUMPALCOLORS_4BPP * sizeof(Color)];
#define Palettes ((RAMDATA Color*)PALRAM)

extern RAMDATA uint16_t SCALERAM[0x100];

extern RAMDATA uint8_t VIDEOCTRL;

extern RAMDATA uint8_t IRQCTRL[4];

extern RAMDATA uint8_t VIDEOREGS[0x20];
#define AlphaValues ((RAMDATA uint8_t*)&VIDEOREGS[0x0])
#define NUMALPHAVALUES 8u
#define SpritePriority ((RAMDATA uint8_t*)&VIDEOREGS[0x08])
#define VideoSettings ((RAMDATA uint8_t*)&VIDEOREGS[0x0E])
#define BgMapBanks ((RAMDATA uint8_t*)&VIDEOREGS[0x18])
#define ScanlinesBank ((RAMDATA uint8_t*)&VIDEOREGS[0x1C])
#define BgMapSettings ((RAMDATA uint8_t*)&VIDEOREGS[0x1E])
