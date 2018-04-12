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

void init(UDPsocket *clientSocket) {
	// Initialize SDL
	if (SDL_Init(0) != 0)
	{
		fprintf(stderr, "Error initializing SDL %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	// Initialize SDL_net
	if (SDLNet_Init() != 0)
	{
		fprintf(stderr, "Error initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	*clientSocket = SDLNet_UDP_Open(0);
	if (!(*clientSocket))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

}

int main(int argc, char **argv)
{
	UDPsocket clientSocket;
	
	init(&clientSocket);

	IPaddress serverAddress;
	SDLNet_ResolveHost(&serverAddress, "127.0.0.1", 12346);

	UDPpacket *packet;
	packet = SDLNet_AllocPacket(1024);
	if (!packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	packet->data = "Hello world";
	packet->len = strlen(packet->data);
	packet->address = serverAddress;


	SDLNet_UDP_Send(clientSocket, -1, packet);
	SDLNet_UDP_Send(clientSocket, -1, packet);

	SDLNet_FreePacket(packet);
	SDLNet_Quit();
	SDL_Quit();
	return(0);
}
