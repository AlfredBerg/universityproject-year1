#pragma once
#include "player.h"

#define LEFT 2
#define RIGHT 3

void jump(Player *player, SDL_Rect *weapon, int *isJumping, int *jumpTime, int *doJump) {

	if ((*doJump == 1))
	{
		if (!*isJumping) {
			*jumpTime = 10;
		}
		if (*jumpTime > 0) {
			*isJumping = 1;
			player->y -= 20;
			weapon->y = player->y + 50;
			--(*jumpTime);
		}
		if (*jumpTime <= 0) {
			if (player->y == 480) {
				*doJump = 0;
				*isJumping = 0;
			}
		}
	}
}

void walk(Player *player, SDL_Rect *weapon, int *prevKey) {
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