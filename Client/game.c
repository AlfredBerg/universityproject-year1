#include "game.h"

#include "player.h"
#include "weapon.h"
#include "gravity.h"
#include "background.h"
#include "projectile.h"
#include "textureManager.h"
#include "pickup.h"
#include "checkCollision.h"
#include "camera.h"


extern Network client;
extern SDL_Rect camera;

void initGame(Game *game) {

	// Initialize SDL and audio system
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//initialize support for loading png and JPEG image
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//initialize the mixer
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

	initBackground(game);

	createWindowIcon(game);
}


int runGame(Game *game, Network *client) {

	char player0Name[] = "Knight";
	char player1Name[] = "Bear";
	char player2Name[] = "Bird";

	Player players[MAXPLAYERS];
	players[0] = createPlayer(game, 0, player0Name, 60, 400, RIGHT, "assets/knightsprite.png");
	players[1] = createPlayer(game, 1, player1Name, 300, 400, LEFT, "assets/bearsprite.png");
	players[2] = createPlayer(game, 2, player2Name, 400, 400, LEFT, "assets/bird.png");

	int nrOfPlayers = 3;

	Weapon weapons[MAXNRWEAPONS];
	weapons[0] = createWeapon(game, 0, 500, 100, 10, 200, 0, "assets/pistol.png", 60, 60);
	weapons[1] = createWeapon(game, 1, 100, 400, 10, 200, 0, "assets/pistol.png", 60, 60);
	weapons[2] = createWeapon(game, 2, 200, 100, 10, 200, 0, "assets/hand.png", 60, 60);
	weapons[3] = createWeapon(game, 3, 600, 100, 10, 200, 0, "assets/beachball.png", 60, 60);


	int nrOfWeapons = 4;

	Projectile projectiles[MAXPROJECTILES];
	projectiles[0] = createProjectile(game, 0, 10, 12, 30, 30, "assets/bullet.png");
	int nrOfProjectiles = 1;

	Pickup pickups[MAX_NR_OF_PICKUPS];
	pickups[0] = createPickup(game, 0, 550, 500, 5, "assets/p_red.png");
	pickups[1] = createPickup(game, 1, 550, 400, 10, "assets/p_green.png");
	int nrOfPickups = 2;

	int running = 1;
	int again = 0;

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

	static int lvl1[MAP_HEIGHT][MAP_WIDTH] = { 0 };
	loadMap("assets/map.map", lvl1);

	//Init map
	Tile map[MAP_HEIGHT][MAP_WIDTH];
	int i, j = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			map[i][j].ID = lvl1[i][j];
			initTiles(game->renderer, &map[i][j], j, i);
		}
	}

	//Sound effects
	Mix_Chunk *footsteps = Mix_LoadWAV("assets/footsteps.wav");
	footsteps->volume = 50;
	Mix_Chunk *jumpsound = Mix_LoadWAV("assets/jumpsound.wav");
	jumpsound->volume = 50;




	while (running)
	{
		updateCameraPosition(&players[client->playerID]);
		//---------------------------Game state------------------------------------
		if (!SDL_TICKS_PASSED(SDL_GetTicks(), renderTick + RENDER_TICK)) {
			//Do between ticks
			updateServer(players, client, projectiles);
			continue;
		}

		renderTick = SDL_GetTicks();
		game->loopCount++;

		SDL_Rect srcrect[3] = { { players[0].currentSprite * 16, 0, 16, 24 },{ players[1].currentSprite * 16, 0, 16, 24 },{ players[2].currentSprite * 40, 0, 40, 40 } };
		SDL_Rect dstrect[3] = { { players[0].rect.x, players[0].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT },{ players[1].rect.x, players[1].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT },{ players[2].rect.x, players[2].rect.y, PLAYER_WIDTH, PLAYER_HEIGHT } };

		//f�r f�geln
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
				players[client->playerID].x -= 10;
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
				players[client->playerID].x += 10;
				leftWall = 1;
			}
		}



		/*if (KeyState[SDL_SCANCODE_D]) {
			if (players[client->playerID].isMoving == 0 && game->loopCount % SPRITESPEED == 0)
				players[client->playerID].currentSprite += 1;
			key = RIGHT;
			if (!Mix_Playing(0) && groundDetected)
				Mix_PlayChannel(0, footsteps, 0);
			if (!checkForWall(map, &players[client->playerID]) || key != prevKey) {
				walkRight(&players[client->playerID], &key, &prevKey);
			}
		}
		else if (KeyState[SDL_SCANCODE_A]) {
			if (players[client->playerID].isMoving == 0 && game->loopCount % SPRITESPEED == 0)
				players[client->playerID].currentSprite -= 1;
			key = LEFT;
			if (!Mix_Playing(0) && groundDetected)
				Mix_PlayChannel(0, footsteps, 0);
			if (!checkForWall(map, &players[client->playerID]) || key != prevKey) {
				walkLeft(&players[client->playerID], &key, &prevKey);
			}
		}*/



		if (KeyState[SDL_SCANCODE_W]) {
			doJump = 1;
			if (groundDetected)
				Mix_PlayChannel(1, jumpsound, 0);
		}
		if (KeyState[SDL_SCANCODE_SPACE]) {
			players[client->playerID].weaponFired = 1;
		}

		gravity(&players[client->playerID], weapons, &groundDetected, &roofDetected, map);

		if (groundDetected == 0) {
			checkForCeiling(map, &players[client->playerID], &jumpTime, &roofDetected, &groundDetected);
		}

		if (!checkForWall(map, &players[client->playerID], &key)) {
			jump(&players[client->playerID], &isJumping, &jumpTime, &doJump, &groundDetected, &roofDetected);
		}

		//else if (checkForWall(map, &players[client->playerID])) { //test f�r hopp vid v�gg: checkForWall = 2 eller 3
		//														  //checkForCeiling(map, &players[client->playerID], &jumpTime, &roofDetected, &groundDetected);
		//														  //jump2(&players[client->playerID], &isJumping, &jumpTime, &doJump, &groundDetected, &roofDetected);
		//														  //gravity(&players[client->playerID], weapons, &groundDetected, &roofDetected, map);
		//}

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

		pickUpPickup(pickups, players);

		weaponActions(weapons, players, client, projectiles);
		moveProjectiles(projectiles);

		

		//---------------------------Render------------------------------------

		//Clear screen with black
		SDL_RenderClear(game->renderer);

		displayBackground(game);

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
		drawPlayers(game, players, srcrect, dstrect, &nrOfPlayers);
		drawWeapons(game, players, weapons);
		drawProjectiles(game, projectiles);
		drawPickups(game, pickups, &nrOfPickups);

		playerHealthbar(players, game->renderer);
		playerNameTag(players, game->renderer);

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
	return player;
}

