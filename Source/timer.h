/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ����������Ĳȡ������
By WIFI���������������˴��⹤����
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"
#include "type.h"



#define full_speed_left 60 //������ڸ���״̬��ռ�ձȣ����ò�����:(30,35,6,25,30,68000,27000,500);
#define full_speed_right 60	//				         				(40,45,6,25,30,68000,27000,500);
#define correct_speed 20 //У��ʱ�ĵ����ֵ�ռ�ձ�
#define turn_speed_left 60
#define turn_speed_right 60	

void Delay_Ms(uint32 t);
void Timer0_Init(void);
void Timer1_Init(void);
void Init_Steer(void);

#endif