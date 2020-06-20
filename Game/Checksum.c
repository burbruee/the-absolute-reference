#include "Checksum.h"

// TODO: Implement a way for the build system to calculate the checksums and
// put them here, if possible.
const ChecksumData Checksum = {
	{ '#', 'C', 'H', 'E', 'C', 'K', 'S', 'U', 'M' },
	{
		CHECKSUM_NORMAL,
			0x5EBDu, 0x643Au, 0xC2F7u,
		CHECKSUM_NORMAL,
			0xB114u, 0x0ECEu, 0xBFE2u,
		CHECKSUM_NORMAL,
			0x5DE2u, 0x03A2u, 0x6184u,
		CHECKSUM_NORMAL,
			0x9EA2u, 0x991Au, 0x37BCu,
		CHECKSUM_NORMAL,
			0x90D9u, 0x2CB4u, 0xBD8Du,
		CHECKSUM_NORMAL,
			0xBFDDu, 0xA8F8u, 0x68D5u,
		CHECKSUM_NORMAL,
			0xC132u, 0x4EF4u, 0x1026u,
		CHECKSUM_NORMAL,
			0x001Bu, 0x18D5u, 0x18F0u,
		CHECKSUM_NORMAL | CHECKSUM_LAST,
			0x0000u, 0x0000u, 0x2888u,
		CHECKSUM_END,
			0x0000u, 0x0000u, 0x0000u
	}
};