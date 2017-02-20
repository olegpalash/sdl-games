#include <stdio.h>
#include <SDL.h>
#include "map.h"

static SDL_Surface* cross		= NULL;
static SDL_Surface* zero		= NULL;
static SDL_Surface* background	= NULL;
static SDL_Surface* surf		= NULL;
static SDL_Window*  win			= NULL;

int init_ui(
	int w,
	int h)
{
	int a;
	
	a = SDL_Init(SDL_INIT_VIDEO);
	if (a != 0)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	win = SDL_CreateWindow(
		"XsOs",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		0);
	if (win == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	surf = SDL_GetWindowSurface(win);
	if (surf == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	cross = SDL_LoadBMP("../share/game-xsos/cross.bmp");
	if (cross == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	a = SDL_SetColorKey(cross, 1, SDL_MapRGB(surf->format, 255, 255, 255));
	if (a != 0)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	zero = SDL_LoadBMP("../share/game-xsos/zero.bmp");
	if (zero == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	a = SDL_SetColorKey(zero, 1, SDL_MapRGB(surf->format, 255, 255, 255));
	if (a != 0)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	background = SDL_LoadBMP("../share/game-xsos/background.bmp");
	if (background == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	return 1;
}

int draw_ui(int** map)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			SDL_Rect rect;
			rect.x = j*surf->w/3;
			rect.y = i*surf->w/3;
			rect.w = rect.h = w/3;

			SDL_BlitScaled(background, NULL, surf, &rect);
			
			if (map[i][j] == MAP_CROSS)
			{
				SDL_BlitScaled(cross, NULL, surf, &rect);
			}
			else if (map[i][j] == MAP_ZERO)
			{
				SDL_BlitScaled(zero, NULL, surf, &rect);
			}
		}
	}
	
	SDL_UpdateWindowSurface(win);
	return 1;
}

void close_ui(void)
{
	if (background != NULL) SDL_FreeSurface(background);
	if (cross != NULL) SDL_FreeSurface(cross);
	if (zero != NULL) SDL_FreeSurface(zero);
	
	if (win != NULL) SDL_CloseWindow(win);
	return;
}

int player_turn(int** map)
{
	SDL_Event ev;
	int cont = 1;
	
	while (cont)
	{
		draw_ui(map);
		
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_MOUSEBUTTONUP)
			{
				int j = ev.button.x/(w/3);
				int i = ev.button.y/(h/3);
				
				if (map[i][j] == MAP_EMPTY)
				{
					map[i][j] = MAP_CROSS;
					break;
				}
			}
			else if (ev.type = SDL_QUIT)
			{
				*cont = 0;
				break;
			}		
		}
	}
	
	return 1;
}

