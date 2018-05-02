#include "map.h"
#include "camera.h"

void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y) {

	tile->x = x * TILE_WIDTH;
	tile->y = y * TILE_HEIGHT;

	SDL_Rect rect = { tile->x, tile->y, TILE_WIDTH, TILE_HEIGHT };


	switch (tile->ID) {
	case 2:
		tile->image = IMG_Load("assets/t2.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 10:
		tile->image = IMG_Load("assets/t10.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 19:
		tile->image = IMG_Load("assets/t19.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	}

}

void drawTiles(SDL_Renderer* renderer, Tile *tile, int x, int y) {
	SDL_Rect rect = { tile->x, tile->y, TILE_WIDTH, TILE_HEIGHT };

	switch (tile->ID) {
	case 2:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 10:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 19:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;
	}

}