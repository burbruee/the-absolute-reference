#include "Sound/Sound.h"
#include "Main/Frame.h"
#include "Input/Credit.h"
#include "Eeprom/Eeprom.h"
#include "BuildData/BuildData.h"
#include "HwSound.h"
#include <stdbool.h>

static const int16_t SoundFNumTable[17][12] = {
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D },
	{ 0x0000, 0x003D, 0x007D, 0x00C2, 0x010A, 0x0157, 0x01A8, 0x01FE, 0x025A, 0x02BA, 0x0321, 0x038D }
};

typedef struct STRUCT_60B1638 {
	int8_t UNK_0;
	uint8_t* UNK_4;
	uint8_t* UNK_8;
} STRUCT_60B1638;
static STRUCT_60B1638 UNK_60B1638[16];
static uint8_t UNK_60B16F8[16][3];

static uint8_t UNK_60B1728[SNDPCM_NUMCHANNELS][2];
static int8_t UNK_60B1758[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1770[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1788[SNDPCM_NUMCHANNELS];
static uint16_t UNK_60B17A0[SNDPCM_NUMCHANNELS];
static uint16_t UNK_60B17D0[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1800[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1818;
bool NoSound;
int16_t PcmFirstLeftChannel;
static uint16_t UNK_60B181E;
static int16_t UNK_60B1820[SNDPCM_NUMCHANNELS];
static uint32_t UNK_60B1850;
static uint32_t UNK_60B1858;
static uint8_t UNK_60B1860;
static uint16_t CurrentWaveTableNum;
static uint8_t UNK_60B1865;
static uint8_t CurrentPcmTotalLevel;
static uint8_t UNK_60B1867;
static uint8_t UNK_60B1868;
static uint8_t CurrentPcmChannel; // TODO: Appears voice numbers are assigned to this.
int16_t UNK_60B186A;
static uint16_t UNK_60B186C;
static uint8_t PcmMixRight;
static uint8_t PcmMixLeft;
static uint32_t UNK_60B1870;
static uint32_t UNK_60B1874;
static uint32_t UNK_60B1878;
static uint32_t UNK_60B187C;

static void UNK_602DBE2(STRUCT_60B1638* const arg0);

void UNK_602DA38(uint32_t arg0) {
	UNK_60B1870 = arg0;
}

uint32_t UNK_602DA48(int32_t arg0) {
	return UNK_60B1858 = (arg0 * 48) / 60;
}

void UNK_602DA6E(uint32_t arg0) {
	UNK_60B187C = arg0;
}

void UNK_602DA90() {
	PcmFirstLeftChannel = 14u;
	UNK_602E586();
	UNK_602EB84();
	StopMusic();
	UNK_602EADC();
	UNK_60B1870 = 0u;
	UNK_60B1874 = 0u;
}

void UNK_602DAD4() {
	UNK_602EF9C();
	UNK_602E166();
	UNK_60B1850 += UNK_60B1858 * NumVblanks;
	NumVblanks = 0u;
	UNK_60B1860 = UNK_60B1850 / 60;
	UNK_60B1850 %= 60;
	UNK_6060028 = 0u;

	for (int8_t i = 0; i < 16; i++) {
		if (UNK_60B1638[i].UNK_4 != NULL) {
			UNK_602DBE2(&UNK_60B1638[i]);
		}
	}

	if (UNK_60B1878 != 0u && UNK_60B1870 != 0u && UNK_60B1874 != 0u) {
		UNK_6064767 = 0xFFu;
		PlayMusic(UNK_60B1874 - 1);
		UNK_60B1874 = 0u;
	}

	if (UNK_60B181E != 0u) {
		UNK_60B181E--;
	}
	UNK_60B1878 = 0u;
}

static void UNK_602DBE2(STRUCT_60B1638* const arg0) {
	for (int8_t var0 = UNK_60B1860, var1; var0 != 0;) {
		if (var0 < arg0->UNK_0) {
			arg0->UNK_0 -= var0;
			return;
		}
		var0 -= arg0->UNK_0;

		for (var1 = *arg0->UNK_4; var1 & 0x80u; var1 = *arg0->UNK_4) {
			arg0->UNK_4++;
			uint8_t var2 = var1 & 0x0Fu;
			uint8_t var3 = var1 & 0xF0u;
			if (var3 == 0x80u) {
				UNK_60B1865 = *arg0->UNK_4++;
				CurrentPcmTotalLevel = *arg0->UNK_4++;
				UNK_60B16F8[var2][1] = CurrentPcmTotalLevel;
				if (UNK_60B16F8[var2][2] == 0) {
					UNK_60B186A = var2;
					if (var2 == 9u) {
						UNK_602F480();
					}
					else {
						UNK_602F0D2(UNK_60B16F8[var2][0]);
					}
				}
			}
			else if (var3 == 0x90u) {
				UNK_60B1865 = *arg0->UNK_4++;
				if (var2 == 9u) {
					UNK_602F57E();
				}
				else {
					UNK_602F2B0(UNK_60B16F8[var2][0]);
				}
			}
			else if (var3 == 0xC0u) {
				UNK_60B16F8[var2][0] = *arg0->UNK_4++;
			}
			else if (var1 == 0xD3u) {
				arg0->UNK_4 = NULL;
				return;
			}
			else if (var3 == 0xA0u) {
				arg0->UNK_8 = arg0->UNK_4;
			}
			else if (var3 == 0xB0u) {
				arg0->UNK_4 = arg0->UNK_8;
				if (var2 == lengthof(UNK_60B16F8) - 1) {
					UNK_60B1878 = 1u;
				}
			}
			else if (var1 == 0xD0u) {
				UNK_60B1858 = *arg0->UNK_4++;
				UNK_60B1850 = 0u;
				UNK_6060028 = 0u;
				NumVblanks = 0u;
			}
		}
		arg0->UNK_0 = var1;
		arg0->UNK_4++;
	}
}

void PlayMusic(Music music) {
	if (NoSound) {
		return;
	}
	if (UNK_60B1870 != 0u && UNK_6064767 != 0xFFu) {
		UNK_60B1874 = music + 1u;
		return;
	}

	UNK_60B1850 = 0u;
	UNK_60B1858 = 60u;
	UNK_6064767 = music;
	UNK_602EB4C();

	for (int32_t i = 0; i < lengthof(UNK_60B16F8); i++) {
		UNK_60B16F8[i][0] = 0u;
		UNK_60B16F8[i][1] = 0u;
		UNK_60B1638[i].UNK_0 = 0u;
		UNK_60B1638[i].UNK_4 = MidiPtr->UNK_F00[music][i];
		UNK_60B1638[i].UNK_8 = UNK_60B1638[i].UNK_4;
	}

	UNK_6060028 = 0u;
	NumVblanks = 0u;
}

void PlaySoundEffect(SoundEffect soundEffect) {
	if (soundEffect == SOUNDEFFECT_COIN) {
		UNK_602E56A();
		UNK_60B181E = 0x3Cu;
	}
	else if (NoSound) {
		return;
	}

	UNK_60B186C = soundEffect & 0x8000u;
	const uint16_t soundEffectNum = soundEffect & 0x7FFFu;

	for (int32_t i = PcmFirstLeftChannel; i < SNDPCM_NUMCHANNELS; i++) {
		if (UNK_60B17D0[i] == soundEffectNum) {
			UNK_602E0FC(i, soundEffectNum);
			return;
		}
	}

	for (int32_t i = 0; i < SNDPCM_NUMCHANNELS; i++) {
		for (int32_t j = PcmFirstLeftChannel; j < SNDPCM_NUMCHANNELS; j++) {
			if (UNK_60B1800[j] == (MidiPtr->UNK_300[soundEffectNum % lengthoffield(MidiData, UNK_300)].UNK_4 & 7) && UNK_60B1758[j] <= 0) {
				UNK_602E0FC(j, soundEffectNum);
				return;
			}
		}

		for (int32_t j = PcmFirstLeftChannel; j < SNDPCM_NUMCHANNELS; j++) {
			if (UNK_60B1800[j] == (MidiPtr->UNK_300[soundEffectNum % lengthoffield(MidiData, UNK_300)].UNK_4 & 7) && UNK_60B1758[j] != 0) {
				UNK_60B1758[j]--;
			}
		}
	}
}

void UNK_602E0FC(uint8_t pcmChannelNum, uint16_t soundEffectNum) {
	if (UNK_60B17A0[pcmChannelNum] == 0u) {
		CurrentPcmChannel = pcmChannelNum;
		SoundFadeOutPcmChannel();
	}

	if (UNK_60B186C != 0u) {
		UNK_60B1758[pcmChannelNum] = 0;
		UNK_60B17D0[pcmChannelNum] = 0xFFFFu;
		UNK_60B17A0[pcmChannelNum] = 0u;
	}
	else {
		UNK_60B17A0[pcmChannelNum] = soundEffectNum + 1u;
	}
}

void UNK_602E166() {
	for (int32_t i = PcmFirstLeftChannel; i < SNDPCM_NUMCHANNELS; i++) {
		if (UNK_60B17A0[i] == 0u) {
			if (UNK_60B1820[i] != 0u) {
				UNK_60B1820[i]--;
				if (UNK_60B1820[i] < 4) {
					CurrentPcmChannel = i;
					SoundFadeOutPcmChannel();
				}
			}
		}
		else {
			uint16_t var0 = UNK_60B17A0[i] - 1;
			UNK_60B17A0[i] = 0;
			UNK_60B17D0[i] = var0;
			if (UNK_60B1800[i] == 0 && UNK_60B181E != 0 && var0 != 0) {
				UNK_60B1758[i] = 0;
				return;
			}
			UNK_60B1758[i] = SNDPCM_NUMCHANNELS - PcmFirstLeftChannel;
			CurrentPcmTotalLevel = MidiPtr->UNK_300[var0].UNK_2;
			UNK_60B1865 = MidiPtr->UNK_300[var0].UNK_5;
			CurrentWaveTableNum = MidiPtr->UNK_300[var0].UNK_0;
			UNK_60B1867 = MidiPtr->UNK_300[var0].UNK_3;
			UNK_60B1868 = 0x20u;
			UNK_60B186A = 16;
			CurrentPcmChannel = i;
			UNK_602E30A();
			UNK_60B1820[i] = 0x1A4u; // TODO: This might be a time value, TIME(0, 7, 0).
		}
	}
}

void UNK_602E30A() {
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0011 8000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 0, dampen volume based on set decay rate.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 8, set output level of left/right channels to negative infinity (no output).
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x38u);

	const int32_t octave = UNK_60B1865 / 12;
	const int32_t var2 = UNK_60B1865 % 12;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS3 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set octave and upper 3 bits of F number.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ((octave - 5) << 4) | ((SoundFNumTable[UNK_60B186A][var2] >> 7) & 7));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS2 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set lower 7 bits of F number and upper one bit of wave table number.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, (SoundFNumTable[UNK_60B186A][var2] << 1) | ((CurrentWaveTableNum >> 8) & 1));

	// BUG: The intent was probably to check whether var3 is negative here, but the original code ended up doing an unsigned comparison. Unsigned comparisons are default when an unsigned value is in a comparison.
	uint32_t var3 = CurrentPcmTotalLevel;
	if (var3 < 0) {
		var3 += 3;
	}

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS4 + CurrentPcmChannel);
	if (UNK_60B1818 != 0) {
		CurrentPcmTotalLevel = ((var3 >> 2) + (CurrentPcmTotalLevel >> 1)) | 1;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ~(CurrentPcmTotalLevel << 1));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_WAVETBLNUMN0TON7 + CurrentPcmChannel);
	UNK_60B1770[CurrentPcmChannel] = 0u;
	if ((int8_t)CurrentPcmChannel < PcmFirstLeftChannel) {
		UNK_60B1788[CurrentPcmChannel] = UNK_60B1868 | 0x87u;
	}
	else {
		UNK_60B1788[CurrentPcmChannel] = UNK_60B1868 | 0x89u;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, CurrentWaveTableNum & 0xFFu);
}

