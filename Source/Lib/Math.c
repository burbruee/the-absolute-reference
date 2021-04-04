#include "Lib/Math.h"
#include "Main/Frame.h"

// TrigTable is indexed with an angle, but the table requires that each quadrant
// be mapped into the range 0 to 64.
static const Fixed32 TrigTable[65] = {
	FIXED(0, 0x0000), FIXED(0, 0x0648), FIXED(0, 0x0C8F), FIXED(0, 0x12D5), FIXED(0, 0x1917), FIXED(0, 0x1F56), FIXED(0, 0x2590), FIXED(0, 0x2BC4),
	FIXED(0, 0x31F1), FIXED(0, 0x3817), FIXED(0, 0x3E33), FIXED(0, 0x4447), FIXED(0, 0x4A50), FIXED(0, 0x504D), FIXED(0, 0x563E), FIXED(0, 0x5C22),
	FIXED(0, 0x61F7), FIXED(0, 0x67BD), FIXED(0, 0x6D74), FIXED(0, 0x7319), FIXED(0, 0x78AD), FIXED(0, 0x7E2E), FIXED(0, 0x839C), FIXED(0, 0x88F5),
	FIXED(0, 0x8E39), FIXED(0, 0x9368), FIXED(0, 0x987F), FIXED(0, 0x9D7F), FIXED(0, 0xA267), FIXED(0, 0xA736), FIXED(0, 0xABEB), FIXED(0, 0xB085),
	FIXED(0, 0xB504), FIXED(0, 0xB968), FIXED(0, 0xBDAE), FIXED(0, 0xC1D8), FIXED(0, 0xC5E4), FIXED(0, 0xC9D1), FIXED(0, 0xCD9F), FIXED(0, 0xD14D),
	FIXED(0, 0xD4DB), FIXED(0, 0xD848), FIXED(0, 0xDB94), FIXED(0, 0xDEBE), FIXED(0, 0xE1C5), FIXED(0, 0xE4AA), FIXED(0, 0xE76B), FIXED(0, 0xEA09),
	FIXED(0, 0xEC83), FIXED(0, 0xEED8), FIXED(0, 0xF109), FIXED(0, 0xF314), FIXED(0, 0xF4FA), FIXED(0, 0xF6BA), FIXED(0, 0xF853), FIXED(0, 0xF9C7),
	FIXED(0, 0xFB14), FIXED(0, 0xFC3B), FIXED(0, 0xFD3A), FIXED(0, 0xFE13), FIXED(0, 0xFEC4), FIXED(0, 0xFF4E), FIXED(0, 0xFFB1), FIXED(0, 0xFFEC),
	FIXED(1, 0x0000)
};

uint8_t UNK_602B4E4(int32_t arg0, int32_t arg1) {
	uint8_t retval;
	
	if (arg0 == 0) {
		if (arg1 < 0) {
			retval = 0x80;
		}
		else {
			retval = 0x00;
		}
	}
	else {
		if (arg1 == 0) {
			if (arg0 < 0) {
				retval = 0xC0;
			}
			else {
				retval = 0x40;
			}
		}
		else {
			int32_t var2 = arg0 * 0x20;
			int32_t var3 = arg1 * 0x20;
			if (arg0 <= 0) {
				int8_t var0;
				int8_t var1;
				if (arg1 < 0) {
					if (arg0 <= arg1) {
						var2 = var3 / arg0;
						return 0xBE - (int8_t)var2;
					}
					var2 /= arg1;
					var1 = (int8_t)var2;
					var0 = -0x7E;
				}
				else {
					if (-arg0 <= arg1) {
						var2 = (arg0 * -0x20) / arg1;
						return -(int8_t)var2 - 2;
					}
					var2 = var3 / -arg0;
					var1 = (int8_t)var2;
					var0 = -0x3E;
				}
				retval = var0 + var1;
			}
			else {
				if (arg1 <= 0) {
					if (-arg1 < arg0) {
						var2 = (arg1 * -0x20) / arg0;
						retval = (int8_t)var2 + 0x42;
					}
					else {
						var2 /= -arg1;
						retval = 0x7E - (int8_t)var2;
					}
				}
				else {
					if (arg0 < arg1) {
						var2 /= arg1;
						retval = (int8_t)var2 + 2;
					}
					else {
						var2 = var3 / arg0;
						retval = 0x3E - (int8_t)var2;
					}
				}
			}
		}
	}
	return retval;
}

static int16_t UNK_60357E4[32] = {
	0x019, 0x032, 0x04C, 0x065,
	0x07E, 0x098, 0x0B2, 0x0CC,
	0x0E6, 0x100, 0x11B, 0x137,
	0x152, 0x16E, 0x18B, 0x1A8,
	0x1C6, 0x1E4, 0x203, 0x223,
	0x244, 0x266, 0x288, 0x2AC,
	0x2D1, 0x2F7, 0x31F, 0x348,
	0x373, 0x3A0, 0x3CF, 0x400
};

