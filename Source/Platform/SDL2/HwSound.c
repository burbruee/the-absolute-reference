#include "HwSound.h"
#include "HwData.h"
#include "Sound/Sound.h"
#include "Lib/Fixed.h"
#include "Lib/Macros.h"
#include "SDL.h"
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>

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

	Fixed32 sampleStride;
	Fixed64 samplePos;

	bool playing;
	int8_t keyOn;
	int8_t bits;

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

	Fixed32 volumes[0x400];
	int32_t panLeft[16], panRight[16];
	Fixed32 mixLevels[8];

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
	uint8_t rom[0x400000];
	uint8_t ram[0x400000];
} SoundData;

static SoundData Sound;

void PcmTimerAdjust(PcmTimerData* const timer, const uint64_t startDelay, const uint64_t next, const uint64_t period) {
	timer->next = next;
	timer->enabled = true;
	timer->start = SDL_GetPerformanceCounter();
	timer->expire = timer->start + startDelay;
	timer->period = period;
}

void PcmTimerReset(PcmTimerData* const timer, const uint64_t startDelay) {
	PcmTimerAdjust(timer, startDelay, timer->next, timer->period);
}

static uint64_t HzToTime(int32_t hz) {
	return (uint64_t)((1.0 / hz) * SDL_GetPerformanceFrequency());
}

void PcmTimerBusyStart(bool pcm) {
	Sound.busy = true;
	PcmTimerAdjust(&Sound.timers[TIMER_BUSY], HzToTime(Sound.hz / (pcm ? 88 : 56)), 0u, TIMER_NEVER);
}

uint64_t PcmTimerRemaining(PcmTimerData* const timer) {
	uint64_t time = SDL_GetPerformanceCounter();
	if (time >= timer->expire) {
		return 0u;
	}
	else {
		return timer->expire - time;
	}
}

static const uint32_t DecayRateTable[64] = {
	0x00000000u,
	0x00000000u,
	0x00000000u,
	0x00000000u,
	0x003C0000u,
	0x00300000u,
	0x00280000u,
	0x00224924u,
	0x001E0000u,
	0x00180000u,
	0x00140000u,
	0x00112492u,
	0x000F0000u,
	0x000C0000u,
	0x000A0000u,
	0x00089249u,
	0x00078000u,
	0x00060000u,
	0x00050000u,
	0x00044924u,
	0x0003C000u,
	0x00030000u,
	0x00028000u,
	0x00022492u,
	0x0001E000u,
	0x00018000u,
	0x00014000u,
	0x00011249u,
	0x0000F000u,
	0x0000C000u,
	0x0000A000u,
	0x00008924u,
	0x00007800u,
	0x00006000u,
	0x00005000u,
	0x00004492u,
	0x00003C00u,
	0x00003000u,
	0x00002800u,
	0x00002249u,
	0x00001E00u,
	0x00001800u,
	0x00001400u,
	0x00001124u,
	0x00000F00u,
	0x00000C00u,
	0x00000A00u,
	0x00000892u,
	0x00000780u,
	0x00000600u,
	0x00000500u,
	0x00000449u,
	0x000003C0u,
	0x00000300u,
	0x00000280u,
	0x00000224u,
	0x000001E0u,
	0x00000180u,
	0x00000140u,
	0x00000112u,
	0x000000F0u,
	0x000000F0u,
	0x000000F0u,
	0x000000F0u
};

