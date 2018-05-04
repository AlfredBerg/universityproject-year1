#pragma once
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
#include "clientNetwork.h"
#include "player.h"
#include "sharedNetwork.h"
#include "projectile.h"

#define MAX_PACKET 1400
#define MAX_SOCKETS 0x10
#define TICK_RATE 20
#define SERVERPORT 12346
#define CLIENTPORT 53132
#define SERVERIP "130.229.177.89"
#define PROJECTILEFIELDSINPACKET 5

#define FIELDS_IN_GAMESTATE 5

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

int initClient(Network *client, char serverIP[]);
void updateServer(Player *player, Network *client, Projectile *projectiles);
void parseData(char serverdata[], Player *player, Network *client, Projectile *projectiles);
int connectToServer(Network *client);
void sendBulletToServer(Network *client, int projectileType, int x, int y, int direction);
int compareString(char str1[], char str2[], int len);
void sendPickupToServer(Network *client, int typeOfPickup, int idOfPickup);
void sendHitToServer(int damage, int player);