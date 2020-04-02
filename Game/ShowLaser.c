#include "ShowLaser.h"
#include "DisplayObject.h"
#include "Object.h"
#include "Entity.h"
#include "Frame.h"

#define laserFrames values[0]
#define laserAnimIndex values[1]

void ShowLaser(Player* player, int16_t row, int16_t col) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityLaser;
		ENTITY_DATA(entity).player = player;
		entity->laserFrames = 0;
		entity->laserAnimIndex = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = player->screenPos[0] - player->matrixWidth * 8 - 4 + col * 8;
		data->y = player->screenPos[1] - (player->matrixHeight - 1) * 8 + (player->matrixHeight - row - 1) * 8 - 6;
		data->objectTable = OBJECTTABLE_LASER;
		data->palNum = 180u;
	}
}

void UpdateEntityLaser(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->laserAnimIndex], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME) {
		if (++entity->laserFrames < 10) {
			entity->laserAnimIndex++;
		}
		else if (entity->laserFrames < 14) {
			if (entity->laserFrames & 1) {
				entity->laserAnimIndex++;
			}
		}
		else if (entity->laserFrames & 2) {
			entity->laserAnimIndex++;
		}

		if (entity->laserAnimIndex >= 16) {
			FreeEntity(entity);
		}
	}
}

static const ObjectData* ObjectTableLaserReticle[16] = {
	&OBJECTTABLE_LASERRETICLE[0],
	&OBJECTTABLE_LASERRETICLE[1],
	&OBJECTTABLE_LASERRETICLE[2],
	&OBJECTTABLE_LASERRETICLE[3],
	&OBJECTTABLE_LASERRETICLE[4],
	&OBJECTTABLE_LASERRETICLE[5],
	&OBJECTTABLE_LASERRETICLE[6],
	&OBJECTTABLE_LASERRETICLE[7],
	&OBJECTTABLE_LASERRETICLE[8],
	&OBJECTTABLE_LASERRETICLE[9],
	&OBJECTTABLE_LASERRETICLE[10],
	&OBJECTTABLE_LASERRETICLE[11],
	&OBJECTTABLE_LASERRETICLE[12],
	&OBJECTTABLE_LASERRETICLE[13],
	&OBJECTTABLE_LASERRETICLE[14],
	&OBJECTTABLE_LASERRETICLE[15]
};

void ShowLaserReticle(Player* player, int16_t row, int16_t col, int16_t animIndex) {
	if (animIndex < 0) {
		animIndex = 0;
	}
	if (animIndex > 15) {
		animIndex = 15;
	}
	DisplayObject(
		ObjectTableLaserReticle[15 - animIndex],
		player->screenPos[1] + (player->matrixHeight - row) * 8 - (player->matrixHeight - 1) * 8 - 10,
		player->screenPos[0] + col * 8 - (player->matrixWidth / 2) * 8 + 4,
		181u,
		124u);
}
