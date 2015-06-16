/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#ifndef _MEM_H_
#define _MEM_H_

#include "type.h"
#include "stc_new_8051.h"
uchar Read_Byte(uint Addr);
void Write_Byte(uint Addr,uchar My_Data);
void Earse_Section(uint Addr);
uint Read_Word(uint Addr);
void Write_Word(uint Addr,uint My_Data);

#endif