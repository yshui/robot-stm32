//#include "sys.h"
//#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
//#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
//#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
u16 USART_TEST_DATA=0;         	//输出的测试数据
u32 USART_TEST_ODRL=0;         	//输出的测试数据
u32 USART_TEST_ODRH=0;         	//输出的测试数据
u8  USART_TEST_CMD='0';         	//端口选择
u8  PERIVAERAL_POWER=OFF;         	//外围电路电源开关

void IOTest(u8 newcmd)
{
	if(newcmd==USART_TEST_CMD)
	{ 
		USART_TEST_DATA=USART_TEST_DATA<<1;
		switch(USART_TEST_DATA)
		{
		case 0x1:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X3;	break;
		case 0x2:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X30;	break;
		case 0x4:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X300;	break;
		case 0x8:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X3000;	break;
		case 0x10:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X30000;	break;
		case 0x20:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X300000;	break;
		case 0x40:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X3000000;	break;
		case 0x80:		USART_TEST_ODRH=0X00000000;USART_TEST_ODRL=0X30000000;	break;

		case 0x100:		USART_TEST_ODRH=0X3;USART_TEST_ODRL=0X00000000;	break;
		case 0x200:		USART_TEST_ODRH=0X30;USART_TEST_ODRL=0X00000000;	break;
		case 0x400:		USART_TEST_ODRH=0X300;USART_TEST_ODRL=0X00000000;	break;
		case 0x800:		USART_TEST_ODRH=0X3000;USART_TEST_ODRL=0X00000000;	break;
		case 0x1000:	USART_TEST_ODRH=0X30000;USART_TEST_ODRL=0X00000000;	break;
		case 0x2000:	USART_TEST_ODRH=0X300000;USART_TEST_ODRL=0X00000000;	break;
		case 0x4000:	USART_TEST_ODRH=0X3000000;USART_TEST_ODRL=0X00000000;	break;
		case 0x8000:	USART_TEST_ODRH=0X30000000;USART_TEST_ODRL=0X00000000;	break;
		}
	}	
	else
	{
	   SetIOAsInput();
	   USART_TEST_CMD = newcmd;
	   USART_TEST_DATA= 0x0001;
	}

	switch(newcmd)
	{
	case  '0':    //'0'		//NO OPERATION
		SetIOAsInput();	
		break;
	case  '1':    //'1'
		RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
		GPIOA->CRH=USART_TEST_ODRH;   	 
		GPIOA->CRL=USART_TEST_ODRL;  	 
	    GPIOA->ODR=USART_TEST_DATA;
		break;
	case  '2':    //'2'
		RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
		GPIOB->CRH=USART_TEST_ODRH;   	 
		GPIOB->CRL=USART_TEST_ODRL;  	 
	    GPIOB->ODR=USART_TEST_DATA;
		break;
	case  '3':   //'3'
		RCC->APB2ENR|=1<<4;    //使能PORTC时钟	   	 
		GPIOC->CRH=USART_TEST_ODRH;   	 
		GPIOC->CRL=USART_TEST_ODRL;  	 
	    GPIOC->ODR=USART_TEST_DATA;
		break;
	case  '4':   //'4'
		RCC->APB2ENR|=1<<5;    //使能PORTD时钟	   	 
		GPIOD->CRH=USART_TEST_ODRH;   	 
		GPIOD->CRL=USART_TEST_ODRL;  	 
	    GPIOD->ODR=USART_TEST_DATA;
		break;
	case  '5':      //'5'
		RCC->APB2ENR|=1<<6;    //使能PORTE时钟	
		GPIOE->CRH=USART_TEST_ODRH;   	 
		GPIOE->CRL=USART_TEST_ODRL;  	 
	    GPIOE->ODR=USART_TEST_DATA;
		break;
	case  '6':      //'6'
		GPIOA->CRH=USART_TEST_ODRH;   	 
		GPIOA->CRL=USART_TEST_ODRL;  	 
	    GPIOA->ODR=~USART_TEST_DATA;
		break;
	case  '7':     //'7'
		GPIOB->CRH=USART_TEST_ODRH;   	 
		GPIOB->CRL=USART_TEST_ODRL;  	 
	    GPIOB->ODR=~USART_TEST_DATA;
		break;
	case  '8':   //'8'
		GPIOC->CRH=USART_TEST_ODRH;   	 
		GPIOC->CRL=USART_TEST_ODRL;  	 
	    GPIOC->ODR=~USART_TEST_DATA;
		break;
	case  '9':   //'9'
		GPIOD->CRH=USART_TEST_ODRH;   	 
		GPIOD->CRL=USART_TEST_ODRL;  	 
	    GPIOD->ODR=~USART_TEST_DATA;
		break;
	case  'a':      //'A'
		GPIOE->CRH=USART_TEST_ODRH;   	 
		GPIOE->CRL=USART_TEST_ODRL;  	 
	    GPIOE->ODR=~USART_TEST_DATA;
		break;
	default:
		USART_TEST_CMD = '0';	
		break;
	}

	printf("%c  %c  0x%x\r\n", newcmd,USART_TEST_CMD,USART_TEST_DATA);
	if(USART_TEST_DATA == 0x80000000)	USART_TEST_DATA=0x0001;	 
}

