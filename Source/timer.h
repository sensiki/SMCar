/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于向他人牟取暴利。
By WIFI机器人网·机器人创意工作室
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"
#include "type.h"



#define full_speed_left 60 //方便调节各个状态的占空比，可用参数组:(30,35,6,25,30,68000,27000,500);
#define full_speed_right 60	//				         				(40,45,6,25,30,68000,27000,500);
#define correct_speed 20 //校正时的低速轮的占空比
#define turn_speed_left 60
#define turn_speed_right 60	

void Delay_Ms(uint32 t);
void Timer0_Init(void);
void Timer1_Init(void);
void Init_Steer(void);

#endif