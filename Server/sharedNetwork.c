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


void sendPacket(char data[], IPaddress ip, UDPsocket socket) {
	UDPpacket *send;

	send = SDLNet_AllocPacket(1024);
	if (!send) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	strcpy(send->data, data);
	send->len = strlen(data);
	send->address = ip;
	SDLNet_UDP_Send(socket, -1, send);
}

int receivePacket(UDPsocket socket, UDPpacket *packet, char string[]) {
	int recvSuccess = SDLNet_UDP_Recv(socket, packet);
	if (!recvSuccess) {
		return 0;
	}

	for (int i = 0; i < packet->len; i++) {
		string[i] = packet->data[i];
	}
	string[packet->len] = '\0';

	return 1;

}

int decode(char indata[], char data[][30], int dataLength, int stringlength){
  char * part;
  int count = 0;
  
  //Initiating part to get access into loop
  part = strtok (indata,SEPARATOR);
  
  //Separating all the parts
  while (part != NULL)
  {

	strcpy(data[count], part);
    part = strtok (NULL, SEPARATOR);
	count++;
  }
  return count;
}