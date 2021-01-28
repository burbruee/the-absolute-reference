#pragma once

#include "Platform/Util/HwSound.h"
#include <stdint.h>

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
