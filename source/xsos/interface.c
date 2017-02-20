#include <stdio.h>
#include <SDL2/SDL.h>
#include "map.h"

static SDL_Surface* cross		= NULL;
static SDL_Surface* zero		= NULL;
static SDL_Surface* background	= NULL;
static SDL_Surface* surf		= NULL;
static SDL_Window*  win			= NULL;

static SDL_Surface* load_sprite(const char*);

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
	
	cross = load_sprite("../share/game-xsos/cross.bmp");
	if (cross == NULL)
	{
		return 0;
	}
	
	zero = load_sprite("../share/game-xsos/zero.bmp");
	if (zero == NULL)
	{
		return 0;
	}

	background = load_sprite("../share/game-xsos/background.bmp");
	if (background == NULL)
	{
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
			rect.x = j*surf->h/3;
			rect.y = i*surf->w/3;
			rect.w = rect.h = surf->w/3;

			int a;
			a = SDL_BlitScaled(background, NULL, surf, &rect);
			if (a != 0)
			{
				printf("SDL error: %s\n", SDL_GetError());
				return 0;
			}
			
			if (map[i][j] == MAP_CROSS)
			{
				a = SDL_BlitScaled(cross, NULL, surf, &rect);
				if (a != 0)
				{
					printf("SDL error: %s\n", SDL_GetError());
					return 0;
				}
			}
			else if (map[i][j] == MAP_ZERO)
			{
				a = SDL_BlitScaled(zero, NULL, surf, &rect);
				if (a != 0)
				{
					printf("SDL error: %s\n", SDL_GetError());
					return 0;
				}
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
	
	if (win != NULL) SDL_DestroyWindow(win);
	return;
}

int player_turn(int** map, int* quit)
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
				int j = ev.button.x/(surf->w/3);
				int i = ev.button.y/(surf->h/3);
				
				if (map[i][j] == MAP_EMPTY)
				{
					map[i][j] = MAP_CROSS;
					cont = 0;
					break;
				}
			}
			else if (ev.type == SDL_QUIT)
			{
				cont = 0;
				*quit = 1;
				break;
			}		
		}
	}
	
	return 1;
}

static SDL_Surface* load_sprite(const char* path)
{
	SDL_Surface* tmp;
	SDL_Surface* ret;
	int a;
	
	tmp = SDL_LoadBMP(path);
	if (tmp == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	
	ret = SDL_ConvertSurface(tmp, surf->format, 0);
	if (ret == NULL)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	
	SDL_FreeSurface(tmp);
	
	a = SDL_SetColorKey(ret, 1, SDL_MapRGB(ret->format, 0, 0, 0));
	if (a != 0)
	{
		printf("SDL error: %s\n", SDL_GetError());
		return NULL;
	}
	
	return ret;
}

