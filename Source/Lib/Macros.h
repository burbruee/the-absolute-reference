#pragma once

#include <stddef.h>
#include <stdint.h>

#define lengthof(array) (sizeof(array) / sizeof(*(array)))
#define sizeoffield(type, field) sizeof(((type*)0)->field)
#define lengthoffield(type, field) lengthof(((type*)0)->field)

// Declare/define generic data array for storing structs of any type, assuming
// they fit the allocated size. An extra machine word is added, if the size
// doesn't exactly fit a whole number of machine words.
//
// This must be used, to ensure pointer alignment for the struct data, which is
// required on most (all?) real platforms, such as x86/x64 and ARM.
#define DATA(name, size) void* name[(size_t)(size) / sizeof(void*) + ((size_t)(size) % sizeof(void*) != 0)]
#define DATAFIELD(data, type, offset) ( \
	*( \
		(type*)( \
			&((void*)(data))[offset] \
		) \
	) \
)

// TIME() takes minutes, seconds, and frames. Use CS() if you want to write
// in centiseconds instead of frames for the third argument.
#define TIME(m, s, f) ((((uint32_t)(m) * 60u + (uint32_t)(s)) * 60u) + (uint32_t)(f))
#define CS(cs) (((uint32_t)(cs) * 60u) / 100u)
