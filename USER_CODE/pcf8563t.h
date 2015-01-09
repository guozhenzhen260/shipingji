/*****************************************************************************
 *   pcf8563t.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __PCF8563T_H 
#define __PCF8563T_H

#define PCF_CENCUTY_BASE	 19				   //最小世纪值
#define PCF_CUR_CENCUTY		 20				   //当前世纪值
#define PCF_CENCUTY_BIT	     (1 << 7)		   //月寄存器中世纪值标志位

#define PCF_MONTH_MASK		 0x1F			   //月值掩码
#define PCF_WEEK_MASK		 0x07			   //星期值掩码
#define PCF_DATE_MASK		 0x3F			   //日期值掩码
#define PCF_HOUR_MASK		 0x3F			   //时值掩码
#define PCF_MIN_MASK		 0x7F			   //分值掩码
#define PCF_SEC_MASK		 0x7F			   //秒值掩码

/* For more info, read Philips's PCF8563T datasheet */
#define PCF8563T_ADDR		 0xA2			   //器件地址
#define PCF8563T_CS1		 0x00			   //控制状态寄存器1
#define PCF8563T_CS2		 0x01			   //控制状态寄存器2
#define PCF8563T_SEC		 0x02			   //秒寄存器
#define PCF8563T_MIN		 0x03			   //分寄存器
#define PCF8563T_HOUR		 0x04			   //时寄存器
#define PCF8563T_DATE		 0x05			   //日期寄存器
#define PCF8563T_WEEK		 0x06			   //星期寄存器
#define PCF8563T_MONTH		 0x07			   //月寄存器
#define PCF8563T_YEAR		 0x08			   //年寄存器
#define PCF8563T_ALARM_MIN	 0x09			   //ALARM分寄存器
#define PCF8563T_ALARM_HOUR	 0x0A			   //ALARM时寄存器
#define PCF8563T_ALARM_DAY	 0x0B			   //ALARM日期寄存器
#define PCF8563T_ALARM_WEEK  0x0C			   //ALARM星期寄存器
#define PCF8563T_CLKOUT_FRQ	 0x0D			   //时钟输出寄存器
#define PCF8563T_TIMER_CTRL	 0x0E			   //控制寄存器
#define PCF8563T_TIMER_VAL   0x0F			   //countdown寄存器

void PCF8563T_Init(void);
void PCF8563T_Start(void);
void PCF8563T_Stop(void);
void PCF8563T_SetTime (uint32_t Timetype, uint32_t TimeValue);
void PCF8563T_SetFullTime(RTC_TIME_Type * pFullTime);
uint32_t PCF8563T_GetTime(uint32_t Timetype);
void PCF8563T_GetFullTime(RTC_TIME_Type * pFullTime);
#endif /* end __PCF8563T_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
