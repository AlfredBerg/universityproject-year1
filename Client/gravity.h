#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

void gravity(SDL_Rect *player, SDL_Rect *weapon) {
	if (player->y < 480 && player->y > 0) {
		player->y += player->y * 0.025;
		weapon->y = player->y + 50;
	}
	else if (player->y >= 480) {
		player->y = 480;
		weapon->y = player->y + 50;
	}
	else {
		player->y += 10;
		weapon->y = player->y + 50;
	}
}