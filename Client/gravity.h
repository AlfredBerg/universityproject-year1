#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "weapon.h"
#include "map.h"

#define GRAVITYSPEED 10

void gravity(Player *player, Weapon weapons[], int *groundDetected, int *roofDetected, Tile map[][MAP_WIDTH]) {

	if (!*groundDetected) {
		player->y += GRAVITYSPEED;;
	}
	else {
		*groundDetected = 1;
		*roofDetected = 0;
	}

	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			continue;
		}

		if (!checkOnlyGround(map, &weapons[i].rect) && weapons[i].y > -20 && weapons[i].y < 600) {
			weapons[i].y += GRAVITYSPEED;
		}
	}
}