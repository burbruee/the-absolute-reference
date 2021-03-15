#include "Sound/Sound.h"
#include "Sound/SoundDefs.h"
#include "Main/Frame.h"
#include "Input/Credit.h"
#include "Eeprom/Eeprom.h"
#include "BuildData/BuildData.h"
#include "HwSound.h"
#include <stdbool.h>

static const int16_t FrequencyLists[17][12] = {
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

typedef struct BarData {
	int8_t measureSeconds;
	uint8_t* currentNote;
	uint8_t* notesStart;
} BarData;
static BarData Bars[NUMBARS];
static uint8_t UNK_60B16F8[NUMBARS][3];

static uint8_t UNK_60B1728[SNDPCM_NUMCHANNELS][2];
static int8_t UNK_60B1758[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1770[SNDPCM_NUMCHANNELS];
static uint8_t KeyLfoPanpot[SNDPCM_NUMCHANNELS];

// This contains sound effect constants with 1 added; when a channel index of this array is 0, that channel has no sound effect assigned to it.
static uint16_t QueuedSoundEffects[SNDPCM_NUMCHANNELS];

static SoundEffect PlayingSoundEffects[SNDPCM_NUMCHANNELS];
static uint8_t UNK_60B1800[SNDPCM_NUMCHANNELS];
static bool Quiet;
bool NoSound;
int16_t NumMusicChannels;
static uint16_t UNK_60B181E;
static int16_t SoundEffectFadeFrames[SNDPCM_NUMCHANNELS];
static uint32_t BarsDuration;
static uint32_t UNK_60B1858;
static uint8_t BarsSeconds;
static uint16_t CurrentWave;
static uint8_t CurrentOctaveFrequency;
static uint8_t CurrentTotalLevel;
static uint8_t UNK_60B1867;
static uint8_t LfoReset;
static uint8_t CurrentChannel; // TODO: Appears voice numbers are assigned to this.
int16_t CurrentFrequencyList;
static uint16_t DeactivateSoundEffect;
static uint8_t PcmMixRight;
static uint8_t PcmMixLeft;
static uint32_t UNK_60B1870;

// This contains a music constant with 1 added; when this is 0, the next music won't be changed on sound update.
static uint32_t QueuedMusic;
static uint32_t UNK_60B1878;
static uint32_t UNK_60B187C;

static void OutputBar(BarData* const bar);

void UNK_602DA38(uint32_t arg0) {
	UNK_60B1870 = arg0;
}

uint32_t UNK_602DA48(int32_t arg0) {
	return UNK_60B1858 = (arg0 * 48) / 60;
}

void UNK_602DA6E(uint32_t arg0) {
	UNK_60B187C = arg0;
}

void ResetSound() {
	NumMusicChannels = 14u;
	FadeSoundEffects();
	UNK_602EB84();
	StopMusic();
	StopSoundEffects();
	UNK_60B1870 = 0u;
	QueuedMusic = 0u;
}

void UpdateSound() {
	SetKeyLfoPanpot();
	OutputSoundEffects();
	BarsDuration += UNK_60B1858 * NumVblanks;
	NumVblanks = 0u;
	BarsSeconds = BarsDuration / TIME(0, 1, 0);
	BarsDuration %= TIME(0, 1, 0);
	UNK_6060028 = 0u;

	for (int8_t i = 0; i < NUMBARS; i++) {
		if (Bars[i].currentNote != NULL) {
			OutputBar(&Bars[i]);
		}
	}

	if (UNK_60B1878 != 0u && UNK_60B1870 != 0u && QueuedMusic != 0u) {
		CurrentMusic = MUSIC_STOP;
		PlayMusic(QueuedMusic - 1);
		QueuedMusic = 0u;
	}

	if (UNK_60B181E != 0u) {
		UNK_60B181E--;
	}
	UNK_60B1878 = 0u;
}

/*
*/

typedef enum NoteType {
	NOTETYPE_80 = 0x80,
	NOTETYPE_90 = 0x90,
	NOTETYPE_A0 = 0xA0,
	NOTETYPE_RESTARTBAR = 0xB0,
	NOTETYPE_C0 = 0xC0,

	NOTETYPE_MASK = 0xF0
} NoteType;

typedef enum NoteInfo {
	NOTEINFO_BAREND = 0xD3
} NoteInfo;

static void OutputBar(BarData* const bar) {
	for (int8_t seconds = BarsSeconds, info; seconds != 0;) {
		if (seconds < bar->measureSeconds) {
			bar->measureSeconds -= seconds;
			return;
		}
		seconds -= bar->measureSeconds;

		for (info = *bar->currentNote; info & 0x80u; info = *bar->currentNote) {
			bar->currentNote++;
			uint8_t frequencyListIndex = info & 0x0Fu;
			NoteType type = info & NOTETYPE_MASK;
			if (type == 0x80u) {
				CurrentOctaveFrequency = *bar->currentNote++;
				CurrentTotalLevel = *bar->currentNote++;
				UNK_60B16F8[frequencyListIndex][1] = CurrentTotalLevel;
				if (UNK_60B16F8[frequencyListIndex][2] == 0) {
					CurrentFrequencyList = frequencyListIndex;
					if (frequencyListIndex == 9u) {
						UNK_602F480();
					}
					else {
						OutputMusicWave(UNK_60B16F8[frequencyListIndex][0]);
					}
				}
			}
			else if (type == 0x90u) {
				CurrentOctaveFrequency = *bar->currentNote++;
				if (frequencyListIndex == 9u) {
					UNK_602F57E();
				}
				else {
					UNK_602F2B0(UNK_60B16F8[frequencyListIndex][0]);
				}
			}
			else if (type == 0xC0u) {
				UNK_60B16F8[frequencyListIndex][0] = *bar->currentNote++;
			}
			else if (info == 0xD3u) {
				bar->currentNote = NULL;
				return;
			}
			else if (type == 0xA0u) {
				bar->notesStart = bar->currentNote;
			}
			else if (type == 0xB0u) {
				bar->currentNote = bar->notesStart;
				if (frequencyListIndex == lengthof(UNK_60B16F8) - 1) {
					UNK_60B1878 = 1u;
				}
			}
			else if (info == 0xD0u) {
				UNK_60B1858 = *bar->currentNote++;
				BarsDuration = 0u;
				UNK_6060028 = 0u;
				NumVblanks = 0u;
			}
		}
		bar->measureSeconds = info;
		bar->currentNote++;
	}
}

void PlayMusic(Music music) {
	if (NoSound) {
		return;
	}
	if (UNK_60B1870 != 0u && CurrentMusic != MUSIC_STOP) {
		QueuedMusic = music + 1u;
		return;
	}

	BarsDuration = 0u;
	UNK_60B1858 = 60u;
	CurrentMusic = music;
	UNK_602EB4C();

	for (int32_t i = 0; i < lengthof(UNK_60B16F8); i++) {
		UNK_60B16F8[i][0] = 0u;
		UNK_60B16F8[i][1] = 0u;
		Bars[i].measureSeconds = 0u;
		Bars[i].currentNote = MidiPtr->barNotes[music][i];
		Bars[i].notesStart = Bars[i].currentNote;
	}

	UNK_6060028 = 0u;
	NumVblanks = 0u;
}

void PlaySoundEffect(SoundEffect soundEffect) {
	if (soundEffect == SOUNDEFFECT_COIN) {
		DisableQuiet();
		UNK_60B181E = TIME(0, 1, 0);
	}
	else if (NoSound) {
		return;
	}

	DeactivateSoundEffect = soundEffect & 0x8000u;
	const SoundEffect soundEffectNum = soundEffect & 0x7FFFu;

	for (int32_t soundEffectChannel = NumMusicChannels; soundEffectChannel < SNDPCM_NUMCHANNELS; soundEffectChannel++) {
		if (PlayingSoundEffects[soundEffectChannel] == soundEffectNum) {
			QueueSoundEffect(soundEffectChannel, soundEffectNum);
			return;
		}
	}

	for (int32_t channel = 0; channel < SNDPCM_NUMCHANNELS; channel++) {
		for (int32_t soundEffectChannel = NumMusicChannels; soundEffectChannel < SNDPCM_NUMCHANNELS; soundEffectChannel++) {
			if (UNK_60B1800[soundEffectChannel] == (MidiPtr->soundEffectWaves[soundEffectNum % lengthoffield(MidiData, soundEffectWaves)].UNK_4 & 7) && UNK_60B1758[soundEffectChannel] <= 0) {
				QueueSoundEffect(soundEffectChannel, soundEffectNum);
				return;
			}
		}

		for (int32_t j = NumMusicChannels; j < SNDPCM_NUMCHANNELS; j++) {
			if (UNK_60B1800[j] == (MidiPtr->soundEffectWaves[soundEffectNum % lengthoffield(MidiData, soundEffectWaves)].UNK_4 & 7) && UNK_60B1758[j] != 0) {
				UNK_60B1758[j]--;
			}
		}
	}
}

void QueueSoundEffect(uint8_t channel, SoundEffect soundEffect) {
	if (QueuedSoundEffects[channel] == 0u) {
		CurrentChannel = channel;
		FadeCurrentChannel();
	}

	if (DeactivateSoundEffect) {
		UNK_60B1758[channel] = 0;
		PlayingSoundEffects[channel] = SOUNDEFFECT_INACTIVE;
		QueuedSoundEffects[channel] = 0u;
	}
	else {
		QueuedSoundEffects[channel] = soundEffect + 1u;
	}
}

void OutputSoundEffects() {
	for (int32_t channel = NumMusicChannels; channel < SNDPCM_NUMCHANNELS; channel++) {
		if (!QueuedSoundEffects[channel]) {
			if (SoundEffectFadeFrames[channel] != 0u) {
				SoundEffectFadeFrames[channel]--;
				if (SoundEffectFadeFrames[channel] < 4) {
					CurrentChannel = channel;
					FadeCurrentChannel();
				}
			}
		}
		else {
			SoundEffect soundEffect = QueuedSoundEffects[channel] - 1;
			QueuedSoundEffects[channel] = 0u;
			PlayingSoundEffects[channel] = soundEffect;
			if (UNK_60B1800[channel] == 0 && UNK_60B181E != 0 && soundEffect != SOUNDEFFECT_COIN) {
				UNK_60B1758[channel] = 0;
				return;
			}
			UNK_60B1758[channel] = SNDPCM_NUMCHANNELS - NumMusicChannels;
			CurrentTotalLevel = MidiPtr->soundEffectWaves[soundEffect].totalLevel;
			CurrentOctaveFrequency = MidiPtr->soundEffectWaves[soundEffect].octaveFrequency;
			CurrentWave = MidiPtr->soundEffectWaves[soundEffect].wave;
			UNK_60B1867 = MidiPtr->soundEffectWaves[soundEffect].UNK_3;
			LfoReset = 0x20u;
			CurrentFrequencyList = 16;
			CurrentChannel = channel;
			OutputSoundEffectCurrentWave();
			SoundEffectFadeFrames[channel] = TIME(0, 7, 0);
		}
	}
}

void OutputSoundEffectCurrentWave() {
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0011 1000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 0, dampen volume based on set decay rate.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 8, set output level of left/right channels to negative infinity (no output).
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x38u);

	const int32_t octave = CurrentOctaveFrequency / 12;
	const int32_t frequency = CurrentOctaveFrequency % 12;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS3 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set octave and upper 3 bits of frequency.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ((octave - 5) << 4) | ((FrequencyLists[CurrentFrequencyList][frequency] >> 7) & 7));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS2 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set lower 7 bits of frequency and upper one bit of wave table number.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, (FrequencyLists[CurrentFrequencyList][frequency] << 1) | ((CurrentWave >> 8) & 1));

	// BUG: The intent was probably to check whether totalLevel is negative here, but the original code ended up doing an unsigned comparison. Unsigned comparisons are default when an unsigned value is in a comparison.
	uint32_t totalLevel = CurrentTotalLevel;
	if (totalLevel < 0) {
		totalLevel += 3;
	}

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_TOTALLEVEL + CurrentChannel);
	if (Quiet) {
		CurrentTotalLevel = ((totalLevel >> 2) + (CurrentTotalLevel >> 1)) | 1;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ~(CurrentTotalLevel << 1));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_WAVETBLNUMN0TON7 + CurrentChannel);
	UNK_60B1770[CurrentChannel] = 0u;
	if ((int8_t)CurrentChannel < NumMusicChannels) {
		// Use LFO reset setting, channel key on, set channel sound position to right-only.
		KeyLfoPanpot[CurrentChannel] = LfoReset | SNDPCM_KEY(true) | SNDPCM_PANPOTRIGHT;
	}
	else {
		// Use LFO reset setting, channel key on, set channel sound position to left-only.
		KeyLfoPanpot[CurrentChannel] = LfoReset | SNDPCM_KEY(true) | SNDPCM_PANPOTLEFT;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, CurrentWave & 0xFFu);
}

