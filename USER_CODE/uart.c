/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        串口驱动程序
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
#include "mdb.h"
#include "uart.h"

#define  UART2_MDB_MODE	  0
#define  UART2_EVB_MODE	  1

#define  UART3_BUFLEN     255
#define  UART2_BUFLEN     36

volatile uint8_t  Uart3_RdBuf[UART3_BUFLEN];
volatile uint16_t  Uart3_RIndex;
volatile uint16_t  Uart3_FIndex;
volatile uint8_t  Uart2_RdBuf[UART2_BUFLEN];
volatile uint8_t  Uart2_RIndex;
volatile uint8_t  Uart2_FIndex;
volatile uint8_t  Uart2_Mode;
volatile uint8_t  MdbStatus = MDB_RCV_INIT;

/*****************************************************************************
** Function name:	Uart2_IRQHandler	
**
** Descriptions:	串口2中断处理函数	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart2_IRQHandler(void) 
{
	volatile uint8_t dummy;
	uint32_t intsrc, tmp, tmp1;
	uint8_t  udata;
	static   uint8_t  checksum;
		
	OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();

	/* Determine the interrupt source */
	intsrc = LPC_UART2->IIR;
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS) 
	{
		tmp1 = LPC_UART2->LSR;
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		if(tmp1 & UART_LSR_PE) 
		{
			udata = LPC_UART2->RBR & UART_RBR_MASKBIT;			
			if(Uart2_Mode == UART2_MDB_MODE) 
			{				
				if(MdbStatus == MDB_RCV_INIT) 
				{
					Uart2_RdBuf[Uart2_RIndex++] = udata;
					MdbStatus = MDB_RCV_OK;
					Mdb_TransmitByte(MDB_ACK,MDB_DAT);
				} 
				else if(MdbStatus == MDB_RCV_DATA) 
				{
					if(Uart2_RIndex < UART2_BUFLEN)	
					{
						Uart2_RdBuf[Uart2_RIndex++] = udata;
						if(udata == checksum) 
						{
							MdbStatus = MDB_RCV_OK;
							Mdb_TransmitByte(MDB_ACK,MDB_DAT);
						} 
						else 
						{		
							//120716 by cq ICTCompatible
						
							MdbStatus = MDB_CHKSUM_ERR;
							Mdb_TransmitByte(MDB_NAK,MDB_DAT);	

							//MdbStatus = MDB_RCV_OK;
							//Mdb_TransmitByte(MDB_ACK,MDB_DAT);
						}
					} 
					else 
					{
						MdbStatus = MDB_OUT_RANGE;
						Mdb_TransmitByte(MDB_NAK,MDB_DAT);
					}
				}	
			}
		}
		else if(tmp1) 
		{		
		    dummy = LPC_UART2->RBR & UART_RBR_MASKBIT;
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) 
	{	    
		if(Uart2_RIndex < UART2_BUFLEN)	
		{		    
			udata = LPC_UART2->RBR & UART_RBR_MASKBIT;
			Uart2_RdBuf[Uart2_RIndex++] = udata;
			if(Uart2_Mode == UART2_MDB_MODE) 
			{
				if(MdbStatus == MDB_RCV_INIT) 
				{
					checksum = 0;
				}
				MdbStatus = MDB_RCV_DATA;
				checksum += udata;
			}
		} 
		else
		{
		    dummy = LPC_UART2->RBR & UART_RBR_MASKBIT;
			if(Uart2_Mode == UART2_MDB_MODE)
			{
				MdbStatus = MDB_OUT_RANGE;
				Mdb_TransmitByte(MDB_NAK,MDB_DAT);
			}
		}
	}

    OSIntExit();
    OS_EXIT_CRITICAL();	//   ucosii add
}

/*****************************************************************************
** Function name:		Uart2_Init
**
** Descriptions:		串口初始化
**						
**
** parameters:			
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
void Uart2_Init(void)
{
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART2,ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART2,CLKPWR_PCLKSEL_CCLK_DIV_4);

	LPC_PINCON->PINSEL0 &= ~0x00F00000;
	LPC_PINCON->PINSEL0 |= 0x00500000;       /* RxD2 is P0.11 and TxD2 is P0.10 */

	LPC_UART2->LCR = UART_LCR_DLAB_EN;		 /* DLAB = 1 */
    LPC_UART2->DLM = 0;							
    LPC_UART2->DLL = 125;
    LPC_UART2->FDR = 0x41;							
	LPC_UART2->LCR = UART_LCR_WLEN8;		/* 8 bits, no Parity, 1 Stop bit */
	LPC_UART2->FCR = UART_FCR_TX_RS|UART_FCR_RX_RS|UART_FCR_FIFO_EN;		/* Enable and reset TX and RX FIFO. */

    LPC_UART2->IER = UART_IER_RBRINT_EN | UART_IER_RLSINT_EN;	 /* Enable UART2 interrupt */
    zyIsrSet((UART2_IRQn +16), (unsigned long)Uart2_IRQHandler, PRIO_TEN);
	zyIrqEnable();
}

