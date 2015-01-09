/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Changer.c
** Last modified Date:  2011-10-26
** Last Version:         
** Descriptions:        找零器驱动程序
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-10-26
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
#include "..\config.h"
#include "global.h"
#include "evb.h"
#include "mdb.h"
#include "Hopper.h"
//#include "HopperApp.h"//120401 by cq change structor
#include "MdbCoin.h"
//#include "MdbCoinApp.h"//120401 by cq change structor
#include "Changer.h"
#include "VMC_PC.h"

#define  CHANGERNUM  8

uint8_t  ChangerpBuf[CHANGERNUM];
uint8_t  ChangerCommErr[CHANGERNUM];
uint8_t  ChangerStat[CHANGERNUM];
uint8_t  ChangerDisStat[CHANGERNUM];
uint32_t ChangerLeftAmount;

extern uint8_t CoinAcceptor_GetCoinDecimal(void);  
/*****************************************************************************
** Function name:	Changer_StatInit	
**
** Descriptions:	找零器状态初始化
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_StatInit(void)
{
	uint8_t i;

	for(i = 0; i < CHANGERNUM; i++) 
	{
		ChangerpBuf[i] = 0;
		ChangerCommErr[i] = FALSE;
		ChangerStat[i] = CHANGER_OK;
		ChangerDisStat[i] = FALSE;
	}	
}

/*****************************************************************************
** Function name:	Changer_CommErr	
**
** Descriptions:	用于判断是否有通讯故障
**						
**
** parameters:		chn:找零器编号
** Returned value:	TRUE:故障 FALSE:正常	
** 
*****************************************************************************/
uint8_t Changer_CommErr(uint8_t chn)
{	
	if(chn < CHANGERNUM) 
	{
		return ChangerCommErr[chn];
	} 
	else 
	{
		return TRUE;
	}
}

/*****************************************************************************
** Function name:	Changer_Dis	
**
** Descriptions:	用于判断找零器是否被禁能
**						
**
** parameters:		chn:找零器编号
** Returned value:	TRUE:禁能 FALSE:正常	
** 
*****************************************************************************/
uint8_t Changer_Dis(uint8_t chn)
{	
	if(chn < CHANGERNUM) 
	{
		return ChangerDisStat[chn];
	} 
	else 
	{
		return TRUE;
	}
}

/*****************************************************************************
** Function name:	Changer_SetCommErr	
**
** Descriptions:	设置找零器通讯故障
**						
**
** parameters:		chn:找零器编号
** Returned value:	无	
** 
*****************************************************************************/
void Changer_SetCommErr(uint8_t chn)
{
	if(chn < CHANGERNUM) 
	{
		ChangerCommErr[chn] = TRUE;
	}
}

/*****************************************************************************
** Function name:	Changer_SetDisStat	
**
** Descriptions:	设置找零器禁能状态
**						
**
** parameters:		chn:找零器编号
** Returned value:	无	
** 
*****************************************************************************/
void Changer_SetDisStat(uint8_t chn)
{
	if(chn < CHANGERNUM) 
	{
		ChangerDisStat[chn] = TRUE;
	}
}

/*****************************************************************************
** Function name:	Changer_UpDateStat	
**
** Descriptions:	更新找零器状态
**						
**
** parameters:		status:状态值 chn:找零器编号
** Returned value:	无	
** 
*****************************************************************************/
void Changer_UpDateStat(uint8_t status, uint8_t chn)
{
	if(chn < CHANGERNUM)	
	{
		ChangerStat[chn] = status;
	}
}

/*****************************************************************************
** Function name:	Changer_UpDatepBuf	
**
** Descriptions:	更新找零器出币缓存
**						
**
** parameters:		num:出币数量 chn:找零器编号
** Returned value:	无	
** 
*****************************************************************************/
void Changer_UpDatepBuf(uint8_t num, uint8_t chn)
{
	if(chn < CHANGERNUM) 
	{
		ChangerpBuf[chn] += num;
	}	
	//Trace("\r\n changebuf[0]=%x,changebuf[1]=%x,changebuf[2]=%x,changebuf[3]=%x,changebuf[4]=%x,changebuf[5]=%x,changebuf[6]=%x,changebuf[7]=%x",ChangerpBuf[0],ChangerpBuf[1],ChangerpBuf[2],ChangerpBuf[3],ChangerpBuf[4],ChangerpBuf[5],ChangerpBuf[6],ChangerpBuf[7]);
		   
}

