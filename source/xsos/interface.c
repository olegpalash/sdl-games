#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "map.h"

static SDL_Surface* cross		= NULL;
static SDL_Surface* zero		= NULL;
static SDL_Surface* background	= NULL;

static TTF_Font* font			= NULL;

static SDL_Window*  win			= NULL;
static SDL_Surface* surf		= NULL;

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
	
	a = TTF_Init();
	if (a != 0)
	{
		printf("SDL TTF error: %s\n", TTF_GetError());
		return 0;
	}
	
	a = IMG_Init(IMG_INIT_PNG);
	if (a == 0)
	{
		printf("SDL IMG error: %s\n", IMG_GetError());
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
	
	font = TTF_OpenFont("../share/common/sans.ttf", 72);
	if (font == NULL)
	{
		printf("SDL TTF error: %s\n", TTF_GetError());
		return 0;
	}
	
	cross = load_sprite("../share/game-xsos/cross.png");
	if (cross == NULL) return 0;

	zero = load_sprite("../share/game-xsos/zero.png");
	if (zero == NULL) return 0;

	background = load_sprite("../share/game-xsos/background.png");
	if (background == NULL) return 0;

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
	
	return 1;
}

void close_ui(void)
{
	if (background != NULL) SDL_FreeSurface(background);
	if (cross != NULL) SDL_FreeSurface(cross);
	if (zero != NULL) SDL_FreeSurface(zero);
	
	if (win != NULL) SDL_DestroyWindow(win);
	
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return;
}

int player_turn(int** map, int* quit)
{
	SDL_Event ev;
	int cont = 1;
	int redraw = 1;
	
	while (cont)
	{
		if (redraw)
		{
			draw_ui(map);
			SDL_UpdateWindowSurface(win);
			redraw = 0;
		}
		
		SDL_WaitEvent(&ev);
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
		else if (ev.type == SDL_WINDOWEVENT)
		{
			if (ev.window.event == SDL_WINDOWEVENT_EXPOSED)
			{
				redraw = 1;
			}
		}
	}
	
	return 1;
}

int show_message(int pl, int** map)
{
	char* text;
	switch (pl)
	{
		case 0:
			text = "Ничья";
			break;
		case 1:
			text = "Вы победили!";
			break;
		case 2:
			text = "Вы проиграли.";
			break;
		default:
			return 0;
	}
	
	SDL_Color color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	
	SDL_Surface* rendered;
	rendered = TTF_RenderUTF8_Solid(font, text, color);
	if (rendered == NULL)
	{
		printf("SDL TTF error: %s\n", TTF_GetError());
		return 0;
	}
	
	SDL_Rect rect;
	rect.w = rendered->w;
	rect.h = rendered->h;
	rect.x = (surf->w-rendered->w)/2;
	rect.y = (surf->h-rendered->h)/2;
	
	int cont = 1;
	int redraw = 1;
	while (cont)
	{
		if (redraw)
		{
			draw_ui(map);
			
			int a = SDL_BlitSurface(rendered, NULL, surf, &rect);
			if (a != 0)
			{
				printf("SDL error: %s\n", SDL_GetError());
				return 0;
			}
			
			SDL_UpdateWindowSurface(win);
			redraw = 0;
		}
		
		SDL_Event ev;
		
		SDL_WaitEvent(&ev);
		if (ev.type == SDL_MOUSEBUTTONUP ||
			ev.type == SDL_KEYUP ||
			ev.type == SDL_QUIT)
		{
			cont = 0;
			break;
		}
		else if (ev.type == SDL_WINDOWEVENT)
		{
			if (ev.window.event == SDL_WINDOWEVENT_EXPOSED)
			{
				redraw = 1;
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
	
	tmp = IMG_Load(path);
	if (tmp == NULL)
	{
		printf("SDL IMG error: %s\n", IMG_GetError());
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

