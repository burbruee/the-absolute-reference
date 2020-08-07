#include "BuildData/Checksum.h"
#include "BuildData/BuildData.h"
#include "Game/Graphics/ShowText.h"
#include "Video/UnknownSprite.h"
#include "Game/PalNum.h"
#include "Sound/Sound.h"
#include "Game/Graphics/ShowObject.h"
#include "Game/Graphics/Layer.h"
#include "HwData.h"

// TODO: Implement a way for the build system to calculate the checksums and
// put them here, if possible.
const ChecksumData BuildChecksum = {
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
		{ CHECKSUM_VALUE | CHECKSUM_LAST,
	                      { 0x0000u, 0x0000u, 0x2888u } },
		{   CHECKSUM_END, { 0x0000u, 0x0000u, 0x0000u } },
	}
};

static Checksum ChecksumDataDst[10];
static const Checksum* ChecksumDataSrcPtr;
static Checksum* ChecksumDataDstPtr;
static uint32_t GraphicsCheckBank;
static uint32_t NumChecksumFrames;

static void ShowHexNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool displayZeroes, NumAlign numAlign, int16_t palNum, bool alpha);

bool ChecksumPass() {
	bool success;
	if (ChecksumDataSrcPtr == NULL) {
		success = true;
		for (
			const Checksum* checksumSrc = BuildChecksumDataPtr, * checksumDst = ChecksumDataDst;
			checksumSrc->checksumFlags != CHECKSUM_END;
			checksumSrc++, checksumDst++) {
			if (checksumSrc->checksumFlags & CHECKSUM_LAST) {
				if (checksumSrc->data[2] != checksumDst->data[2]) {
					success = false;
				}
			}
			else {
				if (
					checksumSrc->data[0] != checksumDst->data[0] ||
					checksumSrc->data[1] != checksumDst->data[1] ||
					checksumSrc->data[2] != checksumDst->data[2]) {
					success = false;
				}
			}
		}
	}
	else {
		// Sound ROM checksum calculation.
		if (ChecksumDataSrcPtr->checksumFlags & CHECKSUM_LAST) {
			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 2u);
			_0x602EC6C();
			SOUNDCTRL_WRITE(5, 1u);

			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 3u);
			_0x602EC6C();
			SOUNDCTRL_WRITE(5, 0u);

			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 4u);
			_0x602EC6C();
			SOUNDCTRL_WRITE(5, 0u);

			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 5u);
			_0x602EC6C();
			SOUNDCTRL_WRITE(5, 0u);

			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 6u);
			for (size_t i = 0x400000u; i != 0u; i--) {
				while (SOUNDCTRL_READ(0) & 1);
				ChecksumDataDstPtr->data[2] += SOUNDCTRL_READ(5);
			}
			_0x602EC6C();
			SOUNDCTRL_WRITE(4, 2u);
			_0x602EC6C();
			SOUNDCTRL_WRITE(5, 0u);
		}
		else {
			// Graphics ROM checksum calculation.
			uint32_t checksum0 = 0u, checksum1 = 0u;
			for (size_t i = 0u; i < (ChecksumDataSrcPtr->checksumFlags & 0x0FFFu); i++) {
				uint32_t* graphicsCheck = GRAPHICSCHECK;
				if (GraphicsCheckBank & 0x100u) {
					GRAPHICSCHECK_SETBANK(0, (GraphicsCheckBank >> 8) & 3 | 0x30u);
				}
				uint8_t nextBank = GraphicsCheckBank + 1u;
				GRAPHICSCHECK_SETBANK(1, GraphicsCheckBank);
				for (size_t j = 0x8000u; j != 0u; j -= 2u, graphicsCheck += 2u) {
					uint32_t seed0 = graphicsCheck[0], seed1 = graphicsCheck[1];
					checksum1 += (seed0 >> 16) + (seed1 >> 16);
					checksum0 += seed0 + seed1;
				}
			}
			ChecksumDataDstPtr->data[2] = checksum0 + checksum1;
			ChecksumDataDstPtr->data[0] = checksum0;
			ChecksumDataDstPtr->data[1] = checksum1;
		}

		ChecksumDataDstPtr++;
		ChecksumDataSrcPtr++;
		success = true;
	}
	return success;
}

bool ShowChecksumPass() {
	static const int16_t hexDigits[] = {
		0x0, 0x1, 0x2, 0x3,
		0x4, 0x5, 0x6, 0x7,
		0x8, 0x9, 0xA, 0xB,
		0xC, 0xD, 0xE, 0xF,
	};
	if (ChecksumDataSrcPtr == NULL) {
		ChecksumDataSrcPtr = BuildChecksumDataPtr;
		ChecksumDataDstPtr = ChecksumDataDst;
		NumChecksumFrames = TIME(0, 2, 0);
		GraphicsCheckBank = 0x60u;
		_0x602BC50(0u);
	}

	ChecksumPass();
	_0x602BC58(0x7Du);

	size_t romNum = 0u;
	int16_t offsetY = 0;
	for (const Checksum* checksumSrc = BuildChecksumDataPtr, * checksumDst = ChecksumDataDst; checksumSrc->checksumFlags != CHECKSUM_END; romNum++, offsetY += 12, checksumSrc++, checksumDst++) {
		const int16_t nameX = 56;
		const int16_t statusX = 130;
		const int16_t y = 96 + offsetY;
		if (checksumSrc->checksumFlags & CHECKSUM_LAST) {
			ShowText(nameX, y, "SOUND", PALNUM_SYSTEMTEXT, false);

			if (checksumSrc->data[2] == checksumDst->data[2]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst->data[2] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_CHECKSUMNG;
				}
				ShowText(statusX, y, text, palNum, false);
			}
		}
		else {
			ShowText(nameX, y, "CHAR", PALNUM_SYSTEMTEXT, false);
			ShowHexNum(hexDigits[romNum], y, 92, 4, 1, 2, PALNUM_SYSTEMTEXT, false);
			if (checksumSrc->data[0] == checksumDst->data[0]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst->data[2] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_CHECKSUMNG;
				}
				ShowText(statusX, y, text, palNum, false);
			}

			if (checksumSrc->data[1] == checksumDst->data[1]) {
				ShowText(statusX, y, "OK", PALNUM_SYSTEMTEXT, false);
			}
			else {
				const char* text;
				uint8_t palNum;
				if (checksumDst->data[2] == 0u) {
					text = "--";
					palNum = PALNUM_SYSTEMTEXT;
				}
				else {
					text = "NG";
					palNum = PALNUM_CHECKSUMNG;
				}
				ShowText(162, y, text, palNum, false);
			}
		}
	}

	_0x602BC58(0x7Fu);
	if (ChecksumDataSrcPtr->checksumFlags == CHECKSUM_END) {
		if (NumChecksumFrames == 0u) {
			return false;
		}
		NumChecksumFrames--;
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

static void ShowHexNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool displayZeroes, NumAlign numAlign, int16_t palNum, bool alpha) {
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