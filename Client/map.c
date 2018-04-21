#include <SDL.h>
#include <SDL_image.h>
#include "map.h"

SDL_Rect tiles[NR_OF_TILES];

int map[MAP_HEIGHT][MAP_WIDTH] = 
{	{ 1,1,2,2,2,2,2,2,1,1,2,2,2,2,2,1 },
	{ 1,1,1,1,2,1,1,2,1,1,2,2,2,2,2,1 },
	{ 2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,1 },
	{ 2,1,1,2,2,1,1,2,1,1,2,2,2,2,2,1 },
	{ 2,1,1,4,4,4,1,2,1,1,2,2,2,2,4,1 },
	{ 2,1,1,4,4,4,1,2,1,1,2,2,2,2,2,1 },
	{ 2,1,1,4,4,4,1,2,1,1,2,2,4,2,2,1 },
	{ 2,2,2,4,4,4,2,1,2,3,3,3,4,2,2,1 },
	{ 1,1,2,2,2,2,2,3,4,3,3,3,4,2,2,2 },
	{ 1,1,1,1,2,1,1,2,1,3,3,3,2,2,2,3 },
	{ 2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,1 },
	{ 2,1,1,2,2,1,1,2,1,1,3,2,2,2,4,4 },
	{ 2,1,1,4,2,1,1,2,1,1,3,2,2,2,2,4 },
	{ 2,1,1,1,2,1,1,2,1,1,3,3,3,3,3,4 },
	{ 2,1,1,1,1,1,1,2,1,1,2,2,2,2,4,4 },
	{ 2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,1 }	};

SDL_Rect createRectangle(int x, int y, int w, int h) {
	SDL_Rect rectangle;
	rectangle.x = x;
	rectangle.y = y;
	rectangle.w = w;
	rectangle.h = h;
	return rectangle;
}

void createRect(SDL_Rect *rectangle, int x, int y, int w, int h) {
	rectangle->x = x;
	rectangle->y = y;
	rectangle->w = w;
	rectangle->h = h;
}

void initTiles() {
	int x = 0, y = 0;

	for (int i = 0; i < NR_OF_TILES; i++) {
		createRect(&tiles[i], x, y, 64, 64);
		x += 64;										//move tile to the side, increment to next tile
	}
}

void drawMap() {
	SDL_Surface *tmp = IMG_Load("tiles.png");			//test 
	SDL_FreeSurface(tmp);

	int tile = 4;
	int height = 16;
	int width = 16;

	//loop through the map
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tile = map[i][j];

			//render here

		}
	}

	//render here
}