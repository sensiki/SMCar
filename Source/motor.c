/*
°æÈ¨ÉùÃ÷£º
WIFI»úÆ÷ÈËÍø¡¤»úÆ÷ÈË´´Òâ¹¤×÷ÊÒ°æÈ¨ËùÓÐ www.wifi-robots.com
Äú¿ÉÒÔÈÎÒâÐÞ¸Ä±¾³ÌÐò£¬²¢Ó¦ÓÃÓÚ×ÔÐÐÑÐ·¢µÄÖÇÄÜÐ¡³µ»úÆ÷ÈË¼°ÆäËûµç×Ó²úÆ·ÉÏ£¬µ«ÊÇ½ûÖ¹ÓÃÓÚÉÌÒµÄ²Àû¡£
By WIFI»úÆ÷ÈËÍø¡¤»úÆ÷ÈË´´Òâ¹¤×÷ÊÒ
*/
#include "motor.h"
#include "uart.h"
#include "timer.h"
#include "mem.h"
#include "OLED12864.h"

#define m0 0//ºÚÏßm1,°×Ïßm0
#define m1 1

#define	lenth 68000		//????:10000--»100--»500--»2000--80000--76000--68000
#define width 27000		//500--»10-->2000--»60000--30000---»27000
#define check_right 500 //2000--»20--»200--»500
#define midl left1
#define midr right5 


uchar Cruising_Flag =0x00;
uchar  Robots_Run_Status;
uchar Pre_Cruising_Flag = 0x00;
extern uint16 se_timer[8];
extern uchar Duty_left,Duty_right; //×óÓÒÕ¼¿Õ±È±êÖ¾£¬È¡1--100

void delay(long int Delay_time)//????
 {
 uint t=Delay_time;
 while(t--);
 }

/**µç»ú³õÊ¼»¯**/
void Motor_Init(void)
{
	MOTOR_A_EN=1;
	MOTOR_B_EN=1;
	MOTOR_GO_STOP;
}
//¸úËæÄ£Ê½
void Follow_Track(void)
{
  		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		
	     if(Input_Detect1 == 1)	//ÖÐ¼ä´«¸ÐÆ÷OK
		  {
		  
		   	if((Input_Detect0 == 0)&& (Input_Detect2 == 0)) //Á½±ßÍ¬Ê±Ì½²âµ½ÕÏ°­Îï
			{
			
				 Robots_Run_Status=0x04;//Í£Ö¹ 
			} 
			 
			if((Input_Detect0 == 0)&& (Input_Detect2 == 1))//×ó²àÕÏ°­Îï
			{
			
				Robots_Run_Status=0x01;//ÓÒ×ª 
			}
		
			if((Input_Detect0 == 1)&& (Input_Detect2 == 0))//ÓÒ²àÕÏ°­Îï
			{
				Robots_Run_Status=0x02;//×ó×ª 
			}
			 
			if((Input_Detect0 == 1)&& (Input_Detect2 == 1))//ÎÞÈÎºÎÕÏ°­Îï
			{
				Robots_Run_Status=0x03;//Ö±ÐÐ 
			}
		  }
		  else
		  {
		  	   Robots_Run_Status=0x04;
		  }

}

//±ÜÕÏÄ£Ê½
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
			else //·ñÔòµç»úÖ´ÐÐÇ°½ø¶¯×÷
			{
				  Robots_Run_Status=0x03;
			}
}
//ÊÖ±Û¶¯×÷Õ¹Ê¾
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

//ÊÖ±Û¶¯×÷Õ¹Ê¾
void HeadShow(void)
{
	 Beep=~Beep;
	se_timer[7]=170;
	Delay_Ms(1000);

	se_timer[7]=0;
	Delay_Ms(1000);
}

//·¢ËÍ³¬Éù²¨
void Send_wave(void)
{
    uint16 i;

	Trig = 1;
	for(i=0;i<150;i++);
	Trig = 0;
}
//»ñµÃ¾àÀëÖµ
uchar Get_Distance(void)
{
    uint32 Distance = 0;

    Send_wave();
    TH1 = 0;
    TL1 = 0;
	while(TH1<250 && Echo!= 1);	  
	if(TH1 <= 250)	//²â¾à·¶Î§<0.5M
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
//Ñ²ÏßÄ£Ê½
void FollowLine(void)
{
		  if(Get_Distance()<0x0A)
		{
			stop();//Í£Ö¹ 
		}
			else
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
}
//Í¨¹ýÀ×´ï±ÜÕÏ
void AvoidByRadar(void)
{
   if(Get_Distance()<0x1A)//Èç¹ûÀ×´ï»Ø²¨Êý¾ÝÐ¡ÓÚ10ÀåÃ×´¥·¢
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
//Ä£Ê½Ö´ÐÐ×Óº¯Êý£¬¸ù¾Ý±êÖ¾Î»½øÐÐÅÐ¶Ï
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
	   case 0x01:Follow_Track(); break;//¸úËæÄ£Ê½
	   case 0x02:FollowLine(); break;//Ñ²ÏßÄ£Ê½
	   case 0x03:Avoiding(); break;//±ÜÕÏÄ£Ê½
	   case 0x04:AvoidByRadar();break;//³¬Éù²¨±ÚÕÏÄ£Ê½
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
//³µµÆ
void MainLight_Init(void)
{
   MAINLIGHT_TURNOFF;
}
//·äÃùÆ÷
void MainBuzzer_Init(void)
{
   MAINBUZZER_TURNOFF;
}

//-----------------------------------------------
void correct_left()//Ïò×ó½ÃÕý
 {
   Duty_left =correct_speed;
   Duty_right=full_speed_right;
   MOTOR_A_CON1=0;
   MOTOR_A_CON2=1;
   MOTOR_B_CON1=0;
   MOTOR_B_CON2=1;
 }
//------------------------------------------------
 void correct_right()//ÏòÓÒ½ÃÕý
 {
   Duty_left =full_speed_right;
   Duty_right=correct_speed;
   MOTOR_A_CON1=0;
   MOTOR_A_CON2=1;
   MOTOR_B_CON1=0;
   MOTOR_B_CON2=1;
 }
//--------------------------------------------------
void turn_left()//×ó×ª
{
  Duty_left =turn_speed_left;
  Duty_right=turn_speed_right;
  MOTOR_A_CON1=1;	 //???????,????,
  MOTOR_A_CON2=0;
  MOTOR_B_CON1=0;
  MOTOR_B_CON2=1; 		
}
//---------------------------------------------------
void turn_right()//ÓÒ×ª
{
  Duty_left =turn_speed_left;
  Duty_right=turn_speed_right;
  MOTOR_A_CON1=0;	  //???????,????,
  MOTOR_A_CON2=1;	
  MOTOR_B_CON1=1;	
  MOTOR_B_CON2=0; 
}
//-----------------------------------------------------
void straight()	//Ö±ÐÐ
{
  Duty_left =full_speed_left;	//??????????,????????
  Duty_right=full_speed_right;	//?????????????,????????,????????????
  MOTOR_A_CON1=0;	  //???????,????,
  MOTOR_A_CON2=1;	
  MOTOR_B_CON1=0;	
  MOTOR_B_CON2=1; 
}
void stop()	//Í£Ö¹
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