void SetIOAsInput(void)
{
	USART_TEST_DATA=0;	
	switch(USART_TEST_CMD)
	{
	case  '1':      //'1'		//PROGRAM FLASH
		GPIOA->CRH=USART_TEST_DATA;   	 
		GPIOA->CRL=USART_TEST_DATA;   	 
	    GPIOA->ODR=USART_TEST_DATA;
		break;
	case  '2':     //'2'		//ERASE SRAM
		GPIOB->CRH=USART_TEST_DATA;   	 
		GPIOB->CRL=USART_TEST_DATA;   	 
	    GPIOB->ODR=USART_TEST_DATA;
		break;
	case  '3':   //'3'		//RUN FLASH APP
		GPIOC->CRH=USART_TEST_DATA;   	 
		GPIOC->CRL|=USART_TEST_DATA;   	 
	    GPIOC->ODR=USART_TEST_DATA;
		break;
	case  '4':   //'4'		//RUN SRAM APP
		GPIOD->CRH=USART_TEST_DATA;   	 
		GPIOD->CRL=USART_TEST_DATA;   	 
	    GPIOD->ODR=USART_TEST_DATA;
		break;
	case  '5':      //'0'		//NO OPERATION
		GPIOE->CRH=USART_TEST_DATA;   	 
		GPIOE->CRL=USART_TEST_DATA;   	 
	    GPIOE->ODR=USART_TEST_DATA;
		break;
	default:
		break;
	}
}
  
void USART1_IRQHandler(void)
{
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 
		//IOTest(res);
		RobotFuncTest(res);
		uart_showtittle();
	}
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif										 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	//MY_NVIC_Init(3,3,USART1_IRQChannel,2);//组2，最低优先级 
#endif
}

void uart_showtittle(void)
{
    printf("\r\nIO Test Main Menu!!!!!!!!!!:");
    printf("\r\n----TOGGLE POWER--------- 0");
    printf("\r\n----RED   LED ON--------- 1");
    printf("\r\n----GREEN LED ON--------- 2");
    printf("\r\n----ALL   LED ON--------- 3");
    printf("\r\n----ALL   LED OFF-------- 4");
    printf("\r\n----RIGHT BRUSH ON------- 5");
    printf("\r\n----RIGHT BRUSH OFF------ 6");
    printf("\r\n----LEFT  BRUSH ON------- 7");
    printf("\r\n----LEFT  BRUSH OFF------ 8");
    printf("\r\n----RIGHT WHEEL FORWARD-- 9");
    printf("\r\n----RIGHT WHEEL BACKWARD- A");
    printf("\r\n----RIGHT WHEEL OFF------ B");
    printf("\r\n----LEFT WHEEL FORWARD--- C");
    printf("\r\n----LEFT WHEEL BACKWARD-- D");
    printf("\r\n----LEFT WHEEL OFF------- E");
    printf("\r\n----FRONT PRESS --------- F");
    printf("\r\n----FRONT  SENSOR-------- G");
    printf("\r\n----BOTTOM SENSOR-------- H");
    printf("\r\n----TOP SENSOR----------- I");
    printf("\r\n----KEY------------------ J");
    printf("\r\n----BUZ ON--------------- K");
    printf("\r\n----BUZ OFF-------------- L");
    printf("\r\n----FAN ON--------------- M");
    printf("\r\n----FAN OFF-------------- N");
    printf("\r\n----CHARGE STATUS-------- O");
    printf("\r\n----RESIST STATUS-------- P");
    printf("\r\n\r\n");
}


