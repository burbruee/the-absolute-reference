#pragma once

#include "Sound/Music.h"
#include "Sound/SoundEffect.h"
#include <stdint.h>
#include <stdbool.h>

extern bool NoSound;

// TODO: Most of these functions should probably be moved into
// PsikyoSH-specific code, with only a few provided globally to the game/system
// code.

void UNK_602DA38(uint32_t);
void UNK_02DA48(int32_t);
void UNK_602DA6E(uint32_t);
void UNK_602DA90();
void UNK_602DAD4();
void UNK_602DBE2(uint8_t*);
void PlayMusic(Music music);
void PlaySoundEffect(SoundEffect soundEffect);
void UNK_602E0FC(uint8_t, uint16_t);
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
void UNK_602EC6C();
void UNK_602EC98();
void UNK_602ECCA();
void UNK_602ED40();
void UNK_602EF9C();
void UNK_602F026(uint8_t);
void UNK_602F0D2(uint8_t);
void UNK_602F1BC();
void UNK_602F2B0(uint8_t);
void UNK_602F386();
void UNK_602F46C();
void UNK_602F480();
void UNK_602F57E();
