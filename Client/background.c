#pragma once
#include "background.h"

void displayBackground(Game *game) {
	SDL_RenderCopy(game->renderer, game->cloudsBack, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->cloudsFront, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->bgBack, NULL, NULL);

	SDL_RenderCopy(game->renderer, game->bgFront, NULL, NULL);
}