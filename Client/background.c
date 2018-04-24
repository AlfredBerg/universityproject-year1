#pragma once
#include "SDL.h"
#include "game.h"
#include "background.h"
#include "textureManager.h"

void displayBackground(Game *game) {
	SDL_Texture *clouds = loadTexture("assets/CloudsBack.png", game);
	SDL_RenderCopy(game->renderer, clouds, NULL, NULL);

	SDL_Texture *nature = loadTexture("assets/BGFront.png", game);
	SDL_RenderCopy(game->renderer, nature, NULL, NULL);
}