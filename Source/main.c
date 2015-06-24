/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
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
//#define DEBUG //�Ƿ�����ӡ����


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
  		Cruising_Mod();//ģʽ����ִ���Ӻ���
		//ARMShow();
	}
}
