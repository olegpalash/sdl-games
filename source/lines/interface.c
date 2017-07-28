#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "func.h"

static SDL_Surface* balls[6]	= {NULL,NULL,NULL,NULL,NULL,NULL};
static SDL_Surface* background	= NULL;

static TTF_Font* font			= NULL;

static SDL_Window*  win			= NULL;
static SDL_Surface* surf		= NULL;

static SDL_Surface* load_sprite(const char*);
static int draw_frame(SDL_Rect*);

static int selected = 0;
static int sel_x, sel_y;

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
		"Lines",
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
	
	background = load_sprite("../share/game-lines/background.png");
	if (background == NULL) return 0;
	
	balls[0] = load_sprite("../share/game-lines/red.png");
	if (balls[0] == NULL) return 0;

	balls[1] = load_sprite("../share/game-lines/green.png");
	if (balls[1] == NULL) return 0;
	
	balls[2] = load_sprite("../share/game-lines/blue.png");
	if (balls[2] == NULL) return 0;
	
	balls[3] = load_sprite("../share/game-lines/yellow.png");
	if (balls[3] == NULL) return 0;
	
	balls[4] = load_sprite("../share/game-lines/magenta.png");
	if (balls[4] == NULL) return 0;
	
	balls[5] = load_sprite("../share/game-lines/cyan.png");
	if (balls[5] == NULL) return 0;

	return 1;
}

int draw_ui(int** map)
{
	int x, y;
	for (y = 0; y < 10; y++)
	{
		for (x = 0; x < 10; x++)
		{
			SDL_Rect rect;
			rect.x = x*surf->w/10;
			rect.y = y*surf->h/10;
			rect.w = surf->w/10;
			rect.h = surf->h/10;

			int a;
			a = SDL_BlitScaled(background, NULL, surf, &rect);
			if (a != 0)
			{
				printf("SDL error: %s\n", SDL_GetError());
				return 0;
			}
			
			if (map[y][x] != 0)
			{
				a = SDL_BlitScaled(balls[map[y][x]-1], NULL, surf, &rect);
				if (a != 0)
				{
					printf("SDL error: %s\n", SDL_GetError());
					return 0;
				}
			}
			
			if (selected && sel_x == x && sel_y == y)
			{
				draw_frame(&rect);
			}
		}
	}
	
	return 1;
}

void close_ui(void)
{
	if (background != NULL) SDL_FreeSurface(background);
	if (balls[0] != NULL) SDL_FreeSurface(balls[0]);
	if (balls[1] != NULL) SDL_FreeSurface(balls[1]);
	if (balls[2] != NULL) SDL_FreeSurface(balls[2]);
	if (balls[3] != NULL) SDL_FreeSurface(balls[3]);
	if (balls[4] != NULL) SDL_FreeSurface(balls[4]);
	if (balls[5] != NULL) SDL_FreeSurface(balls[5]);
	
	if (win != NULL) SDL_DestroyWindow(win);
	
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return;
}

int player_turn(int** map, int* quit)
{
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
		
		SDL_Event ev;
		
		SDL_WaitEvent(&ev);		
		if (ev.type == SDL_MOUSEBUTTONUP)
		{
			int x = ev.button.x/(surf->w/10);
			int y = ev.button.y/(surf->h/10);
			
			if (selected == 0)
			{
				if (map[y][x] != 0)
				{
					selected = 1;
					sel_x = x;
					sel_y = y;
					redraw = 1;
				}
			}
			else
			{
				if (map[y][x] == 0)
				{
					selected = 0;
					map[y][x] = map[sel_y][sel_x];
					map[sel_y][sel_x] = 0;
					cont = 0;
				}
				else
				{
					sel_x = x;
					sel_y = y;
					redraw = 1;
				}
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

int show_message(int score, int** map)
{
	char text[128];
	
	sprintf(text, "Ваш счет: %i", score);

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	
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

static int draw_frame(SDL_Rect* rect)
{
	Uint32 color = SDL_MapRGB(surf->format, 255, 255, 255);
	DrawHLine(surf, color, rect->x, rect->y, rect->w);
	DrawHLine(surf, color, rect->x, rect->y+rect->h-1, rect->w);
	DrawVLine(surf, color, rect->x, rect->y+1, rect->h-2);
	DrawVLine(surf, color, rect->x+rect->w-1, rect->y+1, rect->h-2);
}

