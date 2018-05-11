#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "player.h"
#include "clientNetwork.h"
#include "game.h"

#define MAX_NR_OF_PICKUPS 6
#define PICKUP_WIDTH 32
#define PICKUP_HEIGHT 32

struct pickup {
	int id, x, y, healing;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Rect rect;
	int isPickedUp;
};
typedef struct pickup Pickup;

void pickUpPickup(Pickup pickups[], Player players[]);
Pickup createPickup(Game *game, int id, int x, int y, int healing, const char imageName[]);
void deletePickup(Pickup pickups[], int pickupIDtoDelete, int *nrOfPickups);
void drawPickups(Game *game, Pickup pickups[], int *nrOfPickups);