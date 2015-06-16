
#ifndef _OLED12864_H_
#define _OLED12864_H_

#include "config.h"

void LCD_Init(void);
void LCD_P6x8Str(unsigned char x, y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x, y,unsigned char ch[]);
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]);
void LCD_CLS(void);