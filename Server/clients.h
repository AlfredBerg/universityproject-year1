#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

#include "serverNetwork.h"

#define MAX_CLIENTS 4 

struct client {
	IPaddress ip;
	int inUse;
	int xPos;
	int yPos;
	int health;
};
typedef struct client Client;

