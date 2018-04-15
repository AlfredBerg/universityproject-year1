#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

struct client {
	int inUse;
	int xPos;
	int yPos;
	int health;
};
typedef struct client Client;