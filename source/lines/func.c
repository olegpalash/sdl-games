#include <SDL2/SDL.h>

static int SetPointNLock(SDL_Surface*, Uint32, int, int);

int SetPoint(
	SDL_Surface* surf,
	Uint32		 color,
	int			 x,
	int			 y)
{
	SDL_LockSurface(surf);
	SetPointNLock(surf, color, x, y);
	SDL_UnlockSurface(surf);
	return 0;
}

int DrawHLine(
	SDL_Surface* surf,
	Uint32 color,
	int x,
	int y,
	int len)
{
	SDL_LockSurface(surf);
	
	int i;
	for (i = 0; i < len; i++)
	{
		SetPointNLock(surf, color, x+i, y);
	}

	SDL_UnlockSurface(surf);
	return 0;
}

int DrawVLine(
	SDL_Surface* surf,
	Uint32 color,
	int x,
	int y,
	int len)
{
	SDL_LockSurface(surf);
	
	int i;
	for (i = 0; i < len; i++)
	{
		SetPointNLock(surf, color, x, y+i);
	}

	SDL_UnlockSurface(surf);
	return 0;
}

static int SetPointNLock(
	SDL_Surface* surf,
	Uint32		 color,
	int			 x,
	int			 y)
{
	Uint8* data = surf->pixels;
	Uint8* ptr = data+surf->pitch*y+surf->format->BytesPerPixel*x;
	
	if (surf->format->BytesPerPixel == 1)
	{
		*ptr = (Uint8) color;
	}
	else if (surf->format->BytesPerPixel == 2)
	{
		*((Uint16*) ptr) = (Uint16) color;
	}
	else if (surf->format->BytesPerPixel == 3)
	{
		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			*ptr = *((Uint8*) &color);
			*(ptr+1) = *(((Uint8*) &color)+1);
			*(ptr+2) = *(((Uint8*) &color)+2);
		}
		else
		{
			*ptr = *(((Uint8*) &color)+1);
			*(ptr+1) = *(((Uint8*) &color)+2);
			*(ptr+2) = *(((Uint8*) &color)+3);
		}
	}
	else if (surf->format->BytesPerPixel == 4)
	{
		*((Uint32*) ptr) = color;
	}
	
	return 0;
}

