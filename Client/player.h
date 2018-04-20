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
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect p1;
};
typedef struct player Player;

void jump(Player *player, SDL_Rect *weapon, int *isJumping, int *jumpTime, int *doJump);
void walk(Player *player, SDL_Rect *weapon, int *prevKey);
