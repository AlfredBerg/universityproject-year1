#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define WINDOW_LENGTH 800
#define WINDOW_HEIGHT 600
#define MAX_PLAYERS 4

struct game {

	int running;
	int livingPlayers[MAX_PLAYERS];
	int connectedPlayers[MAX_PLAYERS];
	SDL_Window* window;
	SDL_Renderer* renderer;

};
typedef struct game Game;

void initGame(Game *game);