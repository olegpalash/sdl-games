#include <stdlib.h>
#include <string.h>
#include "include/statemgr.h"

#define INIT_STACK_SIZE 16
#define INIT_STATES_NUM 16

static unsigned   states_number = 0;
static SM_State** states = NULL;

static unsigned     stack_size = 0;
static unsigned     stack_ptr = 0;
static SM_Instance* stack = NULL;

static int last_error = SM_NO_ERR;

int sm_init(void)
{
	states = calloc(INIT_STATES_NUM, sizeof(SM_State*));
	if (!states)
	{
		last_error = SM_ALLOC_ERR;
		return 0;
	}

	states_number = INIT_STATES_NUM;

	int i;
	for (i = 0; i < INIT_STATES_NUM; i++) states[i] = NULL;

	stack = calloc(INIT_STACK_SIZE, sizeof(SM_Instance));
	if (!stack)
	{
		last_error = SM_ALLOC_ERR;
		return 0;
	}

	stack_size = INIT_STACK_SIZE;

	return 1;
}

int	sm_quit(void)
{
	free(stack);
	stack = NULL;
	
	if (states)
	{
		int i = 0;
		for (i = 0; i < states_number; i++) free(states[i]);
		free(states);
		states = NULL;
	}	
	return 1;
}

SM_ID sm_registerstate(SM_State* st)
{
	if (st == NULL)
	{
		last_error = SM_INVALID_PTR_ERR;
		return 0;
	}
	
	SM_ID id;
	if (st->id != 0)
	{
		id = st->id;
	}
	else
	{
		int i;
		for (i = 0; i < states_number; i++)
		{
			if (states[i] == NULL) break;
		}
		id = i+1;
	}
	
	if (id > states_number)
	{
		size_t new_num = id*2;
		SM_State** new_ptr = calloc(new_num, sizeof(SM_State*));
		if (new_ptr == NULL)
		{
			last_error = SM_ALLOC_ERR;
			return 0;
		}
		
		memcpy(new_ptr, states, states_number*sizeof(SM_State*));
		
		free(states);
		
		states = new_ptr;
		states_number = new_num;
	}
	
	if (states[id-1] == NULL)
	{
		SM_State* new_st = malloc(sizeof(SM_State));
		if (!new_st)
		{
			last_error = SM_ALLOC_ERR;
			return 0;
		}
		
		states[id-1] = new_st;
	}
	
	memcpy(states[id-1], st, sizeof(SM_State));
	
	return id;
}

SM_State* sm_getstate(SM_ID id)
{
	if (id <= states_number)
	{
		return states[id-1];
	}
	else
	{
		last_error = SM_INVALID_ID_ERR;
		return NULL;
	}
}

int	sm_push(SM_ID id, void* data)
{
	if (stack_ptr < stack_size)
	{
		SM_State* ptr = sm_getstate(id);
		if (ptr != NULL)
		{
			if (ptr->init(data))
			{
				stack[stack_ptr].state = ptr;
				stack[stack_ptr].data = data;
				stack_ptr++;
				return 1;
			}
			else
			{
				last_error = SM_INIT_ERR;
				return 0;
			}
		}
		else
		{
			last_error = SM_INVALID_ID_ERR;
			return 0;
		}
	}
	else
	{
		last_error = SM_STACK_OVERFLOW_ERR;
		return 0;
	}
}

SM_State* sm_getcurrent(void)
{
	if (stack_ptr != 0)
	{
		return stack[stack_ptr-1].state;
	}
	else
	{
		last_error = SM_STACK_EMPTY_ERR;
		return NULL;
	}
}

int sm_pop(void)
{
	if (stack_ptr != 0)
	{
		stack_ptr--;
		SM_State* ptr = stack[stack_ptr].state;
		if (ptr->quit(stack[stack_ptr].data))
		{
			return 1;
		}
		else
		{
			last_error = SM_QUIT_ERR;
			return 0;
		}
	}
	else
	{
		last_error = SM_STACK_EMPTY_ERR;
		return 0;
	}
}

int sm_popall(void)
{
	if (stack_ptr != 0)
	{
		while (!sm_isempty()) sm_pop();
		return 1;
	}
	else
	{
		last_error = SM_STACK_EMPTY_ERR;
		return 0;
	}
}

int sm_popuntill(SM_ID id)
{
	if (stack_ptr != 0)
	{
		while (sm_getcurrent()->id != id &&	!sm_isempty())
		{
			sm_pop();
		}
		return 1;
	}
	else
	{
		last_error = SM_STACK_EMPTY_ERR;
		return 0;
	}
	
}

int sm_isempty(void)
{
	return stack_ptr == 0;
}

int	sm_run(void)
{
	while (!sm_isempty())
	{
		SM_State* ptr = stack[stack_ptr-1].state;
		if (!ptr->update(stack[stack_ptr-1].data))
		{
			last_error = SM_UPDATE_ERR;
			return 0;
		}
	}
	return 1;
}

