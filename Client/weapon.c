#pragma once
#include "player.h"
#include "weapon.h"

void pickUpWeapon(Weapon weapons[], Player players[]);

void weaponActions(Weapon weapons[], Player players[]) {
	//fireWeapon();
	pickUpWeapon(weapons, players);
	

	for (int i = 0; i < MAXNRWEAPONS; i++) {
		weapons[i].rect.x = weapons[i].x;
		weapons[i].rect.y = weapons[i].y;
	}

}

void pickUpWeapon(Weapon weapons[], Player players[]) {
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			continue;
		}
		
		for (int j = 0; j < MAXPLAYERS; j++) {
			if (SDL_HasIntersection(&players[j].rect, &weapons[i].rect)) {
				players[i].weaponID = weapons[j].id;
				weapons[j].isPickedUp = 1;
			}
		}

	}
}