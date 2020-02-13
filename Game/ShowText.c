#include "ShowText.h"
#include "Object.h"
#include "HwSprite.h"
#include "VideoDefs.h"
#include "ShowObject.h"

// TODO: Replace this with a pointer that can be changed to an array of the
// widths for the font in use, but using this array for TAP's font.
static const int16_t CharWidths[128] = {
	 5, 16, 16, 16, 13, 13, 16, 16,
	16,  7, 11, 13, 16, 16, 16, 16,
	12, 12, 12, 12, 12, 12, 12, 12,
	16, 16, 16, 16, 16, 16, 16, 11,
	 5,  8,  8,  8,  8,  8,  8,  6,
	 8,  8,  8,  8,  7,  7,  7,  7,
	 8,  8,  8,  8,  8,  8,  8,  8,
	 8,  8,  6,  6,  7,  7,  7,  8,
	 8,  8,  8,  8,  8,  8,  8,  8,
	 8,  5,  8,  9,  7, 10,  9,  8,
	 8,  8,  8,  8,  7,  8,  9, 10,
	 9,  9,  8,  8,  7,  8,  7,  7,
	 5,  7,  7,  7,  7,  7,  7,  7,
	 7,  3,  5,  8,  3,  9,  7,  7,
	 7,  7,  6,  7,  6,  7,  7,  9,
	 8,  7,  8,  8,  6,  8,  8,  8
};

