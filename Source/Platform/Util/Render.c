#include "Platform/Util/Render.h"
#include "Video/VideoDefs.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "HwData.h"
#include "Lib/Macros.h"
#include <stdbool.h>
#include <string.h>

#define PIXELALPHA (UINT32_C(~0))

static const uint32_t AlphaTable[0x100] = {
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0xFBu, 0xF7u, 0xF3u, 0xEFu, 0xEBu, 0xE7u, 0xE3u, 0xDFu, 0xDBu, 0xD7u, 0xD3u, 0xCFu, 0xCBu, 0xC7u, 0xC3u,
	0xBEu, 0xBAu, 0xB6u, 0xB2u, 0xAEu, 0xAAu, 0xA6u, 0xA2u, 0x9Eu, 0x9Au, 0x96u, 0x92u, 0x8Eu, 0x8Au, 0x86u, 0x82u,
	0x7Du, 0x79u, 0x75u, 0x71u, 0x6Du, 0x69u, 0x65u, 0x61u, 0x5Du, 0x59u, 0x55u, 0x51u, 0x4Du, 0x49u, 0x45u, 0x41u,
	0x3Cu, 0x38u, 0x34u, 0x30u, 0x2Cu, 0x28u, 0x24u, 0x20u, 0x1Cu, 0x18u, 0x14u, 0x10u, 0x0Cu, 0x08u, 0x04u, 0x00u
};

void Render(Color* const framebuffer, const uint8_t* const tileData) {
	// TODO: Implement line drawing to replace this.
	for (size_t i = 0u; i < VIDEO_HEIGHT; i++) {
		for (size_t j = 0u; j < VIDEO_WIDTH; j++) {
			framebuffer[i * VIDEO_WIDTH + j] = COLOR(128u, 128u, 128u, 0u);
		}
	}

	if (!(SpriteNames[0] & SPRITENAME_TERMINATE) && !(SpriteNames[1] & SPRITENAME_TERMINATE)) {
		for (size_t i = SPRITE_FIRST; i < MAXSPRITES; i++) {
			const SpriteData* const sprite = &Sprites[SpriteNames[i] & ~SPRITENAME_TERMINATE];
			const int16_t y = OBJECT_GETY(sprite);
			const int16_t x = OBJECT_GETX(sprite);
			const bool flipY = OBJECT_GETFLIPY(sprite);
			//const bool unknown0 = OBJECT_GETUNKNOWN0(sprite);
			//const uint8_t sprPri = OBJECT_GETSPRPRI(sprite);
			const uint32_t h = OBJECT_GETH(sprite) + 1u;
			const int32_t scaleY = SCALERAM[OBJECT_GETSCALEY(sprite)];
			const bool flipX = OBJECT_GETFLIPX(sprite);
			//const bool unknown1 = OBJECT_GETUNKNOWN1(sprite);
			//const uint8_t bgPri = OBJECT_GETBGPRI(sprite);
			const uint32_t w = OBJECT_GETW(sprite) + 1u;
			const uint32_t scaleX = SCALERAM[OBJECT_GETSCALEX(sprite)];
			const uint32_t palNum = OBJECT_GETPALNUM(sprite);
			const Bpp bpp = OBJECT_GETBPP(sprite);
			uint32_t alphaTemp = Alpha[OBJECT_GETALPHA(sprite)];
			if (alphaTemp & 0x80u) {
				alphaTemp = PIXELALPHA;
			}
			else {
				alphaTemp = 0x3Fu - (alphaTemp & 0x3Fu);
				alphaTemp = (alphaTemp << 2) | (alphaTemp >> 4);
			}
			const uint32_t alpha = alphaTemp;
			const uint32_t tile = OBJECT_GETTILE(sprite);

			const int16_t renderW = (((w << 24) / scaleX) + 512) >> 10;
			const int16_t renderH = (((h << 24) / scaleY) + 512) >> 10;
			for (int16_t offsetY = 0; offsetY < renderH; offsetY++) {
				for (int16_t offsetX = 0; offsetX < renderW; offsetX++) {
					// Skip offscreen pixels.
					if (
						x + offsetX < 0 ||
						x + offsetX >= VIDEO_WIDTH ||
						y + offsetY < 0 ||
						y + offsetY >= VIDEO_HEIGHT
					) {
						continue;
					}

					const int32_t sourceX = ((flipX ? renderW - 1 - offsetX : offsetX) * scaleX) / 1024;
					const int32_t sourceY = ((flipY ? renderH - 1 - offsetY : offsetY) * scaleY) / 1024;
					const int32_t tileX = sourceX / 16;
					const int32_t tileY = sourceY / 16;
					const int32_t tileOffsetX = sourceX % 16;
					const int32_t tileOffsetY = sourceY % 16;
					uint8_t palOffset = (
						tileData[
							((tile + w * tileY) * 0x80u * (bpp + 1)) -
							0xC00000u +
							(tileOffsetX / (2 - bpp)) +
							(tileX * 0x80u * (bpp + 1u)) +
							(tileOffsetY * 8u * (bpp + 1u))
						] >> ((4 - ((tileOffsetX & 1) * 4)) * (1u - bpp))
					) & ((0xF0u * bpp) | 0x0Fu);

					if (palOffset == 0u) {
						continue;
					}
					else {
						const Color* const color = &PALRAM[palNum * NUMPALCOLORS_4BPP + palOffset];
						uint32_t blendAlpha = ((alpha == PIXELALPHA) ? AlphaTable[palOffset] : alpha) & 0xFFu;
						Color* const pixel = &framebuffer[(y + offsetY) * VIDEO_WIDTH + x + offsetX];
						*pixel = COLOR(
							((uint32_t)COLOR_GETR(*color) * blendAlpha + (uint32_t)COLOR_GETR(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETG(*color) * blendAlpha + (uint32_t)COLOR_GETG(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							((uint32_t)COLOR_GETB(*color) * blendAlpha + (uint32_t)COLOR_GETB(*pixel) * (0xFFu - blendAlpha)) / 0xFFu,
							0u
						);
					}
				}
			}
			if (SpriteNames[i] & SPRITENAME_TERMINATE) {
                break;
			}
		}
	}
}
