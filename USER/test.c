#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "includes.h"
#include "io.h"
//ALIENTEK战舰STM32开发板实验53
//UCOSII实验1-任务调度  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  	 


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			6 
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

int main(void)
{		 		  
	Stm32_Clock_Init(9); //系统时钟设置
	uart_init(72,115200);
	delay_init(72);	     //延时初始化
	PERIVERAL_POWER_CTRL(ON);	//开启外围电路电源
	OSInit(); 
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	  	 
}


	  
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

//LED0任务
void led0_task(void *pdata)
{	
	while(1)
	{
		LED_RED_CTRL(ON);
		delay_ms(80);
		LED_RED_CTRL(OFF);
		delay_ms(920);
		printf("\r\nTestOK!");
	};
}

//LED1任务
void led1_task(void *pdata)
{	  
	while(1)
	{
	    LED_GREEN_CTRL(ON);
		delay_ms(300);
		LED_GREEN_CTRL(OFF);
		delay_ms(300);
	};
}




















