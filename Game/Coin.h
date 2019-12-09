#pragma once
// TODO: Decide whether this should be a single data header or the data used
// was defined near their primary functions.

#include "HwInput.h"
#include <stdbool.h>
#include <stdint.h>

extern bool AddCoin[NUMPLAYERS];
extern bool AddService;
extern uint8_t AddServiceInputOld;
extern uint8_t NumAddServiceFrames;
extern uint8_t ServiceInputOld;
