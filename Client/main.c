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
#include "sharedNetwork.h"
#include "clientNetwork.h"
#include "map.h"

Network client;

int main(int argc, char** argv)
{	
	Game game;
	char serverIP [16] = "127.0.0.1";

	initGame(&game); 
	//initClient(&client);

	while (game.running) {
		game.running = menu(&game, serverIP);
		initClient(&client, serverIP);
		while (game.running) {
			game.running = runGame(&game, &client);
		}
	}

	quitGame(&game);
	return 0;
}