void EnableQuiet() {
	Quiet = true;
}

void DisableQuiet() {
	Quiet = false;
}

void PlaySoundEffectCoin() {
	DisableQuiet();
	PlaySoundEffect(SOUNDEFFECT_COIN);
}

void FadeSoundEffects() {
	for (CurrentChannel = (uint8_t)NumMusicChannels; CurrentChannel < SNDPCM_NUMCHANNELS - 2; CurrentChannel++) {
		FadeCurrentChannel();
	}
}

void StopMusic() {
	for (CurrentChannel = 0u; CurrentChannel < NumMusicChannels; CurrentChannel++) {
		FadeCurrentChannel();
	}

	for (int32_t i = 0; i < NUMBARS; i++) {
		Bars[i].currentNote = NULL;
	}

	CurrentMusic = MUSIC_STOP;
	QueuedMusic = 0u;
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
		CurrentChannel = i;

		FadeCurrentChannel();

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

	Quiet = false;
	NoSound = false;
	PcmMixRight = 0u;
	PcmMixLeft = 0u;
	UNK_60B187C = 0u;
}

void StopSoundEffects() {
	for (int32_t channel = NumMusicChannels; channel < SNDPCM_NUMCHANNELS; channel++) {
		KeyLfoPanpot[channel] = 0u;
		QueuedSoundEffects[channel] = 0u;
		UNK_60B1758[channel] = 0u;
		UNK_60B1770[channel] = 1u;
	}
	UNK_6060028 = 0u;
}

