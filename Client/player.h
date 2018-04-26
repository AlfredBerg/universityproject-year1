#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#define TWONUMBERS 2

#define NAME_LENGTH 50
#define LEFT 2
#define RIGHT 3

#define MAXPLAYERS 2
#define INVULNERABILITY_TIME 100

struct player {
	char name[NAME_LENGTH];
	int life;
	int x;
	int y;
	int weaponID; //Place that weapon has in weapon array , -1 means no weapon
	int weaponFired;
	int tickThatWeaponFired;
	int tickThatLostHealth;
	int lastDirection;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect;
};
typedef struct player Player;

void jump(Player *player, int *isJumping, int *jumpTime, int *doJump);
void walk(Player *player, int *key, int *enableWalk, int *prevKey);
void loseHealth(Player *player, int damage);
int handleCollision(Player *player, int tileX, int tileY, int *enableWalk, int *prevKey);