static const uint32_t AttackRateTable[64] = {
	0x00000000u,
	0x00000000u,
	0x00000000u,
	0x00000000u,
	0x00043000u,
	0x00035999u,
	0x0002CAAAu,
	0x00026492u,
	0x00021800u,
	0x0001ACCCu,
	0x00016555u,
	0x00013249u,
	0x00010C00u,
	0x0000D666u,
	0x0000B2AAu,
	0x00009924u,
	0x00008600u,
	0x00006B33u,
	0x00005955u,
	0x00004C92u,
	0x00004300u,
	0x00003599u,
	0x00002CAAu,
	0x00002649u,
	0x00002180u,
	0x00001ACCu,
	0x00001655u,
	0x00001324u,
	0x000010C0u,
	0x00000D66u,
	0x00000B2Au,
	0x00000992u,
	0x00000860u,
	0x000006B3u,
	0x00000595u,
	0x000004C9u,
	0x00000430u,
	0x00000359u,
	0x000002CAu,
	0x00000264u,
	0x00000218u,
	0x000001ACu,
	0x00000165u,
	0x00000132u,
	0x0000010Cu,
	0x000000D6u,
	0x000000B2u,
	0x00000099u,
	0x00000086u,
	0x0000006Bu,
	0x00000059u,
	0x0000004Cu,
	0x00000043u,
	0x00000035u,
	0x0000002Cu,
	0x00000026u,
	0x00000021u,
	0x0000001Au,
	0x00000016u,
	0x00000013u,
	0x00000011u,
	0x00000011u,
	0x00000011u,
	0x00000000u
};

static void IrqCheck() {
	Sound.irqLine = !!Sound.currentIrq;
}

static int32_t PcmChannelComputeRate(PcmChannelData* const channel, int32_t value) {
	int32_t result, octave;

	if (value == 0) {
		return 0;
	}
	else if (value == 15) {
		return 63;
	}
	else if (channel->rateCorrection != 15) {
		octave = channel->octave;
		if (octave & 8) {
			octave |= -8;
		}
		result = (octave + channel->rateCorrection) * 2 + (channel->frequency & 0x200 ? 1 : 0) + value * 4;
	}
	else {
		result = value * 4;
	}

	if (result < 0) {
		return 0;
	}
	else if (result > 63) {
		return 63;
	}
	else {
		return result;
	}
}

static uint32_t PcmChannelComputeDecayEnvelopeVolumeStep(PcmChannelData* const channel, int32_t value) {
	int32_t rate;

	if (channel->damping) {
		rate = 56;
	}
	else if (channel->preverb && channel->envelopeVolume > ((6 * 8) << 23)) {
		channel->envelopePreverb = 1;
		rate = 5;
	}
	else {
		rate = PcmChannelComputeRate(channel, value);
	}

	if (rate < 4) {
		return 0u;
	}
	else {
		return (UINT32_C(256) << 23) / DecayRateTable[rate];
	}
}

void PcmChannelComputeFrequencyStride(PcmChannelData* const channel) {
	int32_t octave = channel->octave;
	if (octave & 8) {
		octave |= -8;
	}

	channel->sampleStride.value = ((channel->frequency | 0x400) << (octave + 8)) >> 3;
}

void PcmChannelComputeEnvelope(PcmChannelData* const channel) {
	switch (channel->envelopeStride) {
	case 0: {
		const int32_t rate = PcmChannelComputeRate(channel, channel->attackRate);
		channel->envelopeVolume = UINT32_C(0x100) << 23;
		channel->envelopeVolumeLimit = (UINT32_C(0x100) << 23) - 1;

		if (rate == 63) {
			channel->envelopeVolume = 0u;
			channel->envelopeStride++;
			PcmChannelComputeEnvelope(channel);
		}
		else if (rate < 4) {
			channel->envelopeVolumeStride = 0u;
		}
		else {
			channel->envelopeVolumeStride = ~((UINT32_C(0x100) << 23) / AttackRateTable[rate]);
		}
		break;
	}

	case 1:
		if (channel->decayLevel) {
			channel->envelopeVolumeStride = PcmChannelComputeDecayEnvelopeVolumeStep(channel, channel->decay1Rate);
			channel->envelopeVolumeLimit = (channel->decayLevel * 8) << 23;
		}
		else {
			channel->envelopeStride++;
			PcmChannelComputeEnvelope(channel);
		}
		break;

	case 2:
		channel->envelopeVolumeStride = PcmChannelComputeDecayEnvelopeVolumeStep(channel, channel->decay2Rate);
		channel->envelopeVolumeLimit = UINT32_C(0x100) << 23;
		break;

	case 3:
		channel->envelopeVolume = UINT32_C(0x100) << 23;
		channel->envelopeVolumeStride = 0u;
		channel->envelopeVolumeLimit = 0u;
		channel->playing = false;
		break;

	case 4:
		channel->envelopeVolumeStride = PcmChannelComputeDecayEnvelopeVolumeStep(channel, channel->releaseRate);
		channel->envelopeVolumeLimit = UINT32_C(0x100) << 23;
		break;

	case 5:
		channel->envelopeVolume = UINT32_C(0x100) << 23;
		channel->envelopeVolumeStride = 0u;
		channel->envelopeVolumeLimit = 0u;
		channel->playing = false;
		break;

	default:
		break;
	}
}

