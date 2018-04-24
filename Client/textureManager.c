#include "textureManager.h"

SDL_Texture* loadTexture(const char* fileName, Game *game) {
	SDL_Surface *tmp = IMG_Load(fileName);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);
	return texture;
}