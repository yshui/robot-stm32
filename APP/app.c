#include "includes.h"

OS_STK task_led2_stk[TASK_LED2_STK_SIZE];		  //定义栈
OS_STK task_led3_stk[TASK_LED3_STK_SIZE];		  //定义栈

void Task_Start(void *p_arg)
{
    (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告

	OSTaskCreate(Task_LED2,(void *)0,		  	//创建任务2
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

	OSTaskCreate(Task_LED3,(void *)0,		   	//创建任务3
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);

    while (1)
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED1( OFF);   
		OSTimeDlyHMSM(0, 0,0,100);     
    }
}

//任务2
void Task_LED2(void *p_arg)
{
    (void)p_arg;                	
	
    while (1)
    {
        LED2( ON );
        OSTimeDlyHMSM(0, 0,0,200);
        LED2( OFF);
		OSTimeDlyHMSM(0, 0,0,200);	
    }
}

//任务3
void Task_LED3(void *p_arg)
{
    (void)p_arg;      
	
    while (1)
    {
        LED3( ON );
        OSTimeDlyHMSM(0, 0,0,300);
        LED3( OFF);
		OSTimeDlyHMSM(0, 0,0,300);        
    }
}
