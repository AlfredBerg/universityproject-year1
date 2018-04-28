#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

#include "serverNetwork.h"

#define MAX_CLIENTS 4 
#define MAXPROJECTILEOBJECTS 50

#define BULLET 0
struct projectiles {
	int id;
	int projectileType;
	int x;
	int y;
	int direction;
};
typedef struct projectiles Projectiles;

struct projectileData {
	int id;
	int dmg;
	int speed;
	int w;
	int h;
	int nrProjectilesShot;
	Projectiles Projectiles[MAXPROJECTILEOBJECTS];
};
typedef struct projectileData ProjectileData;


struct client {
	IPaddress ip;
	int inUse;
	int xPos;
	int yPos;
	int health;
};
typedef struct client Client;

