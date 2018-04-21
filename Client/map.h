#pragma once
#define NR_OF_TILES 5
#define MAP_HEIGHT 16
#define MAP_WIDTH 16

SDL_Rect createRectangle(int x, int y, int w, int h);
void createRect(SDL_Rect *rectangle, int x, int y, int w, int h);
void initTiles();
void drawMap();