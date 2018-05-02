#pragma once
#include "camera.h"
#include "game.h"
#include "player.h"

SDL_Rect camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

void renderCopyMoveWithCamera(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, const double angle, SDL_Point *center, SDL_RendererFlip flip) {
	if (dstRect) {
		dstRect->x = dstRect->x - camera.x;
		dstRect->y = dstRect->y - camera.y;
	}
	SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, angle, center, flip);
}

void updateCameraPosition(Player *player){
	//Center the camera over the dot
	camera.x = (player->rect.x + player->rect.w / 2) - WINDOW_WIDTH / 2;
	camera.y = (player->rect.y + player->rect.h / 2) - WINDOW_HEIGHT / 2;
}