uint8_t UNK_602B5C8(int32_t arg0, int32_t arg1) {
    if (arg0 == 0) {
        if (-1 < arg1) {
            return 0x00u;
        }
		else {
			return 0x80u;
		}
    }
    if (arg1 == 0) {
        if (-1 < arg0) {
            return 0x40u;
        }
		else {
			return 0xC0u;
		}
    }

    int8_t var0;
    int8_t var1 = -1;
    int32_t var2 = arg0;
    if (arg0 < 1) {
        if (-1 < arg1) {
            if (arg1 < -arg0) {
                var0 = -0x40;
                var1 = 1;
                var2 = arg1;
                arg1 = -arg0;
            }
            else {
                var0 = 0;
                var2 = -arg0;
            }
        }
        else if (arg0 <= arg1) {
            var0 = -0x40;
            var2 = arg1;
            arg1 = arg0;
        }
		else {
			var0 = -0x80;
			var1 = 1;
		}
    }
    else if (arg1 < 1) {
		arg1 = -arg1;
		if (arg1 < arg0) {
			var0 = 0x40;
			var1 = 1;
			var2 = arg1;
			arg1 = arg0;
		}
		else {
			var0 = -0x80;
			var1 = 1;
		}
	}
	else if (arg1 <= arg0) {
		var0 = 0x40;
		var2 = arg1;
		arg1 = arg0;
	}
	else {
		var0 = 0;
		var1 = 1;
    }

	int32_t var3 = (var2 << 10) / arg1;

    int16_t var4 = 0x10;
	for (int16_t var5 = 0, var6 = 0x20; var5 != var4 && var6 != var4;) {
		int16_t var7;
        if (var3 < UNK_60357E4[var4]) {
            var7 = var5;
            var6 = var4;
        }
		else {
			var7 = var4;
		}
        var4 = (var7 + var6) >> 1;
        var5 = var7;
    }
    return (uint8_t)((var4 + 1) * var1 + var0);
}

Fixed32 Sin(Angle angle) {
	if (angle < 64u) {
		return TrigTable[angle];
	}
	else if (angle >= 64u && angle < 128u) {
		return TrigTable[128u - angle];
	}
	else if (angle >= 128u && angle < 192u) {
		return (Fixed32) { .value = -TrigTable[angle - 128u].value };
	}
	else {
		return (Fixed32) { .value = -TrigTable[256u - angle].value };
	}
}

Fixed32 Cos(Angle angle) {
	if (angle < 64u) {
		return TrigTable[64u - angle];
	}
	else if (angle >= 64u && angle < 128u) {
		return (Fixed32) { .value = -TrigTable[angle - 64u].value };
	}
	else if (angle >= 128u && angle < 192u) {
		return (Fixed32) { .value = -TrigTable[192u - angle].value };
	}
	else {
		return TrigTable[angle - 192u];
	}
}

// This is reimplements an SH-2 multiply instruction that provides the full
// 64-bit result in two 32-bit registers of a multiply between two 32-bit
// integers. Only Rand uses the dmuls instruction. Use this only on platforms
// supporting 64-bit integers.
static void dmuls(int32_t * RmMACH, int32_t * RnMACL) {
	int64_t result = (int64_t)*RmMACH * (int64_t)*RnMACL;
	*RmMACH = result >> 32;
	*RnMACL = result & 0xFFFFFFFF;
}

// Another SH-2 instruction, only used by Rand.
// Considers Rn the upper 32 bits and Rm the lower 32 bits of a 64-bit integer,
// returning the middle 32 bits of that 64-bit integer.
static uint32_t xtrct(uint32_t Rm, uint32_t Rn) {
	return
		((Rm << 16) & 0xFFFF0000) |
		((Rn >> 16) & 0x0000FFFF);
}

// Shuffles each byte to a specified location.
//
// Taking a value, such as:
// 0x03020100
// And that value is big endian byte order:
// byte3 is the index where the upper 0x03 byte will go, byte2 is the index
// where the next lower 0x02 byte will go, etc.
//
// Example usage:
//
// RandSeed = SHUFFLE(RandSeed, 1, 0, 2, 3);
//
//                            RandSeed == 123456789ABCDEFGHIJKLMNOPQRSTUVW
// ((RandSeed >> 24) & 0xFF) << (1 << 3): 00000000000000001234567800000000
// ((RandSeed >> 16) & 0xFF) << (0 << 3): 0000000000000000000000009ABCDEFG
// ((RandSeed >>  8) & 0xFF) << (2 << 3): 00000000HIJKLMNO0000000000000000
// ((RandSeed >>  0) & 0xFF) << (3 << 3): PQRSTUVW000000000000000000000000
// SHUFFLE(RandSeed, 1, 0, 2, 3) == PQRSTUVWHIJKLMNO123456789ABCDEFG
// 
// Usage demonstrating the identity:
// SHUFFLE(RandSeed, 3, 2, 1, 0) == RandSeed
// Convert endianness, assuming 32-bit value size:
// SHUFFLE(RandSeed, 0, 1, 2, 3)
#define SHUFFLE(value, byte3, byte2, byte1, byte0) ( \
	(((((value) >> (8 * 3))) & 0xFF) << ((byte3) << 3)) | \
	(((((value) >> (8 * 2))) & 0xFF) << ((byte2) << 3)) | \
	(((((value) >> (8 * 1))) & 0xFF) << ((byte1) << 3)) | \
	(((((value) >> (8 * 0))) & 0xFF) << ((byte0) << 3)) \
)

uint32_t RandSeed = 0u;

uint32_t Rand(uint32_t upperBound) {
	RandSeed = RandScale * ((RandSeed << 3) - RandSeed + ScreenTime);
	RandSeed = SHUFFLE(RandSeed, 1, 0, 2, 3);

	// TODO: For SH-2 ports, change this part to generate result using inline
	// SH-2 assembly.
	int32_t m, n;
	m = Cos(RandSeed).value;
	n = RandSeed;
	dmuls(&m, &n);
	uint32_t result = xtrct(m, n);

	return result % upperBound;
}