void drawPlayers(Game *game, Player players[], SDL_Rect srcrect[], SDL_Rect dstrect[], int *nrOfPlayers) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].life > 0) {
			if (players[i].lastDirection == LEFT) {
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if (players[i].lastDirection == RIGHT)
				renderCopyMoveWithCamera(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, 0);
		}

		else
			deletePlayer(players, players[i].id, nrOfPlayers);
	}
}

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


//*****************moved from runGame*********************
//int SourcePosition = 0;
//int SourcePosition2 = 0;
//int whynotwork = 1;

//if (KeyState[SDL_SCANCODE_R]) {
//	swordRect = sword1;
//	SourcePosition = swordRect.x;
//	swordRect.x += 10;
//	rPressed = 1;
//}
//
//if (SourcePosition != swordRect.x && swordRect.x <= 800 && rPressed == 1)
//swordRect.x += 10; */


//*****************moved from runGame************************
/*Checking if sword hit player1
if (bild6.x >= players[1].p1.x + 40 && bild6.x <= players[1].p1.x + 50) {
if (bild6.y <= players[1].p1.y + 99 && bild6.y >= players[1].p1.y) {
SDL_DestroyTexture(players[1].Texture);
SDL_DestroyTexture(images_Texture[4]);
SDL_DestroyTexture(images_Texture[5]);
if (bild6.x >= enemy.p1.x + 40 && bild6.x <= enemy.p1.x + 50) {
if (bild6.y <= enemy.p1.y + 99 && bild6.y >= enemy.p1.y) {
SDL_DestroyTexture(enemy.Texture);
whynotwork = 0;
//again = 1;
running = 0;
}
}
//Checking if sword hit player2
if (bild8.x <= players[0].p1.x + 40 && bild8.x >= players[0].p1.x - 50)
if (bild8.y <= players[0].p1.y + 120 && bild8.y >= players[0].p1.y - 20) {
SDL_DestroyTexture(players[0].Texture);
SDL_DestroyTexture(images_Texture[2]);
SDL_DestroyTexture(images_Texture[3]);
whynotwork = 2;
//again = 1;
running = 0;
}
*/