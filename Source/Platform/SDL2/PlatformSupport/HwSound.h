#pragma once

#include "Sound/SoundDefs.h"
#include <stdint.h>

typedef enum WaveBitLength {
	WAVEBITLEN_8,
	WAVEBITLEN_12,
	WAVEBITLEN_16,
	WAVEBITLEN_PROHIBITED
} WaveBitLength;

typedef struct WaveTableHeader {
	WaveBitLength bitLength;

	uint32_t startAddress;
	uint16_t loopAddress;
	uint16_t endAddress;

	uint8_t lfo;
	uint8_t vib;
	uint8_t ar;
	uint8_t d1r;
	uint8_t dl;
	uint8_t d2r;
	uint8_t rateCorrection;
	uint8_t rr;
	uint8_t am;
} WaveTableHeader;

void SoundReset();

void SoundControlStartInit();
// Does the first part of sound hardware init. Call first, before any further init. Be sure to set the expansion FM register immediately after calling this.
#define SNDCTRL_STARTINIT() SoundControlStartInit()

#define SNDCTRL_STARTWAIT()

void SoundControlStatusWait(const SoundStatusFlag flags);
// Wait until the status flags are clear.
#define SNDCTRL_STATUSWAIT(flags) SoundControlStatusWait((SoundStatusFlag)(flags))

uint8_t SoundControlRead(const uint8_t i);
#define SNDCTRL_READ(i) SoundControlRead((uint8_t)(i))

void SoundControlWrite(const uint8_t i, const uint8_t value);
#define SNDCTRL_WRITE(i, value) SoundControlWrite((uint8_t)(i), (uint8_t)(value))
