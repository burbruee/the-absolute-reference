#pragma once

#include "Lib/Macros.h"

// Temporary data available for any use.
extern void* Temp[0x400];

#define TEMPPTR(type, name) type* name = (type*)Temp
