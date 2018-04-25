#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "player.h"
#include "clientNetwork.h"

#define MAX_NR_OF_PICKUPS 1

struct pickup {
	int id, x, y, healing;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Rect rect;
	int isPickedUp;
};
typedef struct pickup Pickup;
