#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>

void receivePacket(UDPsocket socket, UDPpacket *packet, char string[]);
void sendPacket(char data[], IPaddress ip, UDPsocket socket);