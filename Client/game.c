#include "game.h"
#include "clientNetwork.h"
#include "player.h"
#include "weapon.h"
#include "gravity.h"
#include "background.h"
#include "projectile.h"
#include "textureManager.h"
#include "tile.h"
#include "map.h"

static int map[MAP_HEIGHT][MAP_WIDTH] =
{ { 1,1,2,2,2,2,2,2,1,1,2,2,2,2,2,1 },
{ 1,1,1,1,2,1,1,2,1,1,2,2,2,2,2,1 },
{ 2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,1 },
{ 2,1,1,2,2,1,1,2,1,1,2,2,2,2,2,1 },
{ 2,1,1,4,4,4,1,2,1,1,2,2,2,2,4,1 },
{ 2,1,1,4,4,4,1,2,1,1,2,2,2,2,2,1 },
{ 2,1,1,4,4,4,2,2,1,1,2,2,4,2,2,1 },
{ 2,2,2,4,4,4,2,2,2,3,3,3,4,2,2,1 },
{ 1,1,2,2,2,2,2,3,4,3,3,3,4,2,2,2 },
{ 1,1,1,1,2,2,2,2,2,3,3,3,2,2,2,3 },
{ 2,1,1,1,2,2,2,2,2,2,2,2,2,2,2,1 },
{ 2,1,1,2,2,2,1,2,2,2,3,2,2,2,4,4 },
{ 2,1,1,4,2,2,1,2,2,2,3,2,2,2,2,4 },
{ 2,1,1,1,2,2,1,2,2,1,3,3,3,3,3,4 },
{ 2,1,1,1,1,1,1,2,2,1,2,2,2,2,4,4 },
{ 2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,1 } };

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
}


int runGame(Game *game, Network *client) {

	//Create two players
	Player players[MAXPLAYERS] = {
		{ "Erik", 100, 60, 400, 1, 0, SDL_GetTicks(), IMG_Load("mansprite.png"), SDL_CreateTextureFromSurface(game->renderer, players[0].Image),{ 60, 400, 70, 120 } },
		{ "Skull", 100, 300, 400, 0, 0, SDL_GetTicks(), IMG_Load("deathsprite.png"), SDL_CreateTextureFromSurface(game->renderer, players[1].Image),{ 500, 50, 52, 100 } }
	};

	Weapon weapons[MAXNRWEAPONS] = {
		{ 0, 400, 40, 10, 500, 0, IMG_Load("pistol.png"), SDL_CreateTextureFromSurface(game->renderer, weapons[0].Image),{ 50, 50, 46, 31 }, 0 }
	};

	Projectile projectiles[MAXPROJECTILES] = {
		{ 0, 10, 10, 0, 30, 30, IMG_Load("bullet.png"), SDL_CreateTextureFromSurface(game->renderer, projectiles[0].Image) }
	};

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
	int isJumping = 0;
	int jumpTime = 0;
	int doJump = 0;
	int loopCount = 0;

	Uint32 startTimer = SDL_GetTicks(), renderTick = SDL_GetTicks();

	//Init map
	Tile createMap[MAP_WIDTH][MAP_HEIGHT];
	int i, j = 0;
	for (i = 0; i < MAP_WIDTH; i++) {
		for (j = 0; j < MAP_HEIGHT; j++) {
			createMap[i][j].ID = map[i][j];
			initTiles1(game, &createMap[i][j], i, j);
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

		if (sprite[client->playerID] >= 8)
			sprite[client->playerID] = 1;
		else if (sprite[client->playerID] <= 0)
			sprite[client->playerID] = 7;


		//for sprite
		//Uint32 ticks = SDL_GetTicks(); (time based)
		//Uint32 sprite = (ticks / 100) % 4; (time based)

		SDL_Rect srcrect = { sprite[0] * 75, 0, 75, 132 };
		SDL_Rect dstrect = { players[0].rect.x, players[0].rect.y, 75, 132 };

		SDL_Rect srcrect2 = { sprite[1] * 64 + 17, 64 + 15, 64, 64 };
		SDL_Rect dstrect2 = { players[1].rect.x, players[1].rect.y, 120, 140 };


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
			prevKey = RIGHT;
		}
		else if (KeyState[SDL_SCANCODE_A]) {
			if (!(loopCount % 3))
				sprite[client->playerID] -= 1;
			prevKey = LEFT;
		}
		if (KeyState[SDL_SCANCODE_W]) {
			doJump = 1;
		}
		if (KeyState[SDL_SCANCODE_SPACE]) {
			players[client->playerID].weaponFired = 1;
		}

		walk(&players[client->playerID], &prevKey);
		jump(&players[client->playerID], &isJumping, &jumpTime, &doJump);
		gravity(&players[client->playerID], weapons);

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

		weaponActions(weapons, players, client, projectiles);
		moveProjectiles(projectiles);

		//---------------------------Render------------------------------------

		//Clear screen with black
		SDL_RenderClear(game->renderer);

		displayBackground(game);

		drawMap(game);

		//Display map
		int i, j = 0;
		for (i = 0; i < MAP_WIDTH; i++) {
			for (j = 0; j < MAP_HEIGHT; j++) {
				drawTiles1(game, &createMap[i][j], i, j);
			}
		}

		//-----------------------------DEBUG MODE-----------------------------------
		if (game->debug == 1) {
			SDL_RenderDrawRect(game->renderer, &players[1].rect);
			SDL_RenderDrawRect(game->renderer, &players[0].rect);
			SDL_RenderDrawRect(game->renderer, &weapons[0].rect);


			if (SDL_HasIntersection(&players[1].rect, &players[0].rect)) {
				printf("COLLISION\n");
			}

			if (SDL_HasIntersection(&players[1].rect, &players[0].rect)) {
				printf("COLLISION\n");
			}

			for (i = 0; i < MAP_WIDTH; i++) {
				for (j = 0; j < MAP_HEIGHT; j++) {
					if (SDL_HasIntersection(&players[1].rect, &createMap[i][j].rect)) {
						printf("COLLISION with tile\n");
					}
				}
			}
			

			for (int i = 0; i < MAXPROJECTILEOBJECTS; i++) {
				SDL_RenderDrawRect(game->renderer, &projectiles[0].rect[i]);
			}
		}

		//Draw players
		SDL_RenderCopy(game->renderer, players[0].Texture, &srcrect, &dstrect);
		SDL_RenderCopy(game->renderer, players[1].Texture, &srcrect2, &dstrect2);

		//Draw weapons / pickups
		SDL_RenderCopy(game->renderer, weapons[0].Texture, NULL, &weapons[0].rect);

		for (int i = 0; i < MAXPROJECTILES; i++) {
			for (int j = 0; j < MAXPROJECTILEOBJECTS; j++) {
				SDL_RenderCopy(game->renderer, projectiles[i].Texture, NULL, &projectiles[i].rect[j]);
			}
		}

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



//*****************moved from initGame********************
//Window icon
/*SDL_Surface *icon = IMG_Load("sword1.png");
SDL_SetWindowIcon(game->window, icon);
SDL_FreeSurface(icon);*/

//SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);



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