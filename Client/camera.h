#pragma once
#include "SDL.h"
#include "player.h"

void updateCameraPosition(Player *player);
void renderCopyMoveWithCamera(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, const double angle, SDL_Point *center, SDL_RendererFlip flip);