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
#include "playerStruct.h"
#include "sharedNetwork.h"


void positionToString(Player *player, char string[]) {
	//x1;y1
	sprintf(string, "%d;%d", player->p1.x, player->p1.y);
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

void updateServer(Player *fighter, Network *client) {
	int num_rdy = SDLNet_CheckSockets(client->socketSet, 0);
	char data[MAX_PACKET];
	if ((SDLNet_SocketReady(client->serverSocket))) {
		printf("\nIncoming packet!\n");

		
		receivePacket(client->serverSocket, client->packet, data);
		puts(data);
		
		//parse the data...

	}

	//Send a update to the server about my position
	
	//printf("\nMy data: %s\n", data);
	sendPositionToServer(client, fighter);
}