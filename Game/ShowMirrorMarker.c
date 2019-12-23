#include "ShowMirrorMarker.h"
#include "Object.h"

void ShowMirrorMarker(Player* player, int16_t col) {
	DisplayObject(
		OBJECT_MIRRORMARKER,
		player->fieldPos[1] - (player->matrixHeight - 1) * 8 + (player->matrixHeight - MATRIX_HEIGHT + 1) * 8 - 6,
		player->fieldPos[0] - (player->matrixWidth / 2) * 8 + col * 8,
		186u,
		115u);
}