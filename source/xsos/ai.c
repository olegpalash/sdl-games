#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "game.h"

#define AI_NUMBER 2
#define SEARCH_DEPTH 10

typedef struct
{
	int i;
	int j;
	int di;
	int dj;
} pass;

typedef struct
{
	int i;
	int j;
} point;

struct node
{
	struct node* parent;
	point cell;
	int turn;
	float value;
	int level;
	int** map;
	int childs_num;
	struct node** childs;
};

typedef struct node* pnode;

/*static pass passes[8] = 
{
	{0, 0, 1, 1},	// диагонали
	{0, 2, 1, -1},
	{0, 0, 0, 1},	// по горизонтали
	{1, 0, 0, 1},
	{2, 0, 0, 1},
	{0, 0, 1, 0},	// по вертикали
	{0, 1, 1, 0},
	{0, 2, 1, 0}
};*/

static pnode alloc_node(void);
static int make_child(pnode, int);
static float eval_node(pnode);
static void free_node(pnode);

static int** alloc_map(void);
static void free_map(int**);
static void copy_map(const int**, int**);

int ai_turn(int** map)
{
	pnode root = alloc_node();
	root->parent = NULL;
	root->turn = (AI_NUMBER == 2) ? 1 : 2;
	root->level = 0;
	root->map = alloc_map();
	copy_map((const int**) map, root->map);
	make_child(root, SEARCH_DEPTH);
	if (root->childs_num == 0) return 1;
	
	eval_node(root);
	
	int i;
	int max = 0;
	for (i = 0; i < root->childs_num; i++)
	{
		if (root->childs[i]->value > root->childs[max]->value) max = i;
		/*printf(
			"%d: (%d, %d) = %f\n",
			i,
			root->childs[i]->cell.i,
			root->childs[i]->cell.j,
			root->childs[i]->value);*/
	}
	
	//printf("---------- %d ----------\n", max);
	
	int j;
	i = root->childs[max]->cell.i;
	j = root->childs[max]->cell.j;
	
	map[i][j] = AI_NUMBER;
	free_node(root);
	return 1;
}

static pnode alloc_node(void)
{
	pnode ret = (pnode) malloc(sizeof(struct node));
	if (ret == NULL) return NULL;

	ret->parent = NULL;
	ret->cell.i = -1;
	ret->cell.j = -1;
	ret->turn = 0;
	ret->value = 0.0;
	ret->level = 0;
	ret->map = NULL;
	ret->childs_num = 0;
	ret->childs = NULL;

	return ret;
}

static int make_child(
	pnode nd,
	int max_lev)
{
	if (nd->level >= max_lev) return 1;
	if (get_winner(nd->map) != -1) return 1;
		
	int i, j;
	point pts[9];
	int n = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (nd->map[i][j] == 0)
			{
				pts[n].i = i;
				pts[n].j = j;
				n++;
			}			
		}
	}
	if (n == 0) return 1;
		
	nd->childs_num = n;
	nd->childs = calloc(n, sizeof(pnode));
	for (i = 0; i < n; i++)
	{
		pnode child = alloc_node();
		nd->childs[i] = child;
		
		child->parent = nd;
		child->cell.i = pts[i].i;
		child->cell.j = pts[i].j;
		child->turn = (nd->turn == 1) ? 2 : 1;
		child->level = nd->level+1;
		child->map = alloc_map();
		copy_map((const int**) nd->map, child->map);
		child->map[pts[i].i][pts[i].j] = child->turn;
		make_child(child, max_lev);
	}
	
	return 1;
}

static float eval_node(
	pnode nd)
{
	int winner = get_winner(nd->map);
	if (winner != -1)
	{
		if (winner == 0) nd->value = 0.0;
		else if (winner == AI_NUMBER) nd->value = 1.0;
		else nd->value = -1.0;
	}
	else
	{
		int i;
		float sum = 0.0;
		for (i = 0; i < nd->childs_num; i++)
		{
			sum += eval_node(nd->childs[i]);
		}
		nd->value = sum/nd->childs_num;
	}
	return nd->value;
}

static void free_node(
	pnode nd)
{
	int i;
	for (i = 0; i < nd->childs_num; i++)
	{
		free_node(nd->childs[i]);
	}

	free_map(nd->map);
	free(nd);
}

static int** alloc_map(void)
{
	int** ret = calloc(3, sizeof(int*));
	
	int i, j;
	for (i = 0; i < 3; i++)
	{
		ret[i] = calloc(3, sizeof(int));
		for (j = 0; j < 3; j++) ret[i][j] = 0;
	}
	
	return ret;
}

static void free_map(
	int** map)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		free(map[i]);
	}
	
	free(map);
	return;
}

static void copy_map(
	const int** src,
	int** dst)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++) dst[i][j] = src[i][j];
	}
	return;
}

