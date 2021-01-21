#pragma once

#include "Sound/Music.h"
#include "Sound/SoundEffect.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct STRUCT_40388 {
	uint16_t UNK_0;
	uint8_t UNK_2;
	uint8_t UNK_3;
	uint8_t UNK_4;
	uint8_t UNK_5;
} STRUCT_40388;

typedef struct STRUCT_MidiData_100 {
	uint16_t UNK_0;
	uint8_t UNK_2[2];
} STRUCT_MidiData_100;

typedef struct MidiData {
	uint16_t UNK_0[128];
	STRUCT_MidiData_100 UNK_100[128];
	STRUCT_40388 UNK_300[512];
	uint8_t* UNK_F00[8][16]; // TODO: Appears this is an array of MIDI data pointers (lists of notes to play back).
} MidiData;

extern ROMDATA MidiData Midi;

extern bool NoSound;
extern int16_t UNK_60B181C;
extern int16_t UNK_60B186A;

// TODO: Remove functions here that are only used in Sound.c.
void UNK_602DA38(uint32_t);
uint32_t UNK_602DA48(int32_t);
void UNK_602DA6E(uint32_t);
void UNK_602DA90();
void UNK_602DAD4();
void PlayMusic(Music music);
void PlaySoundEffect(SoundEffect soundEffect);
void UNK_602E0FC(uint8_t pcmChannelNum, uint16_t soundEffectNum);
void UNK_602E166();
void UNK_602E30A();
void UNK_602E560();
void UNK_602E56A();
void PlaySoundEffectCoin();
void UNK_602E586();
void StopMusic();
void DisableSound();
void EnableSound();
void UNK_602E64C(uint8_t, uint8_t);
uint8_t UNK_602E68C(uint8_t);
void UNK_602E6B8();
void UNK_602E6F4();
void UNK_602E72A(uint8_t);
void UNK_602E780(uint8_t);
void InitSound();
void UNK_602EADC();
void UNK_602EB4C();
void UNK_602EB84();
void UNK_602EC5C();
void SoundStatusWaitNotBusy();
void SoundStatusWaitNotLoading();
void SoundFadeOutPcmChannel();
void UNK_602ED40();
void UNK_602EF9C();
void UNK_602F026(uint8_t);
void UNK_602F0D2(uint8_t);
uint8_t UNK_602F1BC();
void UNK_602F2B0(uint8_t);
uint8_t UNK_602F386();
void UNK_602F46C();
void UNK_602F480();
void UNK_602F57E();
