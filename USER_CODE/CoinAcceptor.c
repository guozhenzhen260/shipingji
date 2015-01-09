/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           CoinAcceptor.c
** Last modified Date:  2011-10-27
** Last Version:         
** Descriptions:        硬币器驱动程序
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-10-27
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
#include "mdb.h"
#include "MdbCoin.h"
//#include "MdbCoinApp.h"//120401 by cq change structor
#include "PulseCoin.h"
//#include "PulseCoinApp.h"//120401 by cq change structor
#include "CoinAcceptor.h"
#include "VMC_PC.h"


#if 0
//#define IsLessCoinStat()           (g_CoinTypeNum[g_ChangeMinCh] < MDB_COINNUMMIN)
#define IsLessCoinStat()		   (CoinLeft < CoinPara.Decimal * MDB_COINNUMMIN)
#endif

CoinParaStruct CoinPara;

extern uint8_t Changer_LowStat(void);

/*接收到硬币参数结构体*/
struct CoinDeposit
{
	uint8_t Route;	      //有接收到硬币为COINACCEPTOR_STACKED，有退币为COINACCEPTOR_RETURN，否则为0
	uint8_t Channel;	  //硬币通道
} CoinDpst;

uint8_t  CoinRstStat;	  //硬币器刚刚是否复位标志
uint8_t  CoinCommErr;     //硬币器通讯状态 TRUE:正常 FALSE:异常
uint8_t  CoinValueErr;	  //硬币器各通道面值设置是否正确 TRUE:正常 FALSE:异常
uint8_t  CoinTubeFull;    //硬币器储币状态 TRUE:满 FALSE:未满
uint16_t CoinStat;		  //硬币器状态,状态值见硬币器相关协议手册
uint32_t CoinsInTube;	  //储币器硬币总量

