/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           gprs.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        GPRS
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
#include "..\driver\lpc_types.h"
#include "..\driver\UartDev.h"
#include "..\config.h"
#include "global.h"
#include "gprs.h"
//#include "uart0.h"


void GPRS_Test(void)
{
    int  sizeTx,sizeRx;
	char GPRSSndBuf[16],GPRSRcvBuf[32];

	extern UartDev_IOB *pUart;
				
	//GPRS_POWEROFF();
	OSTimeDly(OS_TICKS_PER_SEC * 2);
	//GPRS_POWERON();				
	//GPRS_POWERKEY_CLR();
 	OSTimeDly(OS_TICKS_PER_SEC * 3);
	//GPRS_POWERKEY_SET();
	OSTimeDly(OS_TICKS_PER_SEC * 3);

	while(1) {	   				
		strcpy(GPRSSndBuf,"AT\r");
		sizeTx = strlen(GPRSSndBuf);
		//UART0_Write(GPRSSndBuf,sizeTx);
		//pUart->BufTx ((void *)GPRSSndBuf, &sizeTx, UartDev_FLAG_NONBLOCKING);
	    OSTimeDly(OS_TICKS_PER_SEC * 2);
		sizeRx = 20;
		pUart->BufRx ((void *)GPRSRcvBuf, &sizeRx, UartDev_FLAG_NONBLOCKING);
		if(sizeRx > 0) {
			 //UART0_Write(GPRSRcvBuf,sizeRx);
			 if(!strncmp(GPRSRcvBuf,"AT\r\r\nOK\r\n",9)) {
			     BEEPON();
				 OSTimeDly(OS_TICKS_PER_SEC / 5);
				 BEEPOFF();
				 break;
			 }
		} 
        OSTimeDly(OS_TICKS_PER_SEC * 2);
	}
	while(1) {
	    strcpy(GPRSSndBuf,"AT+CSQ\r");
		sizeTx = strlen(GPRSSndBuf);
		//UART0_Write(GPRSSndBuf,sizeTx);
		//pUart->BufTx ((void *)GPRSSndBuf, &sizeTx, UartDev_FLAG_NONBLOCKING);
	    OSTimeDly(OS_TICKS_PER_SEC * 2);
		sizeRx = 32;
		pUart->BufRx ((void *)GPRSRcvBuf, &sizeRx, UartDev_FLAG_NONBLOCKING);
		//UART0_Write(GPRSRcvBuf,sizeRx);
		
		OSTimeDly(OS_TICKS_PER_SEC * 2);
	}
}
