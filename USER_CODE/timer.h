/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           coin.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        定时器驱动程序相关函数声明
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
#ifndef __TIMER_H 
#define __TIMER_H

	
#define TIME_INTERVAL	(24000000/100 - 1)

uint32_t init_timer( uint8_t timer_num, uint32_t timerInterval );
void enable_timer( uint8_t timer_num );
void disable_timer( uint8_t timer_num );
void reset_timer( uint8_t timer_num );
void TIMER0_IRQHandler (void);
void TIMER1_IRQHandler (void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
