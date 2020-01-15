#include "ShowColumnMarker.h"
#include "Object.h"

void ShowColumnMarker(Player* player, int16_t col) {
	DisplayObject(
		OBJECT_COLUMNMARKER,
		player->screenPos[1] - (player->matrixHeight - 1) * 8 + (player->matrixHeight - MATRIX_HEIGHT + 1) * 8 - 6,
		player->screenPos[0] - (player->matrixWidth / 2) * 8 + col * 8,
		186u,
		115u);
}