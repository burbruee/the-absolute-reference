#include "UnknownSprite.h"
#include "Video.h"
#include "BuildData.h"

void _0x602BB0C() {
	for (size_t i = _0x6061932.tempSprite[6] + 1u; i < lengthof(_0x606106C); i++) {
		if (_0x606006C[_0x606106C[i]]._0x2B == 5u) {
			_0x606006C[_0x606106C[i]]._0x2B = 0u;
			_0x606006C[_0x606106C[i]]._0x2C = 0x8000u;
			_0x6061932.tempSprite[6]++;
			if (_0x6061932.tempSprite[6] != i) {
				uint16_t* var0 = &_0x606106C[i];

				// Swap *var0 and _0x606106C[_0x6061932.tempSprite[6]].
				*var0 ^= _0x606106C[_0x6061932.tempSprite[6]];
				_0x606106C[_0x6061932.tempSprite[6]] ^= *var0;
				*var0 ^= _0x606106C[_0x6061932.tempSprite[6]];
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

void _0x602C5C2() {
	// TODO
}
