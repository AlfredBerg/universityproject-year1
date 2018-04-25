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

void walk(Player *player, int *key, int *enableWalk, int *prevKey) {
	if (*key == RIGHT && player->x < 980 && *enableWalk) {
		player->x += 10;
	}
	else if (*key == LEFT && player->x > -10 && *enableWalk) {
		player->x -= 10;
	}
	*prevKey = *key;
	*key = 0;
	*enableWalk = 1;
}

void loseHealth(Player *player, int damage) {
	player->life -= damage;
}


int handleCollision(Player *player, int tileX, int tileY, int *key, int *prevKey) {
	int enable = 1;
	if (*key == LEFT && *key == *prevKey) {
		if (tileX + 32 > player->x) { // 32 = tile width
			enable = 0;
		}
		else enable = 1;
	}

	else if (*key == RIGHT && *key == *prevKey) {
		if (tileX < player->x + 75) { // 75 = player width
			enable = 0;
		}
		else enable = 1;
	}

	//printf("\ntile x   = %d", tileX + 32); // 32 = tile width
	//printf("\nplayer x = %d\n", player->x);

	return enable;
}

