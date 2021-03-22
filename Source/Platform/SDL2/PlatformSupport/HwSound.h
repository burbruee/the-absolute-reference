#pragma once

#include "Sound/SoundDefs.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// The OPL4 runs at half the main oscillator frequency.
#define CLOCKHZ (57272700 / 2)

#define TIMER_NEVER (~UINT64_C(0))

typedef struct PcmTimerData {
	bool enabled;
	uint8_t count;
	uint64_t start;
	uint64_t next;
	uint64_t period;
	uint64_t expire;
} PcmTimerData;

typedef enum TimerNum {
	TIMER_A,
	TIMER_B,
	TIMER_BUSY,
	TIMER_LOADING,
	NUMTIMERS
} TimerNum;

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

typedef struct PcmChannelData {
	size_t num;

	uint16_t wave;
	int16_t frequency;
	int8_t octave;
	int8_t preverb;
	int8_t damping;
	int8_t dataOut;
	int8_t levelDirect;
	int8_t totalLevel;
	int8_t panpot;
	int8_t lfo;
	int8_t vibrato;
	int8_t tremolo;

	int8_t attackRate;
	int8_t decay1Rate;
	int8_t decayLevel;
	int8_t decay2Rate;
	int8_t rateCorrection;
	int8_t releaseRate;

	int64_t samplePos;
	int32_t sampleStride;

	bool playing;
	int8_t keyOn;
	WaveBitLength bits;

	uint32_t startAddress;
	uint32_t loopAddress;
	uint32_t endAddress;

	int32_t envelopeStride;
	uint32_t envelopeVolume;
	uint32_t envelopeVolumeStride;
	uint32_t envelopeVolumeLimit;
	int8_t envelopePreverb;
} PcmChannelData;

typedef struct SoundData {
	SoundExpansionFlag expansion;

	uint8_t waveTableHeader;
	bool memMode;
	bool memType;

	bool busy;
	bool loading;
	uint8_t currentIrq;
	bool irqLine;

	uint8_t fmLeft, fmRight;
	uint8_t pcmLeft, pcmRight;

	PcmTimerData timers[NUMTIMERS];
	uint64_t timerBase;
	int32_t hz;
	int32_t rate;

	uint8_t enable;

	uint8_t fmRegs[2][0x100];
	uint8_t nextFmReg;
	void (*writeFmPort)(const uint8_t i, const uint8_t value);
	uint8_t lastFmData;

	uint8_t pcmRegs[0x100];
	uint8_t nextPcmReg;

	PcmChannelData pcmChannels[SNDPCM_NUMCHANNELS];

	size_t address;
	uint8_t ram[0x400000];
} SoundData;

extern SoundData Sound;

extern const int32_t VolumeTable[1024];
extern const int32_t PanLeftTable[16];
extern const int32_t PanRightTable[16];
extern const int32_t MixLevelTable[8];

// The PcmChannel functions access the channels, so be sure to lock the channels mutex before calling them.

int32_t PcmChannelComputeRate(PcmChannelData* const channel, int32_t value);
uint32_t PcmChannelComputeDecayEnvelopeVolumeStep(PcmChannelData* const channel, int32_t value);
void PcmChannelComputeFrequencyStride(PcmChannelData* const channel);
void PcmChannelComputeEnvelope(PcmChannelData* const channel);

void SoundStart();

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
