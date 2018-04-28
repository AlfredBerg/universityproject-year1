#include "pickup.h"


void pickUpPickup(Pickup pickups[], Player players[]) {
	for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
		if (pickups[i].isPickedUp == 1) {
			continue;
		}

		//OBS : Found a bug here ! Prints out 1 extra time
		for (int j = 0; j < MAXPLAYERS; j++) {
			if (SDL_HasIntersection(&players[j].rect, &pickups[i].rect)) {
				printf("Pickup pickup\n");
				printf("Health = + %d\n", pickups[i].healing);
				players[j].pickupID = pickups[i].id;
				pickups[i].isPickedUp = 1;
				players[j].life += pickups[i].healing;
			}
		}

	}
}

Pickup createPickup(Game *game, int id, int x, int y, int healing, const char imageName[], int rectW, int rectH) {
	Pickup pickup;
	pickup.id = id;
	pickup.x = x;
	pickup.y = y;
	pickup.healing = healing;
	pickup.image = IMG_Load(imageName);
	pickup.texture = SDL_CreateTextureFromSurface(game->renderer, pickup.image);
	pickup.rect.x = x;
	pickup.rect.y = y;
	pickup.rect.w = rectW;
	pickup.rect.h = rectH;
	pickup.isPickedUp = 0;
	return pickup;
}

void deletePickup(Pickup pickups[], int pickupIDtoDelete, int *nrOfPickups) {
	for (int i = 0; i < *nrOfPickups; i++) {
		if (pickupIDtoDelete == pickups[i].id) {
			for (int j = 0; j < *nrOfPickups - 1; j++)
				pickups[j] = pickups[j + 1];
			*nrOfPickups = *nrOfPickups - 1;
			printf("Deleted pickup\n");
		}
	}
}

void drawPickups(Game *game, Pickup pickups[], int *nrOfPickups) {
	for (int i = 0; i < *nrOfPickups; i++) {
		if (!pickups[i].isPickedUp)
			SDL_RenderCopy(game->renderer, pickups[i].texture, NULL, &pickups[i].rect);
		else
			deletePickup(pickups, pickups[i].id, nrOfPickups);
	}
}