void RobotFuncTest(u8 newcmd)
{
	switch(newcmd)
	{
	case  '0':    //----TOGGLE POWER--------- '0'
		PERIVERAL_POWER_CTRL(PERIVAERAL_POWER);
		break;
	case  '1':    //----RED   LED ON-------'1'
		LED_RED_CTRL(ON);
		break;
	case  '2':    //----GREEN LED ON-------'2'
		LED_GREEN_CTRL(ON);
		break;
	case  '3':    //----ALL   LED ON-------'3'
		LED_RED_CTRL(ON);
		LED_GREEN_CTRL(ON);
		break;
	case  '4':    //----ALL   LED OFF------'4'
		LED_RED_CTRL(OFF);
		LED_GREEN_CTRL(OFF);
		break;
	case  '5':    //----RIGHT BRUSH ON-----'5'
		Right_Brush_CTRL(ON);
		break;
	case  '6':    //----RIGHT BRUSH OFF----'6'
		Right_Brush_CTRL(OFF);
		break;
	case  '7':    //----LEFT  BRUSH ON-----'7'
		Left_Brush_CTRL(ON);
		break;
	case  '8':    //----LEFT  BRUSH OFF----'8'
		Left_Brush_CTRL(OFF);
		break;
	case  '9':    //----RIGHT WHEEL FORWARD-----'9'
		Right_Wheel_CTRL(FORWARD);
		break;
	case  'A':    //----RIGHT WHEEL BACKWARD----'A'
		Right_Wheel_CTRL(BACKWARD);
		break;
	case  'B':    //----RIGHT  WHEEL OFF--------'B'
		Right_Wheel_CTRL(OFF);
		break;
	case  'C':    //----LEFT WHEEL FORWARD------'C'
		Left_Wheel_CTRL(FORWARD);
		break;
	case  'D':    //----LEFT WHEEL BACKWARD-----'D'
		Left_Wheel_CTRL(BACKWARD);
		break;
	case  'E':    //----LEFT  WHEEL OFF---------'E'
		Left_Wheel_CTRL(OFF);
		break;
	case  'F':    //----FRONT PRESS -------'F'
		break;
	case  'G':    //----FRONT  SENSOR------'G'
		break;
	case  'H':    //----BOTTOM SENSOR------'H'
		break;
	case  'I':    //----TOP SENSOR---------'I'
		TOP_SENSOR_Input();
		break;
	case  'J':    //----KEY----------------'J'
		KEY_Input();
		break;
	case  'K':    //----BUZ ON---------------'K'
		BUZ_CTRL(ON);
		break;
	case  'L':    //----BUZ OFF--------------'L'
		BUZ_CTRL(OFF);
		break;
	case  'M':    //----FAN ON---------------'M'
		FAN_CTRL(ON);
		break;
	case  'N':    //----FAN OFF--------------'N'
		FAN_CTRL(OFF);
		break;
	case  'O':    //----CHARGE STATUS---------'O'
		printf("CHARGE status: %d", CHARGE_STATUS_Input());   	//PC.1);
		break;
	case  'P':    //----RESIST STATUS---------'P'
		RESIST_STATUS_Input();
		break;
	default:
		USART_TEST_CMD = '0';	
		break;
	}
}

