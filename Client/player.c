#pragma once
#include "player.h"
#include "camera.h"
#include "clientNetwork.h"

extern SDL_Rect camera;

void jump(Player *player, int *isJumping, int *jumpTime, int *doJump, int *groundDetected, int *roofDetected) {
	if (!*roofDetected) {
		if (*doJump == 1) {
			if (!*isJumping) {
				*jumpTime = 18;
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
	player->x += WALKSPEED;
	//camera.x -= WALKSPEED;
	*prevKey = *key;
}

void walkLeft(Player *player, int *key, int *prevKey) {
	player->x -= WALKSPEED;
	//camera.x += WALKSPEED;
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

void playerNameTag(Player players[MAXPLAYERS], SDL_Renderer* renderer) {

	int width = 50, height = 20;
	char text0[20]; char text1[20]; char text2[20];
	strcpy(text0, players[0].name);
	strcpy(text1, players[1].name);
	strcpy(text2, players[2].name);

	TTF_Font *font = TTF_OpenFont("assets/pixlig font.ttf", 20);
	SDL_Color color = { 65, 33, 52, 255 };

	SDL_Surface *name[3];
	name[0] = TTF_RenderText_Solid(font, text0, color);
	name[1] = TTF_RenderText_Solid(font, text1, color);
	name[2] = TTF_RenderText_Solid(font, text2, color);

	SDL_Texture *texture[3];
	texture[0] = SDL_CreateTextureFromSurface(renderer, name[0]);
	texture[1] = SDL_CreateTextureFromSurface(renderer, name[1]);
	texture[2] = SDL_CreateTextureFromSurface(renderer, name[2]);

	SDL_Rect names[MAXPLAYERS];

	for (int i = 0; i < MAXPLAYERS; i++) {
		names[i].x = players[i].x + 5;
		names[i].y = players[i].y - 30;
		names[i].h = name[i]->h;
		names[i].w = name[i]->w;

		renderCopyMoveWithCamera(renderer, texture[i], NULL, &names[i], 0.0, NULL, 0);
	}
	SDL_DestroyTexture(texture[0]); SDL_DestroyTexture(texture[1]); SDL_DestroyTexture(texture[2]);
	SDL_FreeSurface(name[0]); SDL_FreeSurface(name[1]); SDL_FreeSurface(name[2]);
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