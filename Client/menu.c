#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "game.h"


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

	SDL_Rect backRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

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