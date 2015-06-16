  /*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#include "stc_new_8051.h"
#include "timer.h"
#include "steer.h"
#include "IR.h"
#include "MEM.h"

uchar m=0,n=0;
uchar Duty_left=80,Duty_right=80; //����ռ�ձȱ�־��ȡ1--100

extern uint8 rec_flag;
extern uint8 rec_flag;
extern uint16 Sampling_cnt;
extern bit IR_38K_En;
extern bit IR_R_T; 
extern bit IR_EN;
uint16 Uart_timeout_count=0;
extern void Steering_Engine_Control(void);
uint16 se_timer[8]={SE1_Deg,SE2_Deg,SE3_Deg,SE4_Deg,SE5_Deg,SE6_Deg,SE7_Deg,SE8_Deg};
/* ms ��ʱ */
void Delay_Ms(uint32 t)
{  
    uint16 i;
	while(t--)
	{
	   for(i=0;i<1050;i++);
	}
}

void Timer0_Init(void)
{
	TMOD   &=   0xf0; 
    TMOD   |=   0x01; 
	AUXR   |=   0XC0;
	IP   |=   0x02;//��ʱ��0�ж����ȼ����
	TH0=(65536-66)/256;
   TL0=(65536-66)%256;

	TR0=1;
	ET0=1;
}

void Timer1_Init(void)
{
    ET1     =	  1;
    TMOD   &=   0x0f; 
    TMOD   |=   0x10;   
    TH1     =   0xA9;   //1MS��ʱ
    TL1     =   0x9A;
    TR1     =   1;    //����ʱ��1

}

void Timer_0(void) interrupt 1 
{
	m++;						//�������ж���ִ��
  n++;
  if(m<=Duty_left) 
     MOTOR_A_EN=1;
  else MOTOR_A_EN=0;  
  if(m>100) 
     {MOTOR_A_EN=1;m=0;}
  if(n<=Duty_right) 
     MOTOR_B_EN=1;
  else MOTOR_B_EN=0;
  if(n>100) 
     {MOTOR_B_EN=1;n=0;}
TH0=(65536-66)/256;	   //ȡԼ150HZ��12M����ÿ�ζ�ʱ66us����100�Σ�������ͷ����ı�������ֱ�ӱ�ʾռ�ձȵ���ֵ
TL0=(65536-66)%256;

}

void Timer_1(void) interrupt 3
{
    static uint16 ms_count=0;

	TH1     =   0xA9;   
    TL1     =   0x9A;

	Uart_timeout_count++;
	if((Uart_timeout_count == 1000) && (rec_flag)) //���ڽ���1�볬ʱ
	{
		rec_flag = 0;
	}

	if(ms_count++ >= 1000)
	{
	     ms_count = 0;
		 if(rec_flag)
		 {
		     LED5 = ~LED5;
		 }
		 else
		 {
		     LED5 = 1;
		 }
	}
	
}
void Init_Steer(void)
{
	uint8 i;

	for(i=0;i<8;i++)
	{
		se_timer[i] = Read_Byte(50*512+i);	//100*512�Ƕ���Ƕȴ洢λ��
	}
	
}