#pragma once
#include "player.h"
#include "weapon.h"
#include "clientNetwork.h"
#include "projectile.h"

#define MAXBULLETS 100

void pickUpWeapon(Weapon weapons[], Player players[]);
void fireWeapon(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]);

void weaponActions(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]) {
	fireWeapon(weapons, players, client, projectiles);

	pickUpWeapon(weapons, players);
	
	//Move with player if picked up
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			for (int j = 0; j < MAXPLAYERS; j++) {
				if (weapons[i].id == players[j].weaponID) {
					weapons[i].x = players[j].x + 30;
					weapons[i].y = players[j].y + 50;
				}
			}

		}

	}

	//Update position
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		weapons[i].rect.x = weapons[i].x;
		weapons[i].rect.y = weapons[i].y;
	}



}

void fireWeapon(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]) {
	if (players[client->playerID].weaponFired == 1) {
		int weaponId = players[client->playerID].weaponID;

		players[client->playerID].weaponFired = 0;

		if (!SDL_TICKS_PASSED(SDL_GetTicks(), players[client->playerID].tickThatWeaponFired + weapons[weaponId].fireRate)) {
			//Detects if the weapon should fire
			return;
		}

		printf("Shots fired!\n");

		fireProjectile(&projectiles[weapons[weaponId].projectileType], 1, weapons[weaponId].x, weapons[weaponId].y);

		players[client->playerID].tickThatWeaponFired = SDL_GetTicks();
	}
}

void pickUpWeapon(Weapon weapons[], Player players[]) {
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			continue;
		}
		
		for (int j = 0; j < MAXPLAYERS; j++) {
			if (SDL_HasIntersection(&players[j].rect, &weapons[i].rect)) {
				printf("Pickup\n");
				players[j].weaponID = weapons[i].id;
				weapons[i].isPickedUp = 1;
			}
		}

	}
}