/*****************************************************************************
** Function name:	CoinAcceptor_StatInit	
**
** Descriptions:	初始化硬币器状态值
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_StatInit(void)
{
	CoinDpst.Route = 0;
	CoinRstStat = FALSE;
	CoinCommErr = FALSE;
	CoinValueErr = FALSE;
	CoinStat = COIN_STAT_OK;
}

/*****************************************************************************
** Function name:	CoinAcceptor_UpDateDeposit	
**
** Descriptions:	有收到硬币时，设置硬币参数
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_UpDateDeposit(uint8_t route, uint8_t chn)
{
	CoinDpst.Route = route;
	CoinDpst.Channel = chn;
}

/*****************************************************************************
** Function name:	CoinAcceptor_ClrDeposit	
**
** Descriptions:	清除上次收币硬币参数
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_ClrDeposit(void)
{
	CoinDpst.Route = 0;
}

/*****************************************************************************
** Function name:	IsCoinStacked	
**
** Descriptions:	轮询后返回信息，判断是否有投币信息返回
**						
**
** parameters:		无
** Returned value:	TRUE:有投币 FALSE:无	
** 
*****************************************************************************/
uint8_t IsCoinStacked(void)
{
	if(CoinDpst.Route == COINACCEPTOR_STACKED) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	IsCoinReturn	
**
** Descriptions:	轮询后返回信息，判断是否有退币信息返回
**						
**
** parameters:		无
** Returned value:	TRUE:有投币 FALSE:无	
** 
*****************************************************************************/
uint8_t IsCoinReturn(void)
{
	if(CoinDpst.Route == COINACCEPTOR_RETURN) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_CommErr	
**
** Descriptions:	用于判断是否有通讯故障
**						
**
** parameters:		无
** Returned value:	TRUE:故障 FALSE:正常	
** 
*****************************************************************************/
uint8_t CoinAcceptor_CommErr(void)
{
	return CoinCommErr;
}

/*****************************************************************************
** Function name:	CoinAcceptor_RstStat	
**
** Descriptions:	用于判断刚刚是否复位了
**						
**
** parameters:		无
** Returned value:	TRUE:复位了 FALSE:没复位	
** 
*****************************************************************************/
uint8_t CoinAcceptor_RstStat(void)
{
	return CoinRstStat;
}

/*****************************************************************************
** Function name:	CoinAcceptor_ValueSetErr	
**
** Descriptions:	用于判断面值设置是否有误
**						
**
** parameters:		无
** Returned value:	TRUE:设置错误 FALSE:正确	
** 
*****************************************************************************/
uint8_t CoinAcceptor_ValueSetErr(void)
{
	return CoinValueErr;
}

/*****************************************************************************
** Function name:	CoinAcceptor_TubeFull	
**
** Descriptions:	用于判断储币器是否为满
**						
**
** parameters:		无
** Returned value:	TRUE:满 FALSE:未满	
** 
*****************************************************************************/
uint8_t CoinAcceptor_TubeFull(void)
{
	return CoinTubeFull;
}

/*****************************************************************************
** Function name:	CoinAcceptor_Available	
**
** Descriptions:	判断硬币器是否可用
**					通讯是否正常，面值设置是否正确等	
**
** parameters:		无
** Returned value:	TRUE:硬币器可用 FALSE:不可用	
** 
*****************************************************************************/
uint8_t CoinAcceptor_Available(void)
{
	if((CoinCommErr == FALSE) && (CoinValueErr == FALSE))  
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_SetTubeFull	
**
** Descriptions:	设置储币器满状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_SetTubeFull(void)
{
	CoinTubeFull = TRUE;
}

/*****************************************************************************
** Function name:	CoinAcceptor_SetCommErr	
**
** Descriptions:	设置通讯故障状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_SetCommErr(void)
{
	CoinCommErr = TRUE;	
}

/*****************************************************************************
** Function name:	CoinAcceptor_SetValueErr	
**
** Descriptions:	设置面值错误状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_SetValueErr(void)
{
	CoinValueErr = TRUE;	
}

/*****************************************************************************
** Function name:	CoinAcceptor_SetCoinPara	
**
** Descriptions:	设置硬币器参数
**						
**
** parameters:		硬币器参数
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_SetCoinPara(CoinParaStruct coinpara)
{
	CoinPara = coinpara;
}

/*****************************************************************************
** Function name:	CoinAcceptor_GetCoinLevel	
**
** Descriptions:	获取硬币器等级
**						
**
** parameters:		无
** Returned value:	硬币器等级	
** 
*****************************************************************************/
uint8_t CoinAcceptor_GetCoinLevel(void)
{
	return CoinPara.Level;
}

/*****************************************************************************
** Function name:	CoinAcceptor_GetCoinScale	
**
** Descriptions:	获取硬币器比例因子
**						
**
** parameters:		无
** Returned value:	硬币器比例因子	
** 
*****************************************************************************/
uint8_t CoinAcceptor_GetCoinScale(void)
{
	return CoinPara.Scale;
}

/*****************************************************************************
** Function name:	CoinAcceptor_GetCoinDecimal	
**
** Descriptions:	获取硬币器小数位数
**						
**
** parameters:		无
** Returned value:	小数位数为n，则返回10^n	
** 
*****************************************************************************/
uint8_t CoinAcceptor_GetCoinDecimal(void)
{
	return CoinPara.Decimal;
}

/*****************************************************************************
** Function name:	CoinAcceptor_SetRstStat	
**
** Descriptions:	设置硬币器复位状态，表示硬币器刚刚复位了
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_SetRstStat(void)
{
    CoinRstStat = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_CLrRstStat	
**
** Descriptions:	清除硬币器复位状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_ClrRstStat(void)
{
    CoinRstStat = FALSE;
}

/*****************************************************************************
** Function name:	CoinAcceptor_UpDateStat	
**
** Descriptions:	用于更新硬币器找零器通讯状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_UpDateStat(uint16_t stat)
{
	CoinStat = stat;
}

/*****************************************************************************
** Function name:	CoinAcceptor_ReadBuf	
**
** Descriptions:	读取硬币器缓存数据
**						
**
** parameters:		buf:数据缓存，len:数据长度
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_ReadBuf(uint8_t *buf, uint8_t len)
{
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			Mdb_ReadBuf(buf,len);
			break;

		default:
			break;
	}	
}

/*****************************************************************************
** Function name:	CoinAcceptor_ErrJudge	
**
** Descriptions:	判断硬币器通讯状态
**						
**
** parameters:		len:根据相关协议，正常时表示数据长度，异常时表示异常状态
** Returned value:	TRUE:正常 FALSE:出错	
** 
*****************************************************************************/
uint8_t CoinAcceptor_ErrJudge(uint8_t len)
{
	uint8_t status = TRUE;

	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			status = Mdb_ErrJudge(len);
			break;

		default:
			break;
	}
	return status;
}

/*****************************************************************************
** Function name:	CoinAcceptor_UpDateSysStat	
**
** Descriptions:	更新当前硬币器状态到SYSPara.CoinsStat
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_UpDateSysStat(void)
{
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			SYSPara1.CoinsStat = CoinStat;
			if(CoinAcceptor_CommErr() == TRUE) 
			{
				SYSPara1.CoinsStat |= COIN_COMMERR_BIT;
			}
			if(CoinAcceptor_ValueSetErr() == TRUE) 
			{
				SYSPara1.CoinsStat |= COIN_VALUE_BIT;
			}
			if(CoinAcceptor_TubeFull() == TRUE) 
			{
				SYSPara1.CoinsStat |= COIN_FULL_BIT;
			} 
			break;

		default:
			break;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_CheckStat	
**
** Descriptions:	更新CoinStat(CoinStat硬币器状态没有具体的状态码，只是用来判断硬币器是否正常等)
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_CheckStat(void)
{
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			if(SYSPara1.CoinsStat & COIN_VALUE_BIT) 
			{
				CashState.CoinState |= COIN_DEV_PERR;
			}
			break;

		case MDB_COIN_EN:
			if(SYSPara1.CoinsStat != COIN_STAT_OK) 
			{			 //故障
			     if(SYSPara1.CoinsStat & COIN_VALUE_BIT) 
				 {
					 //120703 by cq MDBCompatible
				     //CashState.CoinState |= COIN_DEV_PERR;
				     
					 CashState.CoinState = COIN_DEV_OK;
					 if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
					 {
						 CashState.ChangerState &= ~CHANGER_ER;
					 }
				 }
				 if(SYSPara1.CoinsStat & (~COIN_VALUE_BIT)) 
				 {
				     CashState.CoinState |= COIN_DEV_ER;
				 }
				 if((SYSPara.ChangerEnable == MDB_CHANGER_EN) 
				   && ((SYSPara1.CoinsStat & (~COIN_FULL_BIT)) != COIN_STAT_OK)) 
				 {
			         CashState.ChangerState |= CHANGER_ER;
			     }			 			 
			} 
			else
			{
			     CashState.CoinState = COIN_DEV_OK;
				 if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
				 {
			         CashState.ChangerState &= ~CHANGER_ER;
			     }
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

		default:
			CashState.CoinState |= COIN_DEV_NONE;
			break;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_UpDateTubeStat	
**
** Descriptions:	更新储币管状态及剩币量
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_UpDateTubeStat(void)
{
	   uint16_t tbstat;
	 	 
	   tbstat = CoinAcceptor_CheckTubeStat(CashTypeEnBit.CoinType,&CoinsInTube);

	   if(CashTypeEnBit.CoinFullBit != tbstat) 
	   {
		   CashTypeEnBit.CoinFullBit = tbstat;
		   						   
		   OSTimeDly(OS_TICKS_PER_SEC / 10);
		   //币满处理,原先相应通道币满后，会把对应通道禁能。现在不予禁能;by gzz 20121025
		   //CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN & (~CashTypeEnBit.CoinFullBit);
		   CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;
	       CoinAcceptor_Enable(CashTypeEnBit.CoinType);	         //使能硬币器相应币值
	
		   if(CashTypeEnBit.CoinFullBit && ((CashTypeEnBit.CoinFullBit & CashTypeEnBit.CoinTypeEN) == CashTypeEnBit.CoinTypeEN)) 
		   {
			  CoinAcceptor_SetTubeFull();
		   }
	   }	  	   

	   g_CoinLeft = CoinsInTube;

	   if(g_CoinLeft > SYSPara.MoneyMaxin) 
	   {
          g_CoinLeft = SYSPara.MoneyMaxin;
       }
}

/*****************************************************************************
** Function name:	CoinAcceptor_StackProc	
**
** Descriptions:	有投币时，更新系统数据，处理函数
**						
**
** parameters:		moneych:投币通道
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_StackProc(uint8_t moneych)
{
   uint8_t chnbase = 0;
   uint32_t PcValue = 0;

   switch(SYSPara.CoinEnable) 
   {
		case PULSE_COIN_EN:
			chnbase = COIN_PULSE_BASE;
			break;

		case MDB_COIN_EN:
			chnbase = COIN_MDB_BASE;
			break;

		default:
			break;
	}
   
   if(BeSaleStat()) 
   {
	   TranData.IncomeCH[chnbase+moneych]++;
	   TranTotal.IncomeCH[chnbase+moneych]++;
	   TranTotal.Income += SYSPara.CoinsValue[moneych];
	   TranTotal.CoinsIncome += SYSPara.CoinsValue[moneych];

	   //用于季度结账用;by gzz 20121009
	   TranTotal.IncomeJidu += SYSPara.CoinsValue[moneych];	   
	   TranTotal.CoinsIncomeJidu += SYSPara.CoinsValue[moneych];
	   
	   //如果pc有开启，上报pc收硬币
	   if( SYSPara.PCEnable == 1 )//120620 by cq PCcommunication
	   {
	      if(SYSPara.Decimal==1)
		  {
		  	PcValue = SYSPara.CoinsValue[moneych] * 10;
		  }
		  else if(SYSPara.Decimal==2)
		  {
		  	PcValue = SYSPara.CoinsValue[moneych]; 
		  }
	   	  VPMission_Payin_RPT( VP_DEV_COIN, PcValue );		
	   }
   }   
   
   //MsgBack.moneyback = (SYSPara.CoinsValue[moneych]) | 0x8000;  //money编码（16位）：最高位置1，其他为面值	   
   //120621 by cq KeySlowly
   //while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)   	
   //while( OSMboxPost(g_MoneyMsg,&MsgBack.moneyback) != OS_NO_ERR)
   //{
   //		OSTimeDly(OS_TICKS_PER_SEC / 100);
   //}
   //使用邮箱存mdb硬币;by gzz 20130105
    MsgBack.coinPlusback = (SYSPara.CoinsValue[moneych]) | 0x8000;
	OSQPost(g_CoinPlusMsg,&MsgBack.coinPlusback);
	OSTimeDly(OS_TICKS_PER_SEC / 5);   
	//Trace("\r\n coinPlus=%d", MsgBack.coinPlusback&(0x7FFF));
}

/*****************************************************************************
** Function name:	CoinAcceptor_ReturnProc	
**
** Descriptions:	按退币按钮时更新硬币器状态
**						
**
** parameters:		无
** Returned value:	无	
**
*****************************************************************************/ 
void CoinAcceptor_ReturnProc(void)
{
	uint8_t returnkey;//120621 by cq KeySlowly


   switch(SYSPara.CoinEnable) 
   {
	   case PULSE_COIN_EN:
	   		break;

	   case MDB_COIN_EN:
		   if(CoinAcceptor_GetCoinLevel() < 3) 
		   {
			   if(CoinStat == COIN_JAM)//如果原先有卡币 
			   {
			       CoinAcceptor_UpDateStat(0);
				   CoinAcceptor_UpDateSysStat();							   
				   
				   if(SYSPara1.CoinsStat == COIN_STAT_OK) 
				   {						       
					   ClrDevStatus(DEV_ERR_BIT_COIN);
					   if(Changer_LowStat() == TRUE) 
					   {
					       CashState.ChangerState |= CHANGER_LOW;
						   if(SYSPara.ChangeServer == 0) 
						   {
							   SetDevStatus(DEV_ERR_BIT_COINLOW);
						   }
					   }							   						   
				   
					   if((BeSaleStat()) && IsDeviceOK() 
					       && (CashTypeEnBit.CoinType == 0) && (g_Amount < SYSPara.MoneyMaxin)) 
					   {
					       OSTimeDly(OS_TICKS_PER_SEC / 10);
						   //币满处理,原先相应通道币满后，会把对应通道禁能。现在不予禁能;by gzz 20121025
						   //CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN & (~CashTypeEnBit.CoinFullBit);
						   CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;

						   Trace("if((BeSaleStat()) && IsDeviceOK() && (CashTypeEnBit.CoinType == 0) && (g_Amount < SYSPara.MoneyMaxin))");

						   MdbCoin_Enable(CashTypeEnBit.CoinType);	  //使能	
					   }
				   }				   						   
			   }
		   }
		
		   if((BeSaleStat()) && IsDeviceOK()) 
		   {
			   //120621 by cq KeySlowly
			   //MsgBack.moneyback = RETURN_MONEY_INDEX << 8;
			   MsgBack.keyback = RETURN_MONEY_INDEX << 8;
		       //OSMboxPost(g_KeyMsg,&MsgBack.moneyback);

			   /*
		       while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)
			   {
			   		OSTimeDly(OS_TICKS_PER_SEC / 100);
			   }
			   */
			   returnkey=OSQPost(g_KeyMsg,&MsgBack.keyback);//120621 by cq KeySlowly
			   
		   }
		   break;

	   default:
	   	   break;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_Diagnostic	
**
** Descriptions:	判断硬币器状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_Diagnostic(void)
{
	uint8_t buf[16],lenth,i;
	uint16_t status;
	
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			if(CoinAcceptor_GetCoinLevel() >= 3) 
			{
				lenth = MdbCoin_Diagnostic();	     //诊断状态
				
				if(lenth <= 16) 
				{				      
					Mdb_ReadBuf(buf,lenth);
					for(i = 0; i < lenth-1; i=i+2) 
					{	
						/*
						if((buf[i] == 0x03) && (buf[i+1] == 0)) 
						{
						    CoinAcceptor_UpDateStat(0);
						} 
						//121011 by cq NRITubeRemoved
						else if((buf[i] == 0x06) && (buf[i+1] == 0))
						{
						    CoinAcceptor_UpDateStat(0);
						}
						else if((buf[i] & 0x10) && (buf[i+1] > 0)) 
						{
						    if(!((buf[i] == 0x11) && (buf[i+1] == 0x10))) 
							{
							    status = (buf[i] << 8) | buf[i+1];
								CoinAcceptor_UpDateStat(status);
							    break;
						    }
						} 
						else 
						{
						  //
						}
						*/
						//Trace("\r\n dispense=%d,%d",buf[0],buf[1]);
						if(buf[i] > 0x06)
						{
						    status = (buf[i] << 8) | buf[i+1];
							CoinAcceptor_UpDateStat(status);
						    break;
					    }	
						else
						{
						    CoinAcceptor_UpDateStat(0);
						} 	
					}
				}
			}
			break;

		default:
			break;
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_RstProc	
**
** Descriptions:	交易结束后，硬币器复位后，（重新初始化硬币器），重新使能硬币器
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_RstProc(void)
{
	uint8_t lenth;

	CoinAcceptor_ClrRstStat();
	if(BeSaleStat())
	{
		//币满处理,原先相应通道币满后，会把对应通道禁能。现在不予禁能;by gzz 20121025
       //CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN & (~CashTypeEnBit.CoinFullBit);
       CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;
	   if(CashTypeEnBit.CoinType > 0) 
	   {
           lenth = CoinAcceptor_Enable(CashTypeEnBit.CoinType);	 //使能硬币器相应币值

		   if(CoinAcceptor_ErrJudge(lenth) == TRUE) 
		   {
		   		CoinAcceptor_CommErr();
		   }
	   }
	}
}

/*****************************************************************************
** Function name:	CoinAcceptor_PackAnalysis	
**
** Descriptions:	更新硬币器当前数据包信息，并根据当前纸币器处于的状态，进行下一步的相应操作
**						
**
** parameters:		buf:数据缓存，len:数据长度
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_PackAnalysis(uint8_t *buf, uint8_t len)
{
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			MdbCoin_PackAnalysis(buf,len);
			break;

		default:
			break;
	}

	CoinAcceptor_UpDateSysStat();		
	CoinAcceptor_CheckStat();
	
    if(CoinAcceptor_RstStat() == TRUE) 
	{
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		CoinAcceptor_RstProc();
	}
	
	if(IsCoinStacked()) 
	{
		CoinAcceptor_StackProc(CoinDpst.Channel);
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		CoinAcceptor_UpDateTubeStat();	
	}
	else if(IsCoinReturn() == TRUE) 
	{
	    CoinAcceptor_ReturnProc();	
	} 	
}

/*****************************************************************************
** Function name:	CoinAcceptor_Init	
**
** Descriptions:	硬币器初始化
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_Init(void)
{
	CoinAcceptor_StatInit();
	
	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			Pulse_InitProc();
			break;

		case MDB_COIN_EN:
			MdbCoin_InitProc();
			break;

		default:
			break;
	}
	
	CoinAcceptor_UpDateSysStat();
	CoinAcceptor_CheckStat();	
}

/*****************************************************************************
** Function name:	CoinAcceptor_PollProc	
**
** Descriptions:	轮询硬币器，获取状态，并作出相应处理
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void CoinAcceptor_PollProc(void)
{	
	uint8_t len;
	uint8_t CoinStatBuf[16];
	static uint8_t time = 5;

	CoinAcceptor_ClrDeposit();
	len = CoinAcceptor_Poll(CashTypeEnBit.CoinType);

	if(CoinAcceptor_ErrJudge(len) == TRUE) 
	{
		CoinAcceptor_SetCommErr();
	} 
	else 
	{
		len &= 0x0F;
		CoinAcceptor_ReadBuf(CoinStatBuf,len);
		CoinAcceptor_PackAnalysis(CoinStatBuf,len);
	}
	
	if(time-- == 0) 
	{
		time = 5;
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		CoinAcceptor_Diagnostic();
	}
	
	CoinAcceptor_UpDateSysStat();
	CoinAcceptor_CheckStat();		
}

/*****************************************************************************
** Function name:	CoinAcceptor_Enable	
**
** Descriptions:	硬币器使能
**						
**
** parameters:		cointype:硬币器使能通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
** 
*****************************************************************************/
uint8_t CoinAcceptor_Enable(uint16_t cointype)
{
	uint8_t len = 0;

	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			if(cointype > 0) 
			{
				Pulse_Enable();
			} 
			else 
			{
				Pulse_Disable();
			}
			break;

		case MDB_COIN_EN:
			
			Trace("\r\ncase MDB_COIN_EN:");
			
			len = MdbCoin_Enable(cointype);
			break;

		default:
			break;
	}

	return len;
}

/*****************************************************************************
** Function name:	CoinAcceptor_Poll	
**
** Descriptions:	硬币器轮询
**						
**
** parameters:		cointype:硬币器轮询通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
** 
*****************************************************************************/
uint8_t CoinAcceptor_Poll(uint16_t cointype)
{
	uint8_t len = 0;

	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			break;

		case MDB_COIN_EN:
			len = MdbCoin_Poll();
			break;

		default:
			break;
	}

	return len;
}

/*****************************************************************************
** Function name:	CoinAcceptor_CheckTubeStat	
**
** Descriptions:	获取储币管状态及剩币量
**						
**
** parameters:		cointype:硬币器使能通道，coinleft:剩余总币值
** Returned value:	储币管满状态位	
** 
*****************************************************************************/
uint16_t CoinAcceptor_CheckTubeStat(uint16_t cointype, uint32_t *coinleft)
{
	uint16_t status = 0;

	switch(SYSPara.CoinEnable) 
	{
		case PULSE_COIN_EN:
			*coinleft = g_CoinLeft;
			break;

		case MDB_COIN_EN:
			status = MdbCoin_CheckTubeStat(coinleft);
			break;

		default:
			break;
	}

	return status;
}

