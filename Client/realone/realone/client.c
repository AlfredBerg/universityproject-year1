#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_net.h>

/*
int main(int argc, char** arg) {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, "127.0.0.1", 1234);

	TCPsocket client= SDLNet_TCP_Open(&ip);
	char text[100];
	SDLNet_TCP_Recv(client, text, 100);//denna fuckar upp as mkt
	//std::cout << text;

	SDLNet_TCP_Close(client);
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}*/