void UNK_602E560() {
	UNK_60B1818 = 1u;
}

void UNK_602E56A() {
	UNK_60B1818 = 0u;
}

void PlaySoundEffectCoin() {
	UNK_602E56A();
	PlaySoundEffect(SOUNDEFFECT_COIN);
}

void UNK_602E586() {
	for (CurrentPcmChannel = (uint8_t)PcmFirstLeftChannel; CurrentPcmChannel < SNDPCM_NUMCHANNELS - 2; CurrentPcmChannel++) {
		SoundFadeOutPcmChannel();
	}
}

void StopMusic() {
	for (CurrentPcmChannel = 0u; CurrentPcmChannel < PcmFirstLeftChannel; CurrentPcmChannel++) {
		SoundFadeOutPcmChannel();
	}

	for (int32_t i = 0; i < lengthof(UNK_60B1638); i++) {
		UNK_60B1638[i].UNK_4 = NULL;
	}

	UNK_6064767 = 0xFFu;
	UNK_60B1874 = 0u;
}

void DisableSound() {
	NoSound = true;
}

void EnableSound() {
	NoSound = false;
}

void UNK_602E64C(uint8_t arg0, uint8_t arg1) {
	UNK_60B16F8[arg0][2] = arg1 & 1u;
}


uint8_t UNK_602E68C(uint8_t arg0) {
	return UNK_60B16F8[arg0][2];
}