static void PcmChannelRetriggerNote(PcmChannelData* const channel) {
	if (channel->octave != 8) {
		channel->playing = true;
	}

	channel->samplePos = F64(0, 0x00000000u);
	channel->envelopeStride = 0;
	channel->envelopePreverb = 0;

	PcmChannelComputeFrequencyStride(channel);
	PcmChannelComputeEnvelope(channel);
}

static void SoundRegisterFmWritePortA(const uint8_t i, const uint8_t value) {
	switch (i) {
	case SNDREGFM_LSITEST1:
	case SNDREGFM_LSITEST2:
		break;

	case SNDREGFM_TIMER1:
	case SNDREGFM_TIMER2: {
		PcmTimerData* const timer = &Sound.timers[i - SNDREGFM_TIMER1];
		if (value != timer->count) {
			timer->count = value;
			if (Sound.enable & (i - 1)) {
				PcmTimerAdjust(timer, PcmTimerRemaining(timer), 0u, Sound.timerBase * (0x100 - value) * 4 * (i - 1));
			}
		}
		break;
	}

	case SNDREGFM_TIMERCTRL:
		if (value & 0x80u) {
			Sound.currentIrq = 0u;
		}
		else {
			for (size_t j = 0u; j < 2u; j++) {
				PcmTimerData* const timer = &Sound.timers[j];
				if ((Sound.enable ^ value) & (1 << j)) {
					const uint64_t period = (value & 1u) ? Sound.timerBase * (0x100 - timer->count) * 4 * (j + 1) : TIMER_NEVER;
					PcmTimerAdjust(timer, period, 0u, period);
				}
			}

			Sound.enable = value;
			Sound.currentIrq &= ~value;
		}
		IrqCheck();

	default:
		fprintf(stderr, "Unsupported FM port A sound register write of register %"PRIu8" with data 0x%02"PRIX8".\n", i, value);
		break;
	}
}

static void SoundRegisterFmWritePortB(const uint8_t i, const uint8_t value) {
	switch (i) {
	case SNDREGFM_LSITEST1:
	case SNDREGFM_LSITEST2:
		break;

	case SNDREGFM_EXPANSION:
		Sound.expansion = value;
		break;

	default:
		fprintf(stderr, "Unsupported FM port B sound register write of register %"PRIu8" with data 0x%02"PRIX8".\n", i, value);
		break;
	}
}

