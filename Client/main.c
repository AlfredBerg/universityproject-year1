#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include "playerStruct.h"
#include "game.h"
#include "menu.h"
#include "weaponStruct.h"
#include "gravity.h"

void game_init(Game *game);
int menu(Game *game);
int menuOptions(SDL_Event event, bool *menuLoop);
int restart(Game *game);
int rungame(Game *game);
void jump(Player *player, SDL_Rect *weapon, int *isJumping, int *jumpTime);

#define WINDOWLENGTH 800
#define WINDOWHEIGHT 600
#define UP 1
#define LEFT 2
#define RIGHT 3

int main(int argc, char** argv)
{
	Game game;

	game_init(&game);

	while (game.running){
		game.running=menu(&game);
		while (game.running) {
			game.running=rungame(&game);
			//if(game.running)
			//	game.running = restart(&game);
		}
	}
	
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}

void game_init(Game *game)
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
		WINDOWLENGTH, WINDOWHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
}

int menu(Game *game) {

	TTF_Font *font = TTF_OpenFont("fintext.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };

	SDL_Surface *textImage = TTF_RenderText_Solid(font, "START", color);
	SDL_Surface *menuImage = IMG_Load("startscreen.jpg");
		
	SDL_Texture *text = SDL_CreateTextureFromSurface(game->renderer, textImage);
	SDL_FreeSurface(textImage);
	SDL_Texture *background = SDL_CreateTextureFromSurface(game->renderer, menuImage);
	SDL_FreeSurface(menuImage);

	SDL_Rect textRect;
	textRect.x = 100;
	textRect.y = 260;
	textRect.w = 150;
	textRect.h = 80;

	SDL_Rect backRect = { 0, 0, WINDOWLENGTH, WINDOWHEIGHT };

	bool startGame = true;
	bool menuLoop = true;
	SDL_Event event;
	/*while(menuLoop){
		if (SDL_PollEvent(&event) != 0) {
			startGame = menuOptions(event, &menuLoop);
		}

		SDL_RenderClear(game->renderer);
		SDL_RenderCopy(game->renderer, background, NULL, &backRect);
		SDL_RenderCopy(game->renderer, text, NULL, &textRect);
		SDL_RenderPresent(game->renderer);
	}*/
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(background);
	return startGame;
}

int menuOptions(SDL_Event event, bool *menuLoop) {
	bool running = true;
	if (event.type == SDL_QUIT){
		running = false;
		*menuLoop = false;
	}
		
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (event.button.x > 100 && event.button.x < 250 && event.button.y>280 && event.button.y < 340) {
				running = true;
				*menuLoop = false;
			}
		}
	}

	return running;
}