void UNK_602E6B8() {
	for (int8_t i = 0; i < 16; i++) {
		UNK_60B16F8[i][2] = 0u;
	}
}

void UNK_602E6F4() {
	for (int8_t i = 0; i < 16; i++) {
		UNK_60B16F8[i][2] = 1u;
	}
}

void SetPcmVolumeRight(uint8_t volume) {
	PcmMixRight = (7 - (volume & 7)) << 3;

	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_MIXCONTROLPCM);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, PcmMixRight | PcmMixLeft);
}

void SetPcmVolumeLeft(uint8_t volume) {
	PcmMixLeft = 7 - (volume & 7);

	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_MIXCONTROLPCM);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, PcmMixRight | PcmMixLeft);
}

void InitSound() {
	SNDCTRL_STARTINIT();

	// Use OPL4 features.
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_EXPANSION);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, SNDEXP_OPL4);

	// Clear the LSI test registers.
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_LSITEST1);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x00u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_LSITEST2);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x00u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_LSITEST1);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, 0x00u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_LSITEST2);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, 0x00u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_LSITEST1);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, 0x00u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_LSITEST2);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, 0x00u);

	// Set "A" FM registers.
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_FLAGCTRL);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, SNDFM_FLAGCTRL(1, 1, 1));
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_TIMER1);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, 255u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_TIMER2);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, 255u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_EXPANSION);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, 0x00u); // Clear register 5A (the unused counterpart to register 5B, the expansion register).
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_KEYBOARDSPLIT);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, SNDFM_KEYBOARDSPLIT(false));

	// Set "B" FM registers.
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_TIMER1);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, 0u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_TIMER2);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, 0u);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_KEYBOARDSPLIT);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, SNDFM_KEYBOARDSPLIT(false));
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_EFFECTRHYTHM);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, 0x00u);

	for (int8_t i = 0u; i < 22; i++) {
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_SELECTFMREGA, SNDREGFM_TOTALLEVEL + i);
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_WRITEFMREGA, SNDFM_TOTALLEVEL(0x3Fu));
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_SELECTFMREGB, SNDREGFM_TOTALLEVEL + i);
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_WRITEFMREGB, SNDFM_TOTALLEVEL(0x3Fu));
	}

	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_MEMSETTING);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, SNDPCM_MEMSETTING(false, false, 0u));
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_UNUSED7);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0u);

	for (int8_t i = 0; i < SNDPCM_NUMCHANNELS; i++) {
		CurrentPcmChannel = i;

		SoundFadeOutPcmChannel();

		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS3 + i);
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0u);

		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS2 + i);
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0u);

		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS1 + i);
		SoundStatusWaitNotBusy();
		SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0u);

		SoundStatusWaitNotLoading();
	}

	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_MIXCONTROLFM);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x3Fu);

	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_MIXCONTROLPCM);
	SoundStatusWaitNotBusy();
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0u);

	UNK_602EB4C();
	UNK_602EC5C();
	UNK_602E6B8();

	UNK_60B1818 = 0u;
	NoSound = false;
	PcmMixRight = 0u;
	PcmMixLeft = 0u;
	UNK_60B187C = 0u;
}

