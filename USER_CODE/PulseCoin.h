/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           pulse.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        脉冲硬币器驱动程序相关函数声明
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
#ifndef __PULSE_H 
#define __PULSE_H

#include "..\driver\LPC17xx.h"

#ifndef  COIN_VALUE_BIT
#define  COIN_VALUE_BIT	         (1 << 13)	 //通道值对错标志
#endif

#ifndef  COIN_COMMERR_BIT
#define  COIN_COMMERR_BIT        (1 << 15)	 //通讯故障
#endif

#define  CoinP1_Get()  ((LPC_GPIO1->FIOPIN >> 10) & 0x01)        //投币通道1
#define  CoinP2_Get()  ((LPC_GPIO1->FIOPIN >> 9) & 0x01)		 //投币通道2
#define  CoinP3_Get()  ((LPC_GPIO1->FIOPIN >> 8) & 0x01)		 //投币通道3
#define  CoinP4_Get()  ((LPC_GPIO1->FIOPIN >> 4) & 0x01)		 //投币通道4
#define  CoinP5_Get()  ((LPC_GPIO1->FIOPIN >> 1) & 0x01)		 //投币通道5
#define  CoinP6_Get()  ((LPC_GPIO1->FIOPIN >> 0) & 0x01)		 //投币通道6

/*投币通道*/
#define  CoinP_Get()   (CoinP1_Get() |       \
                       (CoinP2_Get() << 1) | \
					   (CoinP3_Get() << 2) | \
					   (CoinP4_Get() << 3) | \
					   (CoinP5_Get() << 4) | \
					   (CoinP6_Get() << 5))

#define  Pulse_Enable()    LPC_GPIO1->FIODIR |= (1UL << 20); LPC_GPIO1->FIOCLR |= (1UL << 20)	 //使能投币
#define  Pulse_Disable()   LPC_GPIO1->FIODIR |= (1UL << 20); LPC_GPIO1->FIOSET |= (1UL << 20)	 //禁能投币


void Coin_Accept(void);


//120401 by cq change structor
//** Descriptions:        脉冲硬币器应用程序相关函数声明
void Pulse_InitProc(void);


#endif
