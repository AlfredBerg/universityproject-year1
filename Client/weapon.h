#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define MAXNRWEAPONS 1

struct weapon {
	int id;
	int x;
	int y;
	int dmg;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect;
	int isPickedUp;
};
typedef struct weapon Weapon;

void weaponActions(Weapon weapons[], Player players[]);
