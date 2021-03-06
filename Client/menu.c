#include "menu.h"

int menu(Game *game, char serverIP[], char playerName[]) {

	//Init menu images
	SDL_Surface *menuImage1 = IMG_Load("assets/meny1.png");
	SDL_Texture *background1 = SDL_CreateTextureFromSurface(game->renderer, menuImage1);
	SDL_FreeSurface(menuImage1);


	game->menuImage2 = IMG_Load("assets/meny2.png");
	game->background2 = SDL_CreateTextureFromSurface(game->renderer, game->menuImage2);
	SDL_FreeSurface(game->menuImage2);

	game->menuImage3 = IMG_Load("assets/meny3.png");
	game->background3 = SDL_CreateTextureFromSurface(game->renderer, game->menuImage3);
	SDL_FreeSurface(game->menuImage3);

	game->font = TTF_OpenFont("assets/pixlig font.ttf", 40);

	game->clickSound = Mix_LoadWAV("assets/clickSound.wav");
	game->clickSound->volume = 100;
	game->typeSound = Mix_LoadWAV("assets/typeSound.wav");
	game->typeSound->volume = 100;
	game->eraseSound = Mix_LoadWAV("assets/eraseSound.wav");
	game->eraseSound->volume = 50;

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

		startGame = menuOptions(&menuLoop, &menuPage, game, serverIP, playerName);
	}

	//  SDL_DestroyTexture(text);
	SDL_DestroyTexture(background1);
	//SDL_DestroyTexture(background2);
	return startGame;
}

int menuOptions(int *menuLoop, int *menuPage, Game *game, char serverIP[], char playerName[]) {
	int running = 1, done = SDL_FALSE;
	SDL_Event event;

	SDL_Rect textRect;
	SDL_Color color = { 65, 33, 52, 255 };
	
	

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) {
			running = 0;
			*menuLoop = 0;
		}

		switch (*menuPage) {
		case 1:
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > 220 && event.button.x < 800 && event.button.y > 210 && event.button.y < 420) {
					*menuPage = 2;
					Mix_PlayChannel(1, game->clickSound, 0);
				}
			}
			else if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)) {
				*menuPage = 2;
				Mix_PlayChannel(1, game->clickSound, 0);
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
							if (isAllowed(event.text.text, serverIP)) {
								strcat(serverIP, event.text.text);
								Mix_PlayChannel(1, game->typeSound, 0);
							}
						break;
					case SDL_KEYDOWN:
						//Handle backspace
						if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(serverIP) > 0) {
							serverIP[strlen(serverIP) - 1] = '\0';
							Mix_PlayChannel(1, game->eraseSound, 0);
						}
						//Handle return
						else if (event.key.keysym.sym == SDLK_RETURN) {
							Mix_PlayChannel(1, game->clickSound, 0);
							if (strlen(serverIP) >= 7 && serverIP[strlen(serverIP) - 1] != '.') {
								*menuPage = 3;
								done = SDL_TRUE;
							}
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT && strlen(serverIP) >= 7) {
							if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
								Mix_PlayChannel(1, game->clickSound, 0);
								if (serverIP[strlen(serverIP) - 1] != '.') {
									*menuPage = 3;
									done = SDL_TRUE;
								}
							}
						}
						break;
					}
				}
				SDL_RenderCopy(game->renderer, game->background2, NULL, NULL);
				render_text(game->renderer, 336, 292, serverIP, game->font, &textRect, &color);
				SDL_RenderPresent(game->renderer);
			}
			SDL_DestroyTexture(game->background2);
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
						if (strlen(playerName) < 15) {
							strcat(playerName, event.text.text);
							Mix_PlayChannel(1, game->typeSound, 0);
						}
						break;
					case SDL_KEYDOWN:
						//Handle backspace
						if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(playerName) > 0) {
							playerName[strlen(playerName) - 1] = '\0';
							Mix_PlayChannel(1, game->eraseSound, 0);
						}
						//Handle return
						else if (event.key.keysym.sym == SDLK_RETURN && strlen(playerName) > 0) {
							running = 1;
							*menuLoop = 0;
							done = SDL_TRUE;
							Mix_PlayChannel(1, game->clickSound, 0);
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT && strlen(playerName) > 0) {
							if (event.button.x > 450 && event.button.x < 570 && event.button.y > 360 && event.button.y < 420) {
								running = 1;
								*menuLoop = 0;
								done = SDL_TRUE;
								Mix_PlayChannel(1, game->clickSound, 0);
							}
						}
						break;
					}
				}
				SDL_RenderCopy(game->renderer, game->background3, NULL, NULL);
				render_text(game->renderer, 336, 292, playerName, game->font, &textRect, &color);
				SDL_RenderPresent(game->renderer);
			}
			SDL_DestroyTexture(game->background3);
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

