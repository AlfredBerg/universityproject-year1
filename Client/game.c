#include "game.h"
#include "clientNetwork.h"
#include "player.h"
#include "weapon.h"
#include "gravity.h"
#include "background.h"
#include "projectile.h"
#include "textureManager.h"
#include "map.h"
#include "pickup.h"

static int lvl1[MAP_HEIGHT][MAP_WIDTH] = {
	{ 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 },
{ 19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0 },
{ 2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0 },
{ 2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0 },
{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 },
{ 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 },
};


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

	game->cloudsBack = loadTexture("assets/CloudsBack.png", game);
	game->cloudsFront = loadTexture("assets/CloudsFront.png", game);
	game->bgBack = loadTexture("assets/BGBack.png", game);
	game->bgFront = loadTexture("assets/BGFront.png", game);

	createWindowIcon(game);
}


int runGame(Game *game, Network *client) {
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

	//Create two players

	Player players[MAXPLAYERS];
	players[0] = createPlayer(game, 0, "Erik", 60, 400, RIGHT, "knightsprite.png", 75, 132);
	players[1] = createPlayer(game, 1, "Skull", 300, 400, LEFT, "bearsprite.png", 75, 132);
	int nrOfPlayers = 2;

	Weapon weapons[MAXNRWEAPONS] = {
		{ 0, 400, 40, 10, 200, 0, IMG_Load("pistol.png"), SDL_CreateTextureFromSurface(game->renderer, weapons[0].Image),{ 50, 50, 46, 31 }, 0 }
	};

	Projectile projectiles[MAXPROJECTILES] = {
		{ 0, 10, 12, 0, 30, 30, IMG_Load("bullet.png"), SDL_CreateTextureFromSurface(game->renderer, projectiles[0].Image) }
	};

	Pickup pickups[MAX_NR_OF_PICKUPS];
	pickups[0] = createPickup(game, 0, 550, 500, 5, "assets/p_red.png", 32, 32);
	pickups[1] = createPickup(game, 1, 550, 400, 10, "assets/p_green.png", 32, 32);
	int nrOfPickups = 2;

	//Fulkod för att avgöra enemyID
	int enemyID;
	if (client->playerID == 1) {
		enemyID = 0;
	}
	else {
		enemyID = 1;
	}

	int running = 1;
	int again = 0;

	SDL_Event event;

	int sprite[2] = { 1, 1 };

	int prevKey = 0;
	int key = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump = 0;
	int loopCount = 0;
	int enableWalk = 1;
	int groundDetected = 0;

	Uint32 startTimer = SDL_GetTicks(), renderTick = SDL_GetTicks();

	//Init map
	Tile map[MAP_HEIGHT][MAP_WIDTH];
	int i, j = 0;
	for (i = 0; i < MAP_HEIGHT; i++) {
		for (j = 0; j < MAP_WIDTH; j++) {
			map[i][j].ID = lvl1[i][j];
			initTiles(game->renderer, &map[i][j], j, i);
		}
	}

	while (running)
	{
		//---------------------------Game state------------------------------------
		if (!SDL_TICKS_PASSED(SDL_GetTicks(), renderTick + RENDER_TICK)) {
			//Do between ticks
			updateServer(players, client);
			continue;
		}

		renderTick = SDL_GetTicks();
		loopCount++;

		if (sprite[client->playerID] >= 5)
			sprite[client->playerID] = 1;
		else if (sprite[client->playerID] <= 0)
			sprite[client->playerID] = 4;


		//for sprite
		//Uint32 ticks = SDL_GetTicks(); (time based)
		//Uint32 sprite = (ticks / 100) % 4; (time based)

		SDL_Rect srcrect[2] = { { sprite[0] * 16, 0, 16, 24 } ,{ sprite[1] * 16, 0, 16, 24 } };
		SDL_Rect dstrect[2] = { { players[0].rect.x, players[0].rect.y, 75, 132 },{ players[1].rect.x, players[1].rect.y, 75, 132 } };


		//SDL_Rect dstTileRect[] = { 400, 200, 70, 70};


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
			if (!(loopCount % 3))
				sprite[client->playerID] += 1;
			key = RIGHT;
			players[client->playerID].lastDirection = RIGHT;

		}
		else if (KeyState[SDL_SCANCODE_A]) {
			if (!(loopCount % 3))
				sprite[client->playerID] -= 1;
			key = LEFT;
			players[client->playerID].lastDirection = LEFT;
		}
		if (KeyState[SDL_SCANCODE_W]) {
			doJump = 1;
		}
		if (KeyState[SDL_SCANCODE_SPACE]) {
			players[client->playerID].weaponFired = 1;
		}

		//Collision detection player <-> tile 
		for (i = 0; i < MAP_HEIGHT; i++) {
			for (j = 0; j < MAP_WIDTH; j++) {
				if (SDL_HasIntersection(&players[client->playerID].rect, &map[i][j].rect)) {
					handleCollision(&players[client->playerID], map[i][j].x, map[i][j].y, &key, &prevKey, &groundDetected, &enableWalk);
				}
			}
		}

		walk(&players[client->playerID], &key, &enableWalk, &prevKey);
		groundDetected = 0;

		//Collision detection player <-> tile	
		for (i = 0; i < MAP_HEIGHT; i++) {
			for (j = 0; j < MAP_WIDTH; j++) {
				if (SDL_HasIntersection(&players[client->playerID].rect, &map[i][j].rect)) {
					handleCollision(&players[client->playerID], map[i][j].x, map[i][j].y, &key, &prevKey, &groundDetected, &enableWalk);
				}
			}
		}

		gravity(&players[client->playerID], weapons, &groundDetected);
		jump(&players[client->playerID], &isJumping, &jumpTime, &doJump, &groundDetected);


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

		printf("%d\n", projectiles[0].w);

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


			for (int i = 0; i < MAXPROJECTILEOBJECTS; i++) {
				SDL_RenderDrawRect(game->renderer, &projectiles[0].rect[i]);
			}
		}

		//Draw players
		drawPlayers(game, players, srcrect, dstrect, &nrOfPlayers);

		playerHealthbar(players, game->renderer);

		//Draw weapon
		if (players[client->playerID].lastDirection == LEFT && weapons[0].isPickedUp == 1) {
			SDL_RenderCopyEx(game->renderer, weapons[0].Texture, NULL, &weapons[0].rect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
		}
		else {
			SDL_RenderCopy(game->renderer, weapons[0].Texture, NULL, &weapons[0].rect);
		}

		for (int i = 0; i < MAXPROJECTILES; i++) {
			for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
				if (projectiles[i].direction[j] == LEFT) {
					SDL_RenderCopyEx(game->renderer, projectiles[i].Texture, NULL, &projectiles[i].rect[j], 0.0, NULL, SDL_FLIP_HORIZONTAL);
				}
				else {
					SDL_RenderCopy(game->renderer, projectiles[i].Texture, NULL, &projectiles[i].rect[j]);
				}

			}
		}

		drawPickup(game, pickups, &nrOfPickups);

		SDL_RenderPresent(game->renderer); //show what was drawn
	}
	running = 1;
	return running;
}

