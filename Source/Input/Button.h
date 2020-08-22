#pragma once

#include "Input/InputTypes.h"
#include <stdint.h>

extern uint8_t SystemButtonsDown[NUMPLAYERS];
// TODO: DIP switches? Check MAME source.
extern uint8_t UNK_6064756[2];
extern uint8_t ButtonsDown[NUMPLAYERS];
extern uint8_t GameButtonsDown[NUMPLAYERS];
extern uint8_t GameButtonsOld[NUMPLAYERS];
extern uint8_t GameButtonsNew[NUMPLAYERS];
