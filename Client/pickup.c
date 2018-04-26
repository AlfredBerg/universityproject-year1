#include "pickup.h"


void pickUpPickup(Pickup pickups[], Player players[]) {
	for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
		if (pickups[i].isPickedUp == 1) {
			continue;
		}

		for (int j = 0; j < MAXPLAYERS; j++) {
			if (SDL_HasIntersection(&players[j].rect, &pickups[i].rect)) {
				printf("Pickup pickup\n");
				players[j].pickupID = pickups[i].id;
				pickups[i].isPickedUp = 1;
				//pickups[i].healing
			}
		}

	}
}