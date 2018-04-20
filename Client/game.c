#include "game.h"
#include "clientNetwork.h"
#include "stdbool.h"
#include "loadImage.h"
#include "player.h"

void initGame(Game *game, Network *client)
{
	// Initialize SDL and audio system
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//initialize support for loading png and JPEG image
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//initialize the mixer
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	if (TTF_Init() < 0) {
		printf("SDL error -> %s\n", SDL_GetError());
		exit(1);
	}

	game->running = 1;
	game->window = SDL_CreateWindow("knifekillers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_LENGTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	client->lastTick = SDL_GetTicks();
	client->connectedToServer = 0;
	client->playerID = 0;

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

	// Initialize SDL_net
	if (SDLNet_Init() != 0) {
		fprintf(stderr, "Erro initializing SDL_NET %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Listen on all interfaces
	if (SDLNet_ResolveHost(&client->serverIP, SERVERIP, SERVERPORT)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	//Open port
	client->serverSocket = SDLNet_UDP_Open(CLIENTPORT);
	if (!(client->serverSocket)) {
		fprintf(stderr, "SDLNet_UDP failed to open port: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->packet = SDLNet_AllocPacket(1024);
	if (!client->packet) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	client->socketSet = SDLNet_AllocSocketSet(1);
	if (client->socketSet == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}

	if (SDLNet_UDP_AddSocket(client->socketSet, client->serverSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	connectToServer(client);

}

int restart(Game* game) {

	TTF_Font *font2 = TTF_OpenFont("fintext.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface *rematch = TTF_RenderText_Solid(font2, "Rematch", color);

	SDL_Texture *rematch_Texture = SDL_CreateTextureFromSurface(game->renderer, rematch);
	SDL_FreeSurface(rematch);

	SDL_Rect RematchFontRect;
	RematchFontRect.x = 200;
	RematchFontRect.y = 260;
	RematchFontRect.w = 150;
	RematchFontRect.h = 80;
	SDL_Event ev;

	bool running = true;
	while (running) {

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				running = false;

			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.button.button == SDL_BUTTON_LEFT) {

					if (ev.button.x > 200 && ev.button.x < 350 && ev.button.y>280 && ev.button.y < 340) {
						SDL_DestroyTexture(rematch_Texture);
						running = true;
						return running;

					}
				}
			}
		}
		SDL_RenderClear(game->renderer);
		SDL_RenderCopy(game->renderer, rematch_Texture, NULL, &RematchFontRect);
		/*SDL_RenderCopy(game->renderer, image2_texture, &srcrect, &dstrect);//draw
		SDL_RenderCopy(game->renderer, image3_texture, &srcrect2, &dstrect2);
		SDL_RenderCopy(game->renderer, image5_texture, NULL, &bild5);
		SDL_RenderCopy(game->renderer, image7_texture, NULL, &bild7);
		*/

		SDL_RenderPresent(game->renderer);


	}
	SDL_DestroyTexture(rematch_Texture);
	return running;

}

int rungame(Game *game, Network *client) {

	Mix_Music *backgroundsound = Mix_LoadMUS("hello.mp3");

	if (!backgroundsound)
		printf("Sound is not working\n");

	int SourcePosition = 0;
	int SourcePosition2 = 0;
	int whynotwork = 1;


	//Create two players
	Player players[2] = {
		{ "Erik", 3, 60, 400, 1, IMG_Load("mansprite.png"),SDL_CreateTextureFromSurface(game->renderer, players[0].Image),{ 60, 400, 140, 200 } },
	{ "Skull", 100, 500, 50, 0,IMG_Load("deathsprite.png"),SDL_CreateTextureFromSurface(game->renderer, players[1].Image),{ 500, 50, 120, 120 } }
	};
	printf("%d, %d\n", players[0].p1.x, players[0].p1.y);
	printf("%d, %d", players[0].x, players[0].y);

	//initialize support for flipping images
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;


	SDL_Surface *images[MAX_IMAGES];
	SDL_Texture *images_Texture[MAX_IMAGES];

	loadImage(&images);
	//load an image file

	int nrOfImages = 8;
	for (int i = 0; i<nrOfImages; i++)
		images_Texture[i] = SDL_CreateTextureFromSurface(game->renderer, images[i]);

	//Fulkod f�r att avg�ra enemyID
	int enemyID;
	if (client->playerID == 1) {
		enemyID = 0;
	}
	else {
		enemyID = 1;
	}

	//Define where on the "screen" we want to draw the texture
	SDL_Rect bild = { 0, 0, WINDOW_LENGTH, WINDOW_HEIGHT }; //(x, y, hight, width)

															//SDL_Rect bild2 = { fighter.x, fighter.y, 140, 200 };
															//SDL_Rect bild3 = { enemy.x, enemy.y, 500, 500};
	SDL_Rect bild4 = { 150, 100, 500, 325 };
	SDL_Rect bild5 = { players[client->playerID].x + 30, players[client->playerID].y + 10, 15, 40 };
	SDL_Rect bild6 = { 100, 450, 15, 40 };
	SDL_Rect bild7 = { players[enemyID].x + 50, players[enemyID].y + 40, 15, 40 };
	SDL_Rect bild8 = { 530, 490, 15, 40 };
	SDL_Rect bild9 = { 150, 100, 550, 300 };

	Mix_PlayMusic(backgroundsound, -1);
	bool pPressed = false;
	bool rPressed = false;
	bool running = true;
	bool again = false;
	SDL_Event event;
	int sprite[2];
	sprite[0] = 1;
	sprite[1] = 1;

	int prevKey = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump = 0;

	Uint32 startTimer = SDL_GetTicks(), renderTick = SDL_GetTicks();

	while (running)
	{
		if (!SDL_TICKS_PASSED(SDL_GetTicks(), renderTick + 20)) {
			//Do between ticks
			updateServer(players, client);
			continue;
		}
		//Do when game tick
		renderTick = SDL_GetTicks();



		if (sprite[client->playerID] >= 8)
			sprite[client->playerID] = 1;
		else if (sprite[client->playerID] <= 0)
			sprite[client->playerID] = 7;


		//for sprite
		//Uint32 ticks = SDL_GetTicks(); (time based)
		//Uint32 sprite = (ticks / 100) % 4; (time based)

		SDL_Rect srcrect = { sprite[0] * 75, 0, 75, 132 };
		SDL_Rect dstrect = { players[0].p1.x, players[0].p1.y, 75, 132 };

		SDL_Rect srcrect2 = { sprite[1] * 64, 64, 64, 64 };
		SDL_Rect dstrect2 = { players[1].p1.x, players[1].p1.y, 120, 120 };



		//SDL_Rect dstTileRect[] = { 400, 200, 70, 70};



		// Check for various events (keyboard, mouse, touch, close)
		while (SDL_PollEvent(&event))
		{
			//const char* key = SDL_GetKeyName(event.key.keysym.sym);
			if (event.type == SDL_QUIT) {
				running = false;
				return running;
			}
		}

		//Move fighter
		const Uint8 *KeyState;
		KeyState = SDL_GetKeyboardState(NULL);
		if (KeyState[SDL_SCANCODE_D]) {
			sprite[client->playerID] += 1;
			prevKey = RIGHT;
		}
		else if (KeyState[SDL_SCANCODE_A]) {
			sprite[client->playerID] -= 1;
			prevKey = LEFT;
		}
		if (KeyState[SDL_SCANCODE_W]) {
			doJump = 1;
		}

		walk(&players[client->playerID], &bild5, &prevKey);
		jump(&players[client->playerID], &bild5, &isJumping, &jumpTime, &doJump);
		gravity(&players[client->playerID], &bild5);

		if (KeyState[SDL_SCANCODE_R]) {
			bild6 = bild5;
			SourcePosition = bild6.x;
			bild6.x += 10;
			rPressed = true;
		}

		if (SourcePosition != bild6.x && bild6.x <= 800 && rPressed == true)
			bild6.x += 10;


		//clear screen with black
		SDL_RenderClear(game->renderer);

		//draw
		SDL_RenderCopy(game->renderer, images_Texture[0], NULL, &bild);

		if (rPressed == true)
			SDL_RenderCopy(game->renderer, images_Texture[3], NULL, &bild6);



		for (int j = 0; j < 4; j++) {
			if (j == client->playerID) {
				players[client->playerID].p1.x = players[client->playerID].x;
				players[client->playerID].p1.y = players[client->playerID].y;
			}
			else {
				players[j].p1.x = players[j].x;
				players[j].p1.y = players[j].y;
			}
		}

		/*
		//Checking if sword hit player1
		if (bild6.x >= players[1].p1.x + 40 && bild6.x <= players[1].p1.x + 50) {
		if (bild6.y <= players[1].p1.y + 99 && bild6.y >= players[1].p1.y) {
		SDL_DestroyTexture(players[1].Texture);
		SDL_DestroyTexture(images_Texture[4]);
		SDL_DestroyTexture(images_Texture[5]);
		if (bild6.x >= enemy.p1.x + 40 && bild6.x <= enemy.p1.x + 50) {
		if (bild6.y <= enemy.p1.y + 99 && bild6.y >= enemy.p1.y) {
		SDL_DestroyTexture(enemy.Texture);
		whynotwork = 0;
		//again = true;
		running = false;
		}
		}
		//Checking if sword hit player2
		if (bild8.x <= players[0].p1.x + 40 && bild8.x >= players[0].p1.x - 50)
		if (bild8.y <= players[0].p1.y + 120 && bild8.y >= players[0].p1.y - 20) {
		SDL_DestroyTexture(players[0].Texture);
		SDL_DestroyTexture(images_Texture[2]);
		SDL_DestroyTexture(images_Texture[3]);
		whynotwork = 2;
		//again = true;
		running = false;
		}
		*/

		if (whynotwork == 0)
			SDL_RenderCopy(game->renderer, images_Texture[6], NULL, &bild9);
		if (whynotwork == 2)
			SDL_RenderCopy(game->renderer, images_Texture[1], NULL, &bild4);
		if (again == true) {

			//restart(window, renderer);

		}

		SDL_RenderCopy(game->renderer, players[0].Texture, &srcrect, &dstrect);//draw
		SDL_RenderCopy(game->renderer, players[1].Texture, &srcrect2, &dstrect2);
		SDL_RenderCopy(game->renderer, images_Texture[2], NULL, &bild5);
		SDL_RenderCopy(game->renderer, images_Texture[4], NULL, &bild7);

		//SDL_RenderCopy(renderer, text, NULL, &textRect);
		SDL_RenderPresent(game->renderer);//show what was drawn


	}
	running = true;
	return running;
}

void quitGame(Game *game){
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}