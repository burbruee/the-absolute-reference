#pragma once

#include "Input/InputTypes.h"
#include <stdint.h>

extern ButtonInput SystemButtonsDown[NUMPLAYERS];
// TODO: DIP switches? Check MAME source.
extern uint8_t UNK_6064756[2];
extern ButtonInput ButtonsDown[NUMPLAYERS];
extern ButtonInput GameButtonsDown[NUMPLAYERS];
extern ButtonInput GameButtonsOld[NUMPLAYERS];
extern ButtonInput GameButtonsNew[NUMPLAYERS];
