#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define MAXPROJECTILES 1
#define MAXPROJECTILEOBJECTS 100

struct projectile {
	int id;
	int dmg;
	int speed;
	int projectilesFired;
	int w;
	int h;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect[MAXPROJECTILEOBJECTS];
	int direction[MAXPROJECTILEOBJECTS];
};
typedef struct projectile Projectile;

void moveProjectiles(Projectile projectiles[]);
void fireProjectile(Projectile *projectile, int direction, int x, int y);

