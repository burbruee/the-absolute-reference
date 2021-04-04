#include "Lib/LibC.h"

#ifndef USESTDC
void MemCopy(size_t n, void* dst, void* src) {
	uint8_t* d = dst, * s = src;
	while (n-- > 0) {
		*d++ = *s++;
	}
}
#endif

#ifndef USESTDC
int8_t StringNCompare(const char* s1, const char* s2, size_t n) {
	if (n == 0) return 0;

	const char* c1 = s1;
	const char* c2 = s2;
	for (size_t i = 0; i < n; i++) {
		if (*++c1 != *++c2 || c1[-1] == '\0') break;
	}

	return c1[-1] - c2[-1];
}
#endif

#ifndef USESTDC
void* MemSet(void* s, int c, size_t n) {
	if (n > 0) {
		uint8_t* b = s;
		for (size_t i = 0; i < n; i++, b++) {
			*b = c;
		}
	}

	return s;
}
#endif
