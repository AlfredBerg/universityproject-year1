#include "clientNetwork.h"


void sendPickupToServer(Network *client, int typeOfPickup, int idOfPickup) {
	char data[MAX_PACKET];
	sprintf(data, "2;%d;%d;", typeOfPickup, idOfPickup);

	sendPacket(data, client->serverIP, client->serverSocket);
	client->lastTick = SDL_GetTicks();
}

void connectToServer(Network *client) {
	char data[MAX_PACKET] = "HELLO\n";
	char decoded[2][30];

	sendPacket(data, client->serverIP, client->serverSocket);

	SDL_Delay(500); //test delay

	receivePacket(client->serverSocket, client->packet, data);
	if (compareString(data, "HELLO CLIENT", 12)) {
		printf("I am now connected to the server!\n");
		decode(data, decoded, 2, 30);
		client->playerID = atoi(decoded[1]);
		client->connectedToServer = 1;
		printf("PLAYER ID: %d \n", client->playerID);
	}
	
	else {
	puts("I could not connect to the server!");
	}
	

}

int compareString(char str1[], char str2[], int len) {
	for (int i = 0; i < len; i++) {
		if (!(str1[i] == str2[i])) {
			return 0;
		}
	}
	return 1;
}

void positionToString(Player *player, char string[]) {
	//x1;y1;
	sprintf(string, "0;%d;%d;", player->rect.x, player->rect.y);
}

void sendBulletToServer(Network *client, int projectileType, int x, int y, int direction) {
	char data[MAX_PACKET];
	sprintf(data, "1;%d;%d;%d;%d;", projectileType, x, y, direction);
	//puts(data);
	sendPacket(data, client->serverIP, client->serverSocket);
	client->lastTick = SDL_GetTicks();
}

void sendPositionToServer(Network *client, Player *fighter) {
	if (SDL_TICKS_PASSED(SDL_GetTicks(), client->lastTick + TICK_RATE)) {
		char data[MAX_PACKET];
		positionToString(fighter, data);
		//puts(data);
		sendPacket(data, client->serverIP, client->serverSocket);
		client->lastTick = SDL_GetTicks();

	}
}

void updateServer(Player *player, Network *client, Projectile *projectiles) {
	if (client->connectedToServer == 0) {
		return;
	}

	sendPositionToServer(client, &player[client->playerID]);

	int num_rdy = SDLNet_CheckSockets(client->socketSet, 0);
	char data[MAX_PACKET];
	while ((SDLNet_SocketReady(client->serverSocket))) {

		int success = receivePacket(client->serverSocket, client->packet, data);

		if (!success) {
			return;
		}

		//printf("Incoming data: %s\n", data);
		parseData(data, player, client, projectiles);
	}

}

void updateGamestate(Network *client, Player *player, char data[][30]) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		player[i].weaponID = atoi(data[i * FIELDS_IN_GAMESTATE + 3]);
		player[i].pickupID = atoi(data[i * FIELDS_IN_GAMESTATE + 4]);
		

		if (client->playerID != i) {
			player[i].x = atoi(data[i * FIELDS_IN_GAMESTATE + 1]);
			player[i].y = atoi(data[i * FIELDS_IN_GAMESTATE + 2]);
		}
	}
}

void updateProjectiles(Projectile *projectiles, char data[][30], int nrFields) {
	for (int i = 0; i < nrFields / PROJECTILEFIELDSINPACKET; i++) {
		int id = atoi(data[5 * i + 1]), projectileType = atoi(data[5 * i + 2]), x = atoi(data[5 * i + 3]), y = atoi(data[5 * i + 4]), direction = atoi(data[5 * i + 5]);
		fireProjectile(&projectiles[projectileType], direction, x, y, id);
	}
}


void parseData(char serverdata[], Player *player, Network *client, Projectile *projectiles) {
	char parsedData[110 * PROJECTILEFIELDSINPACKET][30];
	for (int i = 0; i < 110; i++) {
		parsedData[i][0] = '\0';
	}

	puts(serverdata);

	int nrFields = decode(serverdata, parsedData, 4, 30);

	//printf("%d", nrFields / 5);

	switch (atoi(parsedData[0])) //What kind of data is in this packet?
	{
	case 0: updateGamestate(client, player, parsedData); break;
	case 1: updateProjectiles(projectiles, parsedData, nrFields); break;
	default:
		printf("Unknown data received\n");
		break;
	}
}

void initClient(Network *client, char serverIP[]) {

	client->lastTick = SDL_GetTicks();
	client->connectedToServer = 0;
	client->playerID = 0;

	// Initialize SDL_net
	if (SDLNet_Init() != 0) {
		fprintf(stderr, "Error initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&client->serverIP, serverIP, SERVERPORT)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	client->serverSocket = SDLNet_UDP_Open(CLIENTPORT);
	if (!(client->serverSocket)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->packet = SDLNet_AllocPacket(1024);
	if (!client->packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->socketSet = SDLNet_AllocSocketSet(1);
	if (client->socketSet == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}

	if (SDLNet_UDP_AddSocket(client->socketSet, client->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	connectToServer(client);
}
