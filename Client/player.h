#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#define NAME_LENGTH 50
#define LEFT 2
#define RIGHT 3

struct player {
	char name[NAME_LENGTH];
	int life;
	int x;
	int y;
	int weaponID;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect;
};
typedef struct player Player;

void jump(Player *player, SDL_Rect *weapon, int *isJumping, int *jumpTime, int *doJump);
void walk(Player *player, SDL_Rect *weapon, int *prevKey);