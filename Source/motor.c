/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#include "motor.h"
#include "uart.h"
#include "timer.h"
#include "mem.h"
#include "OLED12864.h"

#define m0 0//黑线m1,白线m0
#define m1 1

#define	lenth 68000		//????:10000--�100--�500--�2000--80000--76000--68000
#define width 27000		//500--�10-->2000--�60000--30000---�27000
#define check_right 500 //2000--�20--�200--�500
#define midl left1
#define midr right5 


uchar Cruising_Flag =0x00;
uchar  Robots_Run_Status;
uchar Pre_Cruising_Flag = 0x00;
extern uint16 se_timer[8];
extern uchar Duty_left,Duty_right; //左右占空比标志，取1--100

void delay(long int Delay_time)//????
 {
 uint t=Delay_time;
 while(t--);
 }

/**电机初始化**/
void Motor_Init(void)
{
	MOTOR_A_EN=1;
	MOTOR_B_EN=1;
	MOTOR_GO_STOP;
}
//跟随模式
void Follow_Track(void)
{
  		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		
	     if(Input_Detect1 == 1)	//中间传感器OK
		  {
		  
		   	if((Input_Detect0 == 0)&& (Input_Detect2 == 0)) //两边同时探测到障碍物
			{
			
				 Robots_Run_Status=0x04;//停止 
			} 
			 
			if((Input_Detect0 == 0)&& (Input_Detect2 == 1))//左侧障碍物
			{
			
				Robots_Run_Status=0x01;//右转 
			}
		
			if((Input_Detect0 == 1)&& (Input_Detect2 == 0))//右侧障碍物
			{
				Robots_Run_Status=0x02;//左转 
			}
			 
			if((Input_Detect0 == 1)&& (Input_Detect2 == 1))//无任何障碍物
			{
				Robots_Run_Status=0x03;//直行 
			}
		  }
		  else
		  {
		  	   Robots_Run_Status=0x04;
		  }

}

//避障模式
void Avoiding(void)
{
   		 switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;
			case 0x05:MOTOR_GO_BACK; break;		   
		 }

		   	if((left2 == 1) || (right4 == 1) || (Input_Detect1==0))
			{
		    
				Robots_Run_Status=0x04;
			} 
			else //否则电机执行前进动作
			{
				  Robots_Run_Status=0x03;
			}
}
//手臂动作展示
void ARMShow(void)
{

	se_timer[0]=100;
	Delay_Ms(1000);
	se_timer[1]=70;
	Delay_Ms(1000);
	se_timer[2]=60;
	Delay_Ms(1000);
	se_timer[3]=50;
	Delay_Ms(1000);

	se_timer[3]=81;
	Delay_Ms(1000);
	se_timer[2]=160;
	Delay_Ms(1000);
	se_timer[1]=120;
	Delay_Ms(1000);
	se_timer[0]=140;
	Delay_Ms(1000);
}

//手臂动作展示
void HeadShow(void)
{
	 Beep=~Beep;
	se_timer[7]=170;
	Delay_Ms(1000);

	se_timer[7]=0;
	Delay_Ms(1000);
}

//发送超声波
void Send_wave(void)
{
    uint16 i;

	Trig = 1;
	for(i=0;i<150;i++);
	Trig = 0;
}
//获得距离值
uchar Get_Distance(void)
{
    uint32 Distance = 0;

    Send_wave();
    TH1 = 0;
    TL1 = 0;
	while(TH1<250 && Echo!= 1);	  
	if(TH1 <= 250)	//测距范围<0.5M
	{
		TH1 = 0;
    TL1 = 0;
	   while(Echo == 1);
	   Distance = TH1;
	   Distance = Distance*256;
	   Distance = Distance + TL1;
	   Distance = Distance * 17; 
	   Distance = Distance / 22118;
	   return (uchar)(Distance&0xFF);
	}
}
//巡线模式
void FollowLine(void)
{

	 if(right5==m1){line_right();}
   else 
   if(left1==m1){line_left();}
   else
   if(left2==m1){correct_left();}
   else
   if(right4==m1){correct_right();}
   else
			 if(mid3==m1)
   line_straight();
			 else
				 stop();

		
}
//通过雷达避障
void AvoidByRadar(void)
{
   if(Get_Distance()<0x1A)//如果雷达回波数据小于10厘米触发
	{
		 MOTOR_GO_STOP;
	}
	else
	{
	  MOTOR_GO_FORWARD;
	}

}
void Send_Distance(void)
{
	   UART_send_byte(0xFF);
	   UART_send_byte(0x03);
	   UART_send_byte(0x00);
	   UART_send_byte(Get_Distance());
	   UART_send_byte(0xFF);
	   Delay_Ms(1000);   
}
//模式执行子函数，根据标志位进行判断
void Cruising_Mod(void)
{
       
	 if(Pre_Cruising_Flag != Cruising_Flag)
	 {
	     if(Pre_Cruising_Flag != 0)
		 {
		     MOTOR_GO_STOP; 
		 }

    	 Pre_Cruising_Flag =  Cruising_Flag;
		 
	 }	

	switch(Cruising_Flag)
	{
		case 0x00:
			if((Get_Distance()<0x10)&&(Robots_Run_Status==0x03))
		 {
			 MOTOR_GO_STOP;
			 MAINLIGHT_TURNON;
		 }
		 else
		 {
			 MAINLIGHT_TURNOFF;
		 }
		break;
	   case 0x01:Follow_Track(); break;//跟随模式
	   case 0x02:FollowLine(); break;//巡线模式
	   case 0x03:Avoiding(); break;//避障模式
	   case 0x04:AvoidByRadar();break;//超声波壁障模式
	   case 0x05:ARMShow();break;
	   case 0x06:HeadShow();break;
	   default:break;
	}	 
}