void UNK_602EB4C() {
	StopMusic();
	StopSoundEffects();
	UNK_60B1870 = 0u;
	QueuedMusic = 0u;
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
	for (int32_t channel = NumMusicChannels; channel < SNDPCM_NUMCHANNELS; channel++) {
		UNK_60B1758[channel] = 0;
		PlayingSoundEffects[channel] = SOUNDEFFECT_INACTIVE;
		UNK_60B1728[channel][0] = 0xFFu;
		UNK_60B1728[channel][1] = 0xFFu;
		SoundEffectFadeFrames[channel] = 0;
	}
}

void UNK_602EC5C() {
	FadeSoundEffects();
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

void FadeCurrentChannel() {
	// Set current channel's volume level to maximum.
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS4 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0xFFu);

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0111 1000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 1, dampen channel volume gradually.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 8, set output level of left/right channels to negative infinity (no output).
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x78u);
}

void OutputCurrentWave() {
	// Immediately set current channel's volume level to minimum.
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS4 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0xFFu);

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Write: 0011 0000
	// Key On = 0, won't repeatedly play notes.
	// Damp = 0, dampen channel volume according to the current dampening setting.
	// LFO Reset = 1, deactivate and reset low frequency oscillator.
	// Ch = 1, make the selected channel output without mixing with FM output.
	// Panpot = 0, set output level of left/right channels to normal level.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, 0x30u);

	const int32_t octave = CurrentOctaveFrequency / 12;
	const int32_t frequency = CurrentOctaveFrequency % 12;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS3 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set current PCM channel's upper three bits of the frequency and the octave.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG,  ((octave - 5) << 4) | (FrequencyLists[CurrentFrequencyList][frequency] >> 7) & 7);

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS2 + CurrentChannel);
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	// Set current PCM channel's lower seven bits of the F number and upper bit of the wave table number.
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, (FrequencyLists[CurrentFrequencyList][frequency] << 1) | ((CurrentWave >> 8) & 1));

	// BUG: The intent was probably to check whether totalLevel is negative here, but the original code ended up doing an unsigned comparison.
	uint32_t totalLevel = CurrentTotalLevel;
	if (totalLevel < 0) {
		totalLevel += 3;
	}

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_TOTALLEVEL + CurrentChannel);
	if (Quiet) {
		CurrentTotalLevel = (totalLevel >> 2) + (CurrentTotalLevel >> 1);
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, ~(CurrentTotalLevel << 1));

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_WAVETBLNUMN0TON7 + CurrentChannel);
	UNK_60B1770[CurrentChannel] = 0u;
	if (CurrentChannel < NumMusicChannels) {
		// Use LFO reset setting, channel key on, set channel sound position to right-only.
		KeyLfoPanpot[CurrentChannel] = LfoReset | SNDPCM_KEY(true) | SNDPCM_PANPOTRIGHT;
	}
	else {
		// Use LFO reset setting, channel key on, set channel sound position to left-only.
		KeyLfoPanpot[CurrentChannel] = LfoReset | SNDPCM_KEY(true) | SNDPCM_PANPOTLEFT;
	}
	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, CurrentWave & 0xFF);
}

