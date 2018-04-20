#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_net.h>

#include "player.h"
#include "game.h"
#include "menu.h"
#include "weapon.h"
#include "gravity.h"
#include "sharedNetwork.h"
#include "clientNetwork.h"
#include "map.h"

int main(int argc, char** argv)
{
	Game game;
	Network client;

	initGame(&game, &client);

	while (game.running) {
		game.running = menu(&game);
		while (game.running) {
			game.running = rungame(&game, &client);
		}
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}


