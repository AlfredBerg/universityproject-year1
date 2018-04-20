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
#include "serverNetwork.h"
#include "sharedNetwork.h"
#include "clients.h"

void updateClients(Network *server, Uint32 *lastTick) {

	if (SDL_TICKS_PASSED(SDL_GetTicks(), *lastTick + TICK_RATE)) {

		char data[MAX_PACKET];
		gamestateToString(server, data);

		//puts(data);

		for (int i = 0; i < MAX_SOCKETS; i++) {
			sendPacket(data, server->clients[i].ip, server->serverSocket);
		}

		*lastTick = SDL_GetTicks();
	}
}

updatePositions(Network *server, char indata[]) {
	char data[5][30];

	decode(indata, data, MAX_PACKET, 2);
	//printf("Who sen packet: %d\n", server->whoSentThePacket);
	server->clients[server->whoSentThePacket].xPos = atoi(data[0]);
	server->clients[server->whoSentThePacket].yPos = atoi(data[1]);
}


int isClient(Network *server) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (server->clients[i].ip.host == server->serverSocketPacket->address.host && server->clients[i].ip.port == server->serverSocketPacket->address.port) {
			server->whoSentThePacket = i;
			return 1;
		}
	}
	return 0;
}

void newClient(int *nrReady, Network *server) {
	int newClientAccepted = AcceptSocket(server);
	(*nrReady)--;

}

int AcceptSocket(Network *server) {
	char packetdata[MAX_PACKET];

	int success = receivePacket(server->serverSocket, server->serverSocketPacket, packetdata);

	if (!success) {
		return 0;
	}

	//printf("\nIncoming: %s\n", packetdata);
	if (!strcmp("HELLO\n", packetdata)) {

		if (server->next_player >= MAX_SOCKETS) {
			fprintf(stderr, "ER: Server full!.\n");
			return 0;
		}

		printf("I got a new client that want to connect\n");

		server->clients[server->next_player].ip = server->serverSocketPacket->address;

		char data[MAX_PACKET];
		sprintf(data, "%s;%d;", "HELLO CLIENT", server->next_player);

		sendPacket(data, server->clients[server->next_player].ip, server->serverSocket);
		server->clients[server->next_player].inUse = 1;
		server->next_player++;
		return 1;
	}
	else if (isClient(server)){
		updatePositions(server, packetdata);
	}
	else {
		puts("Unknown packet recived");
	}

	return 0;
}

void closeSocket(Network *server, int index) {
	if (server->sockets[index] == NULL) {
		fprintf(stderr, "ER: Attempted to delete a NULL socket.\n");
		return;
	}

	if (SDLNet_UDP_DelSocket(server->socketSet, server->sockets[index]) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	memset(&server->clients[index], 0x00, sizeof(Client));
	SDLNet_UDP_Close(server->sockets[index]);
	server->sockets[index] = NULL;
}