int restart(Game* game) {

	TTF_Font *font2 = TTF_OpenFont("fintext.ttf", 20);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface *rematch = TTF_RenderText_Solid(font2, "Rematch", color);

	SDL_Texture *rematch_Texture=SDL_CreateTextureFromSurface(game->renderer, rematch);  
	SDL_FreeSurface(rematch);

	SDL_Rect RematchFontRect;
	RematchFontRect.x = 200;
	RematchFontRect.y = 260;
	RematchFontRect.w = 150;
	RematchFontRect.h = 80;
	SDL_Event ev;

	bool running = true;
	while(running) {

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				running = false;

			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.button.button == SDL_BUTTON_LEFT) {

					if (ev.button.x > 200 && ev.button.x < 350 && ev.button.y>280 && ev.button.y < 340){
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
int rungame(Game *game) {

	Mix_Music *backgroundsound = Mix_LoadMUS("hello.mp3");
	
	if (!backgroundsound)
		printf("got me good");

	int SourcePosition = 0;
	int SourcePosition2 = 0;
	int whynotwork = 1;

	
	//Create two players
	Player fighter = { "Erik", 3, 60, 400, 1, IMG_Load("mansprite.png"),SDL_CreateTextureFromSurface(game->renderer, fighter.Image), {60, 400, 140, 200}};
	Player enemy = { "Skull", 100, 500, 50, 0,IMG_Load("deathsprite.png"),SDL_CreateTextureFromSurface(game->renderer, enemy.Image), {500, 50, 120, 120}};
	printf("%d, %d\n", fighter.p1.x, fighter.p1.y);
	printf("%d, %d", fighter.x, fighter.y);

	//initialize support for flipping images
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

	const Uint8 *KeyState;

	//load an image file


	SDL_Surface *image = IMG_Load("bowser.png");
	SDL_Surface *image2 = IMG_Load("mansprite.png");
	SDL_Surface *image3 = IMG_Load("deathsprite.png");
	SDL_Surface *image4 = IMG_Load("deathwins.jpg");

	SDL_Surface *image5 = IMG_Load("sword1.png");
	SDL_Surface *image6 = IMG_Load("sword1.png");

	SDL_Surface *image7 = IMG_Load("sword2.png");
	SDL_Surface *image8 = IMG_Load("sword2.png");
	SDL_Surface *image9 = IMG_Load("humanwin.png");

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(game->renderer, image);
	SDL_FreeSurface(image);
	//SDL_Texture *image2_texture = SDL_CreateTextureFromSurface(game->renderer, image2);
	//SDL_FreeSurface(image2);
	//SDL_Texture *image3_texture = SDL_CreateTextureFromSurface(game->renderer, image3);
	//SDL_FreeSurface(image3);
	SDL_Texture *image4_texture = SDL_CreateTextureFromSurface(game->renderer, image4);
	SDL_FreeSurface(image4);
	SDL_Texture *image5_texture = SDL_CreateTextureFromSurface(game->renderer, image5);
	SDL_FreeSurface(image5);
	SDL_Texture *image6_texture = SDL_CreateTextureFromSurface(game->renderer, image6);
	SDL_FreeSurface(image6);
	SDL_Texture *image7_texture = SDL_CreateTextureFromSurface(game->renderer, image7);
	SDL_FreeSurface(image7);
	SDL_Texture *image8_texture = SDL_CreateTextureFromSurface(game->renderer, image8);
	SDL_FreeSurface(image8);
	SDL_Texture *image9_texture = SDL_CreateTextureFromSurface(game->renderer, image9);
	SDL_FreeSurface(image9);

	//Define where on the "screen" we want to draw the texture
	SDL_Rect bild = { 0, 0, WINDOWLENGTH, WINDOWHEIGHT }; //(x, y, hight, width)

	//SDL_Rect bild2 = { fighter.x, fighter.y, 140, 200 };
	//SDL_Rect bild3 = { enemy.x, enemy.y, 500, 500};
	SDL_Rect bild4 = { 150, 100, 500, 325 };
	SDL_Rect bild5 = { 100, 450, 15, 40 };
	SDL_Rect bild6 = { 100, 450, 15, 40 };
	SDL_Rect bild7 = { 530, 450, 15, 40 };
	SDL_Rect bild8 = { 530, 490, 15, 40 };
	SDL_Rect bild9 = { 150, 100, 550, 300 };

	Mix_PlayMusic(backgroundsound, -1);
	bool pPressed = false;
	bool rPressed = false;
	bool running = true;
	bool again = false;
	SDL_Event event;
	int sprite = 1;
	int sprite2 = 1;

	int prevKey = 0;
	int isJumping = 0;
	int jumpTime = 0;
	int doJump1 = 0;
	int doJump2 = 0;

	while (running)
	{
		if (sprite >= 8)
			sprite = 1;
		else if (sprite <= 0)
			sprite = 7;
		if (sprite2 >= 8)
			sprite2 = 1;
		else if (sprite2 <= 0)
			sprite2 = 7;

		//for sprite
		//Uint32 ticks = SDL_GetTicks(); (time based)
		//Uint32 sprite = (ticks / 100) % 4; (time based)

		SDL_Rect srcrect = { sprite * 75, 0, 75, 132 };
		SDL_Rect dstrect = { fighter.p1.x, fighter.p1.y, 75, 132 };

		SDL_Rect srcrect2 = { sprite2 * 64, 64, 64, 64 };
		SDL_Rect dstrect2 = { enemy.p1.x, enemy.p1.y, 120, 120 };



		// Check for various events (keyboard, mouse, touch, close)
		while (SDL_PollEvent(&event))
		{
			//const char* key = SDL_GetKeyName(event.key.keysym.sym);
			if (event.type == SDL_QUIT) {
				running = false;
				return running;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_w) {
					printf("W is pressed");
					doJump1 = 1;
				}
				if (event.key.keysym.sym == SDLK_UP) {
					printf("UP is pressed");
					doJump2 = 1;
				}
			}
		}
		KeyState = SDL_GetKeyboardState(NULL);
		if (KeyState[SDL_SCANCODE_D] && fighter.x < 730) {
			sprite += 1;
			fighter.x += 10;
			bild5.x += 10;
			prevKey = RIGHT;
		}
		else if (KeyState[SDL_SCANCODE_A] && fighter.x > -10) {
			sprite -= 1;
			fighter.x -= 10;
			bild5.x -= 10;
			prevKey = LEFT;
		}
		//if (prevKey == UP) {
			
		//}
			jump(&fighter, &bild5, &isJumping, &jumpTime, &doJump1);
			gravity(&fighter, &bild5);
		
		
		/* DOWN ISN'T USED
		else if (KeyState[SDL_SCANCODE_S] && bild2.y < 470) {
			sprite += 1;
			bild2.y += 10;
			bild5.y += 10;
		} */

		if (KeyState[SDL_SCANCODE_RIGHT] && enemy.x < 712) {
			sprite2 += 1;
			enemy.x += 10;
			bild7.x += 10;
			prevKey = RIGHT;
		}
		else if (KeyState[SDL_SCANCODE_LEFT] && enemy.x > -22) {
			sprite2 -= 1;
			enemy.x -= 10;
			bild7.x -= 10;
			prevKey = LEFT;
		}
			jump(&enemy, &bild7, &isJumping, &jumpTime, &doJump2);
			gravity(&enemy, &bild7);
		
		
		/* DOWN ISN'T USED
		else if (KeyState[SDL_SCANCODE_DOWN] && bild3.y < 485) {
			sprite2 += 1;
			bild3.y += 10;
			bild7.y += 10;
		} */

		if (KeyState[SDL_SCANCODE_R]) {
			bild6 = bild5;
			SourcePosition = bild6.x;
			bild6.x += 10;
			rPressed = true;
		}
		if (KeyState[SDL_SCANCODE_P]) {
			bild8 = bild7;
			SourcePosition2 = bild8.x;
			bild8.x -= 10;
			pPressed = true;
		}
		if (SourcePosition != bild6.x && bild6.x <= 800 && rPressed == true)
			bild6.x += 10;

		if (SourcePosition2 != bild8.x && bild8.x >= -10 && pPressed == true)
			bild8.x -= 10;

		//clear screen with black
		SDL_RenderClear(game->renderer);

		//draw
		SDL_RenderCopy(game->renderer, image_texture, NULL, &bild);

		if (rPressed == true)
			SDL_RenderCopy(game->renderer, image6_texture, NULL, &bild6);

		if (pPressed == true)
			SDL_RenderCopy(game->renderer, image8_texture, NULL, &bild8);

		fighter.p1.x = fighter.x;
		fighter.p1.y = fighter.y;
		enemy.p1.x = enemy.x;
		enemy.p1.y = enemy.y;

		//Checking if sword hit player1
		if (bild6.x >= enemy.p1.x + 40 && bild6.x <= enemy.p1.x + 50) {
			if (bild6.y <= enemy.p1.y + 99 && bild6.y >= enemy.p1.y) {
				SDL_DestroyTexture(enemy.Texture);
				SDL_DestroyTexture(image7_texture);
				SDL_DestroyTexture(image8_texture);
				whynotwork = 0;
				//again = true;
				running=false;

			}
		}

		//Checking if sword hit player2
		if (bild8.x <= fighter.p1.x + 40 && bild8.x >= fighter.p1.x - 50)
			if (bild8.y <= fighter.p1.y + 120 && bild8.y >= fighter.p1.y - 20) {
				SDL_DestroyTexture(fighter.Texture);
				SDL_DestroyTexture(image5_texture);
				SDL_DestroyTexture(image6_texture);
				whynotwork = 2;
				//again = true;
				running = false;
			}

		if(whynotwork==0)
		SDL_RenderCopy(game->renderer, image9_texture, NULL, &bild9);
		if (whynotwork == 2)
		SDL_RenderCopy(game->renderer, image4_texture, NULL, &bild4);
		if (again==true) {

		//restart(window, renderer);

		}

		SDL_RenderCopy(game->renderer, fighter.Texture, &srcrect, &dstrect);//draw
		SDL_RenderCopy(game->renderer, enemy.Texture, &srcrect2, &dstrect2);
		SDL_RenderCopy(game->renderer, image5_texture, NULL, &bild5);
		SDL_RenderCopy(game->renderer, image7_texture, NULL, &bild7);
		//SDL_RenderCopy(renderer, text, NULL, &textRect);

		SDL_RenderPresent(game->renderer);//show what was drawn
	}
	running = true;
	return running;
}