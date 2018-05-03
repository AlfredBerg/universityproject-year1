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
	char playerName[16] = "Player";
	int done = SDL_FALSE;
	
	//Grafik
	TTF_Font *font = TTF_OpenFont("assets/pixlig font.ttf", 40);
	SDL_Color color = { 65, 33, 52, 255 };
	SDL_Rect textRect;

	SDL_Surface *menuImage2 = IMG_Load("assets/meny2.png");
	SDL_Texture *background2 = SDL_CreateTextureFromSurface(game->renderer, menuImage2);
	SDL_FreeSurface(menuImage2);

	SDL_Surface *menuImage3 = IMG_Load("assets/meny3.png");
	SDL_Texture *background3 = SDL_CreateTextureFromSurface(game->renderer, menuImage3);
	SDL_FreeSurface(menuImage3);

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
						if (strlen(serverIP) < 15)
							if (isAllowed(event.text.text))
								strcat(serverIP, event.text.text);
						break;
					case SDL_KEYDOWN:
						//Handle backspace
						if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(serverIP) > 0)
							serverIP[strlen(serverIP)-1] = '\0';
						//Handle return
						else if (event.key.keysym.sym == SDLK_RETURN && strlen(serverIP) >= 7) {
							*menuPage = 3;
							done = SDL_TRUE;
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT && strlen(serverIP) >= 7) {
							if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
								*menuPage = 3;
								done = SDL_TRUE;
							}
						}
						break;
					}
				}
				SDL_RenderCopy(game->renderer, background2, NULL, NULL);
				render_text(game->renderer, 336, 292, serverIP, font, &textRect, &color);
				SDL_RenderPresent(game->renderer);
			}
			SDL_DestroyTexture(background2);
			SDL_RenderClear(game->renderer);
			SDL_StopTextInput();
			break;
		case 3:
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
						if (strlen(playerName) < 15)
							strcat(playerName, event.text.text);
						break;
					case SDL_KEYDOWN:
						//Handle backspace
						if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(playerName) > 0)
							playerName[strlen(playerName) - 1] = '\0';
						//Handle return
						else if (event.key.keysym.sym == SDLK_RETURN && strlen(playerName) > 0) {
							running = 1;
							*menuLoop = 0;
							done = SDL_TRUE;
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT && strlen(playerName) > 0) {
							if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
								running = 1;
								*menuLoop = 0;
								done = SDL_TRUE;
							}
						}
						break;
					}
				}
				SDL_RenderCopy(game->renderer, background3, NULL, NULL);
				render_text(game->renderer, 336, 292, playerName, font, &textRect, &color);
				SDL_RenderPresent(game->renderer);
			}
			SDL_DestroyTexture(background3);
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

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, rect);
	SDL_DestroyTexture(texture);
}

int isAllowed(char* ch) {
	if (!strcmp(ch, ".")) return 1;
	else if (!strcmp(ch, "0")) return 1;
	else if (!strcmp(ch, "1")) return 1;
	else if (!strcmp(ch, "2")) return 1;
	else if (!strcmp(ch, "3")) return 1;
	else if (!strcmp(ch, "4")) return 1;
	else if (!strcmp(ch, "5")) return 1;
	else if (!strcmp(ch, "6")) return 1;
	else if (!strcmp(ch, "7")) return 1;
	else if (!strcmp(ch, "8")) return 1;
	else if (!strcmp(ch, "9")) return 1;

	return 0;
}