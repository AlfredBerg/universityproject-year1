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
#include "playerStruct.h"



void positionToString(Player *player, char string[]) {
	//x1;y1
	sprintf(string, "%d;%d", player->p1.x, player->p1.y);
}