/*****************************************************************************
** Function name:		Uart2_ReadMdbStat
**
** Descriptions:		读取当前MDB接收状态
**						
**
** parameters:			无
** Returned value:		当前MDB接收状态
**							
*****************************************************************************/
uint8_t Uart2_ReadMdbStat(void) 
{
	uint8_t status;

	status = MdbStatus;
	MdbStatus = MDB_RCV_INIT;

	return status;
}

/*****************************************************************************
** Function name:		Uart2_SetMdbMode
**
** Descriptions:		设置串口为MDB接收状态
**						
**
** parameters:			无
** Returned value:		无
**							
*****************************************************************************/
void Uart2_SetMdbMode(void) 
{
	Uart2_Mode = UART2_MDB_MODE;
}

/*****************************************************************************
** Function name:		Uart2_SetEvbMode
**
** Descriptions:		设置串口为EVB接收状态
**						
**
** parameters:			无
** Returned value:		无
**							
*****************************************************************************/
void Uart2_SetEvbMode(void) 
{
	Uart2_Mode = UART2_EVB_MODE;
}

/*****************************************************************************
** Function name:		Uart2_SetParity
**
** Descriptions:		设置串口奇偶校验位
**						
**
** parameters:			mode:奇偶校验设置
** Returned value:		无
**							
*****************************************************************************/
void Uart2_SetParity(uint8_t mode) 
{
	switch(mode) 
	{		
		case PARITY_ODD:
		LPC_UART2->LCR = UART_LCR_PARITY_ODD|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
		break;

		case PARITY_EVEN:
		LPC_UART2->LCR = UART_LCR_PARITY_EVEN|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
		break;

		case PARITY_F_1:
		LPC_UART2->LCR = UART_LCR_PARITY_F_1|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
		break;

		case PARITY_F_0:
		LPC_UART2->LCR = UART_LCR_PARITY_F_0|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
		break;

		default:
		LPC_UART2->LCR = UART_LCR_WLEN8;
		break;
	}
}

/*****************************************************************************
** Function name:	Uart2_TransWait	
**
** Descriptions:	等待发送一字节数据结束	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart2_TransmitWait(void) 
{
   while(!(LPC_UART2->LSR & UART_LSR_TEMT));
}

/*****************************************************************************
** Function name:	Uart2_SendByte	
**
** Descriptions:	发送一字节数据	
**						
**
** parameters:		byte	
** Returned value:	无
** 
*****************************************************************************/
void Uart2_SendByte(uint8_t byte) 
{
   while(!(LPC_UART2->LSR & UART_LSR_THRE));
   LPC_UART2->THR = byte;
}

/*****************************************************************************
** Function name:	Uart2_Write	
**
** Descriptions:	发送数据块	
**						
**
** parameters:		buf:数据指针,lenth:数据长度	
** Returned value:	无
** 
*****************************************************************************/
void Uart2_Write(uint8_t *buf,uint8_t lenth) 
{
   while(lenth) 
   {
   		//Trace(" %02x",*buf);
      Uart2_SendByte(*buf);
   	  buf++;
	  lenth--;
   }
}

/*****************************************************************************
** Function name:	Uart2_Read	
**
** Descriptions:	读取串口2缓存数据	
**						
**
** parameters:		buf:数据指针,lenth:数据长度	
** Returned value:	无
** 
*****************************************************************************/
uint8_t Uart2_Read(uint8_t *buf, uint8_t len) 
{
	uint8_t count,i;

	count = Uart2_RIndex - Uart2_FIndex;
	
	if((len == 0) || (count == 0)) 
	{
	    return 0;
	} 
	else if(len > count) 
	{
	    len = count;
	}
	for(i = 0; i < len; i++)	
	{
	    *buf++ = Uart2_RdBuf[Uart2_FIndex++];
		//Trace(" %02x",*(buf-1));
	}

	if(Uart2_FIndex == Uart2_RIndex) 
	{
		Uart2_FIndex = Uart2_RIndex	= 0;
	}
	return len;
}

