#pragma once
#include "player.h"
#include "camera.h"
//#include "game.h"
//#include "map.h"

extern SDL_Rect camera;

void jump(Player *player, int *isJumping, int *jumpTime, int *doJump, int *groundDetected, int *roofDetected) {
	if (!*roofDetected) {
		if (*doJump == 1) {
			if (!*isJumping) {
				*jumpTime = 10;
			}
			if (*jumpTime > 0) {
				*isJumping = 1;
				player->y -= 20;
				--(*jumpTime);
				*groundDetected = 0;
			}
			if (*jumpTime <= 0) {
				if (*groundDetected == 1) {
					*doJump = 0;
					*isJumping = 0;
					*roofDetected = 0;
				}
			}
		}
	}
}

void walkRight(Player *player, int *key, int *prevKey) {
	if (player->x < 980) {
		player->x += WALKSPEED;
		//camera.x -= WALKSPEED;
	}
	*prevKey = *key;
}

void walkLeft(Player *player, int *key, int *prevKey) {
	if (player->x > -10) {
		player->x -= WALKSPEED;
		//camera.x += WALKSPEED;
	}
	*prevKey = *key;
}

void loseHealth(Player *player, int damage) {
	if (!SDL_TICKS_PASSED(SDL_GetTicks(), player->tickThatLostHealth + INVULNERABILITY_TIME)) {
		return;
	}
	printf("LOST HEALTH\n");
	player->tickThatLostHealth = SDL_GetTicks();
	player->life -= damage;
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
		renderCopyMoveWithCamera(renderer, greenTexture, NULL, &healthbar[i], 0.0, NULL, 0);
	}
}

void deletePlayer(Player players[], int id, int *nrOfPlayers) {
	for (int i = 0; i < *nrOfPlayers; i++) {
		if (players[i].id == id){
			for (int j = 0; j < *nrOfPlayers - 1; j++)
				players[j] = players[j + 1];
			*nrOfPlayers = *nrOfPlayers - 1;
			printf("Deleted player\n");
		}
	}
}

void updatePlayerStates(Player players[], int loopCount) {
	for (int i = 0; i < MAXPLAYERS; i++) {

		//Update States
		if (players[i].x != players[i].previousX) {
			players[i].isMoving = 1;

			if (players[i].x > players[i].previousX) {
				players[i].lastDirection = RIGHT;
			}
			else {
				players[i].lastDirection = LEFT;
			}

		}
		else {
			players[i].isMoving = 0;
		}

		players[i].previousX = players[i].x;
		players[i].previousY = players[i].y;


		//update sprites

		if (players[i].isMoving && loopCount % SPRITESPEED == 0) {
			(players[i].currentSprite)++;
		}

		if (players[i].currentSprite >= 5)
			players[i].currentSprite = 1;
		else if (players[i].currentSprite <= 0)
			players[i].currentSprite = 4;
	}
}