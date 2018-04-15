#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include <SDL_image.h>

#define MAX_PACKET 0xFF
#define MAX_SOCKETS 0x10

struct network {
	IPaddress serveraddress;
};
typedef struct network Network;