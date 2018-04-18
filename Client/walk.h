#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define LEFT 2
#define RIGHT 3

void walk1(Player *player, SDL_Rect *weapon, int *prevKey) {
	if (*prevKey == RIGHT && player->x < 730) {
		player->x += 10;
		weapon->x += 10;
	}
	else if (*prevKey == LEFT && player->x > -10) {
		player->x -= 10;
		weapon->x -= 10;
	}
	*prevKey = 0;
}

void walk2(Player *player, SDL_Rect *weapon, int *prevKey) {
	if (*prevKey == RIGHT && player->x < 712) {
		player->x += 10;
		weapon->x += 10;
	}
	else if (*prevKey == LEFT && player->x > -22) {
		player->x -= 10;
		weapon->x -= 10;
	}
	*prevKey = 0;
}