void SetKeyLfoPanpot() {
	SoundStatusWaitNotLoading();
	for (uint8_t regNum = SNDREGPCM_CHANNELREGS5, channel = 0u; channel < SNDPCM_NUMCHANNELS; regNum++, channel++) {
		if (KeyLfoPanpot[channel] != 0u) {
			SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
			SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, regNum);
			SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
			SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, KeyLfoPanpot[channel]);
			KeyLfoPanpot[channel] = 0u;
		}
	}
}

void UNK_602F026(uint8_t keyDampLfoDataOut) {
	UNK_60B1728[CurrentChannel][1] = 0xFFu;
	UNK_60B1728[CurrentChannel][0] = 0xFFu;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_SELECTPCMREG, SNDREGPCM_CHANNELREGS5 + CurrentChannel);

	UNK_60B1758[CurrentChannel] = 0;
	UNK_60B1770[CurrentChannel] = 1u;

	SNDCTRL_STATUSWAIT(SNDSTATUS_BUSY);
	SNDCTRL_WRITE(SNDREGWR_WRITEPCMREG, CurrentChannel < NumMusicChannels ? keyDampLfoDataOut | SNDPCM_PANPOTRIGHT : keyDampLfoDataOut | SNDPCM_PANPOTLEFT);
}

void OutputMusicWave(uint8_t waveIndex) {
	if (MidiPtr->musicWaves[waveIndex] == 0xFFFFu) {
		return;
	}
	CurrentChannel = AllocMusicChannel();
	UNK_60B1728[CurrentChannel][1] = CurrentOctaveFrequency + UNK_60B187C;
	UNK_60B1758[CurrentChannel] = NumMusicChannels + 1u;
	UNK_60B1728[CurrentChannel][0] = waveIndex;
	CurrentWave = MidiPtr->musicWaves[waveIndex];
	LfoReset = 0u;
	OutputCurrentWave();
	for (uint8_t i = 0u; i < NumMusicChannels - 1; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1u;
		}
	}
}

