/*****************************************************************************
 *   pcf8563t.c:  I2C C file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\driver\lpc17xx_rtc.h"
#include "..\config.h"
#include "global.h"
#include "i2c.h"
#include "pcf8563t.h"

uint8_t pcfCurCentury = PCF_CUR_CENCUTY;	//当前世纪值
/*****************************************************************************
** Function name:		PCF8563T_Init
**
** Descriptions:		初始化PCF8563T
**
** parameters:			无
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_Init(void) 
{
	  uint8_t ucdata;
	  uint8_t ucstate;
	 
	  if ( I2C0_Init( (uint8_t)I2C_MASTER ) == FALSE )	/* initialize I2c */
	  {
		  while ( 1 );				/* Fatal error */
	  } 

	  ucdata = 0x00;
	  ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_CS2, &ucdata, 1);

	  if(ucstate == TRUE)
	  {
	      SYSPara1.RTCStat = 0;
	  }
	  else
	  {
	      SYSPara1.RTCStat = 1;
	  }

	  ucdata = 0x80;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_ALARM_MIN, &ucdata, 1);

	  ucdata = 0x80;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_ALARM_HOUR, &ucdata, 1);

	  ucdata = 0x80;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_ALARM_DAY, &ucdata, 1);

	  ucdata = 0x80;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_ALARM_WEEK, &ucdata, 1);

	  ucdata = 0x00;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_CLKOUT_FRQ, &ucdata, 1);

	  ucdata = 0x03;
	  I2C0_Write(PCF8563T_ADDR, PCF8563T_TIMER_CTRL, &ucdata, 1);
}

/*****************************************************************************
** Function name:		PCF8563T_Start
**
** Descriptions:		启动PCF8563T计时
**
** parameters:			无
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_Start(void) 
{
  uint8_t ucdata;
  uint8_t ucstate;

  ucdata = 0x00;
  ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_CS1, &ucdata, 1);

  if(ucstate == TRUE)
  {
      SYSPara1.RTCStat = 0;
  }
  else
  {
      SYSPara1.RTCStat = 1;
  }
}

/*****************************************************************************
** Function name:		PCF8563T_Stop
**
** Descriptions:		停止PCF8563T计时
**
** parameters:			无
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_Stop(void) 
{
    uint8_t ucdata;
	uint8_t ucstate;

    ucdata = 0x20;
    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_CS1, &ucdata, 1);
	if(ucstate == TRUE)
    {
        SYSPara1.RTCStat = 0;
    }
    else
    {
        SYSPara1.RTCStat = 1;
    }
}

/*****************************************************************************
** Function name:		__DEC2BCD
**
** Descriptions:		10进制转BCD码
**
** parameters:			Value：2位10进制数
** Returned value:		单字节BCD码
**				
** 
*****************************************************************************/
static uint8_t __DEC2BCD(uint8_t Value)
{
    uint8_t ucValue;

	ucValue = ((Value / 10) << 4) | (Value % 10);

	return 	ucValue;
}

/*****************************************************************************
** Function name:		__BCD2DEC
**
** Descriptions:		BCD码转10进制
**
** parameters:			Value：单字节BCD码
** Returned value:		2位10进制数
**				
** 
*****************************************************************************/
static uint8_t __BCD2DEC(uint8_t Value)
{
    uint8_t ucValue;

	ucValue = (Value >> 4) * 10 + (Value & 0x0F);

	return 	ucValue;
}

