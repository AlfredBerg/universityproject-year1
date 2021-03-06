#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "game.h"

#define NROFMAPS 5

#define MAP_HEIGHT 42
#define MAP_WIDTH 136
#define TILE_HEIGHT 32
#define TILE_WIDTH 32

struct tile {

	int ID;
	int x;
	int y;
	SDL_Surface *image;
	SDL_Texture *texture, *grassTexture;
	SDL_Rect rect;

};
typedef struct tile Tile;

int loadMap(const char* fileName, int map[][MAP_WIDTH]);
void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);
void drawTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);
void destroyTiles(Tile *tile);