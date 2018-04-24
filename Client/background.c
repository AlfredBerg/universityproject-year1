#pragma once
#include "SDL.h"
#include "game.h"
#include "background.h"

void displayBackground(Game *game) {

	SDL_Surface *image = IMG_Load("bowser.png");
	SDL_Texture *image_Texture = SDL_CreateTextureFromSurface(game->renderer, image);
	SDL_FreeSurface(image);
	SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_RenderCopy(game->renderer, image_Texture, NULL, &rect);
}