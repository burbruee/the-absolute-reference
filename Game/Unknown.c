#include "Unknown.h"
#include "Frame.h"

unknown_type* _0x606005C = NULL;
static void (**_0x6060060)() = NULL;
static uint16_t* _0x6060064 = NULL;
static uint16_t _0x6060068 = 0u;

static unknown_type* _0x60AD218 = NULL;
static uint16_t _0x60AD21E = 0u;

static unknown_type* _0x607D210 = NULL;
static unknown_type* _0x607D214 = NULL;
static unknown_type _0x607D218[192];

void _0x6023788() {
}

void _0x602378C() {
}

void _0x6023790() {
	// TODO
}

void _0x60237DE() {
	CurrentPauseMode = NextPauseMode;
	if (_0x6060068 != 0) {
		do {
			_0x60AD21E = 0u;
			_0x606005C = _0x60AD218;
			if (CurrentPauseMode < PAUSEMODE_GAME) {
				int16_t var0 = _0x60AD218->_0x18;
				void** var1 = &_0x60AD218->_0x300[(1 - var0 + 14) * 4 + 3];
				while (var0 > 0) {
					_0x6060064 = var1 - 3;
					_0x6060060 = var1;
					((void(*)())*var1)();
					if (_0x60AD21E != 0) {
						break;
					}
					var0--;
					var1 += 4;
				}
			}
			else if (_0x60AD218->_0x3F8 != _0x6023788) {
				int16_t var0 = _0x60AD218->_0x18;
				void** var1 = &_0x60AD218->_0x300[(1 - var0 + 14) * 4 + 3];
				while (var0 > 1) {
					_0x6060064 = var1 - 3;
					_0x6060060 = var1;
					((void(*)())*var1)();
					if (_0x60AD21E != 0) {
						break;
					}
					var0--;
					var1 += 4;
				}
				if (_0x60AD21E == 0) {
					_0x6060060 = &_0x60AD218->_0x3F8;
					_0x60AD218->_0x3F8();
				}
			}
			_0x606005C = _0x60AD218->_0x14;
			_0x60AD218 = _0x606005C;
		} while (_0x606005C != NULL);
	}
}

void _0x602392E(int32_t arg0, void (*arg1)()) {
	// TODO
}

void _0x60239B8(int32_t arg0, int32_t arg1) {
	// TODO
}

void _0x6023A0E(unknown_type* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4) {
	// TODO
}

void* _0x6023A98(unknown_type* arg0, void* arg1, void* arg2, void* arg3) {
	// TODO
	return NULL;
}

void _0x6023B76(unknown_type* arg0) {
	// TODO
}

void _0x6023BC4(unknown_type* arg0, void (*arg1)()) {
	// TODO
}

void _0x6023C3E(unknown_type* arg0, void* arg1) {
	// TODO
}

void* _0x6023C8A(unknown_type* arg0, void* arg1) {
	// TODO
	return NULL;
}

int32_t _0x6023CBC(void (*arg0)(), uint32_t arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7) {
	// TODO
	return 0u;
}

void* _0x6023DAE(unknown_type* arg0) {
	// TODO
	return NULL;
}

void _0x6023E5A(unknown_type* arg0) {
	// TODO
	return NULL;
}

void* _0x6023EFE(unknown_type* arg0, unknown_type* arg1) {
	// TODO
	return NULL;
}

void* _0x6023FA4(void (**arg0)(), unknown_type* arg1) {
	// TODO
	return NULL;
}

void _0x6024030(unknown_type* arg0) {
	// TODO
}

void _0x602406E() {
	// TODO
}

void _0x60240A8(void* arg0) {
	// TODO
}
