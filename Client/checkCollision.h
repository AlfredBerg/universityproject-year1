#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include "player.h"
#include "map.h"

void checkForGround(Tile map[][MAP_WIDTH], Player *player, int *key, int *prevKey, int *groundDetected, int *enableWalk) {
	int i, j, flag = 0;

	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			if (SDL_HasIntersection(&player->rect, &map[i][j].rect)) {	
			
				if ((map[i][j].y + TILE_HEIGHT < (player->y + player->rect.h))) {	//wall	

					if (*key == LEFT && *key == *prevKey) {
						if ((map[i][j].x + TILE_WIDTH > player->x)) {
							*enableWalk = 0;
						}
					}
					else if (*key == RIGHT && *key == *prevKey) {
						if ((map[i][j].x < player->x + player->rect.w)) {
							*enableWalk = 0;
						}
					}
				}
				else if (map[i][j].y < player->y + player->rect.h) {				//wall/ground	
					*groundDetected = 1;
				}
				else {
					*enableWalk = 1; 
				}
			}
		}
	}
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
				if (map[i][j].y - TILE_HEIGHT < object->y + object->h) {
					return 1;
				}
			}
		}
	}
	return 0;
}