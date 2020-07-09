#include "Checksum.h"
#include "BuildData.h"
#include "UnknownSprite.h"
#include "ShowText.h"
#include "PalNum.h"
#include "Sound.h"
#include "ShowObject.h"
#include "Layer.h"
#include "HwData.h"

// TODO: Implement a way for the build system to calculate the checksums and
// put them here, if possible.
const BuildChecksumData BuildChecksum = {
	{ 'C', 'H', 'E', 'C', 'K', 'S', 'U', 'M' },
	{
		{ CHECKSUM_VALUE, { 0x5EBDu, 0x643Au, 0xC2F7u } },
		{ CHECKSUM_VALUE, { 0xB114u, 0x0ECEu, 0xBFE2u } },
		{ CHECKSUM_VALUE, { 0x5DE2u, 0x03A2u, 0x6184u } },
		{ CHECKSUM_VALUE, { 0x9EA2u, 0x991Au, 0x37BCu } },
		{ CHECKSUM_VALUE, { 0x90D9u, 0x2CB4u, 0xBD8Du } },
		{ CHECKSUM_VALUE, { 0xBFDDu, 0xA8F8u, 0x68D5u } },
		{ CHECKSUM_VALUE, { 0xC132u, 0x4EF4u, 0x1026u } },
		{ CHECKSUM_VALUE, { 0x001Bu, 0x18D5u, 0x18F0u } },

		{ CHECKSUM_VALUE | CHECKSUM_LAST, { 0x0000u, 0x0000u, 0x2888u } },
		{ CHECKSUM_END, { 0x0000u, 0x0000u, 0x0000 } },
	}
};

static uint16_t ChecksumDataDst[8 * 5];
static const uint16_t* ChecksumDataSrcPtr;
static uint16_t* ChecksumDataDstPtr;
static uint32_t GraphicsCheckBank;
static uint32_t _0x60B19AC;

bool _0x6030100() {
	bool success;
	const uint16_t* checksumSrc;
	uint16_t* checksumDst;
	if (ChecksumDataSrcPtr[0] == CHECKSUM_END) {
		success = true;
		checksumSrc = BuildChecksumDataPtr;
		checksumDst = ChecksumDataDst;
		while (checksumSrc[0] != CHECKSUM_END) {
			if (checksumSrc[0] & CHECKSUM_LAST) {
				if (checksumSrc[3] != checksumDst[3]) {
					success = false;
				}
			}
			else {
				if (
					checksumSrc[1] != checksumDst[1] ||
					checksumSrc[2] != checksumDst[2] ||
					checksumSrc[3] != checksumDst[3]) {
					success = false;
				}
			}

			checksumSrc += 4u;
			checksumDst += 4u;
		}
	}
	else {
		// Sound ROM checksum calculation.
		if (ChecksumDataSrcPtr[0] & CHECKSUM_LAST) {
			_0x602EC6C();
			SOUNDCTRL[4] = 2u;
			_0x602EC6C();
			SOUNDCTRL[5] = 1u;

			_0x602EC6C();
			SOUNDCTRL[4] = 3u;
			_0x602EC6C();
			SOUNDCTRL[5] = 0u;

			_0x602EC6C();
			SOUNDCTRL[4] = 4u;
			_0x602EC6C();
			SOUNDCTRL[5] = 0u;

			_0x602EC6C();
			SOUNDCTRL[4] = 5u;
			_0x602EC6C();
			SOUNDCTRL[5] = 0u;

			_0x602EC6C();
			SOUNDCTRL[4] = 6u;
			for (size_t i = 0x400000u; i != 0u; i--) {
				while (SOUNDCTRL[0] & 1);
				ChecksumDataDstPtr[3] += SOUNDCTRL[5];
			}
			_0x602EC6C();
			SOUNDCTRL[4] = 2u;
			_0x602EC6C();
			SOUNDCTRL[5] = 0u;
		}
		else {
			// Graphics ROM checksum calculation.
			uint16_t checksum0 = 0u, checksum1 = 0u;
			for (size_t i = 0u; i < (*ChecksumDataSrcPtr & 0xFFFu); i++) {
				uint32_t* graphicsCheck = GRAPHICSCHECK;
				if (GraphicsCheckBank & 0x100u) {
					VideoGraphicsCheckBank[0] = ((GraphicsCheckBank >> 8) & 3) | 0x30u;
				}
				uint8_t nextBank = GraphicsCheckBank + 1u;
				VideoGraphicsCheckBank[1] = GraphicsCheckBank;
				for (size_t i = 0x8000u; i != 0u; i -= 2u, graphicsCheck += 2u) {
					uint32_t seed0 = graphicsCheck[0], seed1 = graphicsCheck[1];
					checksum1 += (seed0 >> 16) + (seed1 >> 16);
					checksum0 += seed0 + seed1;
				}
			}
			ChecksumDataDstPtr[3] = checksum0 + checksum1;
			ChecksumDataDstPtr[1] = checksum0;
			ChecksumDataDstPtr[2] = checksum1;
		}

		ChecksumDataDstPtr += 4u;
		ChecksumDataSrcPtr += 4u;
		success = true;
	}

}