/*****************************************************************************
** Function name:		PCF8563T_SetTime
**
** Descriptions:		时间设置
**
** parameters:			Timetype: 
**						PCF8563T_YEAR：年
**						PCF8563T_MONTH：月
**						...
**						TimeValue：设置值
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_SetTime (uint32_t Timetype, uint32_t TimeValue)
{
    uint8_t ucTimevalue;
	uint8_t ucstate;
	
	ucTimevalue = __DEC2BCD((uint8_t)(TimeValue % 100));
	switch(Timetype)
	{
	    case PCF8563T_YEAR:
			pcfCurCentury = TimeValue / 100;
			if(ucTimevalue <= 0x99)
			{
			    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_YEAR, &ucTimevalue, 1); 
			}
			break;
		
		case PCF8563T_MONTH:
			if(ucTimevalue <= 0x12)
			{
			    if((pcfCurCentury - PCF_CENCUTY_BASE) % 2)
				{
				    ucTimevalue &= ~PCF_CENCUTY_BIT;
				}
				else
				{
				    ucTimevalue |= PCF_CENCUTY_BIT;
				}
				ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_MONTH, &ucTimevalue, 1); 
			}
			break;
		
		case PCF8563T_WEEK:
			if(ucTimevalue <= 0x06)
			{
			    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_WEEK, &ucTimevalue, 1); 
			}
			break;
		
		case PCF8563T_DATE:
			if(ucTimevalue <= 0x31)
			{
			    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_DATE, &ucTimevalue, 1); 
			}
			break;
		
		case PCF8563T_HOUR:
			if(ucTimevalue < 0x24)
			{
			    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_HOUR, &ucTimevalue, 1); 
			}
			break;

		case PCF8563T_MIN:
			if(ucTimevalue < 0x60)
			{
			    ucstate= I2C0_Write(PCF8563T_ADDR, PCF8563T_MIN, &ucTimevalue, 1); 
			}
			break;

		default:
			if(ucTimevalue < 0x60)
			{
			    ucstate = I2C0_Write(PCF8563T_ADDR, PCF8563T_SEC, &ucTimevalue, 1); 
			}
			break;
	}

	if(ucstate == TRUE)
    {
        SYSPara1.RTCStat = 0;
    }
    else
    {
        SYSPara1.RTCStat = 1;
    }
}

/*****************************************************************************
** Function name:		PCF8563T_SetFullTime
**
** Descriptions:		设置PCF8563T时间，包括年、月、日、星期、日期、时、分、秒
**
** parameters:			pFullTime：RTC_TIME_Type*结构格式时间
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_SetFullTime(RTC_TIME_Type * pFullTime) 
{	
	PCF8563T_SetTime(PCF8563T_YEAR, pFullTime->YEAR);
	PCF8563T_SetTime(PCF8563T_MONTH, pFullTime->MONTH);
	PCF8563T_SetTime(PCF8563T_WEEK, pFullTime->DOW);
	PCF8563T_SetTime(PCF8563T_DATE, pFullTime->DOM);
	PCF8563T_SetTime(PCF8563T_HOUR, pFullTime->HOUR);
	PCF8563T_SetTime(PCF8563T_MIN, pFullTime->MIN);
	PCF8563T_SetTime(PCF8563T_SEC, pFullTime->SEC);
}

/*****************************************************************************
** Function name:		PCF8563T_GetTime
**
** Descriptions:		获取时间
**
** parameters:			Timetype: 
**						PCF8563T_YEAR：年
**						PCF8563T_MONTH：月
**						...
** Returned value:		时间值
**				
** 
*****************************************************************************/
uint32_t PCF8563T_GetTime (uint32_t Timetype)
{
	uint8_t retval, retval1/*, retval2*/;
	uint8_t ucyear = 0;
	uint8_t ucdata;
	
	if(I2C0_Read(PCF8563T_ADDR, Timetype, &ucdata, 1) == TRUE)
	{ 
	    SYSPara1.RTCStat = 0;
		retval1 = ucdata;
	}
	else
	{
	    SYSPara1.RTCStat = 1;
	}

	switch(Timetype)
	{	    		
		case PCF8563T_YEAR:		
		case PCF8563T_MONTH:
			if(Timetype == PCF8563T_YEAR)
			{
			    /*
				if(I2C0_Read(PCF8563T_ADDR, PCF8563T_MONTH, &ucdata, 1) == TRUE)
				{ 
		            retval2 = ucdata;
				}
				
				if(((retval2 & PCF_CENCUTY_BIT) == 0) ^ ((pcfCurCentury - PCF_CENCUTY_BASE) % 2))
				{
					pcfCurCentury++;
				}
				*/
				ucyear = pcfCurCentury;	
				retval = retval1;		
			}
			else
			{
				/*
				if(((retval1 & PCF_CENCUTY_BIT) == 0) ^ ((pcfCurCentury - PCF_CENCUTY_BASE) % 2))
				{
					pcfCurCentury++;
				}
				*/
				retval = (retval1 & PCF_MONTH_MASK);
			}
			break;
			
		case PCF8563T_WEEK:
			retval = (retval1  & PCF_WEEK_MASK);
			break;
		
		case PCF8563T_DATE:
			retval = (retval1  & PCF_DATE_MASK); 
			break;
		
		case PCF8563T_HOUR:
			retval = (retval1  & PCF_HOUR_MASK);
			break;

		case PCF8563T_MIN:
			retval = (retval1  & PCF_MIN_MASK);
			break;

		default:
			if(Timetype == PCF8563T_SEC)
			{
			    retval = (retval1  & PCF_SEC_MASK);
			}
			break;
	}

	return (ucyear * 100 + __BCD2DEC(retval));
}

/*****************************************************************************
** Function name:		PCF8563T_GetFullTime
**
** Descriptions:		获取PCF8563T时间，包括年、月、日、星期、日期、时、分、秒
**
** parameters:			pFullTime：RTC_TIME_Type*结构格式时间
** Returned value:		无
**				
** 
*****************************************************************************/
void PCF8563T_GetFullTime(RTC_TIME_Type * pFullTime) 
{
	pFullTime->SEC   = PCF8563T_GetTime(PCF8563T_SEC);
	pFullTime->MIN   = PCF8563T_GetTime(PCF8563T_MIN);
	pFullTime->HOUR  = PCF8563T_GetTime(PCF8563T_HOUR);
	pFullTime->DOM   = PCF8563T_GetTime(PCF8563T_DATE);
	pFullTime->DOW   = PCF8563T_GetTime(PCF8563T_WEEK);
	pFullTime->MONTH = PCF8563T_GetTime(PCF8563T_MONTH);
	pFullTime->YEAR  = PCF8563T_GetTime(PCF8563T_YEAR);
}
/******************************************************************************
**                            End Of File
******************************************************************************/

