#include <lpc17xx.h>
#include "type.h"
#include "debug.h"

/*****************************************************************************
** Function name:		
**
** Descriptions:		
**					
**
** parameters:		
** Returned value:	
** 
*****************************************************************************/
void UART3_Init(void)
{
   SC->PCONP |= (1 << 25);
	
   SC->PCLKSEL1 &= ~(3 << 18);
   SC->PCLKSEL1 |= (2 << 18);
   
   PINCON->PINSEL0 &= ~0x0F;
   PINCON->PINSEL0 |= 0x0A;
   UART3->LCR = 0x83;
   UART3->DLM = 0;							
   UART3->DLL = 125;
   UART3->FDR = 0x41;
  // UART3->DLL = 106;
  // UART3->FDR = 0xF8;
   UART3->LCR = 0x03;		/* DLAB = 0 */
   UART3->FCR = 0x07;
}

/*****************************************************************************
** Function name:		
**
** Descriptions:		
**					
**
** parameters:		
** Returned value:	
** 
*****************************************************************************/
void UART3_SendByte(uint8_t byte) 
{
//   uint8_t status = status;

   while(!(UART3->LSR & 0x20));
   UART3->THR = byte;

//   status = UART3->LSR;
}

/*****************************************************************************
** Function name:		
**
** Descriptions:		
**					
**
** parameters:		
** Returned value:	
** 
*****************************************************************************/
void UART3_SendBlock(uint8_t *datptr,uint8_t lenth) 
{
   while(lenth)
   {
      UART3_SendByte(*datptr);
   	  datptr++;
	  lenth--;
   }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
