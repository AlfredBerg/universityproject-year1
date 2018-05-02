#pragma once
#include "menu.h"

void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Rect *rect, SDL_Color *color);

int menu(Game *game, char serverIP[]) {

	//Init menu images
	SDL_Surface *menuImage1 = IMG_Load("assets/meny1.png");
	SDL_Texture *background1 = SDL_CreateTextureFromSurface(game->renderer, menuImage1);
	SDL_FreeSurface(menuImage1);
	

	int startGame = 1;
	int menuLoop = 1;
	int menuPage = 1;

	//Draw
	while (menuLoop) {
		switch (menuPage) {
		case 1:
			SDL_RenderCopy(game->renderer, background1, NULL, NULL);
			SDL_RenderPresent(game->renderer);
			break;
		case 2:
		//	SDL_RenderCopy(game->renderer, background2, NULL, NULL);
		//	SDL_RenderPresent(game->renderer);
			break;
		}

		startGame = menuOptions(&menuLoop, &menuPage, game, serverIP);
	}

	//  SDL_DestroyTexture(text);
	SDL_DestroyTexture(background1);
	//SDL_DestroyTexture(background2);
	return startGame;
}

int menuOptions(int *menuLoop, int *menuPage, Game *game, char serverIP[]) {
	int running = 1;
	SDL_Event event;

	//Init text
	//char ip[16] = "192.186.0.2";
	int done = SDL_FALSE;

	//Anv�nds typ inte
	char *composition;
	Sint32 cursor;
	Sint32 selection_len;
	
	TTF_Font *font = TTF_OpenFont("assets/Capture_it.ttf", 40);
	SDL_Color color = { 65, 33, 52, 255 };
	SDL_Rect textRect;

	SDL_Surface *menuImage2 = IMG_Load("assets/meny2.png");
	SDL_Texture *background2 = SDL_CreateTextureFromSurface(game->renderer, menuImage2);
	SDL_FreeSurface(menuImage2);

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
						*menuPage = 2;
					}
				}
			}
			break;
		case 2:
			SDL_StartTextInput();

			while (!done) {
				SDL_Event event;
				if (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_QUIT:
						done = SDL_TRUE;
						running = 0;
						*menuLoop = 0;
						break;
					case SDL_TEXTINPUT:
						//Add new text onto the end of our text
						if (strlen(serverIP) < 16)
							strcat(serverIP, event.text.text);
						printf("%s \n", serverIP);
						break;
					case SDL_KEYDOWN:
							//Handle backspace
							if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(serverIP) > 0)
							{
								printf("SIZE OF: %d\n", strlen(serverIP));
								serverIP[strlen(serverIP)-1] = '\0';
							}
							break;
					case SDL_TEXTEDITING:
						/*
						Update the composition text.
						Update the cursor position.
						Update the selection length (if any).
						*/
						composition = event.edit.text;
						cursor = event.edit.start;
						selection_len = event.edit.length;
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT) {
							if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
								running = 1;
								*menuLoop = 0;
								done = SDL_TRUE;
							}
						}
						break;
					}
				}
				SDL_RenderCopy(game->renderer, background2, NULL, NULL);
				render_text(game->renderer, 340, 290, serverIP, font, &textRect, &color);
				SDL_RenderPresent(game->renderer);
			}
			
			SDL_StopTextInput();
			break;
		}

	}

	return running;
}

void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Rect *rect, SDL_Color *color) {
	SDL_Surface *surface;
	SDL_Texture *texture;

	//Crash fix
	if (strlen(text) == 0) {
		text = " ";
	}

	surface = TTF_RenderText_Solid(font, text, *color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	rect->x = x;
	rect->y = y;
	rect->w = surface->w;
	rect->h = surface->h;
	/* This is wasteful for textures that stay the same.
	* But makes things less stateful and easier to use.
	* Not going to code an atlas solution here... are we? */
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, rect);
	SDL_DestroyTexture(texture);
}