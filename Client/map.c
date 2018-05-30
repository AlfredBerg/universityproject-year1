#include "map.h"
#include "camera.h"



int loadMap(const char* fileName, int map[][MAP_WIDTH]) {
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
	return 1;
}


void initTiles(SDL_Renderer* renderer, Tile *tile, int x, int y) {

	tile->x = x * TILE_WIDTH;
	tile->y = y * TILE_HEIGHT;

	SDL_Rect rect = { tile->x, tile->y, TILE_WIDTH, TILE_HEIGHT };


	switch (tile->ID) {

		// up tiles
	case 1:
		tile->image = IMG_Load("assets/map/t1.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->image = IMG_Load("assets/map/g4.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->rect = rect;
		break;

	case 2:
		tile->image = IMG_Load("assets/map/t2.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->image = IMG_Load("assets/map/g5.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->rect = rect;
		break;

	case 3:
		tile->image = IMG_Load("assets/map/t3.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->image = IMG_Load("assets/map/g7.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->rect = rect;
		break;

		// middle tiles
	case 4:
		tile->image = IMG_Load("assets/map/t4.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 5:
		tile->image = IMG_Load("assets/map/t5.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 6:
		tile->image = IMG_Load("assets/map/t6.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		// down tiles
	case 7:
		tile->image = IMG_Load("assets/map/t7.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 8:
		tile->image = IMG_Load("assets/map/t8.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 9:
		tile->image = IMG_Load("assets/map/t9.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

		// side tiles
	case 10:
		tile->image = IMG_Load("assets/map/t10.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 11:
		tile->image = IMG_Load("assets/map/t11.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	//
	case 12:
		tile->image = IMG_Load("assets/map/t12.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->image = IMG_Load("assets/map/g7.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 13:
		tile->image = IMG_Load("assets/map/t13.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);

		tile->image = IMG_Load("assets/map/g4.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	//
	case 15:
		tile->image = IMG_Load("assets/map/t15.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;

		tile->image = IMG_Load("assets/map/g4.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		break;

	case 16:
		tile->image = IMG_Load("assets/map/t16.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;

		tile->image = IMG_Load("assets/map/g7.png");
		tile->grassTexture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		break;

	case 18:
		tile->image = IMG_Load("assets/map/t18.png");
		tile->texture = SDL_CreateTextureFromSurface(renderer, tile->image);
		SDL_FreeSurface(tile->image);
		tile->rect = rect;
		break;

	case 19:
		tile->image = IMG_Load("assets/map/t19.png");
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
	case 1:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 2:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 3:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	// middle tiles
	case 4:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 5:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 6:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	// down tiles
	case 7:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 8:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 9:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	// side tiles
	case 10:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 11:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	//
	case 12:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 13:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	//
	case 15:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 16:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		renderCopyMoveWithCamera(renderer, tile->grassTexture, NULL, &rect, 0.0, NULL, 0);
		break;

	//
	case 18:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	case 19:
		renderCopyMoveWithCamera(renderer, tile->texture, NULL, &rect, 0.0, NULL, 0);
		break;

	}

}

void destroyTiles(Tile *tile) {
	SDL_DestroyTexture(tile->texture);
	SDL_DestroyTexture(tile->grassTexture);
}