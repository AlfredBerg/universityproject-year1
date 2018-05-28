#include "pickup.h"
#include "weapon.h"
#include <stdlib.h>
#include <time.h>

extern SDL_Rect camera;

#define HANDPROJECTILE 1

void weaponActions(Weapon weapons[], Player players[], Network *client, Projectile projectiles[], int playerID, SDL_Rect *camera) {
	
	
	fireWeapon(weapons, players, client, projectiles);

	pickUpWeapon(client, weapons, players);

	for (int i = 0; i < MAXPROJECTILES; i++) {
		if (i != 1) {
			detectProjectileColision(&projectiles[i], players, i);
		}
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
	}

	//Update position
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		weapons[i].rect.x = weapons[i].x;
		weapons[i].rect.y = weapons[i].y;
	}
}

void fireWeapon(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]) {
	if (players[client->playerID].weaponID == 2) {
		if (SDL_TICKS_PASSED(SDL_GetTicks(), players[client->playerID].tickThatWeaponFired + weapons[players[client->playerID].weaponID].fireRate)) {
			detectHandColision(&projectiles[1], players, 1, client->playerID, &camera);
			players[client->playerID].tickThatWeaponFired = SDL_GetTicks();
		}
}
	else if (players[client->playerID].weaponFired == 1) {
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
		if (weaponId == 0 || weaponId == 1 || weaponId == 4  || weaponId == 5) {
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

		if (players[client->playerID].lastDirection == LEFT) {
			//if (players[client->playerID].weaponID == 2)
			//sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x + 50, weapons[weaponId].y - (WINDOW_WIDTH / 2), RIGHT);

			//	else
			sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x - 30, weapons[weaponId].y, LEFT);
		}
		else if (players[client->playerID].lastDirection == RIGHT) {
			//if (players[client->playerID].weaponID == 2)
			//sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x + 50, weapons[weaponId].y - (WINDOW_WIDTH / 2), RIGHT);

			//	else
			sendBulletToServer(client, weapons[weaponId].projectileType, weapons[weaponId].x + 50, weapons[weaponId].y, RIGHT);
		}
		else {
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

void moveItemsFromMapCollision(Tile map[][MAP_WIDTH], Weapon weapons[], Pickup pickups[], Player players[]) {
	int wCollision = 1, pCollision = 1, playerCollision = 1;
	while (wCollision || pCollision || playerCollision) {
		wCollision = 0;
		pCollision = 0;
		playerCollision = 0;
		for (int i = 0; i < MAXNRWEAPONS; i++) {
			if(collisionWithMap(map, weapons[i].rect ) && weapons[i].isPickedUp < 1) {
				weapons[i].y -= 10;
				weapons[i].rect.y -= 10;
				wCollision++;
			}
		}
		for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
			if (collisionWithMap(map, pickups[i].rect) && pickups[i].isPickedUp < 1) {
				pickups[i].y -= 10;
				pickups[i].rect.y -= 10;
				pCollision++;
			}
		}
		for (int i = 0; i < MAXPLAYERS; i++) {
			if (collisionWithMap(map, players[i].dstRect)) {
				players[i].y -= 10;
				players[i].dstRect.y -= 10;
				playerCollision++;
			}
		}
	}
}
