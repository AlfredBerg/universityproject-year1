#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

int receivePacket(UDPsocket socket, UDPpacket *packet, char string[]);
void sendPacket(char data[], IPaddress ip, UDPsocket socket);
void decode(char str[], char data[][30], int dataLength, int stringlength);