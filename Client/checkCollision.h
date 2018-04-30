#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include "player.h"
#include "map.h"

int checkForWall(Tile map[][MAP_WIDTH], Player *player) {
	int i, j, wallDetected = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&player->rect, &map[i][j].rect)) {
				if ((map[i][j].y + TILE_HEIGHT < (player->y + player->rect.h))) {
					wallDetected = 1;
				}
				break;
			}
		}
		if (wallDetected) break;
	}
	return wallDetected;
}

void checkForCeiling(Tile map[][MAP_WIDTH], Player *player, int *jumpTime, int *roofDetected, int *groundDetected) {
	int i, j;

	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&player->rect, &map[i][j].rect)) {

				if (map[i][j].y + TILE_HEIGHT < player->y + player->rect.h) {
					if (*jumpTime > 0) {
						*roofDetected = 1;
						*jumpTime = 0;
					}
				}
			}
		}
	}
}

int checkOnlyGround(Tile map[][MAP_WIDTH], SDL_Rect *object) {
	int i, j;

	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(object, &map[i][j].rect)) {
				if (map[i][j].y - 10 < object->y + object->h && map[i][j].y > object->y) { // - 10 f�r s�kerhets skull (om seg dator / server)
					return 1;
				}
			}
		}
	}
	return 0;
}