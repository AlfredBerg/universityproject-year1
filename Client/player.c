#pragma once
#include "player.h"
#include "game.h"

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
	if (!SDL_TICKS_PASSED(SDL_GetTicks(), player->tickThatLostHealth + INVULNERABILITY_TIME)) {
		return;
	}
	printf("LOST HEALTH\n");
	player->tickThatLostHealth = SDL_GetTicks();
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

void playerHealthbar(Player players[MAXPLAYERS], SDL_Renderer* renderer) {

	int width = 100, height = 5;

	SDL_Surface *red, *green;
	red = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	green = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	SDL_Rect rect = { 0, 0, width, height };

	SDL_FillRect(red, &rect, SDL_MapRGB(red->format, 255, 0, 0));

	SDL_FillRect(green, &rect, SDL_MapRGB(green->format, 61, 229, 77));
	
	SDL_Texture *greenTexture = SDL_CreateTextureFromSurface(renderer, green);
	SDL_Texture *redTexture = SDL_CreateTextureFromSurface(renderer, red);


	SDL_Rect healthbar[MAXPLAYERS];
	SDL_Rect healt[] = { 0, 0, width, height };


	for (int i = 0; i < MAXPLAYERS; i++) {
		healthbar[i].x = players[i].x + 5;
		healthbar[i].y = players[i].y - 10;
		healthbar[i].h = height;
		healthbar[i].w = players[i].life / 2;

		SDL_RenderCopy(renderer, greenTexture, NULL, &healthbar[i]);
	}
}