/*****************************************************************************
** Function name:	Changer_ClrpBuf	
**
** Descriptions:	清空找零器出币缓存
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_ClrpBuf(void)
{
	uint8_t i;
	
	for(i = 0; i < CHANGERNUM; i++) 
	{
		ChangerpBuf[i] = 0;
	}
}

/*****************************************************************************
** Function name:	Changer_UpDateLeftAmount	
**
** Descriptions:	更新剩币量
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_UpDateLeftAmount(void)
{
	   uint16_t tbstat;
	 	 
	   tbstat = MdbCoin_CheckTubeStat(&ChangerLeftAmount);

	   if(CashTypeEnBit.CoinFullBit != tbstat) 
	   {
		   CashTypeEnBit.CoinFullBit = tbstat;
		   						   
		   OSTimeDly(OS_TICKS_PER_SEC / 10);
		   //币满处理,原先相应通道币满后，会把对应通道禁能。现在不予禁能;by gzz 20121025
		   //CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN & (~CashTypeEnBit.CoinFullBit);
		   CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;

			
			Trace("\r\nif(CashTypeEnBit.CoinFullBit != tbstat)");
		   
	       MdbCoin_Enable(CashTypeEnBit.CoinType);	         //使能硬币器相应币值
	   }	  	   

	   g_CoinLeft = ChangerLeftAmount;
	   g_CoinRealLeft = ChangerLeftAmount;//显示币斗剩余币值;by gzz 20130105

	   if(g_CoinLeft > SYSPara.MoneyMaxin) 
	   {
          g_CoinLeft = SYSPara.MoneyMaxin;
       }
}

/*****************************************************************************
** Function name:	Changer_UpDateSysPara	
**
** Descriptions:	更新状态到SysPara
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_UpDateSysPara(void)
{
	uint8_t j,chnbase = 0;
	uint32_t  MoneyOut = 0;
	
	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			chnbase = CHANGE_HOPPER_BASE;
			break;

		case MDB_CHANGER_EN:
			chnbase = CHANGE_COIN_BASE;
			break;

		default:
			break;
	}
	
	for(j = 0; j < CHANGERNUM; j++) 
	{
	  if(BeSaleStat()) 
	  {
		  TranData.OutcomeCH[chnbase+j] += ChangerpBuf[j];
	      TranTotal.OutcomeCH[chnbase+j] += ChangerpBuf[j];
	  }
      //MoneyOut += SYSPara.CoinsValue[j] * ChangerpBuf[j];	//实际出币值
	  MoneyOut += OutcomeValue[chnbase+j] * ChangerpBuf[j];						  
    }
    if(BeSaleStat()) 
	{
	   TranTotal.TotalChange += MoneyOut;

	   //用于季度结账用;by gzz 20121009
	   TranTotal.TotalChangeJidu += MoneyOut;
    }
	Trace("\r\n UpDatenow payout");

    MsgBack.pBackMsg = MoneyOut;
	OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

	Changer_ClrpBuf();
}

/*****************************************************************************
** Function name:	Changer_UpDateSysStat	
**
** Descriptions:	更新状态到SysPara
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_UpDateSysStat(void)
{
	uint8_t i;
	
	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			for(i = 0; i < HOPPERNUM; i++) 
			{
				SYSPara1.HopperStat[i] = ChangerStat[i];
				if(Changer_CommErr(i) == TRUE) 
				{
					SYSPara1.HopperStat[i] |= HOPPER_COMMERR_BIT;
				}
				if(Changer_Dis(i) == TRUE) 
				{
					SYSPara1.HopperStat[i] = HOPPER_DISABLE_BIT;
				}
			}		
			break;

		case MDB_CHANGER_EN:		
			break;

		default:
			break;
	}

	for(i = 0; i < HOPPERNUM; i++) 
	{
		if(Changer_Dis(i) == TRUE) 
		{
			SYSPara1.HopperStat[i] = HOPPER_DISABLE_BIT;
		}
	}
}

/*****************************************************************************
** Function name:	Changer_LowStat	
**
** Descriptions:	用于判断找零器是否不足
**						
**
** parameters:		无
** Returned value:	TRUE:不足 FALSE:正常	
** 
*****************************************************************************/
uint8_t Changer_LowStat(void)
{
	uint8_t status = FALSE;

	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			if((SYSPara1.HopperStat[g_ChangeMinCh] & HOPPER_INSUFFICIENT_BIT)) 
			{
				status = TRUE;
			} 
			else 
			{
				status = FALSE;
			}
			break;

		case MDB_CHANGER_EN:
			//f4故障临界;by gzz 20130106
			//Trace("\r\n **zuishao=%d,g_CoinLeft=%d",CoinAcceptor_GetCoinScale() * MDB_COINNUMMIN,g_CoinLeft);	
			//中国:(最小可找硬币)50分*最少硬币数量(例如10)=500分，g_CoinLeft中一枚5角的可找零金额为50分，所以硬币斗只剩10枚50分返回true缺币，只剩11枚50分返回false不缺币
			//土耳其:(最小可找硬币)5分*最少硬币数量(例如10)=50分，g_CoinLeft中一枚5分的可找零金额为5分，所以硬币斗只剩10枚5分返回true缺币，只剩11枚5分返回false不缺币
			if(g_CoinLeft <= CoinAcceptor_GetCoinScale() * MDB_COINNUMMIN) 
			{
				status = TRUE;
			} 
			else 
			{
				status = FALSE;
			}
			break;

		default:
			break;
	}
	return status;
}

