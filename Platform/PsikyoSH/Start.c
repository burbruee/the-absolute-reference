// TODO: Rewrite this as SH-2 assembler, without referring to the disassembled
// TAP code. Or just try to get this C code, with some inline SH-2 assembly, to
// work.

#include "Start.h"
#include "Game/HwSprite.h"
#include "PlatformTypes.h"
#include "Game/HwData.h"
#include "Game/Math.h"
#include "Platform/Util/Macros.h"
#include "Game/Macros.h"

// TODO: In the SH-2 assembly, define the interrupt vector table. It
// immediately precedes _start, and is in ROM address range 0x00000000 to 0x000003FF.

uint32_t MemCheckData[NUMMEMCHECKS];

volatile SpriteScale *Scales = (volatile SpriteScale*)SCALERAM;

// TODO: Define _start at 0x00000400 in ROM.
void _start() {
	// TODO: Init status register. It's or'd with 0xF0.

	// TODO: Analyze other PsikyoSH games, and test on hardware, to
	// understand what this is for. MAME source code might have info, too.
	_0xFFFFFFE0 = 0xA55A02E2;
	_0xFFFFFFE4 = 0xA55A00E4;
	_0xFFFFFFE8 = 0xA55A157F;
	_0xFFFFFFEC = 0xA55A8458;
	_0xFFFFFFF0 = 0xA55A0008;
	_0xFFFFFFF8 = 0xA55A0071;
	_0xFFFFFE92 = 0x10;
	_0xFFFFFE92 = 0x01;

	// For checks of memory other than the ROM, write a test pattern to
	// memory, then check that the pattern was written successfully, to
	// verify memory is functioning correctly. Finally, zero-init memory.
	uint32_t *memCheckData = MemCheckData;
	uint32_t memCheckStatus;

	{
		// Work RAM.
		memCheckStatus = MEMCHECKSTATUS_OKAY;
		register uint32_t randSeedInit; // Intentionally uninitialized. Whatever the data is after restart will be used.
		uint8_t ramInit = 0x00;
		for (size_t i = 0u; i < sizeof(WORKRAM); i++) {
			randSeedInit += ((uint8_t*)WORKRAM)[i];
			WORKRAM[i] = ramInit;
			if (++ramInit >= 0xFF) {
				ramInit = 0x00;
			}
		}

		ramInit = 0x00;
		for (size_t i = 0u; i < sizeof(WORKRAM); i++) {
			if (WORKRAM[i] != ramInit) {
				memCheckStatus = MEMCHECKSTATUS_NOGOOD;
				break;
			}
			
			if (++ramInit >= 0xFF) {
				ramInit = 0x00;
			}
		}

		for (size_t i = 0u; i < sizeof(WORKRAM); i++) {
			WORKRAM[i] = 0x00;
		}

		RandSeed = randSeedInit;
		*memCheckData++ = memCheckStatus;
	}

	{
		// Sprite and background RAM.
		memCheckStatus = MEMCHECKSTATUS_OKAY;
		uint8_t vramInit = 0x00;
		for (size_t i = 0u; i < sizeof(SPRITERAM) + sizeof(BGRAM); i++) {
			GRAPHICSRAM[i] = vramInit;
			if (++vramInit >= 0xFF) {
				vramInit = 0x00;
			}
		}

		uint8_t graphicsRAMInit = 0x00;
		for (size_t i = 0u; i < sizeof(SPRITERAM) + sizeof(BGRAM); i++) {
			if (GRAPHICSRAM[i] != graphicsRAMInit) {
				memCheckStatus = MEMCHECKSTATUS_NOGOOD;
				break;
			}

			if (++graphicsRAMInit >= 0xFF) {
				graphicsRAMInit = 0x00;
			}
		}

		for (size_t i = 0u; i < sizeof(SPRITERAM) + sizeof(BGRAM); i++) {
			SPRITERAM[i] = 0x00;
		}

		*memCheckData++ = memCheckStatus;
	}

	{
		// Palette RAM.
		memCheckStatus = MEMCHECKSTATUS_OKAY;
		for (size_t i = 0u; i < NUMPALS * NUMPALCOLORS_4BPP; i++) {
			PALRAM[i] = 0xA8A8A8A8;
		}

		for (size_t i = 0u; i < NUMPALS * NUMPALCOLORS_4BPP; i++) {
			if ((PALRAM[i] & 0xFCFCFC00) != 0xA8A8A800) {
				memCheckStatus = MEMCHECKSTATUS_NOGOOD;
				break;
			}
		}

		for (size_t i = 0u; i < sizeof(PALRAM); i++) {
			((uint8_t*)PALRAM)[i] = 0x00;
		}

		*memCheckData++ = memCheckStatus;
	}

	{
		// Scale RAM.
		memCheckStatus = MEMCHECKSTATUS_OKAY;
		for (size_t i = 0u; i < sizeof(SCALERAM) / 2; i++) {
			Scales[i] = 0x0000;
		}

		for (size_t i = 0u; i < sizeof(SCALERAM) / 2; i++) {
			if (Scales[i] != i) {
				memCheckStatus = MEMCHECKSTATUS_NOGOOD;
				break;
			}
		}

		for (size_t i = 0u; i < sizeof(SCALERAM) / 2; i++) {
			Scales[i] = 0x0000;
		}

		*memCheckData = memCheckStatus;
		// There should be EEP-ROM checking after scale RAM
		// checking/init, but it appears to be removed, so this steps
		// over EEP-ROM memory check data, to ROM checksum memory check
		// data. So this results in the "OK" status for EEP-ROM being a
		// lie, since it was never checked. This check was probably
		// removed because EEP-ROMs have a limited write lifecycle, so
		// writes should be minimized.
		//
		// If the EEP-ROM check was added, there should be a button
		// combo that enables it, so normally the check wouldn't be
		// run.
		// TODO: Check if TGM2 contains the code for EEP-ROM checking. And if
		// not, maybe check other PsikyoSH games for it.
		memCheckData += 2;
	}

	// Calculate checksum of the ROM preceding the program init data, then
	// write the program code and init data to RAM.
	// TODO: Some setup via an ld script will be required for this to work.
	// TODO: This might have to be refactored, since GCC might initialize
	// data itself, some other way, since the initialization is defined in the code.
	// TODO: Provide a function to calculate the checksum in Platform/Util.
	{
		uint32_t romChecksum = 0x00000000;
		volatile const uint32_t *romEnd = &(volatile const uint32_t*)((*ProgramInitPtr)[*ProgramInitSize]);
		for (volatile const uint32_t *romData = (volatile const uint32_t*)ROM; romData < romEnd; romData++) {
			romChecksum += *romData;
		}
		*memCheckData = romChecksum;

		// TODO: Figure out what this is doing.
		*HWADDR(uint16_t, -0x1A0) = 0x0300;
		*HWADDR(uint16_t, -0x19A) = 0x007F;

		volatile const uint32_t *programCodeSrc = *ProgramCodeSrcPtr;
		volatile const uint32_t* programCodeDst = *ProgramCodeDstPtr;
		size_t i = (*ProgramCodeSize & ~(sizeof(uint32_t) - 1u) + sizeof(uint32_t)) / sizeof(uint32_t);
		do {
			i--;
			programCodeDst[i] = programCodeSrc[i];
		} while (i != 0u);

		volatile const uint32_t *programInitSrc = &(volatile const uint32_t*)*ProgramInitSrcPtr;
		volatile const uint32_t *programInitDst = *ProgramInitDstPtr;
		size_t i = (*ProgramInitSize & ~(sizeof(uint32_t) - 1u) + sizeof(uint32_t)) / sizeof(uint32_t);
		do {
			i--;
			programInitDst[i] = programInitSrc[i];
		} while (i != 0u);
	}

	// TODO: Init status register with inline assembly. It's and'd with 0x00.

	main();

	// NOTE: Halt is set as an interrupt handler, at interrupt table index 4.
	Halt: goto Halt;
}
