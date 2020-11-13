#pragma once

#include "PlatformTypes.h"
#include "Video/VideoDefs.h"
#include "Input/InputTypes.h"
#include <stdint.h>

// NOTE: Don't cast and access these with other types. TAP's SH-2 code accesses
// these using specifically sized types, and that might be required on
// PsikyoSH. The only exceptions to that rule would be accessing ROM and
// WORKRAM, which can be accessed mostly freely.

extern RAMDATA void* WORKRAM[0x100000 / sizeof(void*)];

extern RAMDATA uint8_t INPUTS[NUMINPUTS];

extern RAMDATA uint8_t EEPROM[4];

uint8_t EepromRead(size_t i);
#define EEPROM_READ(i) EepromRead((i))
void EepromWrite(size_t i, uint8_t value);
#define EEPROM_WRITE(i, value) EepromWrite((i), (value))

extern RAMDATA uint8_t SOUNDCTRL[8];

uint8_t SoundControlRead(size_t i);
#define SOUNDCTRL_READ(i) SoundControlRead((i))
void SoundControlWrite(size_t i, uint8_t value);
#define SOUNDCTRL_WRITE(i, value) SoundControlWrite((i), (value))

extern RAMDATA uint32_t GRAPHICSRAM[0x10000 / sizeof(uint32_t)];

#define SPRITERAM ((RAMDATA uint16_t*)&GRAPHICSRAM[0x00000 / sizeof(uint32_t)])
#define BGRAM (&GRAPHICSRAM[0x004000 / sizeof(uint32_t)])
#define BACKDROPRAM ((RAMDATA Color*)(&BGRAM[0x100 / sizeof(uint32_t)]))
#define NUMBACKDROPLINES 0x100u

#define NUMPALS 0x100u
extern RAMDATA Color PALRAM[NUMPALS * NUMPALCOLORS_4BPP];

extern RAMDATA uint16_t SCALERAM[0x100];

extern RAMDATA uint8_t VIDEOCTRL;

uint8_t VideoControlRead(size_t i);
#define VIDEOCTRL_READ(i) VideoControlRead((i))
void VideoControlWrite(size_t i, uint8_t value);
#define VIDEOCTRL_WRITE(i, value) VideoControlWrite((i), (value))

extern RAMDATA uint8_t IRQCTRL[4];

uint8_t InterruptControlRead(size_t i);
#define IRQCTRL_READ(i) InterruptControlRead((i))
void InterruptControlWrite(size_t i, uint8_t value);
#define IRQCTRL_WRITE(i, value) InterruptControlWrite((i), (value))

extern RAMDATA uint8_t VIDEOREGS[0x20];
#define NUMALPHAS 8u
#define Alpha (&VIDEOREGS[0x0])
#define SpritePriority (&VIDEOREGS[0x08])
#define UNK_2405FFEA (VIDEOREGS[0x0A])
#define UNK_2405FFEB (VIDEOREGS[0x0B])
#define UNK_2405FFEC (VIDEOREGS[0x0C])
#define UNK_2405FFED (VIDEOREGS[0x0D])
#define VideoSetting (&VIDEOREGS[0x0E])
#define UNK_2405FFF0 (VIDEOREGS[0x10])
#define UNK_2405FFF1 (VIDEOREGS[0x11])
void GraphicsCheckSetBank(size_t i, uint8_t value);
#define GRAPHICSCHECK_SETBANK(i, value) GraphicsCheckSetBank((i), (value))
#define UNK_2405FFF4 (VIDEOREGS[0x14])
#define UNK_2405FFF5 (VIDEOREGS[0x15])
#define UNK_2405FFF6 (VIDEOREGS[0x16])
#define UNK_2405FFF7 (VIDEOREGS[0x17])
#define BgMapBank (&VIDEOREGS[0x18])
#define ScanlinesBank (&VIDEOREGS[0x1C])
#define BgMapSetting (&VIDEOREGS[0x1E])

// TODO: Implement updating of this. It's used in checksum calculations. Will
// probably have to change it to a macro that does something behind the scenes,
// since it's controlled by a video register.
extern RAMDATA uint32_t GRAPHICSCHECK[0x20000 / sizeof(uint32_t)];

uint32_t GraphicsCheckRead(size_t i);
#define GRAPHICSCHECK_READ(i) GraphicsCheckRead(size_t i);
void GraphicsCheckWrite(size_t i, uint32_t value);
#define GRAPHICSCHECK_WRITE(i, value) GraphicsCheckWrite((i), (value));

extern RAMDATA uint8_t UNK_FFFFFE10;
extern RAMDATA uint8_t UNK_FFFFFE11;
extern RAMDATA uint8_t UNK_FFFFFE12;
extern RAMDATA uint8_t UNK_FFFFFE12;
extern RAMDATA uint8_t UNK_FFFFFE13;
extern RAMDATA uint8_t UNK_FFFFFE14;
extern RAMDATA uint8_t UNK_FFFFFE15;
extern RAMDATA uint8_t UNK_FFFFFE16;

extern RAMDATA uint16_t UNK_FFFFFE60;
extern RAMDATA uint16_t UNK_FFFFFE66;

extern RAMDATA uint8_t UNK_FFFFFE92;

extern RAMDATA uint32_t UNK_FFFFFFE0;
extern RAMDATA uint32_t UNK_FFFFFFE4;
extern RAMDATA uint32_t UNK_FFFFFFE8;
extern RAMDATA uint32_t UNK_FFFFFFEC;
extern RAMDATA uint32_t UNK_FFFFFFF0;
extern RAMDATA uint32_t UNK_FFFFFFF8;
