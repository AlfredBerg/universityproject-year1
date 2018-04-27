#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include "player.h"

#define MAX_PACKET 1024
#define TICK_RATE 30
#define SERVERPORT 12346
#define CLIENTPORT 53132
#define SERVERIP "130.229.131.213"

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

void initClient(Network *client);
void updateServer(Player *player, Network *client);
void parseData(char serverdata[], Player *player, Network *client);
void connectToServer(Network *client);