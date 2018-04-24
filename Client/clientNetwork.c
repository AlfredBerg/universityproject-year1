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

int compareString(char str1[], char str2[], int len);

void connectToServer(Network *client) {
	char data[MAX_PACKET] = "HELLO\n";
	char decoded[2][30];

	sendPacket(data, client->serverIP, client->serverSocket);

	SDL_Delay(500); //test delay

	receivePacket(client->serverSocket, client->packet, data);
	if (compareString(data, "HELLO CLIENT", 12)) {
		printf("I am now connected to the server!\n");
		decode(data, decoded, 2, 30);
		client->playerID = atoi(decoded[1]);
		client->connectedToServer = 1;
		printf("PLAYER ID: %d \n", client->playerID);
	}
	/*
	else {
	puts("I could not connect to the server, quiting");
	exit(0);
	}
	*/

}

int compareString(char str1[], char str2[], int len) {
	for (int i = 0; i < len; i++) {
		if (!(str1[i] == str2[i])) {
			return 0;
		}
	}
	return 1;
}

void positionToString(Player *player, char string[]) {
	//x1;y1
	sprintf(string, "%d;%d;", player->rect.x, player->rect.y);
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

void updateServer(Player *player, Network *client) {
	if (client->connectedToServer == 0) {
		return;
	}

	sendPositionToServer(client, &player[client->playerID]);

	int num_rdy = SDLNet_CheckSockets(client->socketSet, 0);
	char data[MAX_PACKET];
	if ((SDLNet_SocketReady(client->serverSocket))) {

		int success = receivePacket(client->serverSocket, client->packet, data);

		if (!success) {
			return;
		}

		//printf("Incoming data: %s\n", data);
		parseData(data, player, client);
	}

}

void parseData(char serverdata[], Player *player, Network *client) {
	char parsedData[30][30];

	decode(serverdata, parsedData, 4, 30);


	for (int i = 0; i < 4; i++) {
		if (client->playerID == i) {
			continue;
		}
		player[i].x = atoi(parsedData[i * 2]);
		player[i].y = atoi(parsedData[i * 2 + 1]);
	}
}

void initClient(Network *client) {

	client->lastTick = SDL_GetTicks();
	client->connectedToServer = 0;
	client->playerID = 0;

	// Initialize SDL_net
	if (SDLNet_Init() != 0) {
		fprintf(stderr, "Error initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&client->serverIP, SERVERIP, SERVERPORT)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	client->serverSocket = SDLNet_UDP_Open(CLIENTPORT);
	if (!(client->serverSocket)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->packet = SDLNet_AllocPacket(1024);
	if (!client->packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->socketSet = SDLNet_AllocSocketSet(1);
	if (client->socketSet == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}

	if (SDLNet_UDP_AddSocket(client->socketSet, client->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	connectToServer(client);
}
