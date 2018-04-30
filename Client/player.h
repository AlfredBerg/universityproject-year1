#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>


#define SPRITESPEED 3
#define WALKSPEED 10

#define NAME_LENGTH 50
#define LEFT 2
#define RIGHT 3

#define MAXPLAYERS 2
#define INVULNERABILITY_TIME 100

struct player {
	int id;
	char name[NAME_LENGTH];
	int life;
	int x;
	int y;
	int pickupID;
	int weaponID; //Place that weapon has in weapon array , -1 means no weapon
	int weaponFired;
	int tickThatWeaponFired;
	int tickThatLostHealth;
	int lastDirection;
	int isMoving;
	int previousX;
	int previousY;
	int currentSprite;
	SDL_Surface *Image;
	SDL_Texture *Texture;
	SDL_Rect rect;
};
typedef struct player Player;

void jump(Player *player, int *isJumping, int *jumpTime, int *doJump, int *groundDetected, int *roofDetected);
void walkRight(Player *player, int *key, int *prevKey);
void walkLeft(Player *player, int *key, int *prevKey);
void loseHealth(Player *player, int damage);
void playerHealthbar(Player players[MAXPLAYERS], SDL_Renderer* renderer);
void deletePlayer(Player players[], int id, int *nrOfPlayers);
void updatePlayerStates(Player players[], int loopCount);
