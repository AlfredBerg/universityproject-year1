#pragma once
#include "SDL.h"
#include "game.h"

#define MAP_HEIGHT 20
#define MAP_WIDTH 32

struct tile {

	int ID;
	SDL_Surface *image;
	SDL_Texture *texture;
	SDL_Rect rect;

};
typedef struct tile Tile;

void initTiles1(Game *game, Tile *tile, int x, int y);
void drawTiles1(Game *game, Tile *tile, int x, int y);