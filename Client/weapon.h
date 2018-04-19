#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
struct weapon {
	int id;
	int x;
	int y;
	int dmg;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	int isPickedUp;
};
typedef struct weapon Weapon;
