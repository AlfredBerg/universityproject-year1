#pragma once
#include "player.h"

#define LEFT 2
#define RIGHT 3



void jump(Player *player, int *isJumping, int *jumpTime, int *doJump) {

	if ((*doJump == 1))
	{
		if (!*isJumping) {
			*jumpTime = 10;
		}
		if (*jumpTime > 0) {
			*isJumping = 1;
			player->y -= 20;
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

void walk(Player *player, int *prevKey) {
	if (*prevKey == RIGHT && player->x < 750) {
		player->x += 10;
	}
	else if (*prevKey == LEFT && player->x > -10) {
		player->x -= 10;
	}
	*prevKey = 0;
}