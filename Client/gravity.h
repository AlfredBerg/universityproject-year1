#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "weapon.h"

void gravity(Player *player, SDL_Rect *weapon, Weapon weapons[]) {
	if (player->y < 480 && player->y > 0) {
		player->y += 10;
		weapon->y = player->y + 50;
	}
	else if (player->y >= 480) {
		player->y = 480;
		weapon->y = player->y + 50;
	}
	else {
		player->y += 20;
		weapon->y = player->y + 50;
	}

	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			continue;
		}

		if (weapons[i].y < 580 && weapons[i].y > 0) {
			weapons[i].y += 10;
		}
		else if (weapons[i].y >= 580) {
			weapons[i].y = 580;
		}
		else {
			weapons[i].y += 20;
		}
	}
}
