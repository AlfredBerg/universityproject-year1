#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "weapon.h"
#include "map.h"

#define GRAVITYSPEED 10

void gravity(Player *player, Weapon weapons[], int *groundDetected) {

	if (*groundDetected == 0) {
		player->y += GRAVITYSPEED;
	}
	else {
		player->y = player->y;
		*groundDetected = 1;
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

