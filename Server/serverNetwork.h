#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include "clients.h"

#define MAX_PACKET 0xFF
#define MAX_SOCKETS 0x5

struct network {
	int next_player;
	IPaddress serverIP;
	Client clients[MAX_SOCKETS];
	UDPsocket serverSocket;
	SDLNet_SocketSet socketSet;
	UDPsocket sockets[MAX_SOCKETS];
	int running;

};
typedef struct network Network;