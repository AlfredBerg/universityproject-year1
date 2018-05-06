#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "clientNetwork.h"
#include "map.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 640
#define MAX_PLAYERS 4
#define UP 1
#define LEFT 2
#define RIGHT 3
#define RENDER_TICK 20
#define MAX_IMAGES 1000

struct game {
	int running;
	int spectateMode;
	int loopCount;
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
//int restart(Game *game);
int runGame(Game *game, Network *client);
void quitGame(Game *game);
void playBackgroundMusic();
void createWindowIcon(Game *game);

// These functions could not be placed in player-module
Player createPlayer(Game *game, int id, char name[], int x, int y, int lastDirection, const char imageName[], int srcRectW, int srcRectH);
void drawPlayers(Game *game, Player players[], int *nrOfPlayers, int *leftWall, int *rightWall);