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


void sendPacket(char data[], IPaddress ip, UDPsocket socket) {
	UDPpacket *send;

	send = SDLNet_AllocPacket(1024);
	if (!send) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	strcpy(send->data, data);
	send->len = strlen(data);
	send->address = ip;
	SDLNet_UDP_Send(socket, -1, send);
}

void receivePacket(UDPsocket socket, UDPpacket *packet, char string[]) {
	SDLNet_UDP_Recv(socket, packet);

	for (int i = 0; i < packet->len; i++) {
		string[i] = packet->data[i];
	}
	string[packet->len] = '\0';
}