static void SoundRegisterPcmWrite(const uint8_t i, const uint8_t value) {
	uint8_t nextValue = value;
	if (i >= SNDREGPCM_CHANNELREGS1 && i < SNDREGPCM_CHANNELREGS10 + SNDPCM_NUMCHANNELS) {
		const size_t channelNum = (i - SNDREGPCM_CHANNELREGS1) % SNDPCM_NUMCHANNELS;
		PcmChannelData* const channel = &Sound.pcmChannels[channelNum];

		switch ((i - SNDREGPCM_CHANNELREGS1) / SNDPCM_NUMCHANNELS) {
		case 0u: {
			channel->wave = (channel->wave & 0x100u) | value;
			size_t offset;
			if (channel->wave < 0x180u || !Sound.waveTableHeader) {
				offset = (size_t)channel->wave * 12u;
			}
			else {
				offset = ((size_t)Sound.waveTableHeader << 19) + ((size_t)channel->wave - 0x180u) * 12u;
			}
			assert(offset < lengthoffield(SoundData, rom));

			const uint8_t* const data = Sound.rom + offset;
			channel->bits = (data[0] & 0xC0u) >> 6;
			channel->startAddress = (((uint32_t)data[0] & 0x3Fu) << 16) | ((uint32_t)data[1] << 8) | data[2];
			channel->loopAddress = ((uint32_t)data[3] << 24) | ((uint32_t)data[4] << 16);
			channel->endAddress = ((((uint32_t)data[5] << 24) | ((uint32_t)data[6] << 16)) - 0x10000u) ^ 0xFFFF0000u;

			for (size_t j = 7u; j < 12u; j++) {
				SoundRegisterPcmWrite(i, (uint8_t)(SNDREGPCM_CHANNELREGS1 + channelNum + (j - 2u) * 24u, data[j]));
			}

			Sound.loading = true;
			PcmTimerAdjust(&Sound.timers[TIMER_LOADING], HzToTime(10), 0u, TIMER_NEVER);

			if (channel->keyOn) {
				PcmChannelRetriggerNote(channel);
			}
			else if (channel->playing) {
				channel->envelopeStride = 5;
				PcmChannelComputeEnvelope(channel);
			}
			break;
		}

		case 1u:
			channel->wave = ((uint16_t)(value & 1u) << 8) | (channel->wave & 0xFFu);
			channel->frequency = (channel->frequency & 0x380) | (value >> 1);
			if (channel->playing && ((value ^ Sound.pcmRegs[i]) & 0xFEu)) {
				PcmChannelComputeFrequencyStride(channel);
				PcmChannelComputeEnvelope(channel);
			}
			break;

		case 2u:
			channel->frequency = ((int16_t)(value & 7u) << 7) | (channel->frequency & 0x7F);
			channel->preverb = (value & 8) >> 3;
			channel->octave = (value & 0xF0) >> 4;
			if (value != Sound.pcmRegs[i]) {
				channel->playing = channel->octave != 8;

				if (channel->playing) {
					channel->envelopePreverb = 0;
					PcmChannelComputeFrequencyStride(channel);
					PcmChannelComputeEnvelope(channel);
				}
			}
			break;

		case 3u:
			channel->totalLevel = value >> 1;
			channel->levelDirect = value & 1;
			break;

		case 4u:
			channel->dataOut = (value & 0x10u) >> 4;
			channel->panpot = value & 0xFu;
			channel->damping = (value & 0x40u) >> 6;

			if (value & 0x80u) {
				if (channel->keyOn) {
					if ((value ^ Sound.pcmRegs[i]) & 0x40u) {
						PcmChannelComputeEnvelope(channel);
					}
					break;
				}

				PcmChannelRetriggerNote(channel);
			}
			else if (channel->playing) {
				channel->envelopeStride = 4;
				PcmChannelComputeEnvelope(channel);
			}
			channel->keyOn = (value & 0x80u) >> 7;
			break;

		case 5u:
			channel->lfo = (value >> 3) & 7;
			channel->vibrato = value & 7;
			break;

		case 6u:
			channel->attackRate = value >> 4;
			channel->decay1Rate = value & 0xFu;
			if (channel->playing && value != Sound.pcmRegs[i]) {
				PcmChannelComputeEnvelope(channel);
			}
			break;

		case 7u:
			channel->decayLevel = value >> 4;
			channel->decay2Rate = value & 0xFu;
			if (channel->playing && value != Sound.pcmRegs[i]) {
				PcmChannelComputeEnvelope(channel);
			}
			break;

		case 8u:
			channel->rateCorrection = value >> 4;
			channel->releaseRate = value & 0xFu;
			if (channel->playing && value != Sound.pcmRegs[i]) {
				PcmChannelComputeEnvelope(channel);
			}
			break;

		case 9u:
			channel->tremolo = value & 7;
			break;

		default:
			fprintf(stderr, "Unsupported PCM sound register write of register %"PRIu8" with data 0x%02"PRIX8".\n", i, value);
			break;
		}
	}
	else {
		switch (i) {
		case SNDREGPCM_LSITEST1:
		case SNDREGPCM_LSITEST2:
			break;

		case SNDREGPCM_MEMMODE:
			Sound.memMode = value & 1;
			Sound.memType = value & 2;
			Sound.waveTableHeader = (value >> 2) & 7;
			break;

		case SNDREGPCM_MEMADDR1:
			nextValue &= 0x3Fu;
			break;
		case SNDREGPCM_MEMADDR2:
			break;
		case SNDREGPCM_MEMADDR3:
			Sound.address =
				((size_t)Sound.pcmRegs[SNDREGPCM_MEMADDR1] << 16) |
				((size_t)Sound.pcmRegs[SNDREGPCM_MEMADDR2] <<  8) |
				((size_t)Sound.pcmRegs[SNDREGPCM_MEMADDR3] <<  0);
			break;

		case SNDREGPCM_MEMDATA:
			Sound.ram[Sound.address] = value;
			Sound.address = (Sound.address + 1u) & 0x3FFFFFu;
			break;

		case SNDREGPCM_UNUSED7:
			break;

		case SNDREGPCM_MIXCONTROLFM:
			Sound.fmLeft = value & 7;
			Sound.fmRight = (value >> 3) & 7;
			break;

		case SNDREGPCM_MIXCONTROLPCM:
			Sound.pcmLeft = value & 7;
			Sound.pcmRight = (value >> 3) & 7;
			break;

		default:
			fprintf(stderr, "Unsupported PCM sound register write of register %"PRIu8" with data 0x%02"PRIX8".\n", i, value);
			break;
		}
	}

	Sound.pcmRegs[i] = nextValue;
}

void SoundStart() {
	Sound.hz = CLOCKHZ;
	Sound.rate = Sound.hz / 768;
	Sound.timerBase = HzToTime(Sound.hz) * 19 * 36;

	for (size_t i = 0u; i < lengthoffield(SoundData, timers); i++) {
		Sound.timers[i].next = 0u;
		Sound.timers[i].period = TIMER_NEVER;
		Sound.timers[i].start = SDL_GetPerformanceCounter();
		Sound.timers[i].expire = TIMER_NEVER;
	}

	for (size_t i = 0u; i < lengthoffield(SoundData, pcmChannels); i++) {
		Sound.pcmChannels[i].num = i;
	}

	for (size_t i = 0u; i < 0x100; i++) {
		Sound.volumes[i].value = (int32_t)(pow(2.0, (-0.375 / 6.0) * i) * 65536.0);
	}
	for (size_t i = 0x100; i < lengthoffield(SoundData, volumes); i++) {
		Sound.volumes[i] = F32(0, 0x0000u);
	}

	for (size_t i = 0u; i < lengthoffield(SoundData, panLeft); i++) {
		Sound.panLeft[i] = (int32_t)(i < 7u ? i << 3 : i < 9u ? 0x100u : 0x000u);
		Sound.panRight[i] = (int32_t)(i < 8u ? 0u : i < 10u ? 0x100u : (lengthoffield(SoundData, panRight) - i) * 8u);
	}

	for (size_t i = 0u; i < lengthoffield(SoundData, mixLevels) - 1u; i++) {
		Sound.mixLevels[i] = Sound.volumes[8u * i + 13u];
	}
	Sound.mixLevels[lengthoffield(SoundData, mixLevels) - 1u] = F32(0, 0x0000u);

	// TODO: OPL3 init.
}

void SoundReset() {
	for (uint8_t i = 0u; i < 5u; i++) {
		SoundRegisterFmWritePortA(i, 0x00u);
	}
	SoundRegisterFmWritePortB(5u, 0x00u);
	for (uint8_t i = 0u; i < 8u; i++) {
		SoundRegisterPcmWrite(i, 0x00u);
	}
	for (uint8_t i = 0xFFu; i >= 8u; i--) {
		SoundRegisterPcmWrite(i, 0x00u);
	}
	SoundRegisterPcmWrite(SNDREGPCM_MIXCONTROLFM, 0x1Bu);
	Sound.nextFmReg = Sound.nextPcmReg == 0u;
	Sound.writeFmPort = SoundRegisterFmWritePortA;
	Sound.address = 0u;

	for (size_t i = 0u; i < lengthoffield(SoundData, pcmChannels); i++) {
		PcmChannelData* const channel = &Sound.pcmChannels[i];

		channel->lfo = 0;
		channel->vibrato = 0;
		channel->attackRate = 0;
		channel->decay1Rate = 0;
		channel->decayLevel = 0;
		channel->decay2Rate = 0;
		channel->rateCorrection = 0;
		channel->releaseRate = 0;
		channel->tremolo = 0;

		channel->startAddress = 0;
		channel->loopAddress = 0;
		channel->endAddress = 0;

		channel->envelopeStride = 5;
		PcmChannelComputeEnvelope(channel);
	}

	PcmTimerReset(&Sound.timers[TIMER_A], TIMER_NEVER);
	PcmTimerReset(&Sound.timers[TIMER_B], TIMER_NEVER);

	PcmTimerReset(&Sound.timers[TIMER_BUSY], TIMER_NEVER);
	Sound.busy = false;

	PcmTimerReset(&Sound.timers[TIMER_LOADING], TIMER_NEVER);
	Sound.loading = false;

	Sound.irqLine = 0;
	Sound.currentIrq = 0;

	// TODO: OPL3 reset.
}

void SoundControlStartInit() {
	NumMusicChannels = 14u;
	CurrentFrequencyList = 0;
}

void SoundControlStatusWait(const SoundStatusFlag flags) {
	// TODO: This needs to call SoundControlRead(0) followed by updating state in a loop, so loading/busy status can change.
}

uint8_t SoundControlRead(const uint8_t i) {
	uint8_t data = 0u;

	switch (i) {
	case SNDREGRD_STATUS:
		if (Sound.expansion & SNDEXP_NEW2) {
			data |= ((uint8_t)Sound.loading << 1) | (uint8_t)Sound.busy;
		}
		data |= Sound.currentIrq | (Sound.irqLine ? 0x80u : 0x00u);
		break;

	case SNDREGRD_FM1:
	case SNDREGRD_FM3:
		data = Sound.lastFmData;
		break;

	case SNDREGRD_PCM:
		if (Sound.expansion & SNDEXP_NEW2) {
			switch (Sound.nextPcmReg) {
			case SNDREGPCM_DEVICEID:
				data = (Sound.pcmRegs[Sound.nextPcmReg] & 0x1Fu) | 0x20u;
				break;

			case SNDREGPCM_MEMDATA:
				data = Sound.rom[Sound.address];
				Sound.address = (Sound.address + 1) & 0x3FFFFFu;
				break;

			default:
				data = Sound.pcmRegs[Sound.nextPcmReg];
				break;
			}
		}
		break;

	default:
		fprintf(stderr, "Unsupported sound control read of register %"PRIu8"\n", i);
		break;
	}

	return data;
}

void SoundControlWrite(const uint8_t i, const uint8_t value) {
	switch (i) {
	case SNDREGWR_SELECTFMREGA:
	case SNDREGWR_SELECTFMREGB:
		PcmTimerBusyStart(false);
		Sound.writeFmPort = (i & 2u) ? SoundRegisterFmWritePortB : SoundRegisterFmWritePortA;
		Sound.lastFmData = value;
		// TODO: OPL3 write.
		break;

	case SNDREGWR_WRITEFMREGA:
	case SNDREGWR_WRITEFMREGB:
		Sound.writeFmPort(Sound.nextFmReg, value);
		Sound.lastFmData = value;
		// TODO: OPL3 write.
		break;

	case SNDREGWR_SELECTPCMREG:
		PcmTimerBusyStart(true);
		Sound.nextPcmReg = value;
		break;

	case SNDREGWR_WRITEPCMREG:
		PcmTimerBusyStart(true);
		if (Sound.expansion & SNDEXP_NEW2) {
			// TODO: Update audio stream here.
			PcmTimerBusyStart(true);
			SoundRegisterPcmWrite(Sound.nextPcmReg, value);
		}
		break;

	default:
		fprintf(stderr, "Unsupported sound control write of register %"PRIu8" with data 0x%02"PRIX8"\n", i, value);
		break;
	}
}