uint8_t AllocMusicChannel() {
	uint8_t allocChannel = 0u;
	for (int8_t channel = 1; channel < NumMusicChannels - 1; channel++) {
		if (UNK_60B1770[channel] != 0 && UNK_60B1770[allocChannel] < UNK_60B1770[channel]) {
			allocChannel = channel;
		}
	}
	if (UNK_60B1770[allocChannel] != 0) {
		return allocChannel;
	}
	while (true) {
		int8_t allocChannel;
		for (allocChannel = 0; UNK_60B1758[allocChannel] != 0 && allocChannel < NumMusicChannels - 1; allocChannel++);
		if (allocChannel < NumMusicChannels - 1) {
			return allocChannel;
		}
		for (int8_t channel = 0; channel < NumMusicChannels - 1; channel++) {
			if (UNK_60B1758[channel] != 0 && --UNK_60B1758[channel] == 0) {
				UNK_60B1770[channel] = 1u;
			}
		}
	}
}

void UNK_602F2B0(uint8_t waveIndex) {
	if (MidiPtr->musicWaves[waveIndex] == 0xFFFFu) {
		return;
	}
	int8_t channel = 0;
	while (true) {
		if (channel >= NumMusicChannels - 1) {
			return;
		}
		if (UNK_60B1728[channel][1] == CurrentOctaveFrequency && UNK_60B1728[channel][0] == waveIndex) {
			break;
		}
		channel++;
	}
	CurrentChannel = channel;
	UNK_602F026(0u);
	for (int8_t channel = 0; channel < NumMusicChannels - 1; channel++) {
		if (UNK_60B1758[channel] != 0) {
			UNK_60B1758[channel]++;
		}
		if (UNK_60B1770[channel] != 0) {
			UNK_60B1770[channel]++;
		}
	}
}

