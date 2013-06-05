#ifndef __IO_H
#define __IO_H

#include "includes.h"
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "stdio.h"

#define ON   1
#define OFF  0
#define FORWARD   1
#define BACKWARD  2


#define FRONTL0 PAin(2) //向前方向的光敏监测
#define FRONTL1 PBin(1)
#define FRONTL2 PAin(5)
#define FRONTL3 PAin(7)

#define DOWNL0 PAin(4) //向下方向的光敏监测
#define DOWNL1 PCin(4)
#define DOWNL2 PAin(3)
#define DOWNL3 PAin(0)

#define FRONTS0 PBin(5) //障碍物接触开关输入
#define FRONTS1 PBin(13)

#define CHARGE0 PDin(14) //顶部的充电寻路方向的光敏监测
#define CHARGE1 PDin(12) //向前方向的充电寻路方向的光敏监测
#define CHARGE2 PDin(13)

#define WHEELL0 PDin(1) //左侧马达接触开关输入
#define WHEELL1 PDin(3) //左马达光电信号输入
#define WHEELR0 PDin(10) //右侧马达接触开关输入
#define WHEELR1 PDin(2) //右马达光电信号输入


//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

void PERIVERAL_POWER_CTRL(u8 onoff);
void LED_RED_CTRL(u8 onoff);
void LED_GREEN_CTRL(u8 onoff);
void Right_Brush_CTRL(u8 onoff);
void Left_Brush_CTRL(u8 onoff);
void Right_Wheel_CTRL(u8 onoff);
void Left_Wheel_CTRL(u8 onoff);
u8 TOP_SENSOR_Input(void);
u8 KEY_Input(void);
void BUZ_CTRL(u8 onoff);
void FAN_CTRL(u8 onoff);
//void ROBOT_INIT( void );


#endif
