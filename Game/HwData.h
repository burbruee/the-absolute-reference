#pragma once

#include "PlatformTypes.h"
#include "HwInput.h"
#include "VideoDefs.h"
#include <stdint.h>

extern ROMDATA uint8_t ROM[0x100000];
extern RAMDATA uint8_t WORKRAM[0x100000];
extern RAMDATA uint8_t INPUTPORTS[NUMINPUTS];
extern RAMDATA uint8_t EEPROM[4];
extern RAMDATA uint8_t SOUNDCTRL[8];
extern RAMDATA uint8_t GRAPHICSRAM[0x10000];
#define SPRITERAM (&GRAPHICSRAM[0x00000])
#define BGRAM (&GRAPHICSRAM[0x004000])
#define MAXPALS 0x100
extern RAMDATA uint8_t PALRAM[0x400 * NUMPALCOLORS_4BPP * sizeof(Color)];
extern RAMDATA uint8_t SCALERAM[0x200];
extern RAMDATA uint8_t VIDEOCTRL;
extern RAMDATA uint8_t IRQCTRL[4];
extern RAMDATA uint8_t VIDEOREGS[0x20];
