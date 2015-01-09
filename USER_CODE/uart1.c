/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart1.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        串口1驱动程序
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
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc17xx_uart.h"
#include "..\driver\lpc17xx_clkpwr.h"
#include "..\driver\lpc_types.h"
#include "global.h"
#include "uart1.h"
#include "DebugTrace.h"


#define  UART1_BUFLEN     255


volatile uint8_t  Uart1_RdBuf[UART1_BUFLEN];
volatile uint16_t  Uart1_RIndex;
volatile uint16_t  Uart1_FIndex;


/*****************************************************************************
** Function name:	Uart1_ClrBuf	
**
** Descriptions:	清除串口1缓存数据	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart1_ClrBuf(void) 
{
	Uart1_FIndex = 0;
	Uart1_RIndex = 0;
	memset(Uart1_RdBuf,0,sizeof(Uart1_RdBuf));
}

/*****************************************************************************
** Function name:	Uart1_IRQHandler	
**
** Descriptions:	串口1中断处理函数	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart1_IRQHandler(void) 
{
	volatile uint8_t dummy;
	uint32_t intsrc, tmp, tmp1;
		
	OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();

	/* Determine the interrupt source */
	intsrc = LPC_UART1->IIR;
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS) 
	{
		tmp1 = LPC_UART1->LSR;
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		if (tmp1) 
		{
		    dummy = LPC_UART1->RBR & UART_RBR_MASKBIT;
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) 
	{
	    if(Uart1_RIndex < UART1_BUFLEN)	
		{
		    Uart1_RdBuf[Uart1_RIndex++] = LPC_UART1->RBR & UART_RBR_MASKBIT;
		}
		else
		{
		    dummy = LPC_UART1->RBR & UART_RBR_MASKBIT;
		}
	}

    OSIntExit();
    OS_EXIT_CRITICAL();	//   ucosii add
}

/*****************************************************************************
** Function name:		Uart1_Init
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
void Uart1_Init(void)
{	
	uint32_t ulFdiv;
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART1,ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART1,CLKPWR_PCLKSEL_CCLK_DIV_4);
   
    Uart1_RIndex = Uart1_FIndex = 0;
	
	LPC_PINCON->PINSEL4 &= ~0x0000000F;
    LPC_PINCON->PINSEL4 |=  0x0000000A;

	/*LPC_UART1->LCR = UART_LCR_DLAB_EN;		/* DLAB = 1 * /
    LPC_UART1->DLM = 0;							
    LPC_UART1->DLL = 125;
    LPC_UART1->FDR = 0x41;		
	*/
	LPC_UART1->LCR  = 0x83;                                                   //允许设置波特率
    ulFdiv = (FPCLK / 16) / 115200;                               //设置波特率115200
    LPC_UART1->DLM  = ulFdiv / 256;
    LPC_UART1->DLL  = ulFdiv % 256; 
    LPC_UART1->LCR  = 0x03;                                                   //锁定波特率

	//LPC_UART1->LCR = UART_LCR_STOPBIT_SEL|UART_LCR_WLEN8;//UART_LCR_STOPBIT_SEL|UART_LCR_WLEN8;		/* 8 bits, no Parity, 2 Stop bit */
	LPC_UART1->LCR = UART_LCR_WLEN8;		/* 8 bits, no Parity, 1 Stop bit */
	LPC_UART1->FCR = UART_FCR_TX_RS|UART_FCR_RX_RS|UART_FCR_FIFO_EN|UART_FCR_TRG_LEV0;		/* Enable and reset TX and RX FIFO. */

    LPC_UART1->IER = UART_IER_RBRINT_EN | UART_IER_RLSINT_EN;	 /* Enable Uart1 interrupt */

    zyIsrSet((UART1_IRQn +16), (unsigned long)Uart1_IRQHandler, PRIO_TEN);
	zyIrqEnable();
	Uart1_ClrBuf();
}


uint8_t Uart1GetCh( void )
{
	uint8_t ch;
	ch = Uart1_RdBuf[ Uart1_FIndex ];
	//Trace("\r\n++2,Uart1_RdBuf=%x,Uart1_FIndex=%d",Uart1_RdBuf[ Uart1_FIndex ],Uart1_FIndex);
	Uart1_FIndex ++;	
	if ( Uart1_FIndex >= UART1_BUFLEN ) Uart1_FIndex = 0;	
	return ch;
	
}

uint8_t Uart1IsRxBufNull( void )
{
	OSTimeDly(1);
	if ( Uart1_RIndex - Uart1_FIndex == 0 ) 
		return 1;
	else
		return 0;
}



/*****************************************************************************
** Function name:	Uart1_Read	
**
** Descriptions:	读取串口1缓存数据	
**						
**
** parameters:		buf:数据指针,lenth:数据长度	
** Returned value:	无
** 
*****************************************************************************/
uint8_t Uart1_Read(uint8_t *buf, uint8_t len) 
{
	uint8_t count,i,j;
	count=0;

	//Trace("\r\n++READ=Begin");
	while( !Uart1IsRxBufNull() )
	{		
		j=Uart1_RIndex - Uart1_FIndex;
		//Trace("\r\n++RIndex=%d,FIndex=%d",Uart1_RIndex,Uart1_FIndex);
		count += j;
		for( i=0; i<j; i++ )
		//for( i=0; i<8; i++ )
		{
			*buf++ = Uart1GetCh();
			//Trace("\r\n++*buf=%x",*buf);
			//Trace(" %#02x",*(buf-1));
		}		
	}
	Uart1_ClrBuf();	
	//Trace("\r\n++READ=End");
	return count;
	
}

/*****************************************************************************
** Function name:	Uart1_SendByte	
**
** Descriptions:	发送一字节	
**						
**
** parameters:		byte	
** Returned value:	无
** 
*****************************************************************************/
void Uart1_SendByte(uint8_t byte) 
{
   while(!(LPC_UART1->LSR & UART_LSR_THRE));
   LPC_UART1->THR = byte;
   //Trace(" %#02x",byte);//120817 by cq DebugTrace
}

/*****************************************************************************
** Function name:	Uart1_Write	
**
** Descriptions:	发送数据块	
**						
**
** parameters:		buf:数据指针,lenth:数据长度
** Returned value:	无
** 
*****************************************************************************/
void Uart1_Write(uint8_t *buf,uint8_t lenth) 
{
   while(lenth) {
      Uart1_SendByte(*buf);
   	  buf++;
	  lenth--;
   }
}

