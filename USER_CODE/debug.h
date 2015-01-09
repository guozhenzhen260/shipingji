/*****************************************************************************
 *   debug.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __DEBUG_H 
#define __DEBUG_H

#define DEBUG     1 
           		 				 
void UART3_Init(void);
void UART3_SendByte(uint8_t byte);
void UART3_SendBlock(uint8_t *datptr,uint8_t lenth);

#endif /* end __DEBUG_H */

/*****************************************************************************
**                            End Of File
******************************************************************************/

