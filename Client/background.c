#include "background.h"

void initBackground(Game *game) {
	game->cloudsBack = loadTexture("assets/CloudsBack.png", game);
	game->cloudsFront = loadTexture("assets/CloudsFront.png", game);
	game->bgBack = loadTexture("assets/BGBack.png", game);
	game->bgFront = loadTexture("assets/BGFront.png", game);
}

void displayBackground(Game *game) {
	SDL_RenderCopy(game->renderer, game->cloudsBack, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->cloudsFront, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->bgBack, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->bgFront, NULL, NULL);
}