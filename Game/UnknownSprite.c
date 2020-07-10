#include "UnknownSprite.h"
#include "Video.h"
#include "BuildData.h"

int16_t _0x602B7D8(ObjectData* objectTable, int16_t y, int16_t x) {
    int16_t i = SpriteAdderNameTable[_0x6061932.tempSprite[6]];
	_0x6061932.tempSprite[6]--;
    SpriteAdders[i].data = objectTable;
    SpriteAdders[i]._0x2B = 5u;
    SpriteAdders[i].y = y;
    SpriteAdders[i].x = x;
    SpriteAdders[i]._0x34 = _0x6061932._0x10;
    SpriteAdders[i]._0x36 = 1;
    SpriteAdders[i].layer = _0x6061932._0x12;
    SpriteAdders[i]._0x2C &= ~0x8000u;
    SpriteAdders[i].bppAlphaTileTop = 0u;
    SpriteAdders[i].palNum = 0u;
    SpriteAdders[i].flipXBgPri = 3u;
    SpriteAdders[i].verticalHorizontal = 0u;
    SpriteAdders[i].scaleY = UNSCALED;
    SpriteAdders[i].scaleX = UNSCALED;
    return i;
}

void _0x602BB0C() {
	for (size_t i = _0x6061932.tempSprite[6] + 1u; i < lengthof(SpriteAdderNameTable); i++) {
		if (SpriteAdders[SpriteAdderNameTable[i]]._0x2B == 5u) {
			SpriteAdders[SpriteAdderNameTable[i]]._0x2B = 0u;
			SpriteAdders[SpriteAdderNameTable[i]]._0x2C = 0x8000u;
			_0x6061932.tempSprite[6]++;
			if (_0x6061932.tempSprite[6] != i) {
				uint16_t* var0 = &SpriteAdderNameTable[i];

				// Swap *adderIndex and SpriteAdderNameTable[_0x6061932.tempSprite[6]].
				*var0 ^= SpriteAdderNameTable[_0x6061932.tempSprite[6]];
				SpriteAdderNameTable[_0x6061932.tempSprite[6]] ^= *var0;
				*var0 ^= SpriteAdderNameTable[_0x6061932.tempSprite[6]];
			}
		}
	}
}

void _0x602BC50(uint8_t arg0) {
	_0x6061932.tempSprite[7] = arg0;
}

void _0x602BC58(uint8_t arg0) {
	_0x6061932._0x12 = arg0;
}

void _0x602C3EE(AddSpriteData* data, void* sequence, int16_t x, int16_t y, uint16_t type) {
	// TODO
}

void _0x602C5C2() {
	void *sequenceData = SequenceDataTablePtr[1];

	for (int16_t i = _0x6061932.tempSprite[6] + 1, * adderIndex = &SpriteAdderNameTable[i]; i < lengthof(SpriteAdders); i++, adderIndex++) {
		int16_t j = *adderIndex;
		AddSpriteData* adderData = &SpriteAdders[j];

		SpriteAdders[j].flipYSprPriH = 0u;
		SpriteAdders[j].flipXBgPriW = 0u;

		uint16_t type = SpriteAdders[j]._0x36;
		int16_t y = SpriteAdders[j].y, x = SpriteAdders[j].x;
		if (y <= VIDEO_HEIGHT && x <= VIDEO_WIDTH) {
			SpriteAdders[j].palNum = (SpriteAdders[j].palNum == 0u) ? _0x6061932.tempSprite[7] : SpriteAdders[j].palNum;
			SpriteAdders[j].flipXBgPriW = (SpriteAdders[j].flipXBgPri & 3u) << 4;

			switch (SpriteAdders[j]._0x2B) {
			case 5u:
				_0x602C3EE(adderData, sequenceData, x, y, type); // TODO
				break;

			case 6u:
				type &= ~0x8000;
				if (type == 4u || type == 5u || type == 6u) {
					_0x602C0F8(adderData, sequenceData, x, y, type); // TODO
				}
				else {
					_0x602C224(adderData, sequenceData, x, y, type); // TODO
				}
				break;

			default:
				break;
			}
		}
	}
}