static const ObjectData* ObjectTableChars[0x80] = {
	&OBJECTTABLE_CHARS[0x00], &OBJECTTABLE_CHARS[0x01], &OBJECTTABLE_CHARS[0x02], &OBJECTTABLE_CHARS[0x03], &OBJECTTABLE_CHARS[0x04], &OBJECTTABLE_CHARS[0x05], &OBJECTTABLE_CHARS[0x06], &OBJECTTABLE_CHARS[0x07],
	&OBJECTTABLE_CHARS[0x08], &OBJECTTABLE_CHARS[0x09], &OBJECTTABLE_CHARS[0x0A], &OBJECTTABLE_CHARS[0x0B], &OBJECTTABLE_CHARS[0x0C], &OBJECTTABLE_CHARS[0x0D], &OBJECTTABLE_CHARS[0x0E], &OBJECTTABLE_CHARS[0x0F],
	&OBJECTTABLE_CHARS[0x10], &OBJECTTABLE_CHARS[0x11], &OBJECTTABLE_CHARS[0x12], &OBJECTTABLE_CHARS[0x13], &OBJECTTABLE_CHARS[0x14], &OBJECTTABLE_CHARS[0x15], &OBJECTTABLE_CHARS[0x16], &OBJECTTABLE_CHARS[0x17],
	&OBJECTTABLE_CHARS[0x18], &OBJECTTABLE_CHARS[0x19], &OBJECTTABLE_CHARS[0x1A], &OBJECTTABLE_CHARS[0x1B], &OBJECTTABLE_CHARS[0x1C], &OBJECTTABLE_CHARS[0x1D], &OBJECTTABLE_CHARS[0x1E], &OBJECTTABLE_CHARS[0x1F],
	&OBJECTTABLE_CHARS[' '], &OBJECTTABLE_CHARS['!'], &OBJECTTABLE_CHARS['"'], &OBJECTTABLE_CHARS['#'], &OBJECTTABLE_CHARS['$'], &OBJECTTABLE_CHARS['%'], &OBJECTTABLE_CHARS['&'], &OBJECTTABLE_CHARS['\''],
	&OBJECTTABLE_CHARS['('], &OBJECTTABLE_CHARS[')'], &OBJECTTABLE_CHARS['*'], &OBJECTTABLE_CHARS['+'], &OBJECTTABLE_CHARS[','], &OBJECTTABLE_CHARS['-'], &OBJECTTABLE_CHARS['.'], &OBJECTTABLE_CHARS['/'],
	&OBJECTTABLE_CHARS['0'], &OBJECTTABLE_CHARS['1'], &OBJECTTABLE_CHARS['2'], &OBJECTTABLE_CHARS['3'], &OBJECTTABLE_CHARS['4'], &OBJECTTABLE_CHARS['5'], &OBJECTTABLE_CHARS['6'], &OBJECTTABLE_CHARS['7'],
	&OBJECTTABLE_CHARS['8'], &OBJECTTABLE_CHARS['9'], &OBJECTTABLE_CHARS[':'], &OBJECTTABLE_CHARS[';'], &OBJECTTABLE_CHARS['<'], &OBJECTTABLE_CHARS['='], &OBJECTTABLE_CHARS['>'], &OBJECTTABLE_CHARS['?'],
	&OBJECTTABLE_CHARS['@'], &OBJECTTABLE_CHARS['A'], &OBJECTTABLE_CHARS['B'], &OBJECTTABLE_CHARS['C'], &OBJECTTABLE_CHARS['D'], &OBJECTTABLE_CHARS['E'], &OBJECTTABLE_CHARS['F'], &OBJECTTABLE_CHARS['G'],
	&OBJECTTABLE_CHARS['H'], &OBJECTTABLE_CHARS['I'], &OBJECTTABLE_CHARS['J'], &OBJECTTABLE_CHARS['K'], &OBJECTTABLE_CHARS['L'], &OBJECTTABLE_CHARS['M'], &OBJECTTABLE_CHARS['N'], &OBJECTTABLE_CHARS['O'],
	&OBJECTTABLE_CHARS['P'], &OBJECTTABLE_CHARS['Q'], &OBJECTTABLE_CHARS['R'], &OBJECTTABLE_CHARS['S'], &OBJECTTABLE_CHARS['T'], &OBJECTTABLE_CHARS['U'], &OBJECTTABLE_CHARS['V'], &OBJECTTABLE_CHARS['W'],
	&OBJECTTABLE_CHARS['X'], &OBJECTTABLE_CHARS['Y'], &OBJECTTABLE_CHARS['Z'], &OBJECTTABLE_CHARS['['], &OBJECTTABLE_CHARS['\\'],&OBJECTTABLE_CHARS[']'], &OBJECTTABLE_CHARS['^'], &OBJECTTABLE_CHARS['_'],
	&OBJECTTABLE_CHARS['`'], &OBJECTTABLE_CHARS['a'], &OBJECTTABLE_CHARS['b'], &OBJECTTABLE_CHARS['c'], &OBJECTTABLE_CHARS['d'], &OBJECTTABLE_CHARS['e'], &OBJECTTABLE_CHARS['f'], &OBJECTTABLE_CHARS['g'],
	&OBJECTTABLE_CHARS['h'], &OBJECTTABLE_CHARS['i'], &OBJECTTABLE_CHARS['j'], &OBJECTTABLE_CHARS['k'], &OBJECTTABLE_CHARS['l'], &OBJECTTABLE_CHARS['m'], &OBJECTTABLE_CHARS['n'], &OBJECTTABLE_CHARS['o'],
	&OBJECTTABLE_CHARS['p'], &OBJECTTABLE_CHARS['q'], &OBJECTTABLE_CHARS['r'], &OBJECTTABLE_CHARS['s'], &OBJECTTABLE_CHARS['t'], &OBJECTTABLE_CHARS['u'], &OBJECTTABLE_CHARS['v'], &OBJECTTABLE_CHARS['w'],
	&OBJECTTABLE_CHARS['x'], &OBJECTTABLE_CHARS['y'], &OBJECTTABLE_CHARS['z'], &OBJECTTABLE_CHARS['{'], &OBJECTTABLE_CHARS['|'], &OBJECTTABLE_CHARS['}'], &OBJECTTABLE_CHARS['~'], &OBJECTTABLE_CHARS[0x7F]
};

int16_t TextWidth(const char* text) {
	int16_t textWidth = 0;
	for (const char* c = text; *c != '\0'; c++) {
		textWidth += CharWidths[*c];
	}
	return textWidth;
}

void InitSystemTextPal() {
	SetPal(81u, 1u, _0x67910);
	SetPal(82u, 1u, _0x67910);
}

void ShowCenteredSystemText(int16_t y, const char* text, bool alpha) {
	ShowSystemText((VIDEO_WIDTH - TextWidth(text)) / 2, y, text, alpha);
}

void ShowSystemText(int16_t x, int16_t y, const char* text, bool alpha) {
	int16_t charX = x, charY = y;
	for (const char* c = text; *c != '\0'; c++) {
		if (*c == '\n') {
			charX = x;
			charY += TEXT_HEIGHT;
		}
		else if (*c == ' ') {
			charX += TEXT_SPACEWIDTH;
		}
		else {
			ShowObjectEx(ObjectTableChars[*c], charY, charX, 81u, 125u, UNSCALED, UNSCALED, alpha);
			charX += CharWidths[*c];
		}
	}
}

