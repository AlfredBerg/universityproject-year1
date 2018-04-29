#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define SEPARATOR ";"

#ifndef _MSC_VER
#include <unistd.h> 
#endif

#include "SDL.h"
#include "SDL_net.h"

int receivePacket(UDPsocket socket, UDPpacket *packet, char string[]);
void sendPacket(char data[], IPaddress ip, UDPsocket socket);
int decode(char indata[], char data[][30], int dataLength, int stringlength);