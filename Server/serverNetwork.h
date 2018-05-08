#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

#include "clients.h"

#define MAX_PACKET 1400
#define MAX_SOCKETS 0x4

#define PORTNR 12346
#define SOCKET_TIMEOUT 10

#define TICK_RATE 20
#define RENDER_TICK 20

#define FIELDS_IN_GAMESTATE 5

#define DATAFIELDSINPACKET 10
#define NRPROJECTILES 1

#define LEFT 2
#define RIGHT 3

struct network {
	int next_player;
	IPaddress serverIP;
	Client clients[MAX_SOCKETS];
	ProjectileData projectileData[NRPROJECTILES];
	UDPsocket serverSocket;
	UDPpacket *serverSocketPacket;
	int whoSentThePacket;
	SDLNet_SocketSet socketSet;
	UDPsocket sockets[MAX_SOCKETS];
	int nrGameloops;
	int running;
	int allivePlayers;
	Items items;
	int timer;
};
typedef struct network Network;

int AcceptSocket(Network *server);
void closeSocket(Network *server, int index);
void newClient(int *nrReady, Network *server);
void updateClients(Network *server, Uint32 *lastTick);


void sendPacket(char data[], IPaddress ip, UDPsocket socket);

void gamestateToString(Network *server, char string[]);

void updateLobby(Network *server);
void receiveLobby(Network *server, char data[][30]);
void lobbyToString(Network *server, char string[MAX_PACKET]);
void sendVictoryToClient(Network *server, int winnerId);
