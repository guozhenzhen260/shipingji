/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           mdb.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        mdb协议驱动程序
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
#include "..\driver\UartDev.h"
#include "..\config.h"
#include "global.h"
#include "uart.h"
#include "mdb.h"

uint8_t MdbBuf[MAX_BLOCK_SIZE];		 //MDB数据缓存
/*****************************************************************************
** Function name:	Mdb_TransmitByte	
**
** Descriptions:	MDB发送字节	
**					
**
** parameters:		dat:发送的数据 mode:模式（MDB_ADD:地址或MDB_DAT:数据）
** Returned value:	无
** 
*****************************************************************************/
void Mdb_TransmitByte(uint8_t dat,uint8_t mode)
{
	Trace_R(" %02x",dat);//120914 by cq Cashless
	switch(mode) 
	{
		case MDB_DAT:
		Uart2_SetParity(PARITY_F_0);
		Uart2_SendByte(dat);
		break;

		default:
		Uart2_SetParity(PARITY_F_1);
		Uart2_SendByte(dat);
		break;
	}

	Uart2_TransmitWait();
}

/*****************************************************************************
** Function name:	Mdb_Session	
**
** Descriptions:	会话，发送命令，并接收响应	
**					
**
** parameters:		add:MDB命令 dat[]:数据 count:数据长度
** Returned value:	正确响应返回接收数据长度；否则返回错误码
** 
*****************************************************************************/
uint8_t Mdb_Session(uint8_t add,uint8_t dat[],uint8_t count)
{
   uint8_t check,len,i, j, err; 

   #if MDBDEBUG
   int sizeTx;
   extern UartDev_IOB *pUart;
   #endif

   Uart2_ClrBuf();
   Uart2_SetMdbMode();
  
   for(j = 0; j < 8; j++) 
   {    	   
	  #if MDBDEBUG
	  sizeTx = 1;
	  //pUart->BufTx ((void *)&add, &sizeTx, UartDev_FLAG_NONBLOCKING);
	
	  sizeTx = count;
	  //pUart->BufTx ((void *)dat, &sizeTx, UartDev_FLAG_NONBLOCKING);
	  #endif  

      err = 0;
	  check = 0;
      Mdb_TransmitByte(add,MDB_ADD);
      check += add; 
      for(i = 0; i < count; i++) 
	  { 
         Mdb_TransmitByte(dat[i],MDB_DAT);
	     check += dat[i];
      }
      Mdb_TransmitByte(check,MDB_DAT);

	  #if MDBDEBUG
	  sizeTx = 1;
	  //pUart->BufTx ((void *)&check, &sizeTx, UartDev_FLAG_NONBLOCKING);
	  #endif
     
      OSTimeDly(OS_TICKS_PER_SEC / 5);
	  
	  len = Uart2_Read(MdbBuf,MAX_BLOCK_SIZE);
	  
	  //120914 by cq Cashless
	  Trace_R("\r\nR = %02x - ",len); 
	  for(i = 0; i < len; i++)
		  Trace_R(" %02x",MdbBuf[i]);
	  
	  err = Uart2_ReadMdbStat();
	  Trace_R(" 	err = %02x",err);//120914 by cq Cashless
  
	  if(err == MDB_RCV_OK) 
	  {
	    #if MDBDEBUG
		sizeTx = len;
		//pUart->BufTx ((void *)MdbBuf, &sizeTx, UartDev_FLAG_NONBLOCKING);
		#endif
		break;
      }//if(err == MDB_RCV_OK)

      OSTimeDly(OS_TICKS_PER_SEC / 10);
   }//for j

   return (err != MDB_RCV_OK)? err : (len-1);
}

/*****************************************************************************
** Function name:	Mdb_ReadBuf	
**
** Descriptions:	读取MDB数据
**						
**
** parameters:		buf：数据缓存 len：数据长度
** Returned value:	无	
** 
*****************************************************************************/
void Mdb_ReadBuf(uint8_t *buf, uint8_t len)
{
	uint8_t i;

	if(len > MAX_BLOCK_SIZE) 
	{
		len = MAX_BLOCK_SIZE;
	}

	for(i = 0; i < len; i++) 
	{
		buf[i] = MdbBuf[i];
	}
} 

/*****************************************************************************
** Function name:	MDB_ErrJudge	
**
** Descriptions:	判断MDB当前接收数据的状态，有没有错误
**						
**
** parameters:		x:MDB命令返回的状态值
** Returned value:	出错返回TRUE；否则返回FALSE	
** 
*****************************************************************************/
uint8_t Mdb_ErrJudge(uint8_t x)
{
  if(x > MAX_BLOCK_SIZE) 
  {
     return TRUE;
  }
  return FALSE;
} 

