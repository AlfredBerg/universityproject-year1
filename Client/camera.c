#pragma once
#include "camera.h"

//SDL_RenderCopyEx(renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);

renderCopyMoveWithCamera(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, const double angle, SDL_Point *center, SDL_RendererFlip flip) {
	if (dstRect) {
		dstRect->x = dstRect->x + SDL_GetTicks() / 100;
	}
	
	SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, angle, center, flip);
}