int isAllowed(char* ch, char serverIP[16]) {
	int foundDot = 0;

	if (!strcmp(ch, ".") && serverIP[strlen(serverIP) - 1] != '.' && strlen(serverIP) > 0) {
		if (nrOfDots(serverIP) < 3)
			return 1;
		else
			return 0;
	}
	else {
		if (strlen(serverIP) < 3)
			foundDot = 1;
		else
			for (int i = strlen(serverIP) - 1; i > strlen(serverIP) - 4; i--) {
				if (serverIP[i] == '.')
					foundDot = 1;
			}
		if (foundDot) {
			if (!strcmp(ch, "0")) return 1;
			else if (!strcmp(ch, "1")) return 1;
			else if (!strcmp(ch, "2")) return 1;
			else if (!strcmp(ch, "3")) return 1;
			else if (!strcmp(ch, "4")) return 1;
			else if (!strcmp(ch, "5")) return 1;
			else if (!strcmp(ch, "6")) return 1;
			else if (!strcmp(ch, "7")) return 1;
			else if (!strcmp(ch, "8")) return 1;
			else if (!strcmp(ch, "9")) return 1;
		}
	}
	

	return 0;
}

int nrOfDots(char serverIP[16]) {
	int nrOfDots = 0;
	for (int i = 0; i < strlen(serverIP); i++)
		if (serverIP[i] == '.')
			nrOfDots++;

	return nrOfDots;
}

int lobby(Network *client, Game *game, char playerNames[][30]) {
	int timer = 60;
	game->connectedPlayers = 1;
	char lobbyinData[MAX_PACKET];
	char lobbyData[100][30];
	int done = 0;
	int c;

	//Grafik
	TTF_Font *font = TTF_OpenFont("assets/pixlig font.ttf", 58);
	SDL_Color color = { 65, 33, 52, 255 };
	SDL_Color colorW = { 255, 255, 255, 255 };
	SDL_Rect textRect;

	SDL_Surface *lobbyImage1 = IMG_Load("assets/lobby1.png");
	SDL_Texture *background1 = SDL_CreateTextureFromSurface(game->renderer, lobbyImage1);
	SDL_FreeSurface(lobbyImage1);

	SDL_Surface *lobbyImage2 = IMG_Load("assets/lobby2.png");
	SDL_Texture *background2 = SDL_CreateTextureFromSurface(game->renderer, lobbyImage2);
	SDL_FreeSurface(lobbyImage2);

	int lobbyPacketOffset = 0;

	while (!done) {
		c = 4;

		receivePacket(client->serverSocket, client->packet, lobbyinData);

		if (lobbyinData[0] != '4') {
			//done = 1;
			printf("\nNot lobby packet! Exiting\n");
		}
		else {
			decode(lobbyinData, lobbyData, 13, 30);
			game->connectedPlayers = atoi(lobbyData[1]);
			game->seed = atoi(lobbyData[2]);
			timer = atoi(lobbyData[3]);

			SDL_RenderClear(game->renderer);
			if (game->connectedPlayers > 1) {
				SDL_RenderCopy(game->renderer, background2, NULL, NULL);
				render_text(game->renderer, 742, 45, lobbyData[1], font, &textRect, &color);
				render_text(game->renderer, 670, 555, lobbyData[3], font, &textRect, &colorW);
				for (int i = 0; i < game->connectedPlayers; i++) {
					render_text(game->renderer, 500 - (strlen(lobbyData[c + lobbyPacketOffset]) * 16), 150 + i * 70, lobbyData[c + lobbyPacketOffset], font, &textRect, &color);
					strcpy(playerNames[i], lobbyData[c + lobbyPacketOffset]);
					c += 1;
				}
				SDL_Event event;
				if (SDL_PollEvent(&event))
					if (event.type == SDL_QUIT) {
						return 0;
					}
			}
			else {
				SDL_RenderCopy(game->renderer, background1, NULL, NULL);
				SDL_Event event;
				if (SDL_PollEvent(&event))
					if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT || event.type == SDL_KEYDOWN){
						if (event.button.x > 305 && event.button.x < 715 && event.button.y > 550 && event.button.y < 620 || event.key.keysym.sym == SDLK_RETURN)
							done = 1;
					}
					else if (event.type == SDL_QUIT) {
						return 0;
					}
			}
			SDL_RenderPresent(game->renderer);

			if (timer == 0 || game->connectedPlayers == 4)
				done = 1;
		}
	}
	game->spectateMode = 0;

	return 1;
}