uint8_t UNK_602F386() {
    int8_t allocChannel = NumMusicChannels - 1;
	for (int8_t channel = (int8_t)NumMusicChannels; channel < NumMusicChannels; channel++) {
        if (UNK_60B1770[channel] != 0 && UNK_60B1770[allocChannel] < UNK_60B1770[channel]) {
            allocChannel = channel;
        }
    }
    if (UNK_60B1770[allocChannel] != 0) {
        return allocChannel;
    }
    while (true) {
		int8_t allocChannel;
		for (allocChannel = NumMusicChannels - 1; UNK_60B1758[allocChannel] != 0 && allocChannel < NumMusicChannels; allocChannel++);
		if (allocChannel < NumMusicChannels) {
			return allocChannel;
		}
		for (int8_t channel = NumMusicChannels - 1; channel < NumMusicChannels; channel++) {
            if (UNK_60B1758[channel] != 0 && --UNK_60B1758[channel] == 0) {
                UNK_60B1770[channel] = 1u;
            }
        }
    }
}

void UNK_602F46C() {
	// Empty.
}

void UNK_602F480() {
	if (MidiPtr->UNK_100[CurrentOctaveFrequency].wave == 0xFFFFu) {
		return;
	}
	CurrentChannel = UNK_602F386();
	UNK_60B1758[CurrentChannel] = 3u;
	UNK_60B1728[CurrentChannel][1] = CurrentOctaveFrequency;
	UNK_60B1728[CurrentChannel][0] = 0xFFu;
	CurrentWave = MidiPtr->UNK_100[CurrentOctaveFrequency].wave;
	CurrentOctaveFrequency = MidiPtr->UNK_100[CurrentOctaveFrequency].nextOctaveFrequency;
	LfoReset = 0x20u;
	OutputCurrentWave();
	for (int8_t i = NumMusicChannels - 1; i < NumMusicChannels; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1;
		}
	}
}

void UNK_602F57E() {
	if (MidiPtr->UNK_100[CurrentOctaveFrequency].wave == 0xFFFFu) {
		return;
	}
	int8_t channel = NumMusicChannels - 1;
	while (true) {
		if (channel >= NumMusicChannels) {
			return;
		}
		if (UNK_60B1728[channel][1] == CurrentOctaveFrequency) {
			break;
		}
		channel++;
	}
	CurrentChannel = channel;
	UNK_602F026(0u);
	for (int8_t channel = NumMusicChannels - 1; channel < NumMusicChannels; channel++) {
		if (UNK_60B1758[channel] != 0) {
			UNK_60B1758[channel]++;
		}
		if (UNK_60B1770[channel] != 0) {
			UNK_60B1770[channel]++;
		}
	}
}
