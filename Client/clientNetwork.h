#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include "player.h"

#define MAX_PACKET 1024
#define TICK_RATE 10

struct network {
	int playerID;
	IPaddress serverIP;
	UDPsocket serverSocket;
	UDPpacket *packet;
	SDLNet_SocketSet socketSet;
	Uint32 lastTick;
	int connectedToServer;
};
typedef struct network Network;

void updateServer(Player *player, Network *client);
void parseData(char serverdata[], Player *enemy);
void connectToServer(Network *client);