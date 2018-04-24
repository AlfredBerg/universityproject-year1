#pragma once
#include "background.h"

void displayBackground(Game *game) {
	SDL_Texture *cloudsBack = loadTexture("assets/CloudsBack.png", game);
	SDL_RenderCopy(game->renderer, cloudsBack, NULL, NULL);

	SDL_Texture *cloudsFront = loadTexture("assets/CloudsFront.png", game);
	SDL_RenderCopy(game->renderer, cloudsFront, NULL, NULL);

	SDL_Texture *bgBack = loadTexture("assets/BGBack.png", game);
	SDL_RenderCopy(game->renderer, bgBack, NULL, NULL);

	SDL_Texture *bgFront = loadTexture("assets/BGFront.png", game);
	SDL_RenderCopy(game->renderer, bgFront, NULL, NULL);
}