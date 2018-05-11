#include "pickup.h"
#include "camera.h"


void pickUpPickup(Pickup pickups[], Player players[]) {
	for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
		if (pickups[i].isPickedUp == 1) {
			continue;
		}

		for (int j = 0; j < MAXPLAYERS; j++) {

			if (SDL_HasIntersection(&players[j].dstRect, &pickups[i].rect)) {
				printf("Picked up pickup\n");
				players[j].pickupID = pickups[i].id;
				pickups[i].isPickedUp = 1;

				if (players[j].life + pickups[i].healing < 100 || players[j].life + pickups[i].healing == 100) {	// Health only increases if needed
					printf("Health = + %d\n", pickups[i].healing);
					loseHealth(&players[j], -pickups[i].healing);
				}

			}
		}
	}
}

Pickup createPickup(Game *game, int id, int x, int y, int healing, const char imageName[]) {
	Pickup pickup;
	pickup.id = id;
	pickup.x = x;
	pickup.y = y;
	pickup.healing = healing;
	pickup.image = IMG_Load(imageName);
	pickup.texture = SDL_CreateTextureFromSurface(game->renderer, pickup.image);
	SDL_FreeSurface(pickup.image);
	pickup.rect.x = x;
	pickup.rect.y = y;
	pickup.rect.w = PICKUP_WIDTH;
	pickup.rect.h = PICKUP_HEIGHT;
	pickup.isPickedUp = 0;
	return pickup;
}

void deletePickup(Pickup pickups[], int pickupIDtoDelete) {
	pickups[pickupIDtoDelete].rect.h = 0;
	pickups[pickupIDtoDelete].rect.w = 0;
	SDL_DestroyTexture(pickups[pickupIDtoDelete].texture);
}

void drawPickups(Game *game, Pickup pickups[]) {
	for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
		if (!pickups[i].isPickedUp)
			renderCopyMoveWithCamera(game->renderer, pickups[i].texture, NULL, &pickups[i].rect, 0.0, NULL, 0);
		else
			deletePickup(pickups, pickups[i].id);
	}
}