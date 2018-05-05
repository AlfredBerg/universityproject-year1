#include "game.h"

#include "player.h"
#include "weapon.h"
#include "gravity.h"
#include "background.h"
#include "projectile.h"
#include "pickup.h"
#include "checkCollision.h"
#include "camera.h"

#define SPECTATESPEED 10

extern Network client;
extern SDL_Rect camera;

void initGame(Game *game) {

	// Initialize SDL and audio system
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Initialize support for loading png and JPEG image
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	// Initialize the mixer
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	playBackgroundMusic();

	if (TTF_Init() < 0) {
		printf("SDL error -> %s\n", SDL_GetError());
		exit(1);
	}

	game->window = SDL_CreateWindow("Grupp10", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	game->debug = 1;
	game->running = 1;
	game->loopCount = 0;
	game->spectateMode = 0;

	initBackground(game);

	createWindowIcon(game);
}


int runGame(Game *game, Network *client) {

	// Randomization
	srand(time(NULL));

	// Load map from file (.map)
	static int lvl1[MAP_HEIGHT][MAP_WIDTH] = { 0 };
	loadMap("assets/map/map1.map", lvl1);

	// Init map
	Tile map[MAP_HEIGHT][MAP_WIDTH];
	int i, j = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			map[i][j].ID = lvl1[i][j];
			initTiles(game->renderer, &map[i][j], j, i);
		}
	}

	// Player names
	char player0Name[] = "Knight";
	char player1Name[] = "Bear";
	char player2Name[] = "Bird";

	// Create players
	Player players[MAXPLAYERS];
	players[0] = createPlayer(game, 0, player0Name, 60, 250, RIGHT, "assets/knightsprite.png");
	players[1] = createPlayer(game, 1, player1Name, 300, 0, LEFT, "assets/bearsprite.png");
	players[2] = createPlayer(game, 2, player2Name, 400, 500, LEFT, "assets/bird.png");
	int nrOfPlayers = 3;

	// Create weapons
	Weapon weapons[MAXNRWEAPONS];
	weapons[0] = createWeapon(game, 0, 500, 100, 10, 200, 0, "assets/pistol.png");
	weapons[1] = createWeapon(game, 1, 100, 400, 10, 200, 0, "assets/pistol.png");
	weapons[2] = createWeapon(game, 2, 200, 100, 2, 1000, 1, "assets/hand.png");
	weapons[3] = createWeapon(game, 3, 600, 100, 10, 200, 0, "assets/beachball.png");
	int nrOfWeapons = 4;

	// Create projectiles
	Projectile projectiles[MAXPROJECTILES];
	projectiles[0] = createProjectile(game, 0, 10, 12, 30, 30, "assets/bullet.png");
	projectiles[1] = createProjectile(game, 1, 2, 100, 800, 600, "assets/handProjectile.png");
	int nrOfProjectiles = 2;

	// Create pickups
	Pickup pickups[MAX_NR_OF_PICKUPS];
	pickups[0] = createPickup(game, 0, 550, 500, 5, "assets/p_red.png");
	pickups[1] = createPickup(game, 1, 550, 400, 10, "assets/p_green.png");
	int nrOfPickups = 2;

	// For future use! Placing weapons & pickups by randomization & if there's no tile /Sara
	// Place pickups in random spots where there's no tile
	//for (i = 0; i < MAP_HEIGHT; i++) {
	//	for (j = 0; j < MAP_WIDTH; j++) {
	//		if (map[i][j].ID == 0) {
	//			
	//		}
	//	}
	//}

	int running = 1;
	//int again = 0;

	SDL_Event event;

	int prevKey = 0;
	int key = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump = 0;
	int groundDetected = 0;
	int roofDetected = 0;
	int leftWall = 0;
	int rightWall = 0;

	Uint32 startTimer = SDL_GetTicks(), renderTick = SDL_GetTicks();

	// Sound effects
	Mix_Chunk *footsteps = Mix_LoadWAV("assets/footsteps.wav");
	footsteps->volume = 50;
	Mix_Chunk *jumpsound = Mix_LoadWAV("assets/jumpsound.wav");
	jumpsound->volume = 50;




	while (running)
	{

		//---------------------------Game state------------------------------------
		if (!SDL_TICKS_PASSED(SDL_GetTicks(), renderTick + RENDER_TICK)) {
			//Do between ticks
			updateServer(players, client, projectiles);
			continue;
		}

		SDL_RenderClear(game->renderer);
		displayBackground(game);

		if(game->spectateMode == 0)
			updateCameraPosition(&players[client->playerID]);

		playerNameTag(players, game->renderer);
		playerHealthbar(players, game->renderer);


		pickUpPickup(pickups, players);
		weaponActions(weapons, players, client, projectiles);
		//moveProjectiles(projectiles);

		renderTick = SDL_GetTicks();
		game->loopCount++;

		SDL_Rect srcrect[3] = { { players[0].currentSprite * 16, 0, 16, 24 },{ players[1].currentSprite * 16, 0, 16, 24 }, { players[2].currentSprite * 40, 0, 40, 40 } };
		SDL_Rect dstrect[3] = { { players[0].rect.x, players[0].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT }, { players[1].rect.x, players[1].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT }, { players[2].rect.x, players[2].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT } };

		//för fågeln
		if (game->loopCount % SPRITESPEED == 0)
			players[2].currentSprite += 1;

		// Check for various events (keyboard, mouse, touch, close)
		while (SDL_PollEvent(&event))
		{
			//const char* key = SDL_GetKeyName(event.key.keysym.sym);
			if (event.type == SDL_QUIT) {
				running = 0;
				return running;
			}
		}

		//Move fighter
		const Uint8 *KeyState;
		KeyState = SDL_GetKeyboardState(NULL);
		if (players[client->playerID].life > 0) {
			if (KeyState[SDL_SCANCODE_D]) {
				if (players[client->playerID].isMoving == 0 && game->loopCount % SPRITESPEED == 0)
					players[client->playerID].currentSprite += 1;
				key = RIGHT;
				if (!Mix_Playing(0) && groundDetected)
					Mix_PlayChannel(0, footsteps, 0);
				if ((!checkForWall(map, &players[client->playerID], &key) || key != prevKey) && !rightWall) {
					walkRight(&players[client->playerID], &key, &prevKey);
					leftWall = 0;
				}
				else if (checkForWall(map, &players[client->playerID], &key) || key != prevKey) {
					players[client->playerID].x -= 5;
					rightWall = 1;
				}
			}

			else if (KeyState[SDL_SCANCODE_A]) {
				if (players[client->playerID].isMoving == 0 && game->loopCount % SPRITESPEED == 0)
					players[client->playerID].currentSprite -= 1;
				key = LEFT;
				if (!Mix_Playing(0) && groundDetected)
					Mix_PlayChannel(0, footsteps, 0);
				if ((!checkForWall(map, &players[client->playerID], &key) || key != prevKey) && !leftWall) {
					walkLeft(&players[client->playerID], &key, &prevKey);
					rightWall = 0;
				}
				else if (checkForWall(map, &players[client->playerID], &key) || key != prevKey) {
					players[client->playerID].x += 5;
					leftWall = 1;
				}
			}

			if (KeyState[SDL_SCANCODE_W]) {
				doJump = 1;
				if (groundDetected)
					Mix_PlayChannel(1, jumpsound, 0);
			}
			if (KeyState[SDL_SCANCODE_SPACE]) {
				players[client->playerID].weaponFired = 1;
			}
		}
		else {
			game->spectateMode = 1;
			if (KeyState[SDL_SCANCODE_D]) {
				camera.x += SPECTATESPEED;
			}

			if (KeyState[SDL_SCANCODE_A]) {
				camera.x -= SPECTATESPEED;
			}

			if (KeyState[SDL_SCANCODE_W]) {
				camera.y -= SPECTATESPEED;
			}

			if (KeyState[SDL_SCANCODE_S]) {
				camera.y += SPECTATESPEED;
			}
		}


		gravity(&players[client->playerID], weapons, &groundDetected, &roofDetected, map);

		if (groundDetected == 0) {
			checkForCeiling(map, &players[client->playerID], &jumpTime, &roofDetected, &groundDetected);
		}

		if (!checkForWall(map, &players[client->playerID], &key)) {
			jump(&players[client->playerID], &isJumping, &jumpTime, &doJump, &groundDetected, &roofDetected);
		}


		for (int j = 0; j < MAXPLAYERS; j++) {
			if (j == client->playerID) {
				players[client->playerID].rect.x = players[client->playerID].x;
				players[client->playerID].rect.y = players[client->playerID].y;
			}
			else {
				players[j].rect.x = players[j].x;
				players[j].rect.y = players[j].y;
			}
		}


		//Detect projectile collisions with walls
		for (i = 0; i < MAP_HEIGHT; i++) {
			for (j = 0; j < MAP_WIDTH; j++) {
				if (map[i][j].ID != 0) {
					for (int k = 0; k < MAXPROJECTILEOBJECTS; k++) {
						if (SDL_HasIntersection(&projectiles[0].rect[k], &map[i][j].rect)) {
							sendDeleteProjectileToServer(0, k);
						}
					}
				}
			}
		}


		//---------------------------Render------------------------------------

		//Display map
		int i, j = 0;
		for (i = 0; i < MAP_HEIGHT; i++) {
			for (j = 0; j < MAP_WIDTH; j++) {
				drawTiles(game->renderer, &map[i][j], j, i);
			}
		}

		//-----------------------------DEBUG MODE-----------------------------------
		if (game->debug == 1) {
			SDL_RenderDrawRect(game->renderer, &players[1].rect);
			SDL_RenderDrawRect(game->renderer, &players[0].rect);
			SDL_RenderDrawRect(game->renderer, &weapons[0].rect);
			SDL_RenderDrawRect(game->renderer, &weapons[1].rect);

			for (int i = 0; i < MAXPROJECTILEOBJECTS; i++) {
				SDL_RenderDrawRect(game->renderer, &projectiles[0].rect[i]);
			}
		}
		//--------------------------------------------------------------------------


		updatePlayerStates(players, game->loopCount);

		//Draw players, weapons, projectiles & pickups
		drawPlayers(game, players, srcrect, dstrect, &nrOfPlayers, &leftWall, &rightWall);
		drawWeapons(game, players, weapons);
		drawProjectiles(game, projectiles);
		drawPickups(game, pickups, &nrOfPickups);

		//Show what was drawn
		SDL_RenderPresent(game->renderer);
	}
	running = 1;

	return running;
}

