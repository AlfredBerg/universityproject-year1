#include "map.h"
#include "camera.h"



void loadMap(const char* fileName, int map[][MAP_WIDTH]) {
	FILE *fp;
	fp = fopen(fileName, "r");
	if (fp != NULL) {
		char tileChar;
		int tileNr;
		
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if ((fscanf(fp, "%d%c", &tileNr, &tileChar) == 2))
					map[i][j] = tileNr;
			}
		}
		fclose(fp);
	}
	
}


void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y) {

	tile->x = x * TILE_WIDTH;
	tile->y = y * TILE_HEIGHT;

	SDL_Rect rect = { tile->x, tile->y, TILE_WIDTH, TILE_HEIGHT };


	switch (tile->ID) {

		// up tiles
		case 10:
		tile->image = IMG_Load("assets/t1.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 11:
		tile->image = IMG_Load("assets/t2.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 12:
		tile->image = IMG_Load("assets/t3.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		// middle tiles
		case 13:
		tile->image = IMG_Load("assets/t9.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 14:
		tile->image = IMG_Load("assets/t10.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 15:
		tile->image = IMG_Load("assets/t11.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		// down tiles
		case 16:
		tile->image = IMG_Load("assets/t15.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 17:
		tile->image = IMG_Load("assets/t16.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 18:
		tile->image = IMG_Load("assets/t17.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		// side tiles
		case 20:
		tile->image = IMG_Load("assets/t7.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		case 21:
		tile->image = IMG_Load("assets/t13.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	}

}


void drawTiles(SDL_Renderer* renderer, Tile *tile, int x, int y) {
	SDL_Rect rect = { tile->x, tile->y, TILE_WIDTH, TILE_HEIGHT };

	

	switch (tile->ID) {
		// up tiles
		case 10:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 11:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 12:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		// middle tiles
		case 13:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 14:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 15:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		// down tiles
		case 16:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 17:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 18:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		// side tiles
		case 20:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

		case 21:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;
	}
}