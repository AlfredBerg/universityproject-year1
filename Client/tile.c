#include "game.h"
#include "tile.h"

#define MAP_HEIGHT 16
#define MAP_WIDTH 16

void initTiles1(Game *game, Tile *tile, int x, int y) {

	if (tile->ID == 1) {

		tile->image = IMG_Load("bowser.png");
		tile->texture = SDL_CreateTextureFromSurface(game->renderer, tile->image);
		SDL_FreeSurface(tile->image);
		SDL_Rect rect = { x * 64, y * 64, 64, 64 };
		SDL_RenderCopy(game->renderer, tile->texture, NULL, &rect);
	}

}

void drawTiles1(Game *game, Tile *tile, int x, int y) {

	if (tile->ID == 1) {

		SDL_Rect rect = { x * 64, y * 64, 64, 64 };
		SDL_RenderCopy(game->renderer, tile->texture, NULL, &rect);

	}

}
