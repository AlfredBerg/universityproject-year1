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

int menu(Game *game);
int menuOptions(int *menuLoop);