bool _0x60302C4() {
	static const int16_t _0x6035B28[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15,
	};
	if (ChecksumDataSrcPtr == NULL) {
		ChecksumDataSrcPtr = BuildChecksumDataPtr;
		ChecksumDataDstPtr = ChecksumDataDst;
		_0x60B19AC = 0x78u;
		GraphicsCheckBank = 0x60u;
		_0x602BC50(0u);
	}

	_0x6030100();
	_0x602BC58(0x7Du);

	size_t i = 0u;
	const uint16_t* checksumSrc = BuildChecksumDataPtr;
	uint16_t* checksumDst = ChecksumDataDst;
	int16_t offsetY = 0;
	while (checksumSrc[0] != CHECKSUM_END) {
		const int16_t nameX = 56;
		const int16_t statusX = 130;
		const int16_t y = 96 + offsetY;
		if (checksumSrc[0] & CHECKSUM_LAST) {
			ShowText(nameX, y, "SOUND", PALNUM_SYSTEMTEXT, false);

			if (checksumSrc[3] == checksumDst[3]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst[3] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_10;
				}
				ShowText(statusX, y, text, palNum, false);
			}
		}
		else {
			ShowText(nameX, y, "CHAR", PALNUM_SYSTEMTEXT, false);
			ShowHexNum(_0x6035B28[i], y, 92, 4, 1, 2, PALNUM_SYSTEMTEXT, false);
			if (checksumSrc[1] == checksumDst[1]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst[3] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_10;
				}
				ShowText(statusX, y, text, palNum, false);
			}

			if (checksumSrc[2] == checksumDst[2]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst[3] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_10;
				}
				ShowText(162, y, text, palNum, false);
			}
		}

		i++;
		offsetY += 12;
		checksumSrc += 4u;
		checksumDst += 4u;
	}

	_0x602BC58(0x7Fu);
	if (ChecksumDataSrcPtr[0] == CHECKSUM_END) {
		if (_0x60B19AC == 0u) {
			return false;
		}
		_0x60B19AC--;
	}
	return true;
}

static const ObjectData* ObjectTableHexDigits[0x10] = {
	&OBJECTTABLE_HEXDIGITS[0],
	&OBJECTTABLE_HEXDIGITS[1],
	&OBJECTTABLE_HEXDIGITS[2],
	&OBJECTTABLE_HEXDIGITS[3],
	&OBJECTTABLE_HEXDIGITS[4],
	&OBJECTTABLE_HEXDIGITS[5],
	&OBJECTTABLE_HEXDIGITS[6],
	&OBJECTTABLE_HEXDIGITS[7],
	&OBJECTTABLE_HEXDIGITS[8],
	&OBJECTTABLE_HEXDIGITS[9],
	&OBJECTTABLE_HEXDIGITS[10],
	&OBJECTTABLE_HEXDIGITS[11],
	&OBJECTTABLE_HEXDIGITS[12],
	&OBJECTTABLE_HEXDIGITS[13],
	&OBJECTTABLE_HEXDIGITS[14],
	&OBJECTTABLE_HEXDIGITS[15],
};

void ShowHexNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool displayZeroes, NumAlign numAlign, int16_t palNum, bool alpha) {
	int32_t i = numDigits--;
	int32_t base16Place = 1;
	while (numDigits != 0) {
		numDigits--;
		base16Place <<= 4;
	}

	for (; i != 0; i--, base16Place >>= 4) {
		int32_t digit = num / base16Place;
		num -= base16Place * digit;
		if (digit > 0xF) {
			digit %= 0x10;
		}

		ObjectData* digitObject;
		if (digit == 0) {
			if (displayZeroes || i == 1) {
				digitObject = ObjectTableHexDigits[0];
			}
			else {
				digitObject = NULL;
			}
		}
		else {
			digitObject = ObjectTableHexDigits[digit];
			displayZeroes = true;
		}

		if (digitObject != NULL) {
			ShowObjectEx(digitObject, y, x, palNum, 125u, UNSCALED, UNSCALED, alpha);
			x += TEXT_DIGITWIDTH;
		}
		else if (numAlign == NUMALIGN_RIGHT) {
			x += TEXT_DIGITWIDTH;
		}

		if (base16Place < 0) {
			base16Place += 0xF;
		}
	}
}