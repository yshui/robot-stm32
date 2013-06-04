#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "includes.h"
#include "io.h"
//ALIENTEKս��STM32������ʵ��53
//UCOSIIʵ��1-�������  
//����֧�֣�www.openedv.com
//�������������ӿƼ����޹�˾  	 


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			7 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);


//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			6 
//���������ջ��С
#define LED1_STK_SIZE  					64
//�����ջ
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

int main(void)
{		 		  
	Stm32_Clock_Init(9); //ϵͳʱ������
	uart_init(72,115200);
	delay_init(72);	     //��ʱ��ʼ��
	ROBOT_INIT();
	OSInit(); 
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	  	 
}


	  
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

//LED0����
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

//LED1����
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



















