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

	//Init menu images
	SDL_Surface *menuImage1 = IMG_Load("assets/meny1.png");
	SDL_Texture *background1 = SDL_CreateTextureFromSurface(game->renderer, menuImage1);
	SDL_FreeSurface(menuImage1);

	SDL_Surface *menuImage2 = IMG_Load("assets/meny2.png");
	SDL_Texture *background2 = SDL_CreateTextureFromSurface(game->renderer, menuImage2);
	SDL_FreeSurface(menuImage2);

	int startGame = 1;
	int menuLoop = 1;
	int menuPage = 1;

	//Draw
	while (menuLoop) {
		switch (menuPage) {
		case 1:
			SDL_RenderCopy(game->renderer, background1, NULL, NULL);
			//	SDL_RenderCopy(game->renderer, text, NULL, &textRect);
			SDL_RenderPresent(game->renderer);
			break;
		case 2:
			SDL_RenderCopy(game->renderer, background2, NULL, NULL);
			//	SDL_RenderCopy(game->renderer, text, NULL, &textRect);
			SDL_RenderPresent(game->renderer);
			break;
		}

		startGame = menuOptions(&menuLoop, &menuPage);
	}

	//  SDL_DestroyTexture(text);
	SDL_DestroyTexture(background1);
	SDL_DestroyTexture(background2);
	return startGame;
}

int menuOptions(int *menuLoop, int *menuPage) {
	int running = 1;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) {
			running = 0;
			*menuLoop = 0;
		}

		switch (*menuPage) {
		case 1:
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x > 220 && event.button.x < 800 && event.button.y > 210 && event.button.y < 420) {
						//running = 1;
						//*menuLoop = 0;
						*menuPage = 2;
					}
				}
			}
			break;
		case 2:
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
						running = 1;
						*menuLoop = 0;
					}
				}
			}
			break;
		}

	}

	return running;
}