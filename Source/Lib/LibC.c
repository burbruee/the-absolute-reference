#include "Lib/LibC.h"

// TODO: Reimplement the SH-2 code of math operators using C code, without
// directly copying the code instruction-for-instruction. For now, the intended
// implementation in simple C is provided.

int32_t Div32s(int32_t denom, int32_t numer) {
	if (denom == 0) return 0;
	else return numer / denom;
}

uint32_t Div32u(uint32_t denom, uint32_t numer) {
	if (denom == 0u) return 0u;
	else return numer / denom;
}

int16_t Div16s(int16_t denom, int32_t numer) {
	if (denom == 0) return 0;
	else return numer / denom;
}

int32_t Mod32s(int32_t denom, int32_t numer) {
	if (denom == 0) return 0;
	else return numer % denom;
}

uint32_t Mod32u(uint32_t denom, uint32_t numer) {
	if (denom == 0u) return 0u;
	else return numer % denom;
}

#ifndef USESTDC
void MemCopy(size_t n, void* dst, void* src) {
	uint8_t* d = dst, * s = src;
	while (n-- > 0) {
		*d++ = *s++;
	}
}
#endif

int32_t LeftShift(int32_t value, int32_t shifts) {
	if (shifts <= 0) return value;
	if (shifts >= 32) return 0;

	return value << shifts;
}

int32_t RightShift(int32_t value, int32_t shifts) {
	if (shifts <= 0) return value;
	if (shifts >= 32) {
		if (value & 0x80000000) {
			return -1;
		}
		else {
			return 0;
		}
	}

	return value >> shifts;
}

int8_t StringNCompare(const char *s1, const char *s2, size_t n) {
#ifdef USESTDC
	return strncmp(s1, s2, n);
#else
	if (n == 0) return 0;

	const char *c1 = s1;
	const char *c2 = s2;
	for (size_t i = 0; i < n; i++) {
		if (*++c1 != *++c2 || c1[-1] == '\0') break;
	}

	return c1[-1] - c2[-1];
#endif
}

int16_t Mod16s(int16_t denom, int16_t numer) {
	if (denom == 0) return 0;
	else return numer % denom;
}

void *MemSet(void *s, int c, size_t n) {
#ifdef USESTDC
	return memset(s, c, n);
#else
	if (n > 0) {
		uint8_t *b = s;
		for (size_t i = 0; i < n; i++, b++) {
			*b = c;
		}
	}

	return s;
#endif
}
