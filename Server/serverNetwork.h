#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

#include "clients.h"

#define MAX_PACKET 0xFF
#define MAX_SOCKETS 0x4

#define PORTNR 12346
#define SOCKET_TIMEOUT 10
#define TICK_RATE 10

struct network {
	int next_player;
	IPaddress serverIP;
	Client clients[MAX_SOCKETS];
	UDPsocket serverSocket;
	UDPpacket *serverSocketPacket;
	int whoSentThePacket;
	SDLNet_SocketSet socketSet;
	UDPsocket sockets[MAX_SOCKETS];
	int running;
};
typedef struct network Network;

int AcceptSocket(Network *server);
void closeSocket(Network *server, int index);
void newClient(int *nrReady, Network *server);
void updateClients(Network *server, Uint32 *lastTick);


void sendPacket(char data[], IPaddress ip, UDPsocket socket);