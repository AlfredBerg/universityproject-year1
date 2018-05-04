#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "clientNetwork.h"
#include "projectile.h"
#include "player.h"
#include "game.h"

#define MAXNRWEAPONS 4

#define WEAPON_HEIGHT 60
#define WEAPON_WIDTH 60

struct weapon {
	int id;
	int x;
	int y;
	int dmg;
	int fireRate;
	int projectileType;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect;
	int isPickedUp;
};
typedef struct weapon Weapon;

void weaponActions(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]);
void pickUpWeapon(Network *client, Weapon weapons[], Player players[]);
void fireWeapon(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]);
Weapon createWeapon(Game *game, int id, int x, int y, int dmg, int fireRate, int projectileType, const char imageName[]);
Projectile createProjectile(Game *game, int id, int dmg, int speed, int w, int h, const char imageName[]);
void drawWeapons(Game *game, Player players[], Weapon weapons[]);
void drawProjectiles(Game *game, Projectile projectiles[]);