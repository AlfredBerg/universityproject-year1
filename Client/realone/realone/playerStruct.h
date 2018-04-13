#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#define NAMELENGTH 50

struct player {

	char name[NAMELENGTH];
	int life;
	int x;
	int y;
	int weaponID;

	SDL_Texture *Texture;
	SDL_Surface *Image;
}; 
typedef struct player Player;
