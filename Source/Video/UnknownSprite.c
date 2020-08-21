#include "Video/UnknownSprite.h"
#include "Video/Video.h"
#include "Lib/Macros.h"
#include "BuildData/BuildData.h"

int16_t UNK_602B7D8(const void* data, int16_t y, int16_t x) {
    int16_t i = SpriteAdderNameTable[UNK_6061932.tempSprite[6]];
	UNK_6061932.tempSprite[6]--;
    SpriteAdders[i].data = data;
    SpriteAdders[i].type = 5u;
    SpriteAdders[i].y = y;
    SpriteAdders[i].x = x;
    SpriteAdders[i].UNK_34 = UNK_6061932.UNK_10;
    SpriteAdders[i].UNK_36 = 1;
    SpriteAdders[i].layer = UNK_6061932.UNK_12;
    SpriteAdders[i].UNK_2C &= ~0x8000u;
    SpriteAdders[i].bppAlphaTileTop = 0u;
    SpriteAdders[i].palNum = 0u;
    SpriteAdders[i].flipXBgPri = 3u;
    SpriteAdders[i].verticalHorizontal = 0u;
    SpriteAdders[i].scaleY = UNSCALED;
    SpriteAdders[i].scaleX = UNSCALED;
    return i;
}

void UNK_602BB0C() {
	for (size_t i = UNK_6061932.tempSprite[6] + 1u; i < lengthof(SpriteAdderNameTable); i++) {
		if (SpriteAdders[SpriteAdderNameTable[i]].type == 5u) {
			SpriteAdders[SpriteAdderNameTable[i]].type = 0u;
			SpriteAdders[SpriteAdderNameTable[i]].UNK_2C = 0x8000u;
			UNK_6061932.tempSprite[6]++;
			if (UNK_6061932.tempSprite[6] != i) {
				uint16_t* var0 = &SpriteAdderNameTable[i];

				// Swap *adderIndex and SpriteAdderNameTable[UNK_6061932.tempSprite[6]].
				*var0 ^= SpriteAdderNameTable[UNK_6061932.tempSprite[6]];
				SpriteAdderNameTable[UNK_6061932.tempSprite[6]] ^= *var0;
				*var0 ^= SpriteAdderNameTable[UNK_6061932.tempSprite[6]];
			}
		}
	}
}

void UNK_602BC50(uint8_t arg0) {
	UNK_6061932.tempSprite[7] = arg0;
}

void UNK_602BC58(uint8_t arg0) {
	UNK_6061932.UNK_12 = arg0;
}

void UNK_602C0F8(AddSpriteData* data, const void* sequence, int16_t x, int16_t y, uint16_t type) {
	// TODO
}

void UNK_602C224(AddSpriteData* data, const void* sequence, int16_t x, int16_t y, uint16_t type) {
	// TODO
}

void UNK_602C3EE(AddSpriteData* data, const void* sequence, int16_t x, int16_t y, uint16_t type) {
	// TODO
}

void UNK_602C5C2() {
	const void* const sequenceData = SequenceDataTablePtr[1];

	for (int16_t i = UNK_6061932.tempSprite[6] + 1, * adderIndex = &SpriteAdderNameTable[i]; i < lengthof(SpriteAdders); i++, adderIndex++) {
		int16_t j = *adderIndex;
		AddSpriteData* adderData = &SpriteAdders[j];

		SpriteAdders[j].flipYSprPriH = 0u;
		SpriteAdders[j].flipXBgPriW = 0u;

		uint16_t type = SpriteAdders[j].UNK_36;
		int16_t y = SpriteAdders[j].y, x = SpriteAdders[j].x;
		if (y <= VIDEO_HEIGHT && x <= VIDEO_WIDTH) {
			SpriteAdders[j].palNum = (SpriteAdders[j].palNum == 0u) ? UNK_6061932.tempSprite[7] : SpriteAdders[j].palNum;
			SpriteAdders[j].flipXBgPriW = (SpriteAdders[j].flipXBgPri & 3u) << 4;

			switch (SpriteAdders[j].type) {
			case ADDSPRITE_5:
				UNK_602C3EE(adderData, sequenceData, x, y, type); // TODO
				break;

			case ADDSPRITE_ANIM:
				type &= ~0x8000;
				if (type == 4u || type == 5u || type == 6u) {
					UNK_602C0F8(adderData, sequenceData, x, y, type); // TODO
				}
				else {
					UNK_602C224(adderData, sequenceData, x, y, type); // TODO
				}
				break;

			default:
				break;
			}
		}
	}
}
