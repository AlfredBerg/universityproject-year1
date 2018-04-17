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

void jump(SDL_Rect *player, SDL_Rect *weapon, int *isJumping, int *jumpTime, int *doJump) {		
	
	if ((*doJump == 1))				
	{			
		if (!*isJumping) {			
			*jumpTime = 10;			
		}
		if (*jumpTime > 0) {        
			*isJumping = 1;			
			player->y -= 20;
			weapon->y = player->y + 50;
			--(*jumpTime);
		}
		if (*jumpTime <= 0) {
			if (player->y == 480) {
				*doJump = 0;
				*isJumping = 0;
			}
		}
	}
}