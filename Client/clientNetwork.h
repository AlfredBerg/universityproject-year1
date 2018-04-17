#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include "playerStruct.h"

#define MAX_PACKET 1024
#define TICK_RATE 1000

struct network {
	int playerID;
	IPaddress serverIP;
	UDPsocket serverSocket;
	UDPpacket *packet;
	SDLNet_SocketSet socketSet;
	Uint32 lastTick;
};
typedef struct network Network;

void updateServer(Player *fighter, Network *client);