void quitGame(Game *game) {
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}

void playBackgroundMusic() {
	Mix_Music *backgroundMusic = Mix_LoadMUS("hello.mp3");

	if (!backgroundMusic)
		printf("Background music is not working\n");
	Mix_PlayMusic(backgroundMusic, -1);
}

void createWindowIcon(Game *game) {
	SDL_Surface *icon = IMG_Load("assets/crystal.png");
	SDL_SetWindowIcon(game->window, icon);
	SDL_FreeSurface(icon);
}

Player createPlayer(Game *game, int id, char name[], int x, int y, int lastDirection, const char imageName[], int rectW, int rectH) {
	Player player;
	strcpy(player.name, name);
	player.id = id;
	player.life = 100;
	player.x = x;
	player.y = y;
	player.pickupID = -1;
	player.weaponID = -1;
	player.weaponFired = 0;
	player.tickThatWeaponFired = SDL_GetTicks();
	player.tickThatLostHealth = SDL_GetTicks();
	player.lastDirection = lastDirection;
	player.Image = IMG_Load(imageName);
	player.Texture = SDL_CreateTextureFromSurface(game->renderer, player.Image);
	player.rect.x = x;
	player.rect.y = y;
	player.rect.w = rectW;
	player.rect.h = rectH;
	return player;
}

void drawPlayers(Game *game, Player players[], SDL_Rect srcrect[], SDL_Rect dstrect[], int *nrOfPlayers) {
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (players[i].life > 0) {
			if (players[i].lastDirection == LEFT) {
				SDL_RenderCopyEx(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i], 0.0, NULL, SDL_FLIP_HORIZONTAL);
			}
			else if (players[i].lastDirection == RIGHT)
				SDL_RenderCopy(game->renderer, players[i].Texture, &srcrect[i], &dstrect[i]);
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

//*****************moved from runGame************************
//Define where on the "screen" we want to draw the texture
//SDL_Rect bild2 = { fighter.x, fighter.y, 140, 200 };
//SDL_Rect bild3 = { enemy.x, enemy.y, 500, 500};
//SDL_Rect bild4 = { 150, 100, 500, 325 };		//Death wins rect
//SDL_Rect bild8 = { 530, 490, 15, 40 };		//Not used
//SDL_Rect bild9 = { 150, 100, 550, 300 };		//Human wins rect
//SDL_Rect sword1 = { players[client->playerID].x + 30, players[client->playerID].y + 10, 15, 40 };	//first word rect AKA bild5
//SDL_Rect swordRect = { 100, 450, 15, 40 };															//empty sword rect AKA bild6
//SDL_Rect sword2 = { players[enemyID].x + 20, players[enemyID].y + 40, 15, 40 };						//second sword rect AKA bild7