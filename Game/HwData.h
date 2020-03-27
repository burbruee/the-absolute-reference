// TODO: Move the VIDEOREGS defines to more specific headers, like HwSprite.h.
#pragma once

#include "PlatformTypes.h"
#include "HwInput.h"
#include "VideoDefs.h"
#include <stdint.h>

// NOTE: Don't cast and access these with other types. TAP's SH-2 code accesses
// these using specifically sized types, and that might be required on
// PsikyoSH.

extern ROMDATA uint8_t ROM[0x100000];

extern RAMDATA uint8_t WORKRAM[0x100000];

extern RAMDATA uint8_t INPUTS[NUMINPUTS];

extern RAMDATA uint8_t EEPROM[4];

extern RAMDATA uint8_t SOUNDCTRL[8];

extern RAMDATA uint8_t GRAPHICSRAM[0x10000];
#define SPRITERAM (&GRAPHICSRAM[0x00000])
#define BGRAM (&GRAPHICSRAM[0x004000])

#define NUMPALS 0x400
extern RAMDATA uint8_t PALRAM[NUMPALS * NUMPALCOLORS_4BPP * sizeof(Color)];
#define Palettes (&PALRAM[0])

extern RAMDATA uint16_t SCALERAM[0x100];

extern RAMDATA uint8_t VIDEOCTRL;

extern RAMDATA uint8_t IRQCTRL[4];

extern RAMDATA uint8_t VIDEOREGS[0x20];
#define AlphaValues ((uint8_t*)&VIDEOREGS[0x0])
#define NUMALPHAVALUES 8
#define SpritePriority ((uint8_t*)&VIDEOREGS[0x8])
#define VideoSettings ((uint8_t*)&VIDEOREGS[0xE])
#define TilemapBanks ((uint8_t*)&VIDEOREGS[0x18])
#define ScanlinesBank ((uint8_t*)&VIDEOREGS[0x1C])
#define TilemapSettings ((uint8_t*)&VIDEOREGS[0x1E])
