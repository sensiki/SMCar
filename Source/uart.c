/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
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
uint8 rec_flag=0;	//等于0等待接受 等于1正在接受 
extern uchar IR_Type;  
extern uchar IR_Num;
extern uchar Cruising_Flag;
extern uint16 Uart_timeout_count;
extern uchar  Robots_Run_Status;
extern uchar Duty_left,Duty_right; //左右占空比标志，取1--100

void UART_init(void)
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	BRT = RELOAD_COUNT;		//设定独立波特率发生器重装值
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	ES      =   1;    //允许串口中断
    EA      =   1;    //开总中断
	TI      =   1;	
	EX0=0;
	EX1=0;
}

void UART_send_byte(uint8 byte)
{
	ES     =   0;  //关串口中断
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   byte;
    while(TI ==0); //等待发送完成
    TI     =   1;  //清零串口发送完成中断请求标志
    ES     =   1;  //允许串口中断
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
			Robots_Run_Status=0x05;//停止 			
			return;
			case 0x03:MOTOR_GO_LEFT;
			Robots_Run_Status=0x02;//左转 
			return;
		  case 0x04:MOTOR_GO_RIGHT;   
			Robots_Run_Status=0x01;//右转
			return;
			case 0x00:MOTOR_GO_STOP;    
			Robots_Run_Status=0x04;//停止 
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
	else if(buffer[0]==0x04)	  //车灯控制使用灌电流方式	FF040100FF  FF040000FF
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
	else if(buffer[0]==0x05)	  //蜂鸣器
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
	else if(buffer[0]==0x50)	  //遥控器红外接收(学习)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Rec(); 
		}
		return;			  
	}
	else if(buffer[0]==0x51)	  //遥控器红外发射(控制)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Tra(); 
		}
		return;			  
	}
	else if(buffer[0]==0x13)//模式切换开关
	{
	    switch(buffer[1])
		{
		  case 0x01: 
				Cruising_Flag = 0x01; 
			break;//跟随
		  case 0x02: 
				Cruising_Flag = 0x02; 
				LCD_P6x8Str(16,6,"Mode:Follow Line "); 	
			break;//巡线
		  case 0x03: 
				Cruising_Flag = 0x03; 
			break;//避障
		  case 0x04: 
				Cruising_Flag = 0x04; 
			break;//雷达避障
		  case 0x05: 
				Cruising_Flag = 0x05; 
			break;//机械臂演示
		  case 0x06: 
				Cruising_Flag = 0x06;
			break;//摇头演示
		  case 0x00: 
				Cruising_Flag = 0x00; 
				LCD_P6x8Str(16,6,"Mode:Normal      "); 	
			break;//正常模式
		  default:
				Cruising_Flag = 0x00; 
				LCD_P6x8Str(16,6,"Mode:Normal      "); 	
			break;//正常模式
		}
	}
	else if(buffer[0]==0x32)//存储舵机角度
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
					//UART_init(); //	解决串口死机问题
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