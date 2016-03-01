#include timerhub.h

static uint8_t pTask = 0;
static uint8_t taskActivate[MAX_TASK];
static uint16_t runTimer[MAX_TASK];
static void (*pfunc_task[MAX_TASK])(void);


/*================================   TimerCreate   ============================
**    Register a function that is called when the specified time has elapsed.
**    RET Timer handle
**    IN  Timeout value (for Schedule tick)
**    IN  Timeout function address  
**--------------------------------------------------------------------------*/
int8_t TimerCreate(uint16_t timerTicks, void(*pfunc)())
{
	runTimer[pTask] = timerTicks;  //interval time
	pfunc_task[pTask] = pfunc;     //new task func point
	taskActivate[pTask] = 1;

	if(++pTask <= MAX_TASK)        //task handle
		return (pTask-1);
	else
		return -1;
}

/*==========================   TimerSuspend   ============================
**  Suspend the Timer by handle
**-----------------------------------------------------------------------*/
void TimerSuspend(uint8_t handle)
{
	taskActivate[handle] = 0;
}

/*==========================   TimerResume   ============================
**  Resume the Timer by handle
**-----------------------------------------------------------------------*/
void TimerResume(uint8_t handle)
{
	taskActivate[handle] = 1;
}

/*==========================   TimerTickPoll   ============================
**  looping execution this function in the hardwave timer
**-----------------------------------------------------------------------*/
void TimerTickPoll()
{
	uint8_t i;
	static uint32_t tick;
	tick++;

	for(i=0; i<pTask; i++)
	{
		if(tick % runTimer[i] == 0)	//check timeout
		{
			if(taskActivate[i] == 1)
				(*pfunc_task[i])();		//run the task
		}
	}
}
/*---------------------------------------------------------------------------*/