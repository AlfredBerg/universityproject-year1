#pragma once
#include "SDL.h"
#include  "game.h"

struct background {

	int x;
	int y;

};
typedef struct background Background;

void displayBackground(Game *game);