void PERIVERAL_POWER_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	GPIOA->CRL&=0XF0FFFFFF; 
	GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关 
	if(onoff==OFF)
	{
	    GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    PERIVAERAL_POWER = ON;
	} 
	else if(onoff==ON)
	{
	    GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	   PERIVAERAL_POWER = OFF;
	} 
	else
	{
	    GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	   PERIVAERAL_POWER = OFF;
	} 
  printf("\r\nPERIVAERAL POWER--------- %s", (PERIVAERAL_POWER==ON)? "ON" :  "OFF");
}

void LED_RED_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	GPIOC->CRH&=0XFFFFF0FF; 
	GPIOC->CRH|=0X00000300; //PC.10 推挽输出   	 
	if(onoff==ON)
	{
	    GPIOC->ODR |= 1<<10;      //PC.10 输出高
	} 
	else if(onoff==OFF)
	{
	    GPIOC->ODR &= ~(1<<10);      //PC.10 输出高
	} 
}

void LED_GREEN_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	GPIOC->CRH&=0XFFFF0FFF; 
	GPIOC->CRH|=0X00003000; //PC.10 推挽输出   	 
	if(onoff==ON)
	{
	    GPIOC->ODR |= 1<<11;      //PC.11 输出高
	} 
	else if(onoff==OFF)
	{
	    GPIOC->ODR &= ~(1<<11);      //PC.11 输出高
	} 
}

void Right_Brush_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟	   	 
	GPIOB->CRH&=0XF0FFFFFF; 
	GPIOB->CRH|=0X03000000; //PB.14 推挽输出   	 
	if(onoff==ON)
	{
	    GPIOB->ODR |= 1<<14;      //PB.14 输出高
	} 
	else if(onoff==OFF)
	{
	    GPIOB->ODR &= ~(1<<14);      //PB.14 输出高
	} 
}

void Left_Brush_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	//GPIOA->CRL&=0XF0FFFFFF; 
	//GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关   	 
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003; //PA.8 推挽输出  电开关   	 
	//GPIOA->CRH&=0XFFFF0FFF; 
	//GPIOA->CRH|=0X00003000; //PA.11 推挽输出   	 
	GPIOC->CRH&=0XFFFFFFF0; 
	GPIOC->CRH|=0X00000003; //PC.8 推挽输出   	 
	if(onoff==ON)
	{
	    //GPIOA->ODR |= 1<<11;      //PA.11 输出高
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOA->ODR |= 1<<8;      //PA.8 输出高
	    GPIOC->ODR |= 1<<8;      //PC.8 输出高
	} 
	else if(onoff==OFF)
	{
	    //GPIOA->ODR &= ~(1<<11);      //PA.11 输出高
	    //GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	    GPIOA->ODR &= ~(1<<8);      //PA.8 输出高
	    GPIOC->ODR &= ~(1<<8);      //PC.8 输出高
	} 
}

void Right_Wheel_CTRL(u8 onoff)
{
	//RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	//GPIOA->CRL&=0XF0FFFFFF; 
	//GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关   	 
	GPIOB->CRL&=0XF0FFFFFF; 
	GPIOB->CRL|=0X03000000; //PB.6 推挽输出   	 
	GPIOC->CRL&=0Xf0FFFFFF; 
	GPIOC->CRL|=0X03000000; //PC.6 7 8 9 推挽输出   	 
	//GPIOC->CRH&=0XFFFFFF00; 
	//GPIOC->CRH|=0X00000033; //PC.6 7 8 9 推挽输出   	 
	if(onoff==FORWARD)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    //GPIOB->ODR |= 1<<6;      //PB.6 输出高
	    //GPIOC->ODR |= 1<<6;      //PC.6 输出高
	    GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<6);      //PC.6 输出高
	    //GPIOC->ODR |= 1<<7;      //PC.7 输出高
	    //GPIOC->ODR |= 1<<9;      //PC.9 输出高
	} 
	else if(onoff==BACKWARD)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOB->ODR |= 1<<6;      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<6);      //PC.6 输出高
	    //GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    //GPIOC->ODR &= ~(1<<6);      //PC.6 输出高
	    //GPIOC->ODR |= 1<<7;      //PC.7 输出高
	    //GPIOC->ODR |= 1<<9;      //PC.9 输出高
	} 
	else if(onoff==OFF)
	{
	    //GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	    GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<6);      //PC.6 输出高
	    //GPIOC->ODR &= ~(1<<7);      //PC.7 输出高
	    //GPIOC->ODR &= ~(1<<9);      //PC.9 输出高
	} 
}

