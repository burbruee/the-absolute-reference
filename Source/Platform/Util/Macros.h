#pragma once

#include <inttypes.h>

// Use to define things like hardware ports, pointers to mapped address spaces,
// etc. When casting a hardware address, keep volatile with the type. Add const
// to the type, if the hardware address is read-only, even if the address's
// read-out values can change.
#define HWADDR(type, offset) ((volatile type*)(volatile uintptr_t)(offset))
