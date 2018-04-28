#pragma once
#include "player.h"
#include "weapon.h"
#include "projectile.h"


void fireProjectile(Projectile *projectile, int direction, int x, int y, int id) {
	/*
	if (projectile->projectilesFired >= MAXPROJECTILEOBJECTS) {
		projectile->projectilesFired = 0;
	}
	*/

	projectile->rect[id].w = projectile->w;
	projectile->rect[id].h = projectile->h;
	projectile->rect[id].x = x;
	projectile->rect[id].y = y;
	projectile->direction[id] = direction;

	//projectile->projectilesFired++;
}

void moveProjectiles(Projectile projectiles[]) {
	for (int i = 0; i < MAXPROJECTILES; i++) {
		for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
			if (projectiles[i].direction[j] == LEFT) {
				projectiles[i].rect[j].x -= projectiles[i].speed;
			}
			else {
				projectiles[i].rect[j].x += projectiles[i].speed;
			}
		}
	}
}

void detectProjectileColision(Projectile *projectile, Player players[]) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
			if(SDL_HasIntersection(&players[i].rect, &projectile->rect[j])) {
				loseHealth(&players[i], projectile->dmg);
			}
		}
	}
}