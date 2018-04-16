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


void newClient(int *nrReady, Network *server) {
	int newClientAccepted = AcceptSocket(server);
	(*nrReady)--;
	


}

int AcceptSocket(Network *server) {
	if (server->next_player >= MAX_SOCKETS) {
		fprintf(stderr, "ER: Overriding socket at index %d.\n", server->next_player);
		closeSocket(server, server->next_player);
	}

	char packetdata[MAX_PACKET];
	packetDatatoString(server->serverSocketPacket, packetdata);
	printf("\nPacket len: %d Packet data: %s\n", server->serverSocketPacket->len, packetdata);
	if (strcmp("HELLO\n", packetdata)) {
		return 0;
	}

	printf("I got a new client that want to connect\n");


	return 1;
}

void packetDatatoString(UDPpacket *packet, char string[]) {
	for (int i = 0; i < packet->len; i++) {
		string[i] = packet->data[i];
	}
	string[packet->len] = '\0';
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