void Left_Wheel_CTRL(u8 onoff)
{
	//RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	//GPIOA->CRL&=0XF0FFFFFF; 
	//GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关   	 
	//GPIOA->CRH&=0XFFFFFFF0; 
	//GPIOA->CRH|=0X00000003; //PA.8 推挽输出  电开关   	 
	GPIOB->CRL&=0XF0FFFFFF; 
	GPIOB->CRL|=0X03000000; //PB.6 推挽输出   	 
	//GPIOC->CRL&=0X00FFFFFF; 
	//GPIOC->CRL|=0X33000000; //PC.6 7 8 9 推挽输出   	 
	GPIOC->CRH&=0XFFFFFFF0; 
	GPIOC->CRH|=0X00000003; //PC.8 推挽输出   	 
	if(onoff==FORWARD)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOB->ODR |= 1<<6;      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<8);      //PC.8 输出高
	    //GPIOA->ODR |= 1<<8;      //PA.8 输出高
	    //GPIOB->ODR |= 1<<6;      //PB.6 输出高
	    //GPIOC->ODR |= 1<<8;      //PC.8 输出高
	    //GPIOC->ODR |= 1<<7;      //PC.7 输出高
	    //GPIOC->ODR |= 1<<9;      //PC.9 输出高
	printf("LEFT WHEEL FORWARD");   	//PE1);
	} 
	else if(onoff==BACKWARD)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<8);      //PC.8 输出高
	    //GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    //GPIOC->ODR &= ~(1<<8);      //PC.8 输出高
	    //GPIOC->ODR |= 1<<7;      //PC.7 输出高
	    //GPIOC->ODR |= 1<<9;      //PC.9 输出高
	printf("LEFT WHEEL BACKWARD");   	//PE1);
	} 
	else if(onoff==OFF)
	{
	    //GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	    GPIOB->ODR &= ~(1<<6);      //PB.6 输出高
	    GPIOC->ODR &= ~(1<<8);      //PC.8 输出高
	    //GPIOC->ODR &= ~(1<<7);      //PC.7 输出高
	    //GPIOC->ODR &= ~(1<<9);      //PC.9 输出高
	printf("LEFT WHEEL OFF");   	//PE1);
	} 
}



void TOP_SENSOR_Input(void)
{
	RCC->APB2ENR|=1<<6;     //使能PORTE时钟
	GPIOE->CRL&=0XFFFFFFF0;	//PE0设置成输入，默认上拉	  
	GPIOE->CRL|=0X00000008; 
	GPIOE->ODR|=1;	   	//PE0 上拉
	printf("TOP SENSOR status: %ld", PEin(0));   	//PE0);
} 


void KEY_Input(void)
{
	RCC->APB2ENR|=1<<6;     //使能PORTE时钟
	GPIOE->CRL&=0XFFFFFF0F;	//PE1设置成输入，默认下拉	  
	GPIOE->CRL|=0X00000080; 
	GPIOE->ODR|=2;	   	//PE1 上拉
	  
	//GPIOE->CRL&=0XFFF000FF;	//PE2~4设置成输入	  
	//GPIOE->CRL|=0X00088800; 				   
	//GPIOE->ODR|=7<<2;	   	//PE2~4 上拉
	printf("key status: %ld", PEin(1));   	//PE1);
} 

u8 CHARGE_STATUS_Input(void)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	GPIOC->CRL&=0XFFFFFF0F; 
	GPIOC->CRL|=0X00000080; //PC.1 设置成输入，默认下拉   	 
	GPIOC->ODR|=2;	   	//PC.1 上拉
	return 	PCin(1);
} 

void RESIST_STATUS_Input(void)
{
	//PA  0 2 3 4 5 7 
	//PB  1 5 13
	//PC  4
	//PD 1 2 3 10 12 13 14
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	RCC->APB2ENR|=1<<5;     //使能PORTD时钟
	RCC->APB2ENR|=1<<6;     //使能PORTE时钟

	GPIOA->CRL&=0X0F0000F0; 
	GPIOA->CRL|=0X80888808;    	 
	//GPIOA->ODR|=0X43;	   	//上拉

	GPIOB->CRL&=0XFF0FFFF0; 
	GPIOB->CRL|=0X00800008;    	 
	GPIOB->CRH&=0XFF0FFFFF; 
	GPIOB->CRH|=0X00800000;    	 
		   	 
	GPIOC->CRL&=0XFFF0FFFF; 
	GPIOC->CRL|=0X00080000; //PC.4 设置成输入，默认下拉   	 
	GPIOC->ODR|=0X10;	   	//PC.4 上拉

	GPIOD->CRL&=0XFFFFF000; 
	GPIOD->CRL|=0X00000888;    	 
	GPIOD->CRH&=0XF000F0FF; 
	GPIOD->CRH|=0X08880800; 
	 
	GPIOE->CRL&=0XFFFFFFF0;	//PE0设置成输入，默认上拉	  
	GPIOE->CRL|=0X00000008; 
	GPIOE->ODR|=1;	   	//PE0 上拉

	printf("RESIST STATUS: \r\n");   	//PE1);
	printf("PTA: 0(%ld) 2(%ld) 3(%ld) 4(%ld) 5(%ld) 7(%ld)\r\n", 
		PAin(0), PAin(2), PAin(3), PAin(4), PAin(5), PAin(7)); 
	printf("PTB: 1(%ld) 5(%ld) 13(%ld)\r\n", 
		PBin(1), PBin(5), PBin(13)); 
	printf("PTC: 4(%ld)\r\n", 
		PCin(4)); 
	printf("PTD: 1(%ld) 2(%ld) 3(%ld) 10(%ld) 12(%ld) 13(%ld) 14(%ld)\r\n", 
		PDin(1), PDin(2), PDin(3), PDin(10), PDin(12), PDin(13), PDin(14)); 
	printf("PTE: 0(%ld)\r\n", 
		PEin(0)); 
} 

void BUZ_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟	   	 
	//GPIOA->CRL&=0XF0FFFFFF; 
	//GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关   	 
	GPIOA->CRH&=0XFFFF0FFF; 
	GPIOA->CRH|=0X00003000; //PA.11 推挽输出   	 
	if(onoff==ON)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOA->ODR |= 1<<11;      //PA.11 输出高
	} 
	else if(onoff==OFF)
	{
	    //GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	    GPIOA->ODR &= ~(1<<11);      //PA.11 输出高
	} 
}

void FAN_CTRL(u8 onoff)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	//GPIOA->CRL&=0XF0FFFFFF; 
	//GPIOA->CRL|=0X03000000; //PA.6 推挽输出  电开关   	 
	GPIOC->CRH&=0XFFFFFF0F; 
	GPIOC->CRH|=0X00000030; //PC.9 推挽输出   	 
	if(onoff==ON)
	{
	    //GPIOA->ODR |= 1<<6;      //PA.6 输出高
	    GPIOC->ODR |= 1<<9;      //PC.9 输出高
	} 
	else if(onoff==OFF)
	{
	    //GPIOA->ODR &= ~(1<<6);      //PA.6 输出高
	    GPIOC->ODR &= ~(1<<9);      //PC.9 输出高
	} 
}
