/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           BillAcceptor.c
** Last modified Date:  2011-10-26
** Last Version:         
** Descriptions:        纸币器驱动程序
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
#include "MdbNote.h"
#include "Gbahr1.h"
//#include "Gbahr1App.h"//120401 by cq change structor
#include "MdbNote.h"
//#include "MdbNoteApp.h"//120401 by cq change structor
#include "ITL.h"
#include "mdb.h"
#include "BillAcceptor.h"
#include "VMC_PC.h"


extern uint8_t GbaAck;

BillParaStruct BillPara;

/*接收到纸币参数结构体*/
struct BillAccept
{
	uint8_t Route;	      //Bill Routing,可能值BILLACCEPTOR_STACKED等，详见MDB手册
	                      //rount:BILLACCEPTOR_STACKED,BILLACCEPTOR_ESCROWED,BILLACCEPTOR_RETURNED等，表示当前纸币所处的位置
	uint8_t Channel;	  //纸币通道
} BillAcpt;

uint8_t  BillRstStat;	  //纸币器是否刚刚复位标志
uint8_t  BillCommErr;     //纸币器通讯状态 TRUE:正常 FALSE:异常
uint8_t  BillValueErr;	  //纸币各通道面值设置是否正确 TRUE:正常 FALSE:异常
uint8_t  BillStackerFull; //纸币器钞箱状态 TRUE:满 FALSE:未满
uint8_t  BillInValid;	  //收到的纸币是否可识别 TRUE:不可识别纸币 FALSE:正常纸币
uint16_t BillStat;		  //纸币器状态,状态值见纸币器相关协议手册

/*****************************************************************************
** Function name:	BillAcceptor_StatInit	
**
** Descriptions:	初始化纸币器状态值
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_StatInit(void)
{
	BillAcpt.Route = 0;
	BillAcpt.Channel = 0;
	BillRstStat = FALSE;
	BillInValid = FALSE;
	BillStackerFull = FALSE;
	BillValueErr = FALSE;
	BillCommErr = FALSE;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
		BillStat = HR_STAT_OK;	     //初始状态设为正常
		BillPara.EscrowFun = FALSE;	 //GBA HR1不能暂存
		break;

		case MDB_NOTE_EN:
		BillStat = MBV_STAT_OK;
		//BillPara.EscrowFun = TRUE;
		break;

		case ITL_STAT_OK:
		BillStat = ITL_STAT_OK;
		BillPara.EscrowFun = FALSE;	 //ITL不能暂存
		break;

		default:
		break;
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_ClearStat	
**
** Descriptions:	清除上次收到的纸币记录，避免重复累加同一次投币，每次POLL前调用
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_ClearStat(void)
{
	BillAcpt.Route = 0;
	BillAcpt.Channel = 0;
}

/*****************************************************************************
** Function name:	BillAcceptor_CommErr	
**
** Descriptions:	用于判断是否有通讯故障
**						
**
** parameters:		无
** Returned value:	TRUE:故障 FALSE:正常	
** 
*****************************************************************************/
uint8_t BillAcceptor_CommErr(void)
{
	return BillCommErr;
}

/*****************************************************************************
** Function name:	BillAcceptor_RstStat	
**
** Descriptions:	用于判断刚刚是否复位了
**						
**
** parameters:		无
** Returned value:	TRUE:复位了 FALSE:没复位	
** 
*****************************************************************************/
uint8_t BillAcceptor_RstStat(void)
{
	return BillRstStat;
}

/*****************************************************************************
** Function name:	BillAcceptor_ValueSetErr	
**
** Descriptions:	用于判断纸币各通道面值设置是否正确
**						
**
** parameters:		无
** Returned value:	TRUE:设置错误 FALSE:设置正常	
** 
*****************************************************************************/
uint8_t BillAcceptor_ValueSetErr(void)
{
	return BillValueErr;
}

/*****************************************************************************
** Function name:	BillAcceptor_StackerFull	
**
** Descriptions:	用于判断钞箱是否满了
**						
**
** parameters:		无
** Returned value:	TRUE:钞箱满 FALSE:未满	
** 
*****************************************************************************/
uint8_t BillAcceptor_StackerFull(void)
{
	return BillStackerFull;
}

