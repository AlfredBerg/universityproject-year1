#include "map.h"

void initTiles1(Game *game, Tile *tile, int x, int y) {
	SDL_Rect rect = { x * 32, y * 32, 16, 16 };

	switch (tile->ID) {
	case 2:
		tile->image = IMG_Load("assets/t2.png");
		tile->texture = SDL_CreateTextureFromSurface(game->renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 10:
		tile->image = IMG_Load("assets/t10.png");
		tile->texture = SDL_CreateTextureFromSurface(game->renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 19:
		tile->image = IMG_Load("assets/t19.png");
		tile->texture = SDL_CreateTextureFromSurface(game->renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	}

}

void drawTiles1(Game *game, Tile *tile, int x, int y) {
	SDL_Rect rect = { x * 32, y * 32, 32, 32 };

	switch (tile->ID) {
	case 2:
		SDL_RenderCopy(game->renderer, tile->texture, NULL, &rect);
		break;

	case 10:
		SDL_RenderCopy(game->renderer, tile->texture, NULL, &rect);
		break;

	case 19:
		SDL_RenderCopy(game->renderer, tile->texture, NULL, &rect);
		break;
	}

}