#pragma once
#include "SDL.h"
#include "game.h"

#define MAP_HEIGHT 32
#define MAP_WIDTH 32
#define TILE_HEIGHT 32
#define TILE_WIDTH 32

struct tile {

	int ID;
	int x;
	int y;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Rect rect;

};
typedef struct tile Tile;

void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);
void drawTiles(SDL_Renderer* renderer, Tile *tile, int x, int y);