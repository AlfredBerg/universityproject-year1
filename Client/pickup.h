#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "player.h"
#include "clientNetwork.h"
#include "game.h"

#define MAX_NR_OF_PICKUPS 2

struct pickup {
	int id, x, y, healing;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Rect rect;
	int isPickedUp;
};
typedef struct pickup Pickup;

void pickUpPickup(Pickup pickups[], Player players[]);
Pickup createPickup(Game *game, int id, int x, int y, int healing, const char imageName[], int rectW, int rectH);
void deletePickup(Pickup pickups[], int pickupIDtoDelete, int *nrOfPickups);
void drawPickup(Game *game, Pickup pickups[], int *nrOfPickups);