/*****************************************************************************
** Function name:	Uart2_ClrBuf	
**
** Descriptions:	清除串口2缓存数据	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart2_ClrBuf(void) 
{
	Uart2_FIndex = Uart2_RIndex	= 0;
	MdbStatus = MDB_RCV_INIT;
}

/*****************************************************************************
** Function name:	Uart3_ClrBuf	
**
** Descriptions:	清除串口3缓存数据	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart3_ClrBuf(void) 
{
	Uart3_FIndex = 0;
	Uart3_RIndex = 0;
	memset(Uart3_RdBuf,0,sizeof(Uart3_RdBuf));
}

/*****************************************************************************
** Function name:	Uart3_IRQHandler	
**
** Descriptions:	串口3中断处理函数	
**						
**
** parameters:		无	
** Returned value:	无
** 
*****************************************************************************/
void Uart3_IRQHandler(void) 
{
	volatile uint8_t dummy;
	uint32_t intsrc, tmp, tmp1;
		
	OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();

	/* Determine the interrupt source */
	intsrc = LPC_UART3->IIR;
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS) 
	{
		tmp1 = LPC_UART3->LSR;
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		if (tmp1) 
		{
		    dummy = LPC_UART3->RBR & UART_RBR_MASKBIT;
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) 
	{
	    if(Uart3_RIndex < UART3_BUFLEN)	
		{
		    Uart3_RdBuf[Uart3_RIndex++] = LPC_UART3->RBR & UART_RBR_MASKBIT;
		}
		else
		{
		    dummy = LPC_UART3->RBR & UART_RBR_MASKBIT;
		}
	}

    OSIntExit();
    OS_EXIT_CRITICAL();	//   ucosii add
}

/*****************************************************************************
** Function name:		Uart3_Init
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
void Uart3_Init(void)
{	
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART3,ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART3,CLKPWR_PCLKSEL_CCLK_DIV_4);
   
    Uart3_RIndex = Uart3_FIndex = 0;
	
	LPC_PINCON->PINSEL0 &= ~0x0F;
    LPC_PINCON->PINSEL0 |= 0x0A;

	LPC_UART3->LCR = UART_LCR_DLAB_EN;		/* DLAB = 1 */
    LPC_UART3->DLM = 0;							
    LPC_UART3->DLL = 125;
    LPC_UART3->FDR = 0x41;						

	//LPC_UART3->LCR = UART_LCR_PARITY_EVEN|UART_LCR_PARITY_EN|UART_LCR_WLEN7;		/* 7 bits, Even Parity, 1 Stop bit */
	//LPC_UART3->FCR = UART_FCR_TX_RS|UART_FCR_RX_RS|UART_FCR_FIFO_EN;		/* Enable and reset TX and RX FIFO. */

    //LPC_UART3->IER = UART_IER_RBRINT_EN;	 /* Enable UART3 interrupt */
    LPC_UART3->LCR = UART_LCR_WLEN8;		/* 8 bits, no Parity, 1 Stop bit */
	LPC_UART3->FCR = UART_FCR_TX_RS|UART_FCR_RX_RS|UART_FCR_FIFO_EN|UART_FCR_TRG_LEV0;		/* Enable and reset TX and RX FIFO. */

    LPC_UART3->IER = UART_IER_RBRINT_EN | UART_IER_RLSINT_EN;	 /* Enable UART3 interrupt */

    zyIsrSet((UART3_IRQn +16), (unsigned long)Uart3_IRQHandler, PRIO_TEN);
	zyIrqEnable();
	Uart3_ClrBuf();
}


uint8_t Uart3GetCh( void )
{
	uint8_t ch;
	ch = Uart3_RdBuf[ Uart3_FIndex ];
	Uart3_FIndex ++;
	if ( Uart3_FIndex >= UART3_BUFLEN ) Uart3_FIndex = 0;	
	return ch;
	
}

uint8_t Uart3IsRxBufNull( void )
{
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	if ( Uart3_RIndex - Uart3_FIndex == 0 ) 
		return 1;
	else
		return 0;
}



/*****************************************************************************
** Function name:	Uart3_Read	
**
** Descriptions:	读取串口3缓存数据	
**						
**
** parameters:		buf:数据指针,lenth:数据长度	
** Returned value:	无
** 
*****************************************************************************/
uint8_t Uart3_Read(uint8_t *buf, uint8_t len) 
{
	uint8_t count,i,j;
	count=0;


	while( !Uart3IsRxBufNull() )
	{		
		j=Uart3_RIndex - Uart3_FIndex;
		count += j;
		for( i=0; i<j; i++ )
		{
			*buf++ = Uart3GetCh();
		}		
	}
	Uart3_ClrBuf();	
	return count;
	
}

/*****************************************************************************
** Function name:	Uart3_SendByte	
**
** Descriptions:	发送一字节	
**						
**
** parameters:		byte	
** Returned value:	无
** 
*****************************************************************************/
void Uart3_SendByte(uint8_t byte) 
{
   while(!(LPC_UART3->LSR & UART_LSR_THRE));
   LPC_UART3->THR = byte;
}

/*****************************************************************************
** Function name:	Uart3_Write	
**
** Descriptions:	发送数据块	
**						
**
** parameters:		buf:数据指针,lenth:数据长度
** Returned value:	无
** 
*****************************************************************************/
void Uart3_Write(uint8_t *buf,uint8_t lenth) 
{
   while(lenth) {
      Uart3_SendByte(*buf);
   	  buf++;
	  lenth--;
   }
}

