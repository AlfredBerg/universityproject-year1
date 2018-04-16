#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#define MAXPLAYERS 4

struct game {

	int running;
	int livingPlayers[MAXPLAYERS];
	int connectedPlayers[MAXPLAYERS];
	SDL_Window* window;
	SDL_Renderer* renderer;

};
typedef struct game Game;