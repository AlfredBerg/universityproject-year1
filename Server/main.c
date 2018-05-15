#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifndef _MSC_VER
#include <unistd.h> 
#endif

#include "SDL.h"
#include "SDL_net.h"
#include "serverNetwork.h"
#include "sharedNetwork.h"
#include "clients.h"


void init(Network *server);
void quit(Network *server);
void updateClients(Network *server, Uint32 *lastTick);
void updateGamestate(Network *server, Uint32 *lastGamestateTick, int * lobby);
Items createItems(int nrWeapons, int nrPickups);

int main(int argc, char **argv)
{
	Network server;

	server.items = createItems(MAXNRWEAPONS, MAX_NR_OF_PICKUPS);

	init(&server);

	Uint32 lastNetworkTick = SDL_GetTicks();
	Uint32 lastGamestateTick = SDL_GetTicks();
	server.lastNetworkTick = SDL_GetTicks();
	
	int lobby = SDL_TRUE; //Changed from true to false
	int nrReady = 0;
	int looptime;
	int timerCount = 0;

	//Debug configurations:
	int noLobby = 0;

	while (server.running) {
		(server.nrGameloops)++;

		looptime = SDL_GetTicks();

		nrReady = SDLNet_CheckSockets(server.socketSet, SOCKET_TIMEOUT);

		if (lobby) {
			if (server.timer == 0 || server.next_player > 4 || noLobby)
				lobby = SDL_FALSE;
			else
				updateLobby(&server);
		}
		else {
			updateClients(&server, &server.lastNetworkTick);
			updateGamestate(&server, &lastGamestateTick, &lobby);
		}

		if (server.restart) {
			server.restart = restart(&server);
		}

		if (nrReady == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			perror("SDLNet_CheckSockets");
		}
		else if (nrReady == 0) {
			
		}
		else {
			//printf("%d sockets ready", nrReady);
			if (SDLNet_SocketReady(server.serverSocket)) {
				newClient(&nrReady, &server);
			}
		}
	
		//Used to see responsiveness of the server
		if (server.nrGameloops % 100 == 0) {
			printf("Loop time: %d\n", SDL_GetTicks() - looptime);
			timerCount = 0;
		}
		
	}

	quit(&server);

	return(0);
}


