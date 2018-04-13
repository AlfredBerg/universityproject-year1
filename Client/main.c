/* Stephen's Tutorials (http://stephenmeier.net/)
gcc main.c -o game.exe -I./include -L./lib -lSDL2main -lSDL2
running on gcc 4.8.1, SDL 2.0.1
*/

//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------
#define SDL_MAIN_HANDLED
#include "SDL.h"

//----------------------------------------------------------------------
void game_init(void);
void game_quit(void);

static struct {
	// define "attributes"
	SDL_bool running;

	// define "methods"
	void(*init)(void);
	void(*quit)(void);
} Game = {
	SDL_FALSE,
	game_init,
	game_quit
};

//----------------------------------------------------------------------
void game_init(void) {
	printf("game_init()\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL error -> %s\n", SDL_GetError());
		exit(1);
	}

	Game.running = SDL_TRUE;
}

//----------------------------------------------------------------------
void game_quit(void) {
	printf("game_quit()\n");
	SDL_Quit();
	Game.running = SDL_FALSE;
}

//----------------------------------------------------------------------
int main(int argc, char* argv[]) {

	printf("running = %d\n", Game.running);

	Game.init();

	printf("running = %d\n", Game.running);

	while (Game.running) {
		SDL_Delay(5000);
		Game.quit();
	}

	printf("running = %d\n", Game.running);

	return 0;
}