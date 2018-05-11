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

#include "game.h"
#include "player.h"
#include "menu.h"
#include "weapon.h"
#include "sharedNetwork.h"
#include "clientNetwork.h"
#include "map.h"

Network client;

int main(int argc, char** argv)
{
	Game game;
	char serverIP[16] = "127.0.0.1";
	char playerName[16] = "Player";
	char playerNames[4][30] = {"Knight", "Bear", "Bird", "Princess"};
	

	initGame(&game); 

	while (game.running) {
		menu(&game, serverIP, playerName);
		if (initClient(&client, serverIP, playerName))
			lobby(&client, &game, playerNames);
			1 == 1;
		while (game.running) {
			game.running = runGame(&game, &client, playerNames);
		}
	}

	quitGame(&game);
	return 0;
}