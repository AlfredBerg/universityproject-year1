#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "clientNetwork.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_PLAYERS 4
#define UP 1
#define LEFT 2
#define RIGHT 3
#define RENDER_TICK 20
#define MAX_IMAGES 1000

struct game {

	int running;
	int livingPlayers[MAX_PLAYERS];
	int connectedPlayers[MAX_PLAYERS];
	SDL_Window* window;
	SDL_Renderer* renderer;
	int debug;
	SDL_Texture *cloudsBack;
	SDL_Texture *cloudsFront;
	SDL_Texture *bgBack;
	SDL_Texture *bgFront;

};
typedef struct game Game;

void initGame(Game *game); 
int restart(Game *game);
int runGame(Game *game, Network *client);
void quitGame(Game *game);
void playBackgroundMusic();