#include "game.h"
#include "clientNetwork.h"

void initGame(Game *game, Network *client)
{
	// Initialize SDL and audio system
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//initialize support for loading png and JPEG image
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//initialize the mixer
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	if (TTF_Init() < 0) {
		printf("SDL error -> %s\n", SDL_GetError());
		exit(1);
	}

	game->running = 1;
	game->window = SDL_CreateWindow("knifekillers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									WINDOW_LENGTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	client->lastTick = SDL_GetTicks();
	client->connectedToServer = 0;
	client->playerID = 0;

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

	// Initialize SDL_net
	if (SDLNet_Init() != 0) {
		fprintf(stderr, "Erro initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&client->serverIP, SERVERIP, SERVERPORT)) {
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