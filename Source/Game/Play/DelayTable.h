#pragma once

#include <stdint.h>

#define DELAYTABLELENGTH 501

extern const int16_t LockDelay[DELAYTABLELENGTH];
extern const int16_t ClearDelay[DELAYTABLELENGTH];
extern const int16_t EntryDelay[DELAYTABLELENGTH];

extern const int16_t LockDelayTaDeath[DELAYTABLELENGTH];
extern const int16_t ClearDelayTaDeath[DELAYTABLELENGTH];
extern const int16_t EntryDelayTaDeath[DELAYTABLELENGTH];
