#include "game.h"

#include "player.h"
#include "weapon.h"
#include "gravity.h"
#include "background.h"
#include "projectile.h"
#include "pickup.h"
#include "checkCollision.h"
#include "camera.h"
#include "menu.h"

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

	game->window = SDL_CreateWindow("RealOne", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	game->debug = 1;
	game->running = 1;
	game->loopCount = 0;
	game->spectateMode = 0;
	game->replay = 0;

	initBackground(game);
	createWindowIcon(game);
}


int runGame(Game *game, Network *client, char playerNames[][30]) {

	// Init randomization
	srand(game->seed);

	// Load map from file (.map)
	static int lvl1[MAP_HEIGHT][MAP_WIDTH] = { 0 };

	int smallMap = 0;
	int smallMapModulusX = 0;
	int smallMapModulusY = 0;
	int smallMapX = 0;
	int smallMapY = 0;

	switch (rand() % NROFMAPS)
	{
	case 0:	loadMap("assets/map/map1.map", lvl1); break;
	case 1: loadMap("assets/map/map2.map", lvl1); break;
	case 2: loadMap("assets/map/map3.map", lvl1); break;
	case 3: loadMap("assets/map/map4.map", lvl1); break;
	case 4: loadMap("assets/map/smallmap4.map", lvl1); smallMap = 1;  break;
	default:
		break;
	}

	if (smallMap) {
		smallMapModulusX = 2700;
		smallMapModulusY = 700;
		smallMapX = 1450;
		smallMapY = 350;
	}

	// Init map
	Tile map[MAP_HEIGHT][MAP_WIDTH];
	int i, j = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			map[i][j].ID = lvl1[i][j];
			initTiles(game->renderer, &map[i][j], j, i);
		}
	}

	// Create players
	char playerSprites[MAXPLAYERS][30] = { "assets/knightsprite.png", "assets/bearsprite.png", "assets/bird.png", "assets/princesssprite.png" };
	int spawnXPos[MAXPLAYERS] = { randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX) };
	int spawnYPos[MAXPLAYERS] = { randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY) };

	Player players[MAXPLAYERS];
	for (int i = 0; i < MAXPLAYERS; i++) {  // Ska vara game->connectedPlayers men det ger error med kameran
		if (i == 2) // Om det är fågeln
			players[i] = createPlayer(game, i, playerNames[i], spawnXPos[i], spawnYPos[i], RIGHT, playerSprites[i], 40, 40); //Den är i en annan storlek..
		else
			players[i] = createPlayer(game, i, playerNames[i], spawnXPos[i], spawnYPos[i], RIGHT, playerSprites[i], 16, 24);
	}
	int nrOfPlayers = game->connectedPlayers;

	// Create weapons
	char weaponNames[7][21] = { "assets/pistol.png", "assets/pistol.png", "assets/hand.png", "assets/beachball.png", "assets/uzi.png", "assets/ak47.png", "assets/bullet.png" };
	int weaponXpos[7] = { randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX)};
	int weaponYpos[7] = { randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY)};
	int weaponDamage[7] = { 10, 10, 4, 10, 12, 15, 50};
	int weaponFireRate[7] = { 200, 200, 800, 200, 200, 200, 200};
	int weaponProjectileType[7] = { 0, 0, 1, 2, 0, 0, 1};

	Weapon weapons[MAXNRWEAPONS];
	for (int i = 0; i < MAXNRWEAPONS; i++) {
		weapons[i] = createWeapon(game, i, weaponXpos[i], weaponYpos[i], weaponDamage[i], weaponFireRate[i], weaponProjectileType[i], weaponNames[i]);
	}
	int nrOfWeapons = 4;


	// Create projectiles
	Projectile projectiles[MAXPROJECTILES];
	projectiles[0] = createProjectile(game, 0, 10, 12, 30, 30, "assets/bullet.png");
	projectiles[1] = createProjectile(game, 1, 15, 1000, 30, WINDOW_HEIGHT, "assets/handProjectile.png");
	projectiles[2] = createProjectile(game, 2, 10, 12, 30, 30, "assets/beachball.png");
	int nrOfProjectiles = 3;

	// Create pickups
	char pickupNames[MAX_NR_OF_PICKUPS][20] = { "assets/p_red.png", "assets/p_orange.png", "assets/p_yellow.png", "assets/p_green.png", "assets/p_blue.png", "assets/p_purple.png" };
	int pickupXPos[MAX_NR_OF_PICKUPS] = { randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX), randomX(&smallMapModulusX, &smallMapX) };
	int pickupYPos[MAX_NR_OF_PICKUPS] = { randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY), randomY(&smallMapModulusY, &smallMapY) };
	int pickupHealing[MAX_NR_OF_PICKUPS] = { 5, 10, 15, 20, 25, 30 };

	Pickup pickups[MAX_NR_OF_PICKUPS];
	for (int i = 0; i < MAX_NR_OF_PICKUPS; i++)
		pickups[i] = createPickup(game, i, pickupXPos[i], pickupYPos[i], pickupHealing[i], pickupNames[i]);

	SDL_Event event;
	int running = 1;
	int prevKey = 0;
	int key = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump = 0;
	int groundDetected = 0;
	int roofDetected = 0;
	int leftWall = 0;
	int rightWall = 0;

	// Init timer
	Uint32 startTimer = SDL_GetTicks(), renderTick = SDL_GetTicks();
	SDL_Rect timerRect = { 5, 5, 60, 40 };
	SDL_Color color = { 255, 255, 255, 255 };
	TTF_Font *font = TTF_OpenFont("assets/pixlig font.ttf", 50);

	// Sound effects
	Mix_Chunk *footsteps = Mix_LoadWAV("assets/footsteps.wav");
	footsteps->volume = 50;
	Mix_Chunk *jumpSound = Mix_LoadWAV("assets/jumpSound.wav");
	jumpSound->volume = 50;

	moveItemsFromMapCollision(map, weapons, pickups, players);

	while (running)
	{

		//---------------------------Game state------------------------------------
		if (!SDL_TICKS_PASSED(SDL_GetTicks(), renderTick + RENDER_TICK)) {
			//Do between ticks
			updateServer(players, client, projectiles);
			continue;
		}

		for (int j = 0; j < MAXPLAYERS; j++) {
			if (j == client->playerID) {
				players[client->playerID].dstRect.x = players[client->playerID].x;
				players[client->playerID].dstRect.y = players[client->playerID].y;
			}
			else {
				players[j].dstRect.x = players[j].x;
				players[j].dstRect.y = players[j].y;
			}
		}

		SDL_RenderClear(game->renderer);
		displayBackground(game);

		if (game->spectateMode == 0)
			updateCameraPosition(&players[client->playerID]);

		playerNameTag(players, game->renderer);
		playerHealthbar(players, game->renderer);


		pickUpPickup(pickups, players);
		weaponActions(weapons, players, client, projectiles, client->playerID, &camera);

		//moveProjectiles(projectiles);

		renderTick = SDL_GetTicks();
		game->loopCount++;

		//Sprite animation for players
		for (int i = 0; i < MAXPLAYERS; i++) {
			players[i].srcRect.x = players[i].currentSprite * players[i].srcRect.w;
		}

		// Bird only
		if (game->loopCount % SPRITESPEED == 0)
			players[2].currentSprite += 1;

		// Check for various events (keyboard, mouse, touch, close)
		while (SDL_PollEvent(&event))
		{
			//const char* key = SDL_GetKeyName(event.key.keysym.sym);
			if (event.type == SDL_QUIT) {
				running = 0;
				game->replay = 0;
				return running;
			}
		}

		//Move player
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
					Mix_PlayChannel(1, jumpSound, 0);
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
			checkForCeiling(map, &players[client->playerID], &jumpTime, &roofDetected, &groundDetected, &leftWall, &rightWall);
		}

		if (!checkForWall(map, &players[client->playerID], &key)) {
			jump(&players[client->playerID], &isJumping, &jumpTime, &doJump, &groundDetected, &roofDetected, &leftWall, &rightWall);
		}

		//Detect projectile collisions with walls
		for (i = 0; i < MAP_HEIGHT; i++) {
			for (j = 0; j < MAP_WIDTH; j++) {
				for (int p = 0; p < MAXPROJECTILES; p++) {
					for (int k = 0; k < MAXPROJECTILEOBJECTS; k++) {
						if (SDL_HasIntersection(&projectiles[p].rect[k], &map[i][j].rect)) {
							sendDeleteProjectileToServer(p, k);
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
			SDL_Rect temporaryRect;
			for (int i = 0; i < MAXPLAYERS; i++) {
				temporaryRect.w = players[i].dstRect.w;
				temporaryRect.h = players[i].dstRect.h;
				temporaryRect.x = players[i].dstRect.x - camera.x;
				temporaryRect.y = players[i].dstRect.y - camera.y;
				SDL_RenderDrawRect(game->renderer, &temporaryRect);
			}
			for (int i = 0; i < MAXNRWEAPONS; i++) {
				temporaryRect.w = weapons[i].rect.w;
				temporaryRect.h = weapons[i].rect.h;
				temporaryRect.x = weapons[i].rect.x - camera.x;
				temporaryRect.y = weapons[i].rect.y - camera.y;
				SDL_RenderDrawRect(game->renderer, &temporaryRect);
			}
			for (int j = 0; j < MAXPROJECTILES; j++) {
				for (int i = 0; i < MAXPROJECTILEOBJECTS; i++) {
					temporaryRect.w = projectiles[j].rect[i].w;
					temporaryRect.h = projectiles[j].rect[i].h;
					temporaryRect.x = projectiles[j].rect[i].x - camera.x;
					temporaryRect.y = projectiles[j].rect[i].y - camera.y;
					SDL_RenderDrawRect(game->renderer, &temporaryRect);
				}
			}

			for (int i = 0; i < MAX_NR_OF_PICKUPS; i++) {
				temporaryRect.w = pickups[i].rect.w;
				temporaryRect.h = pickups[i].rect.h;
				temporaryRect.x = pickups[i].rect.x - camera.x;
				temporaryRect.y = pickups[i].rect.y - camera.y;
				SDL_RenderDrawRect(game->renderer, &temporaryRect);
			}
		}
		//--------------------------------------------------------------------------


		updatePlayerStates(players, game->loopCount);

		// Draw players, weapons, projectiles & pickups
		drawPlayers(game, players, &nrOfPlayers, &leftWall, &rightWall);
		drawWeapons(game, players, weapons);
		drawProjectiles(game, projectiles);
		drawPickups(game, pickups);

		// Count timer + Display timer
		Uint32 timerNow = (SDL_GetTicks() - startTimer) / 1000;
		static char timerText[5] = { 0 };
		sprintf(timerText, "%d", timerNow);			// Convert int to string
		render_text(game->renderer, 5, 0, timerText, font, &timerRect, &color);

		// Check if somebody won
		running = !victoryCondition(players, game, client->playerID, client, projectiles);

		// Show what was drawn
		SDL_RenderPresent(game->renderer);
	}
	//running = 1;

	return running;
}

void quitGame(Game *game) {
	deleteBackground(game);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void playBackgroundMusic() {
	Mix_Music *backgroundMusic = Mix_LoadMUS("assets/Rolemusic_02_LeaflessQuinceTree.mp3");
	Mix_VolumeMusic(35);

	if (!backgroundMusic)
		printf("Background music is not working\n");
	Mix_PlayMusic(backgroundMusic, -1);
}

void createWindowIcon(Game *game) {
	SDL_Surface *icon = IMG_Load("assets/pistol.png");
	SDL_SetWindowIcon(game->window, icon);
	SDL_FreeSurface(icon);
}


//**************************************** Player functions **************************************************
Player createPlayer(Game *game, int id, char name[], int x, int y, int lastDirection, const char imageName[], int srcRectW, int srcRectH) {
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
	SDL_FreeSurface(player.Image);
	player.currentSprite = 0;
	player.movementSpeed = 10;
	player.iWon = 0;

	player.dstRect.x = x;
	player.dstRect.y = y;
	player.dstRect.w = PLAYER_WIDTH;
	player.dstRect.h = PLAYER_HEIGHT;

	player.srcRect.x = player.currentSprite * srcRectW;
	player.srcRect.y = 0;
	player.srcRect.w = srcRectW;
	player.srcRect.h = srcRectH;

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

void drawPlayers(Game *game, Player players[], int *nrOfPlayers, int *leftWall, int *rightWall) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].life > 0) {
			if ((players[i].lastDirection == RIGHT) && (*leftWall == 1)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &players[i].srcRect, &players[i].dstRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if ((players[i].lastDirection == RIGHT) && (*leftWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &players[i].srcRect, &players[i].dstRect, 0.0, NULL, 0);
			}
			else if ((players[i].lastDirection == LEFT) && (*rightWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &players[i].srcRect, &players[i].dstRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if ((players[i].lastDirection == LEFT) && (*leftWall == 0)) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &players[i].srcRect, &players[i].dstRect, 0.0, NULL, 0);
			}
		}

		else
			deletePlayer(players, players[i].id, nrOfPlayers);
	}
}

int victoryCondition(Player players[], Game *game, int playerid, Network *client, Projectile *projectiles) {
	int choose = 0;

	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].iWon) {
			char sound[30];
			//Victory screen
			if (players[playerid].iWon) {
				game->gameOverImage = IMG_Load("assets/winscreen.png");
				sprintf(sound, "assets/winSound.wav");
			}
			else {
				game->gameOverImage = IMG_Load("assets/losescreen.png");
				sprintf(sound, "assets/loseSound.wav");
			}

			//Sound effects
			Mix_PauseMusic();
			Mix_Chunk *gameOverSound = Mix_LoadWAV(sound);
			gameOverSound->volume = 100;
			Mix_PlayChannel(1, gameOverSound, 0);

			Mix_Chunk *clickSound = Mix_LoadWAV("assets/clickSound.wav");
			clickSound->volume = 100;

			game->gameOverScreen = SDL_CreateTextureFromSurface(game->renderer, game->gameOverImage);
			SDL_FreeSurface(game->gameOverImage);
			SDL_RenderCopy(game->renderer, game->gameOverScreen, NULL, NULL);

			//SDL_Rect victoryRect = { camera.x, camera.y, WINDOW_WIDTH, WINDOW_HEIGHT };

			//renderCopyMoveWithCamera(game->renderer, game->gameOverScreen, NULL, &victoryRect, 0.0, NULL, 0);

			SDL_RenderPresent(game->renderer);

			SDL_Event event;
			while (!choose)
				if (SDL_PollEvent(&event))
					if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
						if (event.button.x > 70 && event.button.x < 450 && event.button.y > 400 && event.button.y < 465) {
							game->replay = 0;
							Mix_PlayChannel(1, clickSound, 0);
							return 1;
						}
						else if (event.button.x > 600 && event.button.x < 940 && event.button.y > 400 && event.button.y < 465) {
							game->replay = 1;
							Mix_PlayChannel(1, clickSound, 0);
							return 1;
						}
					}
					else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
						game->replay = 1;
						Mix_PlayChannel(1, clickSound, 0);
						return 1;
					}
					updateServer(players, client, projectiles);
		}
	}
	return 0;
}

int randomX(int *smallMapModulusX, int *smallMapX) {
	int random = rand() % ((MAP_WIDTH * TILE_WIDTH) - 100 - *smallMapModulusX) + *smallMapX;

	printf("Random x spawns: \n%d, %d \n", *smallMapModulusX, *smallMapX);

	random += 100;
	return random;
}

int randomY(int *smallMapModulusY, int *smallMapY) {
	int random = rand() % ((MAP_HEIGHT * TILE_HEIGHT) - 100 - *smallMapModulusY) + *smallMapY;

	printf("Random Y spawns\n%d, %d \n", *smallMapModulusY, *smallMapY);
	random += 100;
	return random;
}