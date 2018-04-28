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

void bulletsToString(Network *server, char string[MAX_PACKET], int projectileType);


void updateClients(Network *server, Uint32 *lastTick) {

	if (SDL_TICKS_PASSED(SDL_GetTicks(), *lastTick + TICK_RATE)) {

		char data[MAX_PACKET];
		gamestateToString(server, data);

		char bulletData[MAX_PACKET];
		bulletsToString(server, bulletData, BULLET);
		//puts(data);

		for (int i = 0; i < MAX_SOCKETS; i++) {
			sendPacket(data, server->clients[i].ip, server->serverSocket);

			if (server->projectileData[BULLET].nrProjectilesShot) {
				sendPacket(bulletData, server->clients[i].ip, server->serverSocket);
			}
			
		}

		*lastTick = SDL_GetTicks();
	}
}

void updatePositions(Network *server, char data[][30]) {
	printf("Positions update\n");
	server->clients[server->whoSentThePacket].xPos = atoi(data[1]);
	server->clients[server->whoSentThePacket].yPos = atoi(data[2]);
}

void createProjectiles(Network *server, char data[][30]) {
	printf("Projectile created\n");
	//projectile type, x, y, direction
	int id = server->projectileData[BULLET].nrProjectilesShot;

	if (id >= MAXPROJECTILEOBJECTS) {
		id = id % MAXPROJECTILEOBJECTS;
	}

	server->projectileData[BULLET].Projectiles[id].id = id;
	server->projectileData[BULLET].Projectiles[id].projectileType = atoi(data[1]);
	server->projectileData[BULLET].Projectiles[id].x = atoi(data[2]);
	server->projectileData[BULLET].Projectiles[id].y = atoi(data[3]);
	server->projectileData[BULLET].Projectiles[id].direction = atoi(data[4]);

	(server->projectileData[BULLET].nrProjectilesShot)++;

}

void updateServerdata(Network *server, char indata[]) {
	char data[DATAFIELDSINPACKET][30];

	//puts(indata); //Debug see what the server recives


	decode(indata, data, MAX_PACKET, DATAFIELDSINPACKET);


	switch (atoi(data[0])) //What kind of data is in this packet?
	{
	case 0: updatePositions(server, data); break;
	case 1: createProjectiles(server, data); break;
	default:
		printf("Packet from known host but unknown data\n");
		break;
	}
	
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
		updateServerdata(server, packetdata);
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


void gamestateToString(Network *server, char string[]) {
	//x1;y1;x2;y2
	sprintf(string, "0;%d;%d;%d;%d;", server->clients[0].xPos, server->clients[0].yPos, server->clients[1].xPos, server->clients[1].yPos);
}

void bulletsToString(Network *server, char string[MAX_PACKET], int projectileType) {
	int length = 0;

	length += sprintf(string + length, "1;");
	//id, projectile type, x, y, direction
	for (int i = 0; i < MAXPROJECTILEOBJECTS; i++) {
		if (i > server->projectileData[projectileType].nrProjectilesShot - 1) {
			return;
		}
		length += sprintf(string + length, "%d;%d;%d;%d;%d;", server->projectileData[projectileType].Projectiles[i].id, server->projectileData[projectileType].Projectiles[i].projectileType, server->projectileData[projectileType].Projectiles[i].x, server->projectileData[projectileType].Projectiles[i].y, server->projectileData[projectileType].Projectiles[i].direction);
	}
}