void UNK_602EADC() {
	for (int32_t i = PcmFirstLeftChannel; i < SNDPCM_NUMCHANNELS; i++) {
		UNK_60B1788[i] = 0u;
		UNK_60B17A0[i] = 0u;
		UNK_60B1758[i] = 0u;
		UNK_60B1770[i] = 1u;
	}
	UNK_6060028 = 0u;
}

void UNK_602EB4C() {
	StopMusic();
	UNK_602EADC();
	UNK_60B1870 = 0u;
	UNK_60B1874 = 0u;
}

void UNK_602EB84() {
	UNK_60B1800[23] = 0u;
	UNK_60B1800[22] = 0u;
	UNK_60B1800[21] = 2u;
	UNK_60B1800[20] = 2u;
	UNK_60B1800[19] = 2u;
	UNK_60B1800[18] = 3u;
	UNK_60B1800[17] = 3u;
	UNK_60B1800[16] = 3u;
	UNK_60B1800[15] = 5u;
	UNK_60B1800[14] = 5u;
	UNK_60B181E = 0u;
	for (int32_t i = PcmFirstLeftChannel; i < SNDPCM_NUMCHANNELS; i++) {
		UNK_60B1758[i] = 0;
		UNK_60B17D0[i] = 0xFFFFu;
		UNK_60B1728[i][0] = 0xFFu;
		UNK_60B1728[i][1] = 0xFFu;
		UNK_60B1820[i] = 0;
	}
}

