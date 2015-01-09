/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        定时器驱动程序
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
//#include <lpc17xx.h>
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\config.h"
#include "global.h"
#include "timer.h"
#include "PulseCoin.h"
#include "keypad.h"
#include "ITL.h"
#include "vmc_pc.h"

uint8_t Tick;

//extern uint16_t DispModeTimer;


/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		是timer0中断处理函数Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_IRQHandler (void) 
{    
    OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();
	LPC_TIM0 -> IR = 1;			/* clear interrupt flag */

    ReadKey();

    if(SYSPara.CoinEnable == PULSE_COIN_EN) 
	{
	    Coin_Accept(); 
	}

	//120914 by cq Cashless
	if(ReaderPara.iTimeDly > 0)
	{
		ReaderPara.iTimeDly--;
	}

	if(TimerDly.TimeDly > 0) 
	{		   //等待出货/出币结束
	    TimerDly.TimeDly--;
	}

	if(TimerDly.OperateDly > 0) 
	{	  //等待客户操作
	    TimerDly.OperateDly--;
	}

	if(TimerDly.CheckDly > 0) 
	{		  //定时检测设备状态
	    TimerDly.CheckDly--;
	}


	if( sysITLMission.msPoll > 0 )
        sysITLMission.msPoll --;
    if( sysITLMission.msTimer2 > 0 )
        sysITLMission.msTimer2 --;    

    //-------------------------------------------
	if( sysVPMission.msTimer1 > 0 )
		sysVPMission.msTimer1--;
	if( sysVPMission.msTimer2 > 0 )
	    sysVPMission.msTimer2--;
    if( sysVPMission.msTimer3 > 0 )
	    sysVPMission.msTimer3--;    
    if( sysVPMission.msGameTimer1 > 0 )
        sysVPMission.msGameTimer1 --;
	//if( sysVPMission.drvOutTimer > 0 )
    //    sysVPMission.drvOutTimer --;
    if( sysITLMission.msPoll > 0 )
        sysITLMission.msPoll --;
    if( sysITLMission.msTimer2 > 0 )
        sysITLMission.msTimer2 --;    
	if( sysVPMission.dspTimer2 > 0 )
        sysVPMission.dspTimer2 --; 
    sysVPMission.dspTimer1 ++;
	if( sysVPMission.dspErrTimer > 0 )
        sysVPMission.dspErrTimer --;  

	//120621 by cq LCDswitch
	//if(DispModeTimer > 0) 
	    //DispModeTimer--;
    //===========================================
    
    Tick ++ ;
	if ( Tick >= 100 )
	{
		Tick = 0;
		if( sysITLMission.sTimer1 > 0 )
	        	sysITLMission.sTimer1--;
		if( sysITLMission.sTimer2 > 0 )
	        	sysITLMission.sTimer2--;

		
		if( sysVPMission.sTimer1 > 0 )
			sysVPMission.sTimer1--;
		if( sysVPMission.sTimer2 > 0 )
			sysVPMission.sTimer2--;
		if( sysVPMission.billSTimer > 0 )
			sysVPMission.billSTimer--;
		if( sysVPMission.hopperTimer > 0 )
			sysVPMission.hopperTimer--;
		if( sysVPMission.hopperOutTimer ) 
			sysVPMission.hopperOutTimer --;

		if( sysVPMission.VPDevCtr & 0x01 )
		{
			sysVPMission.sTimerBC++;
		}

		//if( sysVPMission.VPDevCtr & 0x02 )
		{
			sysVPMission.sTimerChanger1++;
		}
		
		if( sysVPMission.VPDevCtr & 0x04 )
		{
			sysVPMission.sTimerChanger2++;
		}
		
		if( sysVPMission.VPDevCtr & 0x08 )
		{
			sysVPMission.sTimerChanger3++;
		}
		
		if( sysVPMission.VPDevCtr & 0x10 )
		{
			sysVPMission.sTimerGOC++;
		}

		sysVPMission.sTimertubeRemoved++;//币管没有关紧检测;by gzz 20110827 
	}

    OSIntExit();
 	OS_EXIT_CRITICAL();	//   ucosii add
}


/******************************************************************************
** Function name:		Timer1Handler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER1_IRQHandler (void)  
{  
    OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();
	LPC_TIM1 -> IR = 1;			/* clear interrupt flag */
	OSIntExit();
 	OS_EXIT_CRITICAL();	//   ucosii add
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		开定时器中断Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
	if ( timer_num == 0 ) 
	{
		LPC_TIM0 -> TCR = 1;
	}
	else 
	{
		LPC_TIM1 -> TCR = 1;
	}
	return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		关定时器中断Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
	if ( timer_num == 0 ) 
	{
		LPC_TIM0 -> TCR = 0;
	}
	else 
	{
		LPC_TIM1 -> TCR = 0;
	}
	return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
	uint32_t regVal;
	
	if ( timer_num == 0 ) 
	{
		regVal = LPC_TIM0 -> TCR;
		regVal |= 0x02;
		LPC_TIM0 -> TCR = regVal;
	} 
	else 
	{
		regVal = LPC_TIM1 -> TCR;
		regVal |= 0x02;
		LPC_TIM1 -> TCR = regVal;
	}
	return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
uint32_t init_timer ( uint8_t timer_num, uint32_t TimerInterval ) 
{
	Tick = 0;
	if ( timer_num == 0 )
	{
		LPC_TIM0 -> MR0 = TimerInterval;
		LPC_TIM0 -> MCR = 3;				/* Interrupt and Reset on MR0 */
		
		//	NVIC_EnableIRQ(TIMER0_IRQn);
		
		zyIsrSet((TIMER0_IRQn +16), (unsigned long)TIMER0_IRQHandler, PRIO_SEVEN );
		zyIrqEnable();
		return (TRUE);
	} 
	else if ( timer_num == 1 ) 
	{
		LPC_TIM1 -> MR0 = TimerInterval;
		LPC_TIM1 -> MCR = 3;				/* Interrupt and Reset on MR1 */
		
		//NVIC_EnableIRQ(TIMER1_IRQn);
		
		zyIsrSet((TIMER1_IRQn +16), (unsigned long)TIMER1_IRQHandler, PRIO_SEVEN );
		zyIrqEnable();
		
		return (0);
	}
	return (1);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
