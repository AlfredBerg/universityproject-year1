#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef _MSC_VER
#include <unistd.h> 
#endif

#include "SDL.h"
#include "SDL_net.h"
#include "clientNetwork.h"
#include "player.h"
#include "sharedNetwork.h"

void connectToServer(Network *client) {
	char data[MAX_PACKET] = "HELLO\n";

	sendPacket(data, client->serverIP, client->serverSocket);

	SDL_Delay(500); //test delay

	receivePacket(client->serverSocket, client->packet, data);

	if (!strcmp(data, "HELLO CLIENT")) {
		printf("I am now connected to the server!\n");
		client->connectedToServer = 1;
	}
	/*
	else {
		puts("I could not connect to the server, quiting");
		exit(0);
	}
	*/

}

void positionToString(Player *player, char string[]) {
	//x1;y1
	sprintf(string, "%d;%d;", player->p1.x, player->p1.y);
}

void sendPositionToServer(Network *client, Player *fighter) {
	if (SDL_TICKS_PASSED(SDL_GetTicks(), client->lastTick + TICK_RATE)) {
		char data[MAX_PACKET];
		positionToString(fighter, data);
		puts(data);
		sendPacket(data, client->serverIP, client->serverSocket);
		client->lastTick = SDL_GetTicks();
		
	}
}

void updateServer(Player *fighter, Player *enemy,Network *client) {
	if (client->connectedToServer == 0) {
		return;
	}

	sendPositionToServer(client, fighter);

	int num_rdy = SDLNet_CheckSockets(client->socketSet, 0);
	char data[MAX_PACKET];
	if ((SDLNet_SocketReady(client->serverSocket))) {

		int success = receivePacket(client->serverSocket, client->packet, data);

		if (!success) {
			return;
		}

		printf("Incoming data: %s\n", data);
		parseData(data, enemy);
	}
	
}

void parseData(char serverdata[], Player *enemy) {
	char parsedData[30][30];

	decode(serverdata, parsedData, 4, 30);

	enemy->x = atoi(parsedData[2]);
	enemy->y = atoi(parsedData[3]);
}