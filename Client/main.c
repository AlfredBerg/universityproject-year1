#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_net.h>

#include "player.h"
#include "game.h"
#include "menu.h"
#include "weapon.h"
#include "gravity.h"
#include "sharedNetwork.h"
#include "clientNetwork.h"
#include "walk.h"
#include "jump.h"
#include "loadImage.h"
#include "map.h"

int menu(Game *game);
int menuOptions(SDL_Event event, bool *menuLoop);
int restart(Game *game);

int rungame(Game *game, Network *client);
void jump(Player *player, SDL_Rect *weapon, int *isJumping, int *jumpTime, int *doJump);
void walk1(Player *player, SDL_Rect *weapon, int *prevKey);
void walk2(Player *player, SDL_Rect *weapon, int *prevKey);

#define UP 1
#define LEFT 2
#define RIGHT 3

int main(int argc, char** argv)
{
	Game game;
	Network client;

	initGame(&game, &client);

	while (game.running) {
		game.running = menu(&game);
		while (game.running) {
			game.running = rungame(&game, &client);
		}
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}

int menu(Game *game) {

	TTF_Font *font = TTF_OpenFont("fintext.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };

	SDL_Surface *textImage = TTF_RenderText_Solid(font, "START", color);
	SDL_Surface *menuImage = IMG_Load("startscreen.jpg");

	SDL_Texture *text = SDL_CreateTextureFromSurface(game->renderer, textImage);
	SDL_FreeSurface(textImage);
	SDL_Texture *background = SDL_CreateTextureFromSurface(game->renderer, menuImage);
	SDL_FreeSurface(menuImage);

	SDL_Rect textRect;
	textRect.x = 100;
	textRect.y = 260;
	textRect.w = 150;
	textRect.h = 80;

	SDL_Rect backRect = { 0, 0, WINDOW_LENGTH, WINDOW_HEIGHT };

	bool startGame = true;
	bool menuLoop = true;

	SDL_DestroyTexture(text);
	SDL_DestroyTexture(background);
	return startGame;
}

int menuOptions(SDL_Event event, bool *menuLoop) {
	bool running = true;
	if (event.type == SDL_QUIT) {
		running = false;
		*menuLoop = false;
	}

	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (event.button.x > 100 && event.button.x < 250 && event.button.y>280 && event.button.y < 340) {
				running = true;
				*menuLoop = false;
			}
		}
	}

	return running;
}

