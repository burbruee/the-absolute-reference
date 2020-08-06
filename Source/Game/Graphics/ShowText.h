#pragma once

#include <stdint.h>
#include <stdbool.h>

#define TEXT_HEIGHT 12
#define TEXT_SPACEWIDTH 5
#define TEXT_DIGITWIDTH 8
#define TEXT_RANKINGCODEDIGITWIDTH 10
int16_t TextWidth(const char* text);

void InitSystemTextPal();
void ShowSystemTextCentered(int16_t y, const char* text, bool alpha);
void ShowSystemText(int16_t x, int16_t y, const char* text, bool alpha);

void ShowText(int16_t x, int16_t y, const char* text, uint8_t palNum, bool alpha);

void ShowSystemSpecialText(int16_t x, int16_t y, const char* text, bool alpha);

// NOTE: Though there is a hint that NUMALIGN_CENTER was in TAP's source code
// (1 is passed as numAlign), true centering isn't done in any of the code.
// It's just my assumption that 1 was for centering. I guess you could add
// support for centering in your fork.
// -Brandon McGriff
typedef enum NumAlign {
	NUMALIGN_LEFT,
	NUMALIGN_CENTER,
	NUMALIGN_RIGHT
} NumAlign;

void ShowSystemNum(int32_t num, int16_t y, int16_t x, int16_t numDigits, bool zeroPad, NumAlign numAlign, bool alpha);

void ShowPalCycleText(int16_t x, int16_t y, const char* text, bool normalSize);
