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

#define PORTNR 12346

void init(UDPsocket *serverSocket, IPaddress *serverSettings) {
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
	*serverSocket = SDLNet_UDP_Open(PORTNR);
	if (!(*serverSocket))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(serverSettings, NULL, PORTNR))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

}

void *handleClient(void *vargp) {

}

int main(int argc, char **argv)
{
	UDPsocket serverSocket;
	IPaddress serverSettings;

	init(&serverSocket, &serverSettings);

	UDPpacket *packet;

	packet = SDLNet_AllocPacket(1024);
	if (!packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	int exit = 0;

	while (!exit) {
		if (SDLNet_UDP_Recv(serverSocket, packet)) {
			printf("I got a packet\n");
			printf("Packet data: %s\n", packet->data);

		}
	}



	SDLNet_FreePacket(packet);
	SDLNet_Quit();
	SDL_Quit();
	return(0);
}
