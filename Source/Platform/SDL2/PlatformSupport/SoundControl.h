#pragma once

#include <stdint.h>

void SoundControlStartInit();
#define SOUNDCTRL_STARTINIT() SoundControlStartInit()

#define SOUNDCTRL_STARTWAIT()

void SoundControlWait(const uint8_t bit);
#define SOUNDCTRL_WAIT(bit) SoundControlWait((uint8_t)(bit))

uint8_t SoundControlRead(const uint8_t i);
#define SOUNDCTRL_READ(i) SoundControlRead((uint8_t)(i))

void SoundControlWrite(const uint8_t i, const uint8_t value);
#define SOUNDCTRL_WRITE(i, value) SoundControlWrite((uint8_t)(i), (uint8_t)(value))
