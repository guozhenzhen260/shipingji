/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           pulse.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        脉冲驱动程序
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
#include "..\config.h"
#include "global.h"
#include "PulseCoin.h"


#include "..\driver\lpc_types.h"//120401 by cq change structor



/*****************************************************************************
** Function name:		Coin_Accept
**
** Descriptions:		获取投币值
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void Coin_Accept(void)
{
   uint8_t pvalue, pChannel, moneych;
   static uint8_t pLChannel, pSure;  
   
   pvalue = (~CoinP_Get()) & 0x3F;

   //Trace("\r\npvalue = %02x",pvalue);

   switch(pvalue) 
   {
      case 0x00:	  //无投币状态
		  if(pSure == 1) 
		  {	     
			 pSure = 0;
			 moneych = pLChannel-1;
			 MsgBack.coinPlusback = SYSPara.CoinsValue[moneych] | 0x8000;
			 //money = SYSPara.CoinsValue[pChannel-1] | 0x8000;	//money编码（16位）：最高位置1，其他为面值
			 //money = COIN_PULSE_BASE | 0x8000;	//money编码（16位）：最高位置1，其他为通道号
			 if(BeSaleStat()) 
			 {
				 TranData.IncomeCH[COIN_PULSE_BASE+moneych]++;
				 TranTotal.IncomeCH[COIN_PULSE_BASE+moneych]++;
				 TranTotal.Income += SYSPara.CoinsValue[moneych];
				 TranTotal.CoinsIncome += SYSPara.CoinsValue[moneych];

				 //用于季度结账用;by gzz 20121009
	   			 TranTotal.IncomeJidu += SYSPara.CoinsValue[moneych];				 
				 TranTotal.CoinsIncomeJidu += SYSPara.CoinsValue[moneych];
			 }
			 //OSMboxPost(g_KeyMsg,&MsgBack.moneyback);
			 //120621 by cq KeySlowly
			 //while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)
		     {
		   		//OSTimeDly(OS_TICKS_PER_SEC / 100);
		     }			 
			 //OSMboxPost(g_MoneyMsg,&MsgBack.moneyback);
			 OSQPost(g_CoinPlusMsg,&MsgBack.coinPlusback);
		  }
		  if(pLChannel > 0) 
		  {
		      #if BEEPDEBUG
			  BEEPOFF();
			  #endif
		  }
		  pChannel = 0;
		  pLChannel = 0;
		  break;
	  
	  //120712 by cq CoinChannel
	  case 0x01:	   //硬币通道1
		  //pChannel = 1;
		  pChannel = 6;
		  break;

	  case 0x02:	   //硬币通道2
		  //pChannel = 2;
		  pChannel = 5;
		  break;

	  case 0x04:	   //硬币通道3
		  //pChannel = 3;
		  pChannel = 4;		  
		  break;

	  case 0x08:	   //硬币通道4
		  //pChannel = 4;		  
		  pChannel = 3;
		  break;
		  
	  case 0x10:	   //硬币通道5 //1Y -- 2
		  //pChannel = 5;
		  pChannel = 2;
		  break;

	  case 0x20:	   //硬币通道6 //5J -- 1
		  //pChannel = 6;
		  pChannel = 1;
		  break;

	  default:		   //出错
		  pChannel = 0;
		  break;
   }

   if(pSure == 0) 
   {
      //两次不等0且相等，则确认投币成功
      if((pLChannel > 0) && (pLChannel == pChannel)) 
	  {
	     pSure = 1;				
		 #if BEEPDEBUG
		 BEEPON();
		 #endif
	  } 
	  else 
	  {
	     pLChannel = pChannel;
	  }
   }
}





/*初始化脉冲硬币器*/
void Pulse_InitProc(void)
{
    uint8_t i;
	
	#if PULSEDEVICE	
		if((SYSPara1.CoinsStat & COIN_VALUE_BIT) == 0) 
		{
		    Pulse_Enable();						       //使能脉冲硬币器
		}

		for(i = 0; i < COIN_PULSE_NUM; i++) 
		{
		   IncomeValue[COIN_PULSE_BASE+i] = SYSPara.CoinsValue[i];
		}
	#endif
}


/******************************************************************************
**                            End Of File
******************************************************************************/
