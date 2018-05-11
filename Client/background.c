#include "background.h"
#include "camera.h"

void initBackground(Game *game) {
	game->cloudsBack = loadTexture("assets/CloudsBack.png", game);
	game->cloudsFront = loadTexture("assets/CloudsFront.png", game);
	game->bgBack = loadTexture("assets/BGBack.png", game);
	game->bgFront = loadTexture("assets/BGFront.png", game);
}

void displayBackground(Game *game) {
	renderCopyMoveWithCamera(game->renderer, game->cloudsBack, NULL, NULL, 0.0, NULL, 0);
	renderCopyMoveWithCamera(game->renderer, game->cloudsFront, NULL, NULL, 0.0, NULL, 0);
	renderCopyMoveWithCamera(game->renderer, game->bgBack, NULL, NULL, 0.0, NULL, 0);
	renderCopyMoveWithCamera(game->renderer, game->bgFront, NULL, NULL, 0.0, NULL, 0);
}

void deleteBackground(Game *game) {
	SDL_DestroyTexture(game->cloudsBack);
	SDL_DestroyTexture(game->cloudsFront);
	SDL_DestroyTexture(game->bgBack);
	SDL_DestroyTexture(game->bgFront);
}