#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>


#define MAXPROJECTILES 3
#define MAXPROJECTILEOBJECTS 50

struct objects {
	int whoShot;
};
typedef struct objects Objects;

struct projectile {
	int id;
	int dmg;
	int speed;
	int projectilesFired;
	int w;
	int h;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	Objects projectileInfo[MAXPROJECTILEOBJECTS];
	SDL_Rect rect[MAXPROJECTILEOBJECTS];
	int direction[MAXPROJECTILEOBJECTS];
};
typedef struct projectile Projectile;

void moveProjectiles(Projectile projectiles[]);
void fireProjectile(Projectile *projectile, int direction, int x, int y, int id);
void detectProjectileColision(Projectile *projectile, Player players[], int projectileType);
void detectHandColision(Projectile *projectile, Player players[], int projectileType, int playerID, SDL_Rect *camera);
void detectFistColision(Projectile *projectile, Player players[], int playerID);


