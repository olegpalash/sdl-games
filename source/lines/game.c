#include <stdlib.h>
#include <SDL2/SDL.h>

int get_lines(int** map)
{
	int x, y;
	int inc_score = 0;	
	
	for (y = 0; y < 10; y++)
	{
		for (x = 0; x < 10; x++)
		{
			if (map[y][x] != 0)
			{
				int found = 0;
				int dx, dy;
				
				if (map[y][x] == map[y][x+1])
				{
					dx = 1;
					dy = 0;
					found = 1;
				}
				else if (map[y][x] == map[y+1][x])
				{
					dx = 0;
					dy = 1;
					found = 1;
				}
				else if (map[y][x] == map[y+1][x+1])
				{
					dx = 1;
					dy = 1;
					found = 1;
				}
				
				if (found)
				{
					int st_x = x;
					int st_y = y;
					int cx = x+2*dx;
					int cy = y+2*dy;
					int len = 2;
					
					while (map[cy][cx] == map[st_y][st_x])
					{
						cx += dx;
						cy += dy;
						len++;	
					}
					
					if (len >= 4)
					{
						int i;
						cx = st_x;
						cy = st_y;

						for (i = 0; i < len; i++)
						{
							map[cy][cx] = 0;
							cx += dx;
							cy += dy;
						}
						
						inc_score += len;
					}
				}
			}
		}
	}
	
	return inc_score;
}

int add_balls(int** map, int cnt)
{
	SDL_Point empty[100];
	int num = 0;
	int x, y;
	
	for (y = 0; y < 10; y++)
	{
		for (x = 0; x < 10; x++)
		{
			if (map[y][x] == 0)
			{
				empty[num].x = x;
				empty[num].y = y;
				num++;
			}
		}
	}
	
	if (num > cnt)
	{
		int i;
		for (i = 0; i < cnt; i++)
		{
			int j = rand()%num;
			int ball = rand()%6+1;
			
			map[empty[j].y][empty[j].x] = ball;
			
			empty[j].x = empty[num-1].x;
			empty[j].y = empty[num-1].y;
			num--;
		}
		
		return num;
	}
	else return 0;
}