/*****************************************************************************
** Function name:	IsBillInValid	
**
** Descriptions:	用于判断收到的纸币是否可识别
**						
**
** parameters:		无
** Returned value:	TRUE:不可识别 FALSE:可识别	
** 
*****************************************************************************/
uint8_t IsBillInValid(void)
{
	return BillInValid;
}

/*****************************************************************************
** Function name:	BillAcceptor_EscrowFun	
**
** Descriptions:	用于判断纸币器是否有暂存功能
**						
**
** parameters:		无
** Returned value:	TRUE:有暂存功能 FALSE:无暂存功能	
** 
*****************************************************************************/
uint8_t BillAcceptor_EscrowFun(void)
{
	return BillPara.EscrowFun;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetCommErr	
**
** Descriptions:	设置纸币器通讯故障状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetCommErr(void)  
{
    BillCommErr = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetValueErr	
**
** Descriptions:	设置纸币器面值设置错误状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetValueErr(void)   
{
	BillValueErr = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetStackerFull	
**
** Descriptions:	设置钞箱满状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetStackerFull(void)
{
    BillStackerFull = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetBillPara	
**
** Descriptions:	设置纸币参数
**						
**
** parameters:		纸币参数结构体
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetBillPara(BillParaStruct billpara)
{
	BillPara = billpara;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetRstStat	
**
** Descriptions:	设置纸币器复位状态，表示纸币器刚刚复位了
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetRstStat(void)
{
    BillRstStat = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_CLrRstStat	
**
** Descriptions:	清除纸币器复位状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_ClrRstStat(void)
{
    BillRstStat = FALSE;
}

/*****************************************************************************
** Function name:	BillAcceptor_SetInValid	
**
** Descriptions:	设置收到的纸币为不可识别
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_SetInValid(void)  
{
    BillInValid = TRUE;
}

/*****************************************************************************
** Function name:	BillAcceptor_ClrInValid	
**
** Descriptions:	设置收到的纸币为可识别
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_ClrInValid(void)  
{
    BillInValid = FALSE;
}

/*****************************************************************************
** Function name:	BillAcceptor_Available	
**
** Descriptions:	判断纸币器是否可用
**					通讯是否正常，面值设置是否正确等	
**
** parameters:		无
** Returned value:	TRUE:纸币器可用 FALSE:不可用	
** 
*****************************************************************************/
uint8_t BillAcceptor_Available(void)
{
	if((BillCommErr == FALSE) && (BillValueErr == FALSE))  
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	IsBillEscrowed	
**
** Descriptions:	判断是否有纸币处于暂存状态
**						
**
** parameters:		无
** Returned value:	TRUE:有 FALSE:无	
** 
*****************************************************************************/
uint8_t IsBillEscrowed(void)
{
	if(BillAcpt.Route == BILLACCEPTOR_ESCROWED) 
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	IsBillStacked	
**
** Descriptions:	判断是否有纸币已压钞
**						
**
** parameters:		无
** Returned value:	TRUE:有 FALSE:无	
** 
*****************************************************************************/
uint8_t IsBillStacked(void)
{
	if(BillAcpt.Route == BILLACCEPTOR_STACKED) 
	{
		return TRUE;
	} 
	else 
	{
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	IsBillReturned	
**
** Descriptions:	判断是否有退钞
**						
**
** parameters:		无
** Returned value:	TRUE:有 FALSE:无	
** 
*****************************************************************************/
uint8_t IsBillReturned(void)
{
	if(BillAcpt.Route == BILLACCEPTOR_RETURNED) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_UpDateStat	
**
** Descriptions:	更新当前纸币器状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_UpDateStat(uint16_t status)
{
	BillStat = status;
}

/*****************************************************************************
** Function name:	BillAcceptor_UpDateSysStat	
**
** Descriptions:	更新当前纸币器状态到SYSPara
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_UpDateSysStat(void)
{
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			SYSPara1.NoteStat = BillStat;
			if(BillAcceptor_CommErr() == TRUE) 
			{
				SYSPara1.NoteStat |= HR_COMMERR_BIT;
			}
			if(BillAcceptor_ValueSetErr() == TRUE) 
			{
				SYSPara1.NoteStat |= HR_VALUE_BIT;
			}
			if(BillAcceptor_StackerFull() == TRUE) 
			{
				SYSPara1.NoteStat |= HR_BILLFULL_BIT;
			}
			break;

		case MDB_NOTE_EN:
			SYSPara1.NoteStat = BillStat;
			if(BillAcceptor_CommErr() == TRUE) 
			{
				SYSPara1.NoteStat |= MBV_COMMERR_BIT;
			}
			if(BillAcceptor_ValueSetErr() == TRUE) 
			{
				SYSPara1.NoteStat |= MBV_VALUE_BIT;
			}
			if(BillAcceptor_StackerFull() == TRUE) 
			{
				SYSPara1.NoteStat |= MBV_BILLFULL_BIT;
			}		
			break;

		case ITL_NOTE_EN:
			SYSPara1.NoteStat = BillStat;
			if(BillAcceptor_CommErr() == TRUE) 
			{
				SYSPara1.NoteStat |= ITL_COMMERR_BIT;
			}
			if(BillAcceptor_ValueSetErr() == TRUE) 
			{
				//SYSPara1.NoteStat |= MBV_VALUE_BIT;
			}
			if(BillAcceptor_StackerFull() == TRUE) 
			{
				//SYSPara1.NoteStat |= MBV_BILLFULL_BIT;
			}		
			break;	

		default:
			break;
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_CheckStat	
**
** Descriptions:	更新NoteStat(NoteStat纸币器状态没有具体的状态码，只是用来判断纸币器是否正常等)
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_CheckStat(void)
{
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			if(SYSPara1.NoteStat != HR_STAT_OK) 
			{
			    if(SYSPara1.NoteStat & HR_VALUE_BIT)	
				{
					CashState.NoteState |= NOTE_DEV_PERR;
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & HR_BILLFULL_BIT)
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & (~HR_VALUE_BIT)) 
				{
				    CashState.NoteState |= NOTE_DEV_ER;
				}
			} 
			else 
			{
				CashState.NoteState &= NOTE_DEV_DIS;
				if(CashTypeEnBit.BillType == 0) 
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				} 
				else 
				{
					CashState.NoteState = NOTE_DEV_OK;
				}
			}	
			break;

		case MDB_NOTE_EN:
			if(SYSPara1.NoteStat != MBV_STAT_OK) 
			{
			    if(SYSPara1.NoteStat & MBV_VALUE_BIT) 
				{
					CashState.NoteState |= NOTE_DEV_PERR;
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & MBV_BILLFULL_BIT) 
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & (~MBV_VALUE_BIT)) 
				{
				    CashState.NoteState |= NOTE_DEV_ER;
				}
			} 
			else 
			{
			    CashState.NoteState &= NOTE_DEV_DIS;
				if(CashTypeEnBit.BillType == 0) 
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				} 
				else 
				{
					CashState.NoteState = NOTE_DEV_OK;
				}
			}
			break;

		case ITL_NOTE_EN:
			if(SYSPara1.NoteStat != ITL_STAT_OK) 
			{
				/*
			    if(SYSPara1.NoteStat & HR_VALUE_BIT)	
				{
					CashState.NoteState |= NOTE_DEV_PERR;
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & HR_BILLFULL_BIT)
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				}
				if(SYSPara1.NoteStat & (~HR_VALUE_BIT)) 
				{
				    CashState.NoteState |= NOTE_DEV_ER;
				}
				*/
				CashState.NoteState |= NOTE_DEV_ER;
			} 
			else 
			{
				CashState.NoteState &= NOTE_DEV_DIS;
				if(CashTypeEnBit.BillType == 0) 
				{
					CashState.NoteState |= NOTE_DEV_DIS;
				} 
				else 
				{
					CashState.NoteState = NOTE_DEV_OK;
				}
			}	
			break;	

		default:
			CashState.NoteState = NOTE_DEV_NONE;
			break;
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_UpdateAcpt	
**
** Descriptions:	收到纸币，更新纸币状态信息
**						
**
** parameters:		rount:BILLACCEPTOR_STACKED,BILLACCEPTOR_ESCROWED,BILLACCEPTOR_RETURNED等，表示当前纸币所处的位置
**					chn:  纸币通道值
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_UpdateAcpt(uint8_t route,uint8_t chn)
{
	 BillAcpt.Route = route;
	 BillAcpt.Channel = chn;
}

/*****************************************************************************
** Function name:	BillAcceptor_UpDateStackerStat	
**
** Descriptions:	更新钞箱状态
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_UpDateStackerStat(void)
{
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			Gbahr1_UpDateStackerStat();
			break;

		case MDB_NOTE_EN:
			MdbNote_UpDateStackerStat();
			break;

		case ITL_NOTE_EN:
			ITL_UpDateStackerStat();
			break;

		default:
			break;
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_RstProc	
**
** Descriptions:	交易结束后，纸币器复位后，（重新初始化纸币器），重新使能纸币器
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_RstProc(void)
{
	uint8_t lenth;

	BillAcceptor_ClrRstStat();
	//121106 by cq BillSingleWork	
	//if(BeSaleStat() && (CashState.ChangerState == CHANGER_OK) && (CashTypeEnBit.BillType > 0)) 
	if((BeSaleStat() && (CashState.ChangerState == CHANGER_OK) && (CashTypeEnBit.BillType > 0)) 
	|| ( isBillSingleWork() && (BeSaleStat()) && (CashTypeEnBit.BillType > 0)))
	{
		lenth = BillAcceptor_Enable(CashTypeEnBit.BillType);

		if(BillAcceptor_ErrJudge(lenth) == TRUE)
		{
			BillAcceptor_SetCommErr();
		}
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_PackAnalysis	
**
** Descriptions:	更新纸币器当前数据包信息，并根据当前纸币器处于的状态，
**					进行下一步的相应操作
**
** parameters:		buf:数据缓存，len:数据长度
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_PackAnalysis(uint8_t *buf, uint8_t len)
{
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			Gbahr1_PackAnalysis(buf,len);
			break;

		case MDB_NOTE_EN:
			MdbNote_PackAnalysis(buf,len);
			break;

		case ITL_NOTE_EN:
			ITLNote_PackAnalysis(buf,len);
			break;

		default:
		break;
	}

	BillAcceptor_UpDateSysStat();		
	BillAcceptor_CheckStat();
	
	if(BillAcceptor_RstStat() == TRUE)
	{
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		BillAcceptor_RstProc();
	}
	
	if(IsBillEscrowed() == TRUE) 
	{		
		BillAcceptor_EscrowProc(BillAcpt.Channel);
	} 
	else if(IsBillStacked()) 
	{
		BillAcceptor_StackProc(BillAcpt.Channel);
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		BillAcceptor_UpDateStackerStat();
		NoteEscrowClr();	
	} 
	else if(IsBillReturned() == TRUE) 
	{
	    if(IsEscrowHold()) 
		{
	       BillAcceptor_ReturnProc(BillAcpt.Channel);
	    }
	    NoteEscrowClr();	
	} 		
	
	if((BillAcceptor_StackerFull() == TRUE) && (CashTypeEnBit.BillType > 0)) 
	{
		CashTypeEnBit.BillType = 0;								  //禁能纸币器
		BillAcceptor_Enable(CashTypeEnBit.BillType);
	}	
}

/*****************************************************************************
** Function name:	BillAcceptor_ReadBuf	
**
** Descriptions:	读取纸币器缓存数据
**						
**
** parameters:		buf:数据缓存，len:数据长度
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_ReadBuf(uint8_t *buf, uint8_t len)
{
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			Gbahr1_ReadBuf(buf,len);
			break;

		case MDB_NOTE_EN:
			Mdb_ReadBuf(buf,len);
			break;
	    case ITL_NOTE_EN:
			//
			break;
			
		default:
			break;
	}	
}

/*****************************************************************************
** Function name:	BillAcceptor_ErrJudge	
**
** Descriptions:	判断纸币器通讯状态
**						
**
** parameters:		len:根据相关协议，正常时表示数据长度，异常时表示异常状态
** Returned value:	TRUE:正常 FALSE:出错	
** 
*****************************************************************************/
uint8_t BillAcceptor_ErrJudge(uint8_t len)
{
	uint8_t status = TRUE;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			status = Gbahr1_ErrJudge(len);
			break;

		case MDB_NOTE_EN:
			status = Mdb_ErrJudge(len);
			break;

		case ITL_NOTE_EN:
			status = ITL_ErrJudge(len);
			break;
		default:
			break;
	}
	return status;
}

/*****************************************************************************
** Function name:	BillAcceptor_Init	
**
** Descriptions:	纸币器初始化
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_Init(void)
{
	BillAcceptor_StatInit();
	
	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			Gbahr1_InitProc();
			break;

		case MDB_NOTE_EN:
			MdbNote_InitProc();
			break;

		case ITL_NOTE_EN:
			ITLMission_Init(0);
			break;

		default:
			break;
	}
	
	BillAcceptor_UpDateSysStat();
	BillAcceptor_CheckStat();	
}

/*****************************************************************************
** Function name:	BillAcceptor_PollProc	
**
** Descriptions:	轮询纸币器，获取状态，并作出相应处理
**						
**
** parameters:		无
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_PollProc(void)
{	
	uint8_t len;
	uint8_t BillStatBuf[16];

	BillAcceptor_ClearStat();
	len = BillAcceptor_Poll(CashTypeEnBit.BillType);

	if(BillAcceptor_ErrJudge(len) == TRUE) 
	{
		BillAcceptor_SetCommErr();
	} 
	else 
	{
		len &= 0x0F;
		BillAcceptor_ReadBuf(BillStatBuf,len);
		BillAcceptor_PackAnalysis(BillStatBuf,len);
	}
	
	BillAcceptor_UpDateSysStat();		
	BillAcceptor_CheckStat();		
}

/*****************************************************************************
** Function name:	BillAcceptor_EscrowProc	
**
** Descriptions:	纸币器处于暂存状态时，判断纸币面值，并作出相应处理
**						
**
** parameters:		moneych:收币通道
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_EscrowProc(uint8_t moneych)
{
	uint32_t PcValue = 0;
	
	//121106 by cq BillSingleWork
	//if(BeSaleStat() && ((IsBillInValid() == TRUE) || (g_Amount+SYSPara.BillValue[moneych] > g_CoinLeft))) 
	if((!isBillSingleWork()) && BeSaleStat() && ((IsBillInValid() == TRUE) || (g_Amount+SYSPara.BillValue[moneych] > g_CoinLeft))) 
	{
	   BillAcceptor_Return(CashTypeEnBit.BillType);
	   MsgBack.moneyback = OVER_VALUE_INDEX << 8;
	   //OSMboxPost(g_KeyMsg,&MsgBack.moneyback);
	   //120621 by cq KeySlowly
	   //while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)
	   while( OSMboxPost(g_MoneyMsg,&MsgBack.moneyback) != OS_NO_ERR)
	   {
	   		OSTimeDly(OS_TICKS_PER_SEC / 100);
	   }
    } 
	else 
	{
       	      /*纸币器有暂存功能*/              /*判断是否允许暂存*/
	//120622 by cq HoldBill		  
	//if((BillAcceptor_EscrowFun() == TRUE) && (moneych >= g_EscrowMinCh))    
	if((BillAcceptor_EscrowFun() == TRUE) &&(BeSaleStat()) && (moneych >= g_EscrowMinCh))    
	   {
	       NoteEscrowHold();
		   //如果pc有开启，上报pc暂存纸币
		   if( SYSPara.PCEnable == 1 )//120620 by cq PCcommunication
		   {
		   	  if(SYSPara.Decimal==1)
			  {
			  	PcValue = SYSPara.BillValue[moneych] * 10;
			  }
			  else if(SYSPara.Decimal==2)
			  {
			  	PcValue = SYSPara.BillValue[moneych]; 
			  }	
			  sysVPMission.billHoldingValue = PcValue;
			  sysVPMission.billHoldingFlag = 1;
		   	  VPMission_Payin_RPT( VP_DEV_ESCROWIN, PcValue );		
		   }
		   MsgBack.moneyback = (SYSPara.BillValue[moneych])|0x8000;   //money编码（16位）：最高位置1，其他为面值				   		 
	       //OSMboxPost(g_KeyMsg,&MsgBack.moneyback);
	       //120621 by cq KeySlowly
	       //while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)		   	
	       while( OSMboxPost(g_MoneyMsg,&MsgBack.moneyback) != OS_NO_ERR)
		   {
		   		OSTimeDly(OS_TICKS_PER_SEC / 100);
		   }
	   }
	   else 
	   {
		   NoteEscrowSet();
		   BillAcceptor_Stack(CashTypeEnBit.BillType);	                 //执行压钞命令
		   if(BeSaleStat()) 
		   {
		       TranTotal.DoubtMoney += SYSPara.BillValue[moneych];
		   }
	   }	  
    }
}

/*****************************************************************************
** Function name:	BillAcceptor_StackProc	
**
** Descriptions:	有纸币压钞时，更新交易数据
**						
**
** parameters:		moneych:收币通道
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_StackProc(uint8_t moneych)
{	
	uint8_t chnbase = 0;
	//uint32_t PcValue = 0;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			chnbase = NOTE_GBA_BASE;
			break;

		case MDB_NOTE_EN:
			chnbase = NOTE_MDB_BASE;
			break;
			
		case ITL_NOTE_EN:
			chnbase = NOTE_ITL_BASE;
			break;

		default:
			break;
	}
	
	if(BeSaleStat()) 
	{
	   TranData.IncomeCH[chnbase+moneych]++;
	   TranTotal.IncomeCH[chnbase+moneych]++;
	   TranTotal.Income += SYSPara.BillValue[moneych];
	   TranTotal.NoteIncome += SYSPara.BillValue[moneych];

	   if(TranTotal.DoubtMoney >= SYSPara.BillValue[moneych]) 
	   {						   				   
           TranTotal.DoubtMoney -= SYSPara.BillValue[moneych];
	   }

	   //用于季度结账用;by gzz 20121009
	   TranTotal.IncomeJidu += SYSPara.BillValue[moneych];
	   TranTotal.NoteIncomeJidu += SYSPara.BillValue[moneych];

	   //如果pc有开启，上报pc收纸币
	   //if( SYSPara.PCEnable == 1 )
	   //{
	   //	  if(SYSPara.Decimal==1)
		//  {
		//  	PcValue = SYSPara.BillValue[moneych] * 10;
		//  }
		//  else if(SYSPara.Decimal==2)
		//  {
		//  	PcValue = SYSPara.BillValue[moneych]; 
		//  }	
	   	//  VPMission_Payin_RPT( VP_DEV_BILL, PcValue );
	   //}
    }
    if(!IsEscrowHold()) 
	{
	    MsgBack.moneyback = (SYSPara.BillValue[moneych])|0x8000;   //money编码（16位）：最高位置1，其他为面值				   		 
	    //OSMboxPost(g_KeyMsg,&MsgBack.moneyback); 
	    //120621 by cq KeySlowly
	    //while( OSMboxPost(g_KeyMsg,&MsgBack.moneyback) != OS_NO_ERR)
	    while( OSMboxPost(g_MoneyMsg,&MsgBack.moneyback) != OS_NO_ERR)
	    {
	   		OSTimeDly(OS_TICKS_PER_SEC / 100);
	    }
    } 
	else 
	{
        MsgBack.pBackMsg = moneych;					 
	    OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);
    }
}

/*****************************************************************************
** Function name:	BillAcceptor_ReturnProc	
**
** Descriptions:	暂存纸币退钞
**						
**
** parameters:		moneych:收币通道
** Returned value:	无	
** 
*****************************************************************************/
void BillAcceptor_ReturnProc(uint8_t moneych)
{
	if(BillAcceptor_EscrowFun() == TRUE) 
	{
		MsgBack.pBackMsg = moneych;					 
		OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);
	}
}

/*****************************************************************************
** Function name:	BillAcceptor_Stack
**
** Descriptions:	压钞
**						
**
** parameters:		billtype:GBAHR1纸币器使能通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
** 
*****************************************************************************/
uint8_t BillAcceptor_Stack(uint16_t billtype)
{
	uint8_t len = 0;
	uint32_t PcValue = 0;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			ACKSwitch();
			len = Gbahr1_Stack(billtype);
			break;

		case MDB_NOTE_EN:
			len = MdbNote_Stack();
			break;

		case ITL_NOTE_EN:
			len = ITLMission_HB_Stack();
			break;	

		default:
			break;
	}
	//如果pc有开启，上报pc收纸币
    if( SYSPara.PCEnable == 1 )//120620 by cq PCcommunication
    {
      if(SYSPara.Decimal==1)
	  {
	  	PcValue = SYSPara.BillValue[BillAcpt.Channel] * 10;
	  }
	  else if(SYSPara.Decimal==2)
	  {
	  	PcValue = SYSPara.BillValue[BillAcpt.Channel]; 
	  }	

	  if(sysVPMission.billHoldingFlag == 1)	
	  {
	  	 VPMission_Payin_RPT( VP_DEV_BILL, sysVPMission.billHoldingValue );
	  	 sysVPMission.billHoldingValue = 0;
		 sysVPMission.billHoldingFlag = 0;
	  }	 
	  else
	  {
	  	VPMission_Payin_RPT( VP_DEV_BILL, PcValue );
	  }
    }

	return len;
}

/*****************************************************************************
** Function name:	BillAcceptor_Return	
**
** Descriptions:	退币
**						
**
** parameters:		billtype:GBAHR1纸币器使能通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度
** 
*****************************************************************************/
uint8_t BillAcceptor_Return(uint16_t billtype)
{
	uint8_t len = 0;
	//uint32_t PcValue = 0;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			ACKSwitch();
			len = Gbahr1_Return(billtype);
			break;

		case MDB_NOTE_EN:
			len = MdbNote_Return();
			break;

		case ITL_NOTE_EN:
			len = ITLMission_Reject();
			break;	

		default:
			break;
	}
	//如果pc有开启，上报pc暂存纸币退出
    if( SYSPara.PCEnable == 1 )//120620 by cq PCcommunication
    {	      
	   	  VPMission_Payin_RPT( VP_DEV_ESCROWOUT, sysVPMission.billHoldingValue );	
		  sysVPMission.billHoldingValue = 0;
		  sysVPMission.billHoldingFlag = 0;
    }
	

	return len;
}

/*****************************************************************************
** Function name:	BillAcceptor_Enable	
**
** Descriptions:	纸币器使能
**						
**
** parameters:		billtype:纸币器使能通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
** 
*****************************************************************************/
uint8_t BillAcceptor_Enable(uint16_t billtype)
{
	uint8_t len = 0;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			ACKSwitch();
			len = Gbahr1_Enable(billtype);
			break;

		case MDB_NOTE_EN:
			len = MdbNote_Enable(billtype);
			break;
		
		case ITL_NOTE_EN:
			len = ITLMission_Enable(billtype);
			break;
		default:
			break;
	}

	return len;
}

/*****************************************************************************
** Function name:	BillAcceptor_Poll
**
** Descriptions:	纸币器轮询
**						
**
** parameters:		billtype:GBAHR1纸币器使能通道
** Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
** 
*****************************************************************************/
uint8_t BillAcceptor_Poll(uint16_t billtype)
{
	uint8_t len = 0;

	switch(SYSPara.GBAEnable) 
	{
		case HR_NOTE_EN:
			ACKSwitch();
			len = Gbahr1_Poll(billtype);
			break;

		case MDB_NOTE_EN:
			len = MdbNote_Poll();
			break;

		case ITL_NOTE_EN:
			len = ITLMission_Poll();
			break;	

		default:
			break;
	}

	return len;
}
