#include "io.h"
#include "sys.h"
#include "usart.h"

u8  PERIVAERAL_POWER=OFF;         	//外围电路电源开关

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

void ROBOT_INIT( void )
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	GPIOC->CRH&=0XFFFFF0FF; 
	GPIOC->CRH|=0X00000300; //PC.10 推挽输出 
	GPIOC->ODR |= 1<<10;  

	RCC->APB2ENR|=1<<4;     //使能PORTC时钟	   	 
	GPIOC->CRH&=0XFFFF0FFF; 
	GPIOC->CRH|=0X00003000; //PC.10 推挽输出 
	GPIOC->ODR &= ~(1<<11);  	 
}

void LED_RED_CTRL(u8 onoff)
{
	 
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


