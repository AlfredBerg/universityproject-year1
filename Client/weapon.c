#include "weapon.h"
#include <stdlib.h>
#include <time.h>
#define HANDPROJECTILE 1

void weaponActions(Weapon weapons[], Player players[], Network *client, Projectile projectiles[], int playerID) {
	fireWeapon(weapons, players, client, projectiles);

	pickUpWeapon(client, weapons, players);
	if (players[client->playerID].weaponID == 2) {
		detectHandColision(&projectiles[1], players, 1, playerID);
	}
	else {
		detectProjectileColision(&projectiles[0], players, 0);
	}


	int id;
	for (int i = 0; i < MAXPLAYERS; i++) {
		id = players[i].weaponID;
		weapons[id].isPickedUp = 1;
	}


	//Move with player if picked up
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			for (int j = 0; j < MAXPLAYERS; j++) {
				if (players[j].lastDirection == LEFT) {
					if (weapons[i].id == players[j].weaponID) {
						weapons[i].x = players[j].x - players[j].dstRect.w;
						weapons[i].y = players[j].y + players[j].dstRect.h / 3;
					}

				}
				else {
					if (weapons[i].id == players[j].weaponID) {
						weapons[i].x = players[j].x + players[j].dstRect.w - 20;
						weapons[i].y = players[j].y + players[j].dstRect.h / 3;
					}
				}

			}

		}
		else {

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
		if (weaponId == -1) {
			return;
		}

		players[client->playerID].weaponFired = 0;

		if (!SDL_TICKS_PASSED(SDL_GetTicks(), players[client->playerID].tickThatWeaponFired + weapons[weaponId].fireRate)) {
			//Detects if the weapon should fire
			return;
		}

		printf("Shots fired!\n");


		//Sound effects for gun
		if (weaponId == 0 || weaponId == 1) {
			int gunshotChoice = rand() % 3 + 1;
			char gunshotPath[30];

			if (gunshotChoice == 1)
				sprintf(gunshotPath, "assets/gunshot1.wav");
			else if (gunshotChoice == 2)
				sprintf(gunshotPath, "assets/gunshot2.wav");
			else if (gunshotChoice == 3)
				sprintf(gunshotPath, "assets/gunshot3.wav");

			Mix_Chunk *gunshot = Mix_LoadWAV(gunshotPath);
			gunshot->volume = 80;
			Mix_PlayChannel(1, gunshot, 0);
		}

		//Offline code
		/*
		if (players[client->playerID].lastDirection == LEFT) {
		fireProjectile(&projectiles[weapons[weaponId].projectileType], LEFT, weapons[weaponId].x - 30, weapons[weaponId].y, 1);
		}
		else {
		fireProjectile(&projectiles[weapons[weaponId].projectileType], RIGHT, weapons[weaponId].x + 70, weapons[weaponId].y, 1);
		}
		*/
		if (players[client->playerID].lastDirection == LEFT) {
			sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x - 30, weapons[weaponId].y, LEFT);
		}
		else {
			sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x + 50, weapons[weaponId].y, RIGHT);
		}

		players[client->playerID].tickThatWeaponFired = SDL_GetTicks();
	}
}

void pickUpWeapon(Network *client, Weapon weapons[], Player players[]) {
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		if (weapons[i].isPickedUp == 1) {
			continue;
		}

		for (int j = 0; j < MAXPLAYERS; j++) {
			if (SDL_HasIntersection(&players[j].dstRect, &weapons[i].rect)) {
				printf("Pickup weapon\n");
				sendPickupToServer(client, 0, weapons[i].id);
				weapons[i].isPickedUp = 1;
			}
		}

	}
}

Weapon createWeapon(Game *game, int id, int x, int y, int dmg, int fireRate, int projectileType, const char imageName[]) {
	Weapon weapon;
	weapon.id = id;
	weapon.x = x;
	weapon.y = y;
	weapon.dmg = dmg;
	weapon.fireRate = fireRate;
	weapon.projectileType = projectileType;
	weapon.Image = IMG_Load(imageName);
	weapon.Texture = SDL_CreateTextureFromSurface(game->renderer, weapon.Image);
	SDL_FreeSurface(weapon.Image);
	weapon.rect.x = x;
	weapon.rect.y = y;
	weapon.rect.w = WEAPON_WIDTH;
	weapon.rect.h = WEAPON_HEIGHT;
	weapon.isPickedUp = 0;
	return weapon;
}

Projectile createProjectile(Game *game, int id, int dmg, int speed, int w, int h, const char imageName[]) {
	Projectile projectile;
	projectile.id = id;
	projectile.dmg = dmg;
	projectile.speed = speed;
	projectile.projectilesFired = 0;
	projectile.w = w;
	projectile.h = h;
	projectile.Image = IMG_Load(imageName);
	projectile.Texture = SDL_CreateTextureFromSurface(game->renderer, projectile.Image);
	SDL_FreeSurface(projectile.Image);
	return projectile;
}

void drawWeapons(Game *game, Player players[], Weapon weapons[]) {
	for (int j = 0; j < MAXNRWEAPONS; j++) {
		if (weapons[j].isPickedUp == 1) {
			for (int i = 0; i < MAXPLAYERS; i++) {
				if (weapons[j].id == players[i].weaponID) {
					if (players[i].lastDirection == LEFT) {
						renderCopyMoveWithCamera(game->renderer, weapons[j].Texture, NULL, &weapons[j].rect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
					}
					else {
						renderCopyMoveWithCamera(game->renderer, weapons[j].Texture, NULL, &weapons[j].rect, 0.0, NULL, NULL);
					}
				}

			}

		}
		else {
			renderCopyMoveWithCamera(game->renderer, weapons[j].Texture, NULL, &weapons[j].rect, 0.0, NULL, NULL);
		}
	}


}


void drawProjectiles(Game *game, Projectile projectiles[]) {
	for (int i = 0; i < MAXPROJECTILES; i++) {
		for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
			if (projectiles[i].direction[j] == LEFT) {
				renderCopyMoveWithCamera(game->renderer, projectiles[i].Texture, NULL, &projectiles[i].rect[j], 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else {
				renderCopyMoveWithCamera(game->renderer, projectiles[i].Texture, NULL, &projectiles[i].rect[j], 0.0, NULL, NULL);
			}

		}
	}
}