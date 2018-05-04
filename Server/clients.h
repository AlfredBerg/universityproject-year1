#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

#include "serverNetwork.h"

#define MAX_CLIENTS 3
#define MAXPROJECTILEOBJECTS 50

#define MAXNRWEAPONS 2
#define MAX_NR_OF_PICKUPS 2

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

struct weapons {
	int id;
	int isPickedUp;
	int owner;
};
typedef struct weapons Weapons;

struct pickups {
	int id;
	int isPickedUp;
	int owner;
};
typedef struct pickups Pickups;

struct items {
	Weapons weapons[MAXNRWEAPONS];
	Pickups pickups[MAX_NR_OF_PICKUPS];
};
typedef struct items Items;

struct client {
	IPaddress ip;
	int inUse;
	int xPos;
	int yPos;
	int weaponId;
	int pickupId;
	int health;
};
typedef struct client Client;

