#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "weapon.h"
#include "map.h"

#define GRAVITYSPEED 10

void gravity(Player *player, Weapon weapons[], int *groundDetected, int *roofDetected) {

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

		if (weapons[i].y < 500 && weapons[i].y > 0) {
			weapons[i].y += GRAVITYSPEED;
		}
		else if (weapons[i].y >= 500) {
			weapons[i].y = 500;
		}
		else {
			weapons[i].y += 20;
		}
	}
}