/*****************************************************************************
** Function name:	Changer_ErrStat	
**
** Descriptions:	用于判断找零器是否故障
**						
**
** parameters:		无
** Returned value:	TRUE:不足 FALSE:正常	
** 
*****************************************************************************/
uint8_t Changer_ErrStat(void)
{
	uint8_t status = FALSE;

	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			if(SYSPara1.HopperStat[g_ChangeMinCh] & HOPPER_MALFUN_MASK) 
			{
				status = TRUE;
			} 
			else 
			{
				status = FALSE;
			}
			break;

		case MDB_CHANGER_EN:		
			break;

		default:
			break;
	}
	return status;
}

/*****************************************************************************
** Function name:	Changer_CheckStat	
**
** Descriptions:	更新找零器状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_CheckStat(void)
{
	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			if(Changer_ErrStat() == TRUE) 
			{
				CashState.ChangerState |= CHANGER_ER;	 
			} 
			else 
			{
			    CashState.ChangerState &= ~CHANGER_ER;	
			}
			if(Changer_LowStat() == TRUE) 
			{
				CashState.ChangerState |= CHANGER_LOW;	 
			} 
			else
			{
			    CashState.ChangerState &= ~CHANGER_LOW;			
			}
			break;

		case MDB_CHANGER_EN:
			Changer_UpDateLeftAmount();
			if(Changer_LowStat() == TRUE) 
			{
			    CashState.ChangerState &= ~CHANGER_LOW;
		    } 
			else
			{
			    CashState.ChangerState |= CHANGER_LOW;
		    }
			break;

		default:
			CashState.ChangerState |= CHANGER_NONE;
		break;
	}	
}

/*****************************************************************************
** Function name:	Changer_Init	
**
** Descriptions:	找零器初始化
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void Changer_Init(void)
{
	uint8_t i;
	
	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:
			EVB1_MODE_SET();//120821 by cq HoppeyUse
			OSTimeDly(2);
			Hopper_InitProc();		
			MDB_MODE_SET();
			OSTimeDly(2);
			break;

		case MDB_CHANGER_EN:
			for(i = 0; i < HOPPERCHMAX; i++) 
			{
			    Changer_SetDisStat(i);			 //Hopper禁能
			}
			break;

		default:
			for(i = 0; i < HOPPERCHMAX; i++) 
			{
			    Changer_SetDisStat(i);			 //Hopper禁能
			}		
			break;
	}
	
	Changer_UpDateSysStat();
	Changer_CheckStat();	
}

/*****************************************************************************
** Function name:	Changer_Payout	
**
** Descriptions:	找零
**						
**
** parameters:		moneyout:找零值
** Returned value:	无	
** 
*****************************************************************************/
void Changer_Payout(uint32_t moneyout)
{
	uint32_t changer1,changer2;
	switch(SYSPara.ChangerEnable) 
	{
		case HOPPER_CHANGER_EN:										   
			EVB1_MODE_SET();//120821 by cq HoppeyUse
			OSTimeDly(2);
			Hopper_PayoutProc(moneyout);
			MDB_MODE_SET();
			OSTimeDly(2);
			break;

		case MDB_CHANGER_EN:
			Trace("\r\ncase MDB_CHANGER_EN: moneyout = %ld",moneyout);
			MdbCoin_PayoutProc(moneyout);
			break;

		default:
			break;
	}
	//如果pc有开启，上报pc找零
	if( SYSPara.PCEnable == 1 )//120620 by cq PCcommunication
	{
		if(SYSPara.Decimal==1)
		{
		  	changer1 = (ChangerpBuf[0] * OutcomeValue[0]) * 10;
			changer2 = (ChangerpBuf[1] * OutcomeValue[1]) * 10;
		}
		else if(SYSPara.Decimal==2)
		{
		  	changer1 = ChangerpBuf[0] * OutcomeValue[0]; 
			changer2 = ChangerpBuf[1] * OutcomeValue[1];
		}		
	     VPMission_Payout_RPT( VP_DEV_COIN, changer1, changer2  );
	}
	Changer_UpDateSysStat();	
	Changer_UpDateSysPara();	
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	Changer_CheckStat();	
}