int restart(Game* game) {

	TTF_Font *font2 = TTF_OpenFont("fintext.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface *rematch = TTF_RenderText_Solid(font2, "Rematch", color);

	SDL_Texture *rematch_Texture = SDL_CreateTextureFromSurface(game->renderer, rematch);
	SDL_FreeSurface(rematch);

	SDL_Rect RematchFontRect;
	RematchFontRect.x = 200;
	RematchFontRect.y = 260;
	RematchFontRect.w = 150;
	RematchFontRect.h = 80;
	SDL_Event ev;

	bool running = true;
	while (running) {

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				running = false;

			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.button.button == SDL_BUTTON_LEFT) {

					if (ev.button.x > 200 && ev.button.x < 350 && ev.button.y>280 && ev.button.y < 340) {
						SDL_DestroyTexture(rematch_Texture);
						running = true;
						return running;

					}
				}
			}
		}
		SDL_RenderClear(game->renderer);
		SDL_RenderCopy(game->renderer, rematch_Texture, NULL, &RematchFontRect);
		/*SDL_RenderCopy(game->renderer, image2_texture, &srcrect, &dstrect);//draw
		SDL_RenderCopy(game->renderer, image3_texture, &srcrect2, &dstrect2);
		SDL_RenderCopy(game->renderer, image5_texture, NULL, &bild5);
		SDL_RenderCopy(game->renderer, image7_texture, NULL, &bild7);
		*/

		SDL_RenderPresent(game->renderer);


	}
	SDL_DestroyTexture(rematch_Texture);
	return running;

}
int rungame(Game *game, Network *client) {

	Mix_Music *backgroundsound = Mix_LoadMUS("hello.mp3");

	if (!backgroundsound)
		printf("Sound is not working\n");

	int SourcePosition = 0;
	int SourcePosition2 = 0;
	int whynotwork = 1;


	//Create two players
	Player players[2] = {
		{ "Erik", 3, 60, 400, 1, IMG_Load("mansprite.png"),SDL_CreateTextureFromSurface(game->renderer, players[0].Image),{ 60, 400, 140, 200 } },
	{ "Skull", 100, 500, 50, 0,IMG_Load("deathsprite.png"),SDL_CreateTextureFromSurface(game->renderer, players[1].Image),{ 500, 50, 120, 120 } }
	};
	printf("%d, %d\n", players[0].p1.x, players[0].p1.y);
	printf("%d, %d", players[0].x, players[0].y);

	//initialize support for flipping images
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;


	SDL_Surface *images[MAX_IMAGES];
	SDL_Texture *images_Texture[MAX_IMAGES];

	loadImage(&images);
	//load an image file

	int nrOfImages = 8;
	for (int i = 0; i<nrOfImages; i++)
		images_Texture[i] = SDL_CreateTextureFromSurface(game->renderer, images[i]);

	//Fulkod för att avgöra enemyID
	int enemyID;
	if (client->playerID == 1) {
		enemyID = 0;
	}
	else {
		enemyID = 1;
	}

	//Define where on the "screen" we want to draw the texture
	SDL_Rect bild = { 0, 0, WINDOW_LENGTH, WINDOW_HEIGHT }; //(x, y, hight, width)

															//SDL_Rect bild2 = { fighter.x, fighter.y, 140, 200 };
															//SDL_Rect bild3 = { enemy.x, enemy.y, 500, 500};
	SDL_Rect bild4 = { 150, 100, 500, 325 };
	SDL_Rect bild5 = { players[client->playerID].x + 30, players[client->playerID].y + 10, 15, 40 };
	SDL_Rect bild6 = { 100, 450, 15, 40 };
	SDL_Rect bild7 = { players[enemyID].x + 50, players[enemyID].y + 40, 15, 40 };
	SDL_Rect bild8 = { 530, 490, 15, 40 };
	SDL_Rect bild9 = { 150, 100, 550, 300 };

	Mix_PlayMusic(backgroundsound, -1);
	bool pPressed = false;
	bool rPressed = false;
	bool running = true;
	bool again = false;
	SDL_Event event;
	int sprite[2];
	sprite[0] = 1;
	sprite[1] = 1;

	int prevKey1 = 0;
	int prevKey2 = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump1 = 0;
	int doJump2 = 0;
	int direction = 0;

	while (running)
	{
		updateServer(players, client);

		if (sprite[client->playerID] >= 8)
			sprite[client->playerID] = 1;
		else if (sprite[client->playerID] <= 0)
			sprite[client->playerID] = 7;
		/*
		if (sprite2 >= 8)
		sprite2 = 1;
		else if (sprite2 <= 0)
		sprite2 = 7;
		*/

		//for sprite
		//Uint32 ticks = SDL_GetTicks(); (time based)
		//Uint32 sprite = (ticks / 100) % 4; (time based)

		SDL_Rect srcrect = { sprite[0] * 75, 0, 75, 132 };
		SDL_Rect dstrect = { players[0].p1.x, players[0].p1.y, 75, 132 };

		SDL_Rect srcrect2 = { sprite[1] * 64, 64, 64, 64 };
		SDL_Rect dstrect2 = { players[1].p1.x, players[1].p1.y, 120, 120 };



		//SDL_Rect dstTileRect[] = { 400, 200, 70, 70};



		// Check for various events (keyboard, mouse, touch, close)
		while (SDL_PollEvent(&event))
		{
			//const char* key = SDL_GetKeyName(event.key.keysym.sym);
			if (event.type == SDL_QUIT) {
				running = false;
				return running;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_w) {
					//printf("W is pressed");
					doJump1 = 1;
				}
				if (event.key.keysym.sym == SDLK_UP) {
					//printf("UP is pressed");
					doJump2 = 1;
				}
			}

		}

		const Uint8 *KeyState;
		//Move fighter
		KeyState = SDL_GetKeyboardState(NULL);

		if (KeyState[SDL_SCANCODE_D] && players[client->playerID].x < 730) {
			sprite[client->playerID] += 1;
			players[client->playerID].x += 10;
			bild5.x += 10;
			prevKey1 = RIGHT;
		}
		else if (KeyState[SDL_SCANCODE_A] && players[client->playerID].x > -10) {
			sprite[client->playerID] -= 1;
			players[client->playerID].x -= 10;
			bild5.x -= 10;
			prevKey1 = LEFT;
		}
		//if (prevKey == UP) {

		//}
		jump(&players[client->playerID], &bild5, &isJumping, &jumpTime, &doJump1);
		gravity(&players[client->playerID], &bild5);


		if (KeyState[SDL_SCANCODE_R]) {
			bild6 = bild5;
			SourcePosition = bild6.x;
			bild6.x += 10;
			rPressed = true;
		}
		/*
		if (SourcePosition2 != bild8.x && bild8.x >= -10 && pPressed == true)
		bild8.x -= 10;
		*/
		if (SourcePosition != bild6.x && bild6.x <= 800 && rPressed == true)
			bild6.x += 10;



		//clear screen with black
		SDL_RenderClear(game->renderer);

		//draw
		SDL_RenderCopy(game->renderer, images_Texture[0], NULL, &bild);

		if (rPressed == true)
			SDL_RenderCopy(game->renderer, images_Texture[3], NULL, &bild6);
		/*
		if (pPressed == true)
		SDL_RenderCopy(game->renderer, images_Texture[5], NULL, &bild8);
		*/

		for (int j = 0; j < 4; j++) {
			if (j == client->playerID) {
				players[client->playerID].p1.x = players[client->playerID].x;
				players[client->playerID].p1.y = players[client->playerID].y;
			}
			else {
				players[j].p1.x = players[j].x;
				players[j].p1.y = players[j].y;
			}
		}

		/*
		//Checking if sword hit player1
		if (bild6.x >= players[1].p1.x + 40 && bild6.x <= players[1].p1.x + 50) {
		if (bild6.y <= players[1].p1.y + 99 && bild6.y >= players[1].p1.y) {
		SDL_DestroyTexture(players[1].Texture);
		SDL_DestroyTexture(images_Texture[4]);
		SDL_DestroyTexture(images_Texture[5]);
		if (bild6.x >= enemy.p1.x + 40 && bild6.x <= enemy.p1.x + 50) {
		if (bild6.y <= enemy.p1.y + 99 && bild6.y >= enemy.p1.y) {
		SDL_DestroyTexture(enemy.Texture);
		whynotwork = 0;
		//again = true;
		running = false;
		}
		}
		//Checking if sword hit player2
		if (bild8.x <= players[0].p1.x + 40 && bild8.x >= players[0].p1.x - 50)
		if (bild8.y <= players[0].p1.y + 120 && bild8.y >= players[0].p1.y - 20) {
		SDL_DestroyTexture(players[0].Texture);
		SDL_DestroyTexture(images_Texture[2]);
		SDL_DestroyTexture(images_Texture[3]);
		whynotwork = 2;
		//again = true;
		running = false;
		}
		*/

		if (whynotwork == 0)
			SDL_RenderCopy(game->renderer, images_Texture[6], NULL, &bild9);
		if (whynotwork == 2)
			SDL_RenderCopy(game->renderer, images_Texture[1], NULL, &bild4);
		if (again == true) {

			//restart(window, renderer);

		}

		SDL_RenderCopy(game->renderer, players[0].Texture, &srcrect, &dstrect);//draw
		SDL_RenderCopy(game->renderer, players[1].Texture, &srcrect2, &dstrect2);
		SDL_RenderCopy(game->renderer, images_Texture[2], NULL, &bild5);
		SDL_RenderCopy(game->renderer, images_Texture[4], NULL, &bild7);

		//SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_RenderPresent(game->renderer);//show what was drawn
	}
	running = true;
	return running;
}