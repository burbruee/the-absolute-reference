#pragma once

#include "Sound/Music.h"
#include "Sound/SoundEffect.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct SoundEffectWaveData {
	uint16_t wave;
	uint8_t totalLevel;
	uint8_t UNK_3;
	uint8_t UNK_4;
	uint8_t octaveFrequency;
} SoundEffectWaveData;

typedef struct STRUCT_MidiData_100 {
	uint16_t wave;
	uint8_t nextOctaveFrequency;
} STRUCT_MidiData_100;

#define NUMBARS 16

typedef struct MidiData {
	uint16_t musicWaves[128];
	STRUCT_MidiData_100 UNK_100[128];
	SoundEffectWaveData soundEffectWaves[512];
	uint8_t* barNotes[NUMMUSIC][NUMBARS];
} MidiData;

extern ROMDATA MidiData Midi;

extern bool NoSound;
extern int16_t NumMusicChannels;
extern int16_t CurrentFrequencyList;

// TODO: Remove functions here that are only used in Sound.c.
void UNK_602DA38(uint32_t);
uint32_t UNK_602DA48(int32_t);
void UNK_602DA6E(uint32_t);
void ResetSound();
void UpdateSound();
void PlayMusic(Music music);
void PlaySoundEffect(SoundEffect soundEffect);
void QueueSoundEffect(uint8_t pcmChannelNum, SoundEffect soundEffectNum);
void OutputSoundEffects();
void OutputSoundEffectCurrentWave();
void EnableQuiet();
void DisableQuiet();
void PlaySoundEffectCoin();
void FadeSoundEffects();
void StopMusic();
void DisableSound();
void EnableSound();
void UNK_602E64C(uint8_t, uint8_t);
uint8_t UNK_602E68C(uint8_t);
void UNK_602E6B8();
void UNK_602E6F4();
void SetPcmVolumeRight(uint8_t);
void SetPcmVolumeLeft(uint8_t);
void InitSound();
void StopSoundEffects();
void UNK_602EB4C();
void UNK_602EB84();
void UNK_602EC5C();
void SoundStatusWaitNotBusy();
void SoundStatusWaitNotLoading();
void FadeCurrentChannel();
void OutputCurrentWave();
void SetKeyLfoPanpot();
void UNK_602F026(uint8_t);
void OutputMusicWave(uint8_t);
uint8_t AllocMusicChannel();
void UNK_602F2B0(uint8_t);
uint8_t UNK_602F386();
void UNK_602F46C();
void UNK_602F480();
void UNK_602F57E();
