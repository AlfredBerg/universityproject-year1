#pragma once
#include "menu.h"

int menu(Game *game) {

	/*
	//Init text
	TTF_Font *font = TTF_OpenFont("assets/Capture_it.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface *textImage = TTF_RenderText_Solid(font, "START", color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(game->renderer, textImage);
	SDL_FreeSurface(textImage);
	SDL_Rect textRect = { 100, 260, 150, 80 };
	*/

	//Init menu image
	SDL_Surface *menuImage = IMG_Load("assets/meny.png");
	SDL_Texture *background = SDL_CreateTextureFromSurface(game->renderer, menuImage);
	SDL_FreeSurface(menuImage);

	int startGame = 1;
	int menuLoop = 1;

	//Draw
	while (menuLoop) {
		SDL_RenderCopy(game->renderer, background, NULL, NULL);
	//	SDL_RenderCopy(game->renderer, text, NULL, &textRect);
		SDL_RenderPresent(game->renderer);

		startGame = menuOptions(&menuLoop);
	}

	//  SDL_DestroyTexture(text);
	SDL_DestroyTexture(background);
	return startGame;
}

int menuOptions(int *menuLoop) {
	int running = 1;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) {
			running = 0;
			*menuLoop = 0;
		}

		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 220 && event.button.x < 800 && event.button.y > 210 && event.button.y < 420) {
					running = 1;
					*menuLoop = 0;
				}
			}
		}

	}

	return running;
}