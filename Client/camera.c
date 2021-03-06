#pragma once
#include "camera.h"
#include "game.h"
#include "player.h"
#include "map.h"

SDL_Rect camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

void renderCopyMoveWithCamera(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect, const double angle, SDL_Point *center, SDL_RendererFlip flip) {
	int oldX, oldY;
	if (dstRect) {
		oldX = dstRect->x;
		oldY = dstRect->y;
		dstRect->x = dstRect->x - camera.x;
		dstRect->y = dstRect->y - camera.y;
	}

	SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, angle, center, flip);

	if (dstRect) {
		dstRect->x = oldX;
		dstRect->y = oldY;
	}
}

void updateCameraPosition(Player *player) {
	camera.x = (player->x + player->dstRect.w / 2) - WINDOW_WIDTH / 2;
	camera.y = (player->y + player->dstRect.h / 2) - WINDOW_HEIGHT / 1.5;

	int maxX = (TILE_WIDTH * MAP_WIDTH) - WINDOW_WIDTH;
	int maxY = (TILE_HEIGHT * MAP_HEIGHT) - WINDOW_HEIGHT;
	int minX = 0;
	int minY = 0;

	if (camera.x > maxX)
		camera.x = maxX;

	if (camera.x < minX)
		camera.x = minX;

	if (camera.y > maxY)
		camera.y = maxY;

	if (camera.y < minY)
		camera.y = minY;
}

int correctPosX(int x) {
	return x - camera.x;
}

int correctPosY(int y) {
	return y - camera.y;
}
