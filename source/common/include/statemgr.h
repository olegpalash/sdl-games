#ifndef _STATEMGR_H_
#define _STATEMGR_H_

enum SM_Error
{
	SM_NO_ERR,
	SM_ALLOC_ERR,
	SM_INVALID_PTR_ERR,
	SM_INVALID_ID_ERR,
	SM_STACK_OVERFLOW_ERR,
	SM_STACK_EMPTY_ERR,
	SM_INIT_ERR,
	SM_UPDATE_ERR,
	SM_QUIT_ERR
};

typedef unsigned SM_ID;

typedef struct _SM_state
{
	SM_ID id;
	int (*init)(void*);
	int (*update)(void*);
	int (*quit)(void*);
} SM_State;

typedef struct _SM_Instance
{
	SM_State* state;
	void*     data;
} SM_Instance;

int		sm_init(void);
int		sm_quit(void);

SM_ID		sm_registerstate(SM_State*);
SM_State*	sm_getstate(SM_ID);

int		sm_push(SM_ID, void*);
SM_State* sm_getcurrent(void);
int     sm_pop(void);
int     sm_popall(void);
int     sm_popuntill(SM_ID);
int		sm_isempty(void);

int		sm_run(void);

#endif

