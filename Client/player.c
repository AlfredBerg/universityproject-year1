#pragma once
#include "player.h"

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
	if (*prevKey == RIGHT && player->x < 980) {
		player->x += 10;
	}
	else if (*prevKey == LEFT && player->x > -10) {
		player->x -= 10;
	}
	*prevKey = 0;
}

void loseHealth(Player *player, int damage) {
	if (!SDL_TICKS_PASSED(SDL_GetTicks(), player->tickThatLostHealth + INVULNERABILITY_TIME)) {
		return;
	}
	printf("LOST HEALTH\n");
	player->tickThatLostHealth = SDL_GetTicks();
	player->life -= damage;
}