void Delay_ForBarrier(uint32 t)
{  
    uint16 i;
	while(t--)
	{
	   for(i=0;i<1050;i++);
	}
}
//车灯
void MainLight_Init(void)
{
   MAINLIGHT_TURNOFF;
}
//蜂鸣器
void MainBuzzer_Init(void)
{
   MAINBUZZER_TURNOFF;
}

//-----------------------------------------------
void correct_left()//向左矫正
 {
   Duty_left =correct_speed;
   Duty_right=full_speed_right;
   MOTOR_A_CON1=0;
   MOTOR_A_CON2=1;
   MOTOR_B_CON1=0;
   MOTOR_B_CON2=1;
 }
//------------------------------------------------
 void correct_right()//向右矫正
 {
   Duty_left =full_speed_right;
   Duty_right=correct_speed;
   MOTOR_A_CON1=0;
   MOTOR_A_CON2=1;
   MOTOR_B_CON1=0;
   MOTOR_B_CON2=1;
 }
//--------------------------------------------------
void turn_left()//左转
{
  Duty_left =turn_speed_left;
  Duty_right=turn_speed_right;
  MOTOR_A_CON1=1;	 //???????,????,
  MOTOR_A_CON2=0;
  MOTOR_B_CON1=0;
  MOTOR_B_CON2=1; 		
}
//---------------------------------------------------
void turn_right()//右转
{
  Duty_left =turn_speed_left;
  Duty_right=turn_speed_right;
  MOTOR_A_CON1=0;	  //???????,????,
  MOTOR_A_CON2=1;	
  MOTOR_B_CON1=1;	
  MOTOR_B_CON2=0; 
}
//-----------------------------------------------------
void straight()	//直行
{
  Duty_left =full_speed_left;	//??????????,????????
  Duty_right=full_speed_right;	//?????????????,????????,????????????
  MOTOR_A_CON1=0;	  //???????,????,
  MOTOR_A_CON2=1;	
  MOTOR_B_CON1=0;	
  MOTOR_B_CON2=1; 
	Robots_Run_Status=0x03;
}
void stop()	//停止
{
  Duty_left =0;	//??????????,????????
  Duty_right=0;	//?????????????,????????,????????????
  MOTOR_A_CON1=0;	  //???????,????,
  MOTOR_A_CON2=0;	
  MOTOR_B_CON1=0;	
  MOTOR_B_CON2=0; 
}
//-----------------------------------------------------
void line_straight()reentrant //?????reentrant??????,//???????
{
  straight();
  if(right5==m1){line_right();}
  else	
  if(left1==m1){line_left();}
  else
  if(left2==m1)				  //?????,??????,??2,4???????????????,?????????
    while(left2==m1)
     {correct_left();
       if(right5==m1)
          {
		   line_right();
		   goto label3;
		   }
	   else if(left1==m1)
		 {line_left();
	     goto label3;}
     }
  else 
  if(right4==m1)			   //?????,??????,??2,4???????????????,?????????
   while(right4==m1)
    {correct_right();
     if(right5==m1)
       { line_right();  goto label3;}
     else if(left1==m1)
		{line_left();goto label3;}
	}
  else
  if((left1==m0)&&(left2==m0)&&(mid3==m0)&&(right4==m0)&&(right5==m0))
    {
	  straight();
	  //delay(lenth);
	  while(right4==m0) //??????mid3,?????????,??right4;???,??left2??
	    {turn_right();}
	  if(mid3==m1)
	    {line_straight();}
	
	}

	label3: ; //?????
}
//-------------------------------------------------------------------------
void line_right() //??????
{
	straight();//??????????????????
	delay(lenth);
	if(mid3==m1)
	{
	  turn_right();//????????
label:delay(width);	//?width??????mid3?????????????
	  if(mid3==m0)
	    while(right4==m0)
	     {}
	  else
	  	  goto label;
		
	 }

	else
	if(mid3==m0)
	   {
	     turn_right();
	     while(right4==m0)
	       {}	
	     if(midr==m1)
	       {line_straight();}
       }		 
}
//-----------------------------------------------------------------
void line_left() //???????
{ 
  while(left1==m1)
  {
	if(right5==m1) 
	{
	line_right();
	goto label2;
	} 
   }

   delay(check_right);//???????,????????,????????,??????????,
			   //?????,??line_right()??
	if(right5==m1) 
	{
	line_right();
	goto label2;
	}

   if((mid3==m1)||(left2==m1)||(right4==m1)){line_straight();}
	  else
	  {
	     while(left2==m0)
		    {turn_left();}
	     if(midl==m1)
	        line_straight();    
	  }
label2: ;
}
