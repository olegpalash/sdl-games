#include "map.h"

typedef struct
{
	int i;
	int j;
	int di;
	int dj;
} pass;

typedef struct
{
	int pl;
	int ai;
	int sum;
} line;

static pass passes[8] = 
{
	{0, 0, 1, 1},	// диагонали
	{0, 2, 1, -1},
	{0, 0, 0, 1},	// по горизонтали
	{1, 0, 0, 1},
	{2, 0, 0, 1},
	{0, 0, 1, 0},	// по вертикали
	{0, 1, 1, 0},
	{0, 2, 1, 0}
};

/*
	возвращаемое значение:
		0 -  нет победителя
		-1 - ничья
		1 -  игрок (X)
		2 -  ИИ (O)
*/
int get_winner(int** map)
{
	int n;
	line lines[8];
	
	for (n = 0; n < 8; n++)
	{
		lines[n].pl = 0;
		lines[n].ai = 0;
		
		int i = passes[n].i;
		int j = passes[n].j;
		int m;		
		
		for (m = 0; m < 3; m++)
		{
			if (map[i][j] == MAP_ZERO)
			{
				lines[n].ai++;
			}
			else if (map[i][j] == MAP_CROSS)
			{
				lines[n].pl++;
			}
						
			i += passes[n].di;
			j += passes[n].dj;
		}
		
		lines[n].sum = lines[n].pl+lines[n].ai;
	}
	
	int ret = 0;
	int fill = 1;
	for (n = 0; n < 8; n++)
	{
		if (lines[n].ai == 3) ret = 2;
		else if (lines[n].pl == 3) ret = 1;
		else if (lines[n].sum != 3) fill = 0; 
	}
	
	if (fill && ret == 0) ret = -1;
		
	return ret;
}

