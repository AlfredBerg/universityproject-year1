#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "game.h"

struct menu {
	IPaddress ip;
	int nrOfPlayers;
	int play;
	int countdown;
	int sound;
	int exit;
};
typedef struct menu Menu;

int menu(Game *game, char serverIP[]);
int menuOptions(int *menuLoop, int *menuPage, Game *game, char serverIP[]);
void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Rect *rect, SDL_Color *color);
int isAllowed(char* ch, char serverIP[]);
int nrOfDots(char serverIP[16]);
int lobby(Network *client, Game *game, char playerNames[][30]);