void quitGame(Game *game) {
	//SDL_DestroyTexture !!!

	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void playBackgroundMusic() {
	Mix_Music *backgroundMusic = Mix_LoadMUS("hello.mp3");
	Mix_VolumeMusic(50);

	if (!backgroundMusic)
		printf("Background music is not working\n");
	Mix_PlayMusic(backgroundMusic, -1);
}

void createWindowIcon(Game *game) {
	SDL_Surface *icon = IMG_Load("assets/crystal.png");
	SDL_SetWindowIcon(game->window, icon);
	SDL_FreeSurface(icon);
}


//**************************************** Player functions **************************************************

Player createPlayer(Game *game, int id, char name[], int x, int y, int lastDirection, const char imageName[]) {
	Player player;
	strcpy(player.name, name);
	player.id = id;
	player.life = 100;
	player.x = x;
	player.y = y;
	player.previousX = x;
	player.previousY = y;
	player.pickupID = -1;
	player.weaponID = -1;
	player.weaponFired = 0;
	player.tickThatWeaponFired = SDL_GetTicks();
	player.tickThatLostHealth = SDL_GetTicks();
	player.lastDirection = lastDirection;
	player.Image = IMG_Load(imageName);
	player.Texture = SDL_CreateTextureFromSurface(game->renderer, player.Image);
	player.currentSprite = 0;
	player.rect.x = x;
	player.rect.y = y;
	player.rect.w = PLAYER_WIDTH;
	player.rect.h = PLAYER_HEIGHT;
	player.isMoving = 0;

	//Player name
	TTF_Font *font = TTF_OpenFont("assets/pixlig font.ttf", 20);
	SDL_Color color = { 65, 33, 52, 255 };

	player.nameText = TTF_RenderText_Solid(font, player.name, color);

	player.nameTexture = SDL_CreateTextureFromSurface(game->renderer, player.nameText);

	//Hp bar
	int hpBarwidth = 100, hpBarheight = 5;

	player.hpBarSurface = SDL_CreateRGBSurface(0, hpBarwidth, hpBarheight, 32, 0, 0, 0, 0);

	SDL_Rect rect = { 0, 0, hpBarwidth, hpBarheight };

	SDL_FillRect(player.hpBarSurface, &rect, SDL_MapRGB(player.hpBarSurface->format, 61, 229, 77));
	player.hpBarTexture = SDL_CreateTextureFromSurface(game->renderer, player.hpBarSurface);


	return player;
}

void drawPlayers(Game *game, Player players[], SDL_Rect srcrect[], SDL_Rect dstrect[], int *nrOfPlayers, int *leftWall, int *rightWall) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].life > 0) {
			if ((players[i].lastDirection == RIGHT) && (*leftWall == 1)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if ((players[i].lastDirection == RIGHT) && (*leftWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, 0);
			}
			else if ((players[i].lastDirection == LEFT) && (*rightWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if ((players[i].lastDirection == LEFT) && (*leftWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, 0);
			}
		}

		else
			deletePlayer(players, players[i].id, nrOfPlayers);
	}
}



//void drawPlayers(Game *game, Player players[], SDL_Rect srcrect[], SDL_Rect dstrect[], int *nrOfPlayers, int *leftWall, int *rightWall) {
//
//	for (int i = 0; i < MAXPLAYERS; i++) {
//		if (players[i].life > 0) {
//
//			if ((players[i].lastDirection == RIGHT) && (*leftWall == 1)) {
//				SDL_RenderCopyEx(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
//			}
//			else if ((players[i].lastDirection == RIGHT) && (*leftWall == 0)) {
//				SDL_RenderCopy(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i]);
//			}
//			else if ((players[i].lastDirection == LEFT) && (*rightWall == 0)) {
//				SDL_RenderCopyEx(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
//			}
//			else if ((players[i].lastDirection == LEFT) && (*leftWall == 0)) {
//				SDL_RenderCopy(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i]);
//			}
//		}
//		else
//			deletePlayer(players, players[i].id, nrOfPlayers);
//	}
//}


//*****************restart() not yet implemented********************
//int restart(Game* game) {
//
//	TTF_Font *font2 = TTF_OpenFont("fintext.ttf", 20);
//	SDL_Color color = { 255, 255, 255, 255 };
//	SDL_Surface *rematch = TTF_RenderText_Solid(font2, "Rematch", color);
//
//	SDL_Texture *rematch_Texture = SDL_CreateTextureFromSurface(game->renderer, rematch);
//	SDL_FreeSurface(rematch);
//
//	SDL_Rect RematchFontRect = { 200, 260, 150, 80 };
//
//	SDL_Event ev;
//
//	int running = 1;
//	while (running) {
//
//		while (SDL_PollEvent(&ev) != 0)
//		{
//			if (ev.type == SDL_QUIT)
//				running = 0;
//
//			else if (ev.type == SDL_MOUSEBUTTONDOWN)
//			{
//				if (ev.button.button == SDL_BUTTON_LEFT) {
//
//					if (ev.button.x > 200 && ev.button.x < 350 && ev.button.y>280 && ev.button.y < 340) {
//						SDL_DestroyTexture(rematch_Texture);
//						running = 1;
//						return running;
//
//					}
//				}
//			}
//		}
//		SDL_RenderClear(game->renderer);
//		SDL_RenderCopy(game->renderer, rematch_Texture, NULL, &RematchFontRect);
//		/*SDL_RenderCopy(game->renderer, image2_texture, &srcrect, &dstrect);//draw
//		SDL_RenderCopy(game->renderer, image3_texture, &srcrect2, &dstrect2);
//		SDL_RenderCopy(game->renderer, image5_texture, NULL, &bild5);
//		SDL_RenderCopy(game->renderer, image7_texture, NULL, &bild7);
//		*/
//
//		SDL_RenderPresent(game->renderer);
//
//
//	}
//	SDL_DestroyTexture(rematch_Texture);
//	return running;
//}