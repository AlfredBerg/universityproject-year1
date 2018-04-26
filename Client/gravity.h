#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "weapon.h"
#include "map.h"

void gravity(Player *player, Weapon weapons[], int *detectGround) {

	if (*detectGround == 0) {

		player->y += 10;

		for (int i = 0; i < MAXNRWEAPONS; i++) {
			if (weapons[i].isPickedUp == 1) {
				continue;
			}

			if (weapons[i].y < 500 && weapons[i].y > 0) {
				weapons[i].y += 10;
			}
			else if (weapons[i].y >= 500) {
				weapons[i].y = 500;
			}
			else {
				weapons[i].y += 20;
			}
		}
	}
	else {
		player->y = player->y;
		*detectGround = 1;
	}
}

