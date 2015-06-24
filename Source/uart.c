/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/

#include "uart.h"
#include <stdio.h>
#include "motor.h"
#include "steer.h"
#include "mem.h"
#include "IR.h"
#include "timer.h"
#include "OLED12864.h"

extern uint16 se_timer[8];
uint8 xdata buffer[3];
uint8 rec_flag=0;	//����0�ȴ����� ����1���ڽ��� 
extern uchar IR_Type;  
extern uchar IR_Num;
extern uchar Cruising_Flag;
extern uint16 Uart_timeout_count;
extern uchar  Robots_Run_Status;
extern uchar Duty_left,Duty_right; //����ռ�ձȱ�־��ȡ1--100

void UART_init(void)
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	BRT = RELOAD_COUNT;		//�趨���������ʷ�������װֵ
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	ES      =   1;    //�������ж�
    EA      =   1;    //�����ж�
	TI      =   1;	
	EX0=0;
	EX1=0;
}

void UART_send_byte(uint8 byte)
{
	ES     =   0;  //�ش����ж�
    TI     =   0;  //���㴮�ڷ�������ж������־
    SBUF   =   byte;
    while(TI ==0); //�ȴ��������
    TI     =   1;  //���㴮�ڷ�������ж������־
    ES     =   1;  //�������ж�
}

void UART_send(uint8 *Buffer, uint8 Length)
{
	while(Length != 0)
	{
		UART_send_byte(*Buffer);
		Buffer++;
		Length--;
	}

}

void Communication_Decode(void)  
{	
	uint8 i;

	if(buffer[0]==0x00)
	{
		switch(buffer[1])
		{
			case 0x01:
					MOTOR_GO_FORWARD; 
			Robots_Run_Status=0x03;
			return;
			case 0x02:MOTOR_GO_BACK;   
			Robots_Run_Status=0x05;//ֹͣ 			
			return;
			case 0x03:MOTOR_GO_LEFT;
			Robots_Run_Status=0x02;//��ת 
			return;
		  case 0x04:MOTOR_GO_RIGHT;   
			Robots_Run_Status=0x01;//��ת
			return;
			case 0x00:MOTOR_GO_STOP;    
			Robots_Run_Status=0x04;//ֹͣ 
			return;
			default: return;
		}	
	}
	else if(buffer[0]==0x01)
	{
		if(buffer[2]>180)
			return;
		switch(buffer[1])
		{
			case 0x01:se_timer[0]=buffer[2]; return;
			case 0x02:se_timer[1]=buffer[2]; return;
			case 0x03:se_timer[2]=buffer[2]; return;
			case 0x04:se_timer[3]=buffer[2]; return;
			case 0x05:se_timer[4]=buffer[2]; return;
			case 0x06:se_timer[5]=buffer[2]; return;
			case 0x07:se_timer[6]=buffer[2]; return;
			case 0x08:se_timer[7]=buffer[2]; return;
			default : return;
		}
	}
	else if(buffer[0]==0x02)
	{
		switch(buffer[1])
		{
			case 0x01:Duty_left=buffer[2]*10;
				break;
			case 0x02:Duty_right=buffer[2]*10;
				break;
		}
	}
	else if(buffer[0]==0x04)	  //���ƿ���ʹ�ù������ʽ	FF040100FF  FF040000FF
	{
		  switch(buffer[1])
		   {
		       case 0x01:
				 MAINLIGHT_TURNON;
				 break;
				case 0x00:
				  MAINLIGHT_TURNOFF;
				  break;
				  default:return;
		   }
					  
	}
	else if(buffer[0]==0x05)	  //������
	{
		  switch(buffer[1])
		   {
		       case 0x01:
				 MAINBUZZER_TURNON;
				 break;
				case 0x00:
				  MAINBUZZER_TURNOFF;
				  break;
				  default:return;
		   }
					  
	}
	else if(buffer[0]==0x50)	  //ң�����������(ѧϰ)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Rec(); 
		}
		return;			  
	}
	else if(buffer[0]==0x51)	  //ң�������ⷢ��(����)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Tra(); 
		}
		return;			  
	}
	else if(buffer[0]==0x13)//ģʽ�л�����
	{
	    switch(buffer[1])
		{
		  case 0x01: 
				Cruising_Flag = 0x01; 
			break;//����
		  case 0x02: 
				Cruising_Flag = 0x02; 
				LCD_P6x8Str(16,6,"Mode:Follow Line "); 	
			break;//Ѳ��
		  case 0x03: 
				Cruising_Flag = 0x03; 
			break;//����
		  case 0x04: 
				Cruising_Flag = 0x04; 
			break;//�״����
		  case 0x05: 
				Cruising_Flag = 0x05; 
			break;//��е����ʾ
		  case 0x06: 
				Cruising_Flag = 0x06;
			break;//ҡͷ��ʾ
		  case 0x00: 
				Cruising_Flag = 0x00; 
				LCD_P6x8Str(16,6,"Mode:Normal      "); 	
			break;//����ģʽ
		  default:
				Cruising_Flag = 0x00; 
				LCD_P6x8Str(16,6,"Mode:Normal      "); 	
			break;//����ģʽ
		}
	}
	else if(buffer[0]==0x32)//�洢����Ƕ�
	{
	  	Earse_Section(50*512);
		for(i=0;i<8;i++)
		{
			//Write_Byte(50*512+i,se_timer[i]);	
		}
	}
	else
	{
		return;
	}
}

void UART_Interrupt_Receive(void) interrupt 4
{
	static uint8 i;

    if(RI==1)
    {
        RI  =   0;
        if(rec_flag==0)
		{
			if(SBUF==0xff)
			{
				rec_flag=1;	
				Uart_timeout_count = 0;
				i=0;
			}			
		}
		else
		{
			if(SBUF==0xff)
			{	
				rec_flag=0;	
				if(i==3)
				{
					Communication_Decode();	
					//UART_init(); //	���������������
				}
				i=0;
			}
			else
			{
				buffer[i]=SBUF;
				i++;
			}
		}		
    }
    else
    {
        TI  =  0;
    }
		
}