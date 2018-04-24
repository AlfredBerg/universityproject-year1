#pragma once
#include "player.h"
#include "weapon.h"
#include "projectile.h"

void fireProjectile(Projectile *projectile, int direction, int x, int y) {
	if (projectile->projectilesFired >= MAXPROJECTILEOBJECTS) {
		projectile->projectilesFired = 0;
	}
	

	projectile->rect[projectile->projectilesFired].w = projectile->w;
	projectile->rect[projectile->projectilesFired].h = projectile->h;
	projectile->rect[projectile->projectilesFired].x = x;
	projectile->rect[projectile->projectilesFired].y = y;
	projectile->direction[projectile->projectilesFired] = direction;

	projectile->projectilesFired++;
}

void moveProjectiles(Projectile projectiles[]) {
	
}

void renderProjectiles(Projectile projectiles[]) {

}