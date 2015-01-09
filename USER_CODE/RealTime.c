/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           RealTime.c
** Last modified Date:  2011-12-02
** Last Version:         
** Descriptions:        实时时钟驱动程序
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-12-02
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
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\driver\lpc17xx_rtc.h"
#include "..\config.h"
#include "global.h"
#include "pcf8563t.h"
#include "RealTime.h"

/*****************************************************************************
** Function name:		RTCn_Init
**
** Descriptions:		初始化实时时钟
**
** parameters:			无
** Returned value:		无
**				
** 
*****************************************************************************/
void RTCn_Init(void) 
{
    #if RTC_TYPE == PCF8563T_CFG
		PCF8563T_Init();
		PCF8563T_Start();
	#else
		/* RTC Block section ------------------------------------------------------ */
		// Init RTC module
		RTC_Init(LPC_RTC);

		/* Enable rtc (starts increase the tick counter and second counter register) */
		RTC_ResetClockTickCounter(LPC_RTC);
		RTC_Cmd(LPC_RTC, ENABLE);
		RTC_CalibCounterCmd(LPC_RTC, ENABLE);
		RTC_CalibConfig(LPC_RTC, 10612, RTC_CALIB_DIR_FORWARD);
		
		SYSPara1.RTCStat = 0;
	#endif
}

/*****************************************************************************
** Function name:		RTCn_SetTime
**
** Descriptions:		时间设置
**
** parameters:			Timetype: 
**						RTCN_YEAR：年
**						RTCN_MONTH：月
**						...
**						TimeValue：设置值
** Returned value:		无
**				
** 
*****************************************************************************/
void RTCn_SetTime(uint32_t Timetype, uint32_t TimeValue)
{
    #if RTC_TYPE == PCF8563T_CFG
		PCF8563T_SetTime(Timetype, TimeValue);
	#else
		RTC_SetTime(LPC_RTC, Timetype, TimeValue);
	#endif
}

/*****************************************************************************
** Function name:		RTCn_SetFullTime
**
** Descriptions:		设置时间，包括年、月、日、星期、日期、时、分、秒
**
** parameters:			pFullTime：RTC_TIME_Type*结构格式时间
** Returned value:		无
**				
** 
*****************************************************************************/
void RTCn_SetFullTime(RTC_TIME_Type *pFullTime) 
{
    #if RTC_TYPE == PCF8563T_CFG
		PCF8563T_SetFullTime(pFullTime);
	#else
		RTC_SetFullTime(LPC_RTC, pFullTime);
	#endif
}

/*****************************************************************************
** Function name:		RTCn_GetTime
**
** Descriptions:		获取时间
**
** parameters:			Timetype: 
**						RTCN_YEAR：年
**						RTCN_MONTH：月
**						...
** Returned value:		时间值
**				
** 
*****************************************************************************/
uint32_t RTCn_GetTime (uint32_t Timetype)
{
    #if RTC_TYPE == PCF8563T_CFG
		return PCF8563T_GetTime(Timetype);
	#else
		return RTC_GetTime(LPC_RTC, Timetype);
	#endif
}

 /*****************************************************************************
** Function name:		RTCn_GetFullTime
**
** Descriptions:		获取时间，包括年、月、日、星期、日期、时、分、秒
**
** parameters:			pFullTime：RTC_TIME_Type*结构格式时间
** Returned value:		无
**				
** 
*****************************************************************************/
void RTCn_GetFullTime(RTC_TIME_Type *pFullTime) 
{
    #if RTC_TYPE == PCF8563T_CFG
		PCF8563T_GetFullTime(pFullTime);
	#else
		RTC_GetFullTime(LPC_RTC, pFullTime);
	#endif
}
