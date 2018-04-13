#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


int main(int argc, char** arg) {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);
	
	TCPsocket server = SDLNet_TCP_Open(&ip);
	TCPsocket client;
	
	const char* text = ("hello client!");
	while(1){
		
		client = SDLNet_TCP_Accept(server);
		if (client) {
		//here you can communicate with the client
			SDLNet_TCP_Send(client, text, strlen(text) + 1);
			SDLNet_TCP_Close(client);
			break;
		}
	}
	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}