void UNK_602EC5C() {
	UNK_602E586();
	UNK_602EB84();
}

void SoundStatusWaitNotBusy() {
	SNDCTRL_STARTWAIT();
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
}

void SoundStatusWaitNotLoading() {
	SNDCTRL_STARTWAIT();
	SNDCTRL_STATUSWAIT(SNDSTATUS_LOADING);
}

void SoundFadeOutPcmChannel() {
	// Set current channel's volume level to maximum.
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS4 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0xFFu);

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0111 1000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 1, dampen channel volume gradually.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 8, set output level of left/right channels to negative infinity (no output).
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x78u);
}

void UNK_602ED40() {
	const int8_t channel = CurrentPcmChannel;

	// Set current channel's volume level to maximum.
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS4 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0xFFu);

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentPcmChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0011 1000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 0, dampen channel volume according to the current dampening setting.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 8, set output level of left/right channels to negative infinity (no output).
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x30u);

	const int32_t octave = UNK_60B1865 / 12;
	const int32_t var2 = UNK_60B1865 % 12;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS3 + channel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set current PCM channel's upper three bits of the F number and the octave.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ((SoundFNumTable[UNK_60B186A][var2] >> 7) & 7) | ((octave - 5) << 4));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS2 + channel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set current PCM channel's lower seven bits of the F number and upper bit of the wave table number.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, (SoundFNumTable[UNK_60B186A][var2] << 1) | ((CurrentWaveTableNum >> 8) & 1));

	// BUG: The intent was probably to check whether var3 is negative here, but the original code ended up doing an unsigned comparison.
	uint32_t var3 = CurrentPcmTotalLevel;
	if (var3 < 0) {
		var3 += 3;
	}

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_TOTALLEVEL + channel);
	if (UNK_60B1818 != 0) {
		CurrentPcmTotalLevel = (var3 >> 2) + (CurrentPcmTotalLevel >> 1);
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ~(CurrentPcmTotalLevel << 1));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_WAVETBLNUMN0TON7 + channel);
	UNK_60B1770[channel] = 0u;
	if (channel < PcmFirstLeftChannel) {
		UNK_60B1788[channel] = UNK_60B1868 | 0x87u;
	}
	else {
		UNK_60B1788[channel] = UNK_60B1868 | 0x89u;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, CurrentWaveTableNum & 0xFF);
}

void UNK_602EF9C() {
	SoundStatusWaitNotLoading();
	for (uint8_t regNum = SNDREGPCM_CHANNELREGS5, channel = 0u; channel < SNDPCM_NUMCHANNELS; regNum++, channel++) {
		if (UNK_60B1788[channel] != 0u) {
			SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
			SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, regNum);
			SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
			SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, UNK_60B1788[channel]);
			UNK_60B1788[channel] = 0u;
		}
	}
}

void UNK_602F026(uint8_t arg0) {
	const int8_t channel = CurrentPcmChannel;
	UNK_60B1728[channel][1] = 0xFFu;
	UNK_60B1728[channel][0] = 0xFFu;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + channel);
	UNK_60B1758[channel] = 0;
	UNK_60B1770[channel] = 1u;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, channel < PcmFirstLeftChannel ? arg0 | SNDPCM_PANPOTRIGHT : arg0 | SNDPCM_PANPOTLEFT);
}

void UNK_602F0D2(uint8_t arg0) {
	if (MidiPtr->UNK_0[arg0] == 0xFFFFu) {
		return;
	}
	CurrentPcmChannel = UNK_602F1BC();
	UNK_60B1728[CurrentPcmChannel][1] = UNK_60B1865 + UNK_60B187C;
	UNK_60B1758[CurrentPcmChannel] = PcmFirstLeftChannel + 1u;
	UNK_60B1728[CurrentPcmChannel][0] = arg0;
	CurrentWaveTableNum = MidiPtr->UNK_0[arg0];
	UNK_60B1868 = 0u;
	UNK_602ED40();
	for (uint8_t i = 0u; i < PcmFirstLeftChannel - 1; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1u;
		}
	}
}

