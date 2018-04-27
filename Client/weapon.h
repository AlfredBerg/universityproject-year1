#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "clientNetwork.h"
#include "projectile.h"
#include "player.h"
#include "game.h"

#define MAXNRWEAPONS 1
#define MAXBULLETS 100

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
void pickUpWeapon(Weapon weapons[], Player players[]);
void fireWeapon(Weapon weapons[], Player players[], Network *client, Projectile projectiles[]);
Weapon createWeapon(Game *game, int id, int x, int y, int dmg, int fireRate, int projectileType, const char imageName[], int rectW, int rectH);