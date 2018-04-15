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

#define PORTNR 12346
#define SOCKET_TIMEOUT 0

void init(Network *server);
void quit(Network *server, UDPpacket *packet);
void closeSocket(Network *server, int index);

int main(int argc, char **argv)
{
	Network server;

	init(&server);

	UDPpacket *packet;

	packet = SDLNet_AllocPacket(1024);
	if (!packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/*
	int exit = 0;

	while (!exit) {
		if (SDLNet_UDP_Recv(server.serverSocket, packet)) {
			printf("I got a packet\n");
			printf("Packet data: %s\n From %d\n", packet->data, packet->address);

		}
	}
	*/

	int nrReady = 0;
	while (server.running) {
		nrReady = SDLNet_CheckSockets(server.socketSet, SOCKET_TIMEOUT);

		if (nrReady == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			perror("SDLNet_CheckSockets");
		}
		else if (nrReady == 0) {//Might need to be changed to prioritize game state
			//No sockets ready, do server activity
		}
		else {
			printf("%d sockets ready", nrReady);
			if (SDLNet_SocketReady(server.serverSocket)) {
				//newClient();
			}
		}
	}

	quit(&server, packet);

	return(0);
}

void quit(Network *server, UDPpacket *packet) {
	if (SDLNet_UDP_DelSocket(server->socketSet, server->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}
	SDLNet_UDP_Close(server->serverSocket);

	//TODO, figure out why the code bellow wont delete and close the sockets
	/*
	int i;
	for (i = 0; i < MAX_SOCKETS; ++i) {
		if (server->sockets[i] == NULL) continue;
		closeSocket(server, i);
	}
	*/

	SDLNet_FreeSocketSet(server->socketSet);
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
	SDL_Quit();
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


void init(Network *server) {
	// Initialize SDL
	if (SDL_Init(0) != 0)
	{
		fprintf(stderr, "Erro initializing SDL %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}


	// Initialize SDL_net
	if (SDLNet_Init() != 0)
	{
		fprintf(stderr, "Erro initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	server->serverSocket = SDLNet_UDP_Open(PORTNR);
	if (!(server->serverSocket))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&server->serverIP, NULL, PORTNR))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	server->socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS + 1);

	if (server->socketSet == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}
	
	if (SDLNet_UDP_AddSocket(server->socketSet, server->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}
	server->running = 1;

}


