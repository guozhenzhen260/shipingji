#define   _DEBUG_TRACE
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc17xx_uart.h"
#include "..\driver\lpc17xx_clkpwr.h"
#include "..\driver\lpc_types.h"
#include "global.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "DebugTrace.h"





//============================================================================
/*****************************************************************************
** Function name:		Uart0_Init
**
** Descriptions:		串口初始化
**					
**
** parameters:			无
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
void Uart0_Init(void)
{	
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0,ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART0,CLKPWR_PCLKSEL_CCLK_DIV_4);
   
    //Uart0_RIndex = Uart0_FIndex = 0;
	
	LPC_PINCON->PINSEL0 &= ~0xF0;
    LPC_PINCON->PINSEL0 |= 0x50;

	LPC_UART0->LCR = UART_LCR_DLAB_EN;		/* DLAB = 1 */
    LPC_UART0->DLM = 0;							
    LPC_UART0->DLL = 125;
    LPC_UART0->FDR = 0x41;						
	LPC_UART0->LCR = UART_LCR_WLEN8;		/* 8 bits, no Parity, 1 Stop bit */
	LPC_UART0->FCR = UART_FCR_TX_RS|UART_FCR_RX_RS|UART_FCR_FIFO_EN;		/* Enable and reset TX and RX FIFO. */

    LPC_UART0->IER = UART_IER_RBRINT_EN;	 /* Enable UART0 interrupt */

    //zyIsrSet((UART0_IRQn +16), (unsigned long)Uart0_IRQHandler, PRIO_TEN);
	zyIrqEnable();
}



/*****************************************************************************
** Function name:	Uart0_SendByte	
**
** Descriptions:	发送一字节	
**						
**
** parameters:		byte	
** Returned value:	无
** 
*****************************************************************************/
void Uart0_SendByte(uint8_t byte) 
{
   while(!(LPC_UART0->LSR & UART_LSR_THRE));
   LPC_UART0->THR = byte;
}

/*****************************************************************************
** Function name:	Uart0_Write	
**
** Descriptions:	发送数据块	
**						
**
** parameters:		buf:数据指针,lenth:数据长度	
** Returned value:	无
** 
*****************************************************************************/
void Uart0_Write(uint8_t *buf,uint8_t lenth) 
{
   while(lenth) 
   {
      Uart0_SendByte(*buf);
   	  buf++;
	  lenth--;
   }
}


/*****************************************************************************
** Function name:	Trace	
**
** Descriptions:	Trace调试	
**						
**
** parameters:		format:数据指针,第二个参数可有可无，表示字符串格式	
** Returned value:	无
** 
*****************************************************************************/
void Trace( uint8_t *format , ... )
{

	va_list arg_ptr;
	uint8_t StringTemp[ 1024 ];
	uint16_t i;
	uint16_t len;

	#ifdef _DEBUG_TRACE
	va_start(arg_ptr, format);
	//len = vsprintf( StringTemp , format , arg_ptr );
	len = vsprintf( (char *)StringTemp , (const char *)format , arg_ptr );//120331 by cq disable warning
	va_end(arg_ptr);
	for( i = 0 ; i < len ; i ++ )
	{
		Uart0_SendByte( StringTemp[ i ] );
	}
	#endif
	
}

