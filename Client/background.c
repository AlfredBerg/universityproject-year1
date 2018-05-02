#pragma once
#include "background.h"

void displayBackground(Game *game) {
	renderCopyMoveWithCamera(game->renderer, game->cloudsBack, NULL, NULL, 0.0, NULL, 0);
	renderCopyMoveWithCamera(game->renderer, game->cloudsFront, NULL, NULL, 0.0, NULL, 0);

	renderCopyMoveWithCamera(game->renderer, game->bgBack, NULL, NULL, 0.0, NULL, 0);

	renderCopyMoveWithCamera(game->renderer, game->bgFront, NULL, NULL, 0.0, NULL, 0);
}