uint8_t UNK_602F1BC() {
	uint8_t j = 0u;
	for (uint8_t i = 1u; i < PcmFirstLeftChannel - 1; i++) {
		if (UNK_60B1770[i] != 0 && UNK_60B1770[j] < UNK_60B1770[i]) {
			j = i;
		}
	}
	if (UNK_60B1770[j] != 0) {
		return j;
	}
	int8_t i;
	while (true) {
		for (i = 0; UNK_60B1758[i] != 0 && i < PcmFirstLeftChannel - 1; i++);
		if (i < PcmFirstLeftChannel - 1) {
			break;
		}
		for (int8_t j = 0; j < PcmFirstLeftChannel - 1; j++) {
			if (UNK_60B1758[j] != 0 && --UNK_60B1758[j] == 0) {
				UNK_60B1770[j] = 1u;
			}
		}
	}
	return i;
}

void UNK_602F2B0(uint8_t arg0) {
	
	if (MidiPtr->UNK_0[arg0] == 0xFFFFu) {
		return;
	}

	uint8_t i = 0u;
	while (true) {
		if (i >= PcmFirstLeftChannel - 1) {
			return;
		}
		if (UNK_60B1728[i][1] == UNK_60B1865 && UNK_60B1728[i][0] == arg0) {
			break;
		}
		i++;
	}
	CurrentPcmChannel = i;
	UNK_602F026(0u);
	for (uint8_t i = 0u; i < PcmFirstLeftChannel - 1; i++) {
		if (UNK_60B1758[i] != 0) {
			UNK_60B1758[i]++;
		}
		if (UNK_60B1770[i] != 0) {
			UNK_60B1770[i]++;
		}
	}
}

uint8_t UNK_602F386() {
    int8_t j = PcmFirstLeftChannel - 1;
	for (int8_t i = (int8_t)PcmFirstLeftChannel; i < PcmFirstLeftChannel; i++) {
        if (UNK_60B1770[i] != 0 && UNK_60B1770[j] < UNK_60B1770[i]) {
            j = i;
        }
    }
    if (UNK_60B1770[j] != 0) {
        return j;
    }
	int8_t i;
    while (true) {
		for (i = (int8_t)(PcmFirstLeftChannel - 1); UNK_60B1758[i] != 0 && i < PcmFirstLeftChannel; i++);
		if (i < PcmFirstLeftChannel) {
			break;
		}
		for (i = PcmFirstLeftChannel - 1; i < PcmFirstLeftChannel; i++) {
            if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
                UNK_60B1770[i] = 1u;
            }
        }
    }
	return i;
}

void UNK_602F46C() {
	// Empty.
}

void UNK_602F480() {
	if (MidiPtr->UNK_100[UNK_60B1865].UNK_0 == 0xFFFFu) {
		return;
	}
	CurrentPcmChannel = UNK_602F386();
	UNK_60B1758[CurrentPcmChannel] = 3u;
	UNK_60B1728[CurrentPcmChannel][1] = UNK_60B1865;
	UNK_60B1728[CurrentPcmChannel][0] = 0xFFu;
	CurrentWaveTableNum = MidiPtr->UNK_100[UNK_60B1865].UNK_0;
	UNK_60B1865 = MidiPtr->UNK_100[UNK_60B1865].UNK_2[0];
	UNK_60B1868 = 0x20u;
	UNK_602ED40();
	for (int8_t i = PcmFirstLeftChannel - 1; i < PcmFirstLeftChannel; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1;
		}
	}
}

void UNK_602F57E() {
	uint8_t i;
	
	if (MidiPtr->UNK_100[UNK_60B1865].UNK_0 == 0xFFFFu) {
		return;
	}
	i = PcmFirstLeftChannel - 1;
	while (true) {
		if (i >= PcmFirstLeftChannel) {
			return;
		}
		if (UNK_60B1728[i][1] == UNK_60B1865) {
			break;
		}
		i += 1;
	}
	CurrentPcmChannel = i;
	UNK_602F026(0u);
	for (int8_t i = PcmFirstLeftChannel - 1; i < PcmFirstLeftChannel; i++) {
		if (UNK_60B1758[i] != 0) {
			UNK_60B1758[i]++;
		}
		if (UNK_60B1770[i] != 0) {
			UNK_60B1770[i]++;
		}
	}
}
