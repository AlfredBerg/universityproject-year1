#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "game.h"

#define MAP_HEIGHT 32 // map1: 32 & map2: 42
#define MAP_WIDTH 90 // map1: 90 & map2: 136
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

void loadMap(const char* fileName, int map[][MAP_WIDTH]);
void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);
void drawTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);