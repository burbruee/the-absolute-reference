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
//
// The size is divided by four, rather than sizeof(void*), because originally 
// the sizes of data buffers were selected assuming the size of pointers is four 
// bytes; by dividing by four, rather than sizeof(void*), which could be eight, 
// the original size is expanded to still accommodate eight byte pointers or 
// four byte pointers, while keeping the size requested in the code the same for 
// all platforms. This minimizes the amount of memory required, while ensuring 
// data buffers can store the same number of eight byte pointers as the number 
// of four byte pointers TAP originally assumed was possible to store. And this 
// will work no matter the size of pointers, so long as they're four bytes or 
// larger. This also assumes all data types are aligned in arrays that are a 
// whole number multiple of the size of pointers, which seems to be true of all 
// platforms with four or eight byte pointers.
// -Brandon McGriff
#define DATA(name, size) void* name[(size_t)(size) / 4 + ((size_t)(size) % 4 != 0)]
#define DATAFIELD(data, type, offset) ( \
	*( \
		(type*)( \
			&((void*)(data))[offset] \
		) \
	) \
)

// TIME() takes minutes, seconds, and frames. Use CS() if you want to write
// in centiseconds instead of frames for the third argument.
#define TIME(minutes, seconds, frames) ((((uint32_t)(minutes) * 60u + (uint32_t)(seconds)) * 60u) + (uint32_t)(frames))
#define CS(centiseconds) (((uint32_t)(centiseconds) * 60u) / 100u)