void ShowText(int16_t x, int16_t y, const char* text, uint8_t palNum, bool alpha) {
	int16_t charX = x, charY = y;
	for (const char* c = text; *c != '\0'; c++) {
		if (*c == '\n') {
			charX = x;
			charY += TEXT_HEIGHT;
		}
		else if (*c == ' ') {
			charX += TEXT_SPACEWIDTH;
		}
		else {
			ShowObjectEx(ObjectTableChars[*c], charY, charX, palNum, 124u, UNSCALED, UNSCALED, alpha);
			charX += CharWidths[*c];
		}
	}
}

// TODO: Change the OBJECTTABLE_CHARS references here to use char constants
// defined for each PsikyoSH font character, once all the non-ASCII characters
// have been documented. Knowing what each character is should help a lot in
// naming this function, too.
void _0x600DDA0(int16_t x, int16_t y, const char* text, bool alpha) {
	for (const char* c = text; *c != '\0'; c++) {
		switch (*c) {
		case 'N':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x1D], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case 'O':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x1B], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case 'X':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x1A], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case '1':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x06], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case '2':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x07], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case 'W':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x08], y - 2, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		case '>':
			ShowObjectEx(&OBJECTTABLE_CHARS[0x0A], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;

		default:
			ShowObjectEx(&OBJECTTABLE_CHARS['?'], y, x, 82u, 125u, UNSCALED, UNSCALED, alpha);
			break;
		}
	}
}

static const ObjectData* ObjectTableCharDigits[10] = {
	OBJECTTABLE_CHARS['0'],
	OBJECTTABLE_CHARS['1'],
	OBJECTTABLE_CHARS['2'],
	OBJECTTABLE_CHARS['3'],
	OBJECTTABLE_CHARS['4'],
	OBJECTTABLE_CHARS['5'],
	OBJECTTABLE_CHARS['6'],
	OBJECTTABLE_CHARS['7'],
	OBJECTTABLE_CHARS['8'],
	OBJECTTABLE_CHARS['9']
};

void ShowTextNum(int32_t num, int16_t y, int16_t x, int16_t numDigits, bool zeroPad, NumAlign numAlign, bool alpha) {
	int32_t base10Place = 1;
	for (int16_t i = numDigits - 1; i != 0; i--) {
		base10Place *= 10;
	}

	int32_t digits = num;
	bool displayZeroes = zeroPad;
	int16_t digitX = x;
	const ObjectData** zeroDigitObject = &ObjectTableCharDigits[0];
	for (int16_t i = numDigits; i != 0; i--, base10Place /= 10) {
		int32_t digit = digits / base10Place;
		digits -= digit * base10Place;
		if (digit > 9) {
			digit %= 10;
		}

		const ObjectData* digitObject;
		if (digit == 0) {
			if (displayZeroes || i == 1) {
				digitObject = *zeroDigitObject;
			}
			else {
				digitObject = NULL;
			}
		}
		else {
			digitObject = ObjectTableCharDigits[digit];
			displayZeroes = true;
		}

		if (digitObject != NULL) {
			ShowObjectEx(digitObject, y, digitX, 81u, 125u, UNSCALED, UNSCALED, alpha);
			digitX += TEXT_DIGITWIDTH;
		}
		else if (numAlign == NUMALIGN_RIGHT) {
			digitX += TEXT_DIGITWIDTH;
		}
	}
}

void ShowPalCycleText(int16_t x, int16_t y, const char* text, bool normalSize) {
	int16_t charX = x, charY = y;
	for (const char* c = text; *c != '\0'; c++) {
		if (*c == '\n') {
			charX = x;
			charY += TEXT_HEIGHT;
		}
		else if (*c == ' ') {
			charX += TEXT_SPACEWIDTH;
		}
		else if (normalSize) {
			ShowObjectEx(ObjectTableChars[*c], charY, charX, 159u, 124u, UNSCALED, UNSCALED, false);
			charX += CharWidths[*c];
		}
		else {
			// TODO: Change 0x5F to something like "150", as 0x5F is a 150% scale.
			ShowObjectEx(ObjectTableChars[*c], charY, charX, 159u, 125u, 0x5F, 0x5F, false);
			charX += (CharWidths[*c] * 3) / 2;
		}
	}
}
