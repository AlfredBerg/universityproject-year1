#pragma once
#include "player.h"
#include "camera.h"
#include "clientNetwork.h"
#include "map.h"


void jump(Player *player, int *isJumping, int *jumpTime, int *doJump, int *groundDetected, int *roofDetected, int *leftWall, int *rightWall) {
	if (!*roofDetected) {
		if (*doJump == 1) {
			if (!*isJumping) {
				*jumpTime = 15;
				*leftWall = 0;
				*rightWall = 0;
			}
			if (*jumpTime > 0) {
				*isJumping = 1;
				player->y -= 20;
				--(*jumpTime);
				*groundDetected = 0;
			}
			if (*jumpTime < 0 || *jumpTime == 0) {
				if (*groundDetected == 1) {
					*doJump = 0;
					*isJumping = 0;
					*roofDetected = 0;
					*leftWall = 0;
					*rightWall = 0;
				}
			}
		}
	}
}


void walkRight(Player *player, int *key, int *prevKey) {
	player->x += player->movementSpeed;

	int max = (TILE_WIDTH * MAP_WIDTH) - 1;

	if (player->x > max)
		player->x = max;

	*prevKey = *key;

}

void walkLeft(Player *player, int *key, int *prevKey) {
	player->x -= player->movementSpeed;

	if (player->x < 0)
		player->x = 0;

	*prevKey = *key;
}

void loseHealth(Player *player, int damage) {
	if (!SDL_TICKS_PASSED(SDL_GetTicks(), player->tickThatLostHealth + INVULNERABILITY_TIME)) {
		return;
	}
	printf("LOST HEALTH\n");
	player->tickThatLostHealth = SDL_GetTicks();
	player->life -= damage;
	sendHitToServer(damage, player->id);

}

void playerHealthbar(Player players[MAXPLAYERS], SDL_Renderer* renderer) {

	int width = 100, height = 5;

	SDL_Rect healthbar[MAXPLAYERS];
	for (int i = 0; i < MAXPLAYERS; i++) {
		healthbar[i].x = players[i].x;
		healthbar[i].y = players[i].y - 10;
		healthbar[i].h = height;
		healthbar[i].w = players[i].life / 2.44;
		renderCopyMoveWithCamera(renderer, players[i].hpBarTexture, NULL, &healthbar[i], 0.0, NULL, 0);
	}
}

void playerNameTag(Player players[MAXPLAYERS], SDL_Renderer* renderer) {
	SDL_Rect names[MAXPLAYERS];
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].life > 0) {
			names[i].x = players[i].x + 20 - (strlen(players[i].name) * 5.5);
			names[i].y = players[i].y - 30;
			names[i].h = players[i].nameText->h;
			names[i].w = players[i].nameText->w;

			renderCopyMoveWithCamera(renderer, players[i].nameTexture, NULL, &names[i], 0.0, NULL, 0);
		}
	}
}

void destroyPlayerObject(Player *player) {
	//Text
	SDL_DestroyTexture(player->nameTexture);
	SDL_FreeSurface(player->nameText);
	//Healthbar
	SDL_DestroyTexture(player->hpBarTexture);
	SDL_FreeSurface(player->hpBarSurface);

}

void deletePlayer(Player players[], int id, int *nrOfPlayers) {
	//destroyPlayerObject(&players[id]);
	//printf("Deleted player\n");

	players[id].dstRect.h = 0;
	players[id].dstRect.w = 0;
	SDL_DestroyTexture(players[id].Texture);

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