void quit(Network *server) {
	if (SDLNet_UDP_DelSocket(server->socketSet, server->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_UDP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}
	SDLNet_UDP_Close(server->serverSocket);

	int i;
	for (i = 0; i < MAX_SOCKETS; ++i) {
		if (server->sockets[i] == NULL) continue;
		closeSocket(server, i);
	}

	SDLNet_FreeSocketSet(server->socketSet);
	SDLNet_FreePacket(server->serverSocketPacket);
	SDLNet_Quit();
	SDL_Quit();
}


void init(Network *server) {
	// Initialize SDL
	if (SDL_Init(0) != 0)
	{
		fprintf(stderr, "Erro initializing SDL %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}


	// Initialize SDL_net
	if (SDLNet_Init() != 0)
	{
		fprintf(stderr, "Erro initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&server->serverIP, NULL, PORTNR))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	server->serverSocket = SDLNet_UDP_Open(PORTNR);
	if (!(server->serverSocket))
	{
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	server->serverSocketPacket = SDLNet_AllocPacket(1024);
	if (!server->serverSocketPacket) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	server->socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS + 1);
	if (server->socketSet == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}
	
	if (SDLNet_UDP_AddSocket(server->socketSet, server->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	for (int i = 0; i < MAX_SOCKETS; i++) {
		server->sockets[i] = NULL;
	}

	srand(time(NULL));

	server->seed = rand();
	server->running = 1;
	server->next_player = 0;
	server->timer = 10;
	server->alivePlayers = 0;
	server->restart = 0;

	//id, dmg, speed, w, h
	server->projectileData[BULLET].id = 0;
	server->projectileData[BULLET].dmg = 10;
	server->projectileData[BULLET].speed = 6;
	server->projectileData[BULLET].w = 30;
	server->projectileData[BULLET].h = 30;
	server->projectileData[BULLET].nrProjectilesShot = 0;

	server->projectileData[BEACHBALL].id = 2;
	server->projectileData[BEACHBALL].dmg = 10;
	server->projectileData[BEACHBALL].speed = 10;
	server->projectileData[BEACHBALL].w = 30;
	server->projectileData[BEACHBALL].h = 30;
	server->projectileData[BEACHBALL].nrProjectilesShot = 0;
	

	for (int i = 0; i < MAX_CLIENTS; i++) {
		server->clients[i].health = 100;
		server->clients[i].pickupId = -1;
		server->clients[i].weaponId = -1;
	}


	//Hardcoded stuff, remove me
	server->clients[0].xPos = 50;
	server->clients[0].yPos = 50;
	server->clients[1].xPos = 300;
	server->clients[1].yPos = 50;
	server->clients[2].xPos = 300;
	server->clients[2].yPos = 370;
	server->clients[3].xPos = 300;
	server->clients[3].yPos = 370;
	strcpy(server->clients[0].name, "spelare1");
	strcpy(server->clients[1].name, "spelare2");
	strcpy(server->clients[2].name, "spelare3");	
	strcpy(server->clients[3].name, "spelare4");
}


void updateGamestate(Network *server, Uint32 *lastGamestateTick, int * lobby) {
	if (!SDL_TICKS_PASSED(SDL_GetTicks(), *lastGamestateTick + RENDER_TICK)) {
		return;
	}

	for (int i = 0; i < 1; i++) {

		for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
			if (server->projectileData[i].Projectiles[j].direction == RIGHT) {
				server->projectileData[i].Projectiles[j].x += server->projectileData[i].speed;
			}
			else if(server->projectileData[i].Projectiles[j].direction == LEFT){
				server->projectileData[i].Projectiles[j].x -= server->projectileData[i].speed;
			}
			else if (server->projectileData[i].Projectiles[j].direction == BOTTOMLEFT) {
				server->projectileData[i].Projectiles[j].x -= server->projectileData[i].speed;
				server->projectileData[i].Projectiles[j].y += server->projectileData[i].speed;
			}
			else if (server->projectileData[i].Projectiles[j].direction == TOPLEFT) {
				server->projectileData[i].Projectiles[j].x -= server->projectileData[i].speed;
				server->projectileData[i].Projectiles[j].y -= server->projectileData[i].speed;
			}
			else if (server->projectileData[i].Projectiles[j].direction == TOPRIGHT) {
				server->projectileData[i].Projectiles[j].x += server->projectileData[i].speed;
				server->projectileData[i].Projectiles[j].y -= server->projectileData[i].speed;
			}
			else if (server->projectileData[i].Projectiles[j].direction == BOTTOMRIGHT) {
				server->projectileData[i].Projectiles[j].x += server->projectileData[i].speed;
				server->projectileData[i].Projectiles[j].y += server->projectileData[i].speed;
			}
			else {
				//printf("You picked a non valid direction for the bullet");
			}

			
		}

	}

	server->alivePlayers = 0;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (server->clients[i].health > 0) {
			server->alivePlayers++;
		}
	}

	if (server->alivePlayers == 1) {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (server->clients[i].health > 0) {
				sendVictoryToClient(server, i);
				*lobby = SDL_TRUE;
				server->restart = 1;
			}
		}
		
	}

	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (server->clients[i].yPos > FALLOUTOFMAPDEPTH) {
			server->clients[i].health = 0;
		}
	}

	*lastGamestateTick = SDL_GetTicks();
}

Items createItems(int nrWeapons, int nrPickups) {
	Items items;

	for (int i = 0; i < nrWeapons; i++) {
		items.weapons[i].id = i;
		items.weapons[i].isPickedUp = 0;
		items.weapons[i].owner = -1;
	}

	for (int i = 0; i < nrPickups; i++) {
		items.pickups[i].id = i;
		items.pickups[i].isPickedUp = 0;
		items.pickups[i].owner = -1;
	}

	return items;
}

int restart(Network *server) {

	server->running = 1;
	server->timer = 10;
	server->alivePlayers = server->next_player - 1;
	server->restart = 0;

	for (int i = 0; i < MAX_CLIENTS; i++) {
		server->clients[i].health = 100;
		server->clients[i].pickupId = -1;
		server->clients[i].weaponId = -1;
	}

	//Hardcoded stuff, remove me
	server->clients[0].xPos = 50;
	server->clients[0].yPos = 50;
	server->clients[1].xPos = 300;
	server->clients[1].yPos = 50;
	server->clients[2].xPos = 300;
	server->clients[2].yPos = 370;
	server->clients[3].xPos = 300;
	server->clients[3].yPos = 370;

	server->items = createItems(MAXNRWEAPONS, MAX_NR_OF_PICKUPS);

	return 0;
}