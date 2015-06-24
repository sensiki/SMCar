/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#include "stc_new_8051.h"
#include "motor.h"
#include "uart.h"
#include <stdio.h>
#include "type.h"
#include "timer.h"
#include "SelfTest.h"
#include "OLED12864.h"
extern void UART_send_byte(uint8 byte);
//#define DEBUG //是否开启打印开关


void main(void)
{
	LCD_Init();
	LCD_P8x16Str(30,0,"FarSight");   
	LCD_P8x16Str(25,2,"Smart Car");  
	LCD_P6x8Str(16,6,"Initializing.....");    
	MainLight_Init();
	MainBuzzer_Init();
	#ifndef DEBUG
	{
		TestApp_Init();
	}
	#endif
	LCD_P6x8Str(16,6,"UART_init!       "); 
	UART_init();
	LCD_P6x8Str(16,6,"Motor_Init!      "); 
	Motor_Init();
	LCD_P6x8Str(16,6,"Timer0_Init!     "); 
	Timer0_Init();
	LCD_P6x8Str(16,6,"Timer1_Init!     "); 
	Timer1_Init();
	
	//Init_Steer();

	LCD_P6x8Str(16,6,"Working!         ");    

    while(1)
	{
  		Cruising_Mod();//模式功能执行子函数
		//ARMShow();
	}
}
