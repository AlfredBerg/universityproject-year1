#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#define MAX_IMAGES 1000

void loadImage(SDL_Surface *images[MAX_IMAGES]) {

	images[0] = IMG_Load("bowser.png");
	images[1] = IMG_Load("deathwins.jpg");
	images[2] = IMG_Load("sword1.png");
	images[3] = IMG_Load("sword1.png");
	images[4] = IMG_Load("sword2.png");
	images[5] = IMG_Load("sword2.png");
	images[6] = IMG_Load("humanwin.png");
	images[7] = IMG_Load("Tileset.png");
}