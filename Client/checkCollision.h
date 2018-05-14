#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include "player.h"
#include "map.h"

int collisionWithMap(Tile map[][MAP_WIDTH], SDL_Rect inRect) {
	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&inRect, &map[i][j].rect)) {
				return 1;
			}
		}
	}
	return  0;
}

int checkForWall(Tile map[][MAP_WIDTH], Player *player, int *key) {
	int i, j, wallDetected = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&player->dstRect, &map[i][j].rect)) {
				if ((map[i][j].y + TILE_HEIGHT < (player->y + player->dstRect.h))) {
					if (*key == LEFT) wallDetected = LEFT;
					else wallDetected = RIGHT;
				}
				break;
			}
		}
		if (wallDetected) break;
	}
	//printf("\nWALL = %d", wallDetected);
	return wallDetected;
}

int checkForCeiling(Tile map[][MAP_WIDTH], Player *player, int *jumpTime, int *roofDetected, int *groundDetected, int *leftWall, int *rightWall) {
	int i, j, ceilingDetected = 0;

	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&player->dstRect, &map[i][j].rect)) {
				if (map[i][j].y + TILE_HEIGHT < player->y + player->dstRect.h) {
					ceilingDetected = 1;
					if (*jumpTime > 0) {
						*roofDetected = 1;
						*jumpTime = 0;
						*leftWall = 0;
						*rightWall = 0;
					}
					break;
				}
			}
		}
		if (ceilingDetected) break;
	}
	return ceilingDetected;
}

int checkOnlyGround(Tile map[][MAP_WIDTH], SDL_Rect *object) {
	int i, j, groundDetected = 0;

	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(object, &map[i][j].rect)) {
				if ((map[i][j].y - 10 < object->y + object->h) && (map[i][j].y > object->y)) { // - 10 för säkerhets skull (om seg dator / server)
					groundDetected = 1;	
					break;
				}
			}
		}
		if (groundDetected) break;
	}
	return groundDetected;
}
