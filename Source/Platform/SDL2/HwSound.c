#include "HwSound.h"
#include "HwData.h"
#include "Sound/Sound.h"
#include "Lib/Fixed.h"
#include "Lib/Macros.h"
#include "Platform/Util/AccessData.h"
#include "SDL.h"
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>

SoundData Sound;

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

const int32_t VolumeTable[1024] = {
	0x00010000, 0x0000F525, 0x0000EAC0, 0x0000E0CC, 0x0000D744, 0x0000CE24, 0x0000C567, 0x0000BD08, 0x0000B504, 0x0000AD58, 0x0000A5FE, 0x00009EF5, 0x00009837, 0x000091C3, 0x00008B95, 0x000085AA,
	0x00008000, 0x00007A92, 0x00007560, 0x00007066, 0x00006BA2, 0x00006712, 0x000062B3, 0x00005E84, 0x00005A82, 0x000056AC, 0x000052FF, 0x00004F7A, 0x00004C1B, 0x000048E1, 0x000045CA, 0x000042D5,
	0x00004000, 0x00003D49, 0x00003AB0, 0x00003833, 0x000035D1, 0x00003389, 0x00003159, 0x00002F42, 0x00002D41, 0x00002B56, 0x0000297F, 0x000027BD, 0x0000260D, 0x00002470, 0x000022E5, 0x0000216A,
	0x00002000, 0x00001EA4, 0x00001D58, 0x00001C19, 0x00001AE8, 0x000019C4, 0x000018AC, 0x000017A1, 0x000016A0, 0x000015AB, 0x000014BF, 0x000013DE, 0x00001306, 0x00001238, 0x00001172, 0x000010B5,
	0x00001000, 0x00000F52, 0x00000EAC, 0x00000E0C, 0x00000D74, 0x00000CE2, 0x00000C56, 0x00000BD0, 0x00000B50, 0x00000AD5, 0x00000A5F, 0x000009EF, 0x00000983, 0x0000091C, 0x000008B9, 0x0000085A,
	0x00000800, 0x000007A9, 0x00000756, 0x00000706, 0x000006BA, 0x00000671, 0x0000062B, 0x000005E8, 0x000005A8, 0x0000056A, 0x0000052F, 0x000004F7, 0x000004C1, 0x0000048E, 0x0000045C, 0x0000042D,
	0x00000400, 0x000003D4, 0x000003AB, 0x00000383, 0x0000035D, 0x00000338, 0x00000315, 0x000002F4, 0x000002D4, 0x000002B5, 0x00000297, 0x0000027B, 0x00000260, 0x00000247, 0x0000022E, 0x00000216,
	0x00000200, 0x000001EA, 0x000001D5, 0x000001C1, 0x000001AE, 0x0000019C, 0x0000018A, 0x0000017A, 0x0000016A, 0x0000015A, 0x0000014B, 0x0000013D, 0x00000130, 0x00000123, 0x00000117, 0x0000010B,
	0x00000100, 0x000000F5, 0x000000EA, 0x000000E0, 0x000000D7, 0x000000CE, 0x000000C5, 0x000000BD, 0x000000B5, 0x000000AD, 0x000000A5, 0x0000009E, 0x00000098, 0x00000091, 0x0000008B, 0x00000085,
	0x00000080, 0x0000007A, 0x00000075, 0x00000070, 0x0000006B, 0x00000067, 0x00000062, 0x0000005E, 0x0000005A, 0x00000056, 0x00000052, 0x0000004F, 0x0000004C, 0x00000048, 0x00000045, 0x00000042,
	0x00000040, 0x0000003D, 0x0000003A, 0x00000038, 0x00000035, 0x00000033, 0x00000031, 0x0000002F, 0x0000002D, 0x0000002B, 0x00000029, 0x00000027, 0x00000026, 0x00000024, 0x00000022, 0x00000021,
	0x00000020, 0x0000001E, 0x0000001D, 0x0000001C, 0x0000001A, 0x00000019, 0x00000018, 0x00000017, 0x00000016, 0x00000015, 0x00000014, 0x00000013, 0x00000013, 0x00000012, 0x00000011, 0x00000010,
	0x00000010, 0x0000000F, 0x0000000E, 0x0000000E, 0x0000000D, 0x0000000C, 0x0000000C, 0x0000000B, 0x0000000B, 0x0000000A, 0x0000000A, 0x00000009, 0x00000009, 0x00000009, 0x00000008, 0x00000008,
	0x00000008, 0x00000007, 0x00000007, 0x00000007, 0x00000006, 0x00000006, 0x00000006, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000004, 0x00000004, 0x00000004, 0x00000004, 0x00000004,
	0x00000004, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002,
	0x00000002, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001
	// Init remaining elements to zero.
};

const int32_t PanLeftTable[16] = {
	0x0000, 0x0008, 0x0010, 0x0018, 0x0020, 0x0028, 0x0030, 0x0100, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
const int32_t PanRightTable[16] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0100, 0x0100, 0x0030, 0x0028, 0x0020, 0x0018, 0x0010, 0x0008
};

const int32_t MixLevelTable[8] = {
	0x91C3, 0x6712, 0x48E1, 0x3389, 0x2470, 0x19C4, 0x1238, 0x0000
};

static void IrqCheck() {
	Sound.irqLine = !!Sound.currentIrq;
}

int32_t PcmChannelComputeRate(PcmChannelData* const channel, int32_t value) {
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

uint32_t PcmChannelComputeDecayEnvelopeVolumeStep(PcmChannelData* const channel, int32_t value) {
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

void PcmChannelComputeSampleStride(PcmChannelData* const channel) {
	int32_t octave = channel->octave;
	if (octave & 8) {
		octave |= -8;
	}

	channel->sampleStride = ((channel->frequency | 0x400) << (octave + 8)) >> 3;
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

	channel->samplePos = 0u;
	channel->envelopeStride = 0;
	channel->envelopePreverb = 0;

	PcmChannelComputeSampleStride(channel);
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

			const uint8_t* const data = SoundRomData + offset;
			channel->bits = (data[0] & 0xC0u) >> 6;
			channel->startAddress = (((uint32_t)data[0] & 0x3Fu) << 16) | ((uint32_t)data[1] << 8) | data[2];
			channel->loopAddress = ((uint32_t)data[3] << 24) | ((uint32_t)data[4] << 16);
			channel->endAddress = ((((uint32_t)data[5] << 24) | ((uint32_t)data[6] << 16)) - 0x10000u) ^ 0xFFFF0000u;

			for (size_t j = 7u; j < 12u; j++) {
				SoundRegisterPcmWrite((uint8_t)(SNDREGPCM_CHANNELREGS1 + channelNum + (j - 2u) * 24u), data[j]);
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
				PcmChannelComputeSampleStride(channel);
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
					PcmChannelComputeSampleStride(channel);
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
				data = SoundRomData[Sound.address];
				Sound.address = (Sound.address + 1) & 0x3FFFFFu;
				break;

			default:
				data = Sound.pcmRegs[Sound.nextPcmReg];
				break;
			}
		}
		break;

	default:
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
		Sound.nextFmReg = value;
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
			PcmTimerBusyStart(true);
			SoundRegisterPcmWrite(Sound.nextPcmReg, value);
		}
		break;

	default:
		break;
	}
}
