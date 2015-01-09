/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           lcd.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        LCD驱动程序相关函数声明
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-07-22
** Version:              
** Descriptions:        The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
********************************************************************************************************/
#ifndef __LCD_H 
#define __LCD_H

//#include <lpc17xx.h>
#include "..\driver\LPC17xx.h"
/*
#define CS_SET()     {GPIO1->FIODIR |= (1 << 24); GPIO1->FIOSET |= (1 << 24);}
#define CS_CLR()     {GPIO1->FIODIR |= (1 << 24); GPIO1->FIOCLR |= (1 << 24);}
#define SID_SET()    {GPIO1->FIODIR |= (1 << 25); GPIO1->FIOSET |= (1 << 25);} 
#define SID_CLR()    {GPIO1->FIODIR |= (1 << 25); GPIO1->FIOCLR |= (1 << 25);} 
#define SCLK_SET()   {GPIO1->FIODIR |= (1 << 26); GPIO1->FIOSET |= (1 << 26);}  
#define SCLK_CLR()   {GPIO1->FIODIR |= (1 << 26); GPIO1->FIOCLR |= (1 << 26);}
#define POWER_ON()   {GPIO1->FIODIR |= (1 << 28); GPIO1->FIOCLR |= (1 << 28);}
#define POWER_OFF()  {GPIO1->FIODIR |= (1 << 28); GPIO1->FIOSET |= (1 << 28);}
#define BUZZER_SET() {GPIO1->FIODIR |= (1 << 31); GPIO1->FIOSET |= (1 << 31);}
#define BUZZER_CLR() {GPIO1->FIODIR |= (1 << 31); GPIO1->FIOCLR |= (1 << 31);}

#define CS_GET()     (GPIO1->FIOPIN & (1 << 24))
#define SID_GET()    (GPIO1->FIOPIN & (1 << 25))
#define SCLK_GET()   (GPIO1->FIOPIN & (1 << 26))
*/
#define LCD_CMD 0		//命令
#define LCD_DAT 1		//数据

#define LINE0  0x80		//第一行起始地址
#define LINE01 0x81		//第1.1行起始地址
#define LINE02 0x82		//第1.2行起始地址
#define LINE03 0x83		//第1.3行起始地址
#define LINE1  0x90		//第二行起始地址
#define LINE11 0x91		//第2.1行起始地址
#define LINE12 0x92		//第2.2行起始地址
#define LINE13 0x93		//第2.3行起始地址
#define LINE2  0xE0		//第三行起始地址
#define LINE21 0xE1		//第3.1行起始地址
#define LINE22 0xE2		//第3.2行起始地址
#define LINE23 0xE3		//第3.3行起始地址
#define LINE3  0xF0		//第四行起始地址
#define LINE31 0xF1		//第4.1行起始地址
#define LINE32 0xF2		//第4.2行起始地址
#define LINE33 0xF3		//第4.3行起始地址


#define LCDUP   0x01    //上半屏
#define LCDDOWN 0x02    //下半屏



#define CHAR_PER_LINE  20	//单行最多字符数


//void delay_nus(uint8_t t);
void delay_nms(uint8_t t);
void delay_ns(uint8_t t);
void lcd_send(uint8_t command_data,uint8_t mode);
void LCD_Init(void);
void ClearScreen(void);
void Write_LCD(uint8_t position,const char *dat,uint8_t lenth);
void WriteFull_LCD(uint8_t position,const char *dat,uint8_t lenth);
void Line_Reverse(uint8_t line);
void LCD_WhiteBlock(uint8_t flag);
void Cursor_On(uint8_t chipsel);
void Cursor_Off(uint8_t chipsel);
void SetCursorPost(uint8_t position);
void Draw_ClrGDRam(void);
void Draw_FullBmp(const uint8_t * dp);
void Draw_RECT(uint8_t Yaddr,uint8_t Xaddr,const uint8_t * dp);


//添加品途屏幕的兼容;by gzz 20121025
//==================
//==用于品途屏幕设备
//==================
void Pitu_LCDInit(void);
void Pitu_LCDSetCursor(unsigned char x,unsigned char y);
void Pitu_LCDDisplaySquare(unsigned char *squ_tab);
void Pitu_LCDPutBmp(const uint8_t *data);
void Pitu_one_word(unsigned char x,unsigned char y,unsigned char *Lib,unsigned char ch_num,unsigned char widthw);
void Pitu_LCDPutCmd(unsigned char cmd);
void Pitu_LCDPutData(unsigned char data);
void Pitu_LCDPutByte(unsigned char data);
void Pitu_LCDClrScreen(void);
void Pitu_LCDClearLine(unsigned char position, uint8_t lineMode,uint8_t ziti);
void Pitu_LCDPrintf(unsigned char position,unsigned char positiony,const char *libstr,uint8_t lenth,uint8_t reverse,uint8_t ziti);//拼图屏幕样式;by gzz 20121106
void Pitu_LCDPutLOGOBmp(unsigned char position,unsigned char y,unsigned char mode);//拼图屏幕样式;by gzz 20121106
void Pitu_LCDPutPicBmp(unsigned char mode);



#endif
