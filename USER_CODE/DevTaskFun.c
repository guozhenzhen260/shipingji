/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           DevTaskFun.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        纸币器硬币器禁能，使能，配置与使用的相关程序。在这里不具体划分那一款的收币找零设备
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
#include "..\config.h"
#include "global.h"
#include "drv.h"
#include "PulseCoin.h"
#include "hopper.h"
#include "BillAcceptor.h"
#include "CoinAcceptor.h"
#include "Changer.h"
#include "DevTaskFun.h"



//121106 by cq BillSingleWork
uint8_t cBillSingleWork = 0;
uint8_t isBillSingleWork()
{
	return cBillSingleWork;
}

//121108 by cq GOCErrorSevice
uint8_t cGOCErrorSevice = 0;
uint8_t isGOCErrorSevice()
{
	if(cGOCErrorSevice)
		SYSPara.GOCCtr = 1;

	return cGOCErrorSevice;
}

//121108 by cq RecordPassword
uint8_t cRecordPassword = 0;
uint8_t isRecordPassword()
{
	return cRecordPassword;
}

//121108 by cq DontReduceNum 
uint8_t cDontReduceNum = 0;
uint8_t isDontReduceNum()
{
	return cDontReduceNum;
}




/*初始化非MDB投币找零设备状态*/
void InitMoneyDevStat(void)
{
    uint8_t i;
		
	CashState.CoinState = COIN_DEV_OK;
	CashState.NoteState = NOTE_DEV_OK;
	CashState.ChangerState = CHANGER_OK;

    SYSPara1.CoinsStat &= 0;//COIN_VALUE_BIT;		  //初始化状态值，以重新获取
	if(SYSPara.GBAEnable == HR_NOTE_EN) 
	{
		SYSPara1.NoteStat &= 0;//HR_VALUE_BIT;
	} 
	else 
	{
	    SYSPara1.NoteStat &= 0;//GBA_VALUE_BIT;
	}	
	
    //找零器被禁能
	if(SYSPara.ChangerEnable == 0) 
	{					   
	    CashState.ChangerState |= CHANGER_NONE;
		for(i = 0; i < HOPPERCHMAX; i++) 
		{
		    SYSPara1.HopperStat[i] = HOPPER_DISABLE_BIT;   //找零器禁能
		}
	}

	//纸币器被禁能
	if(SYSPara.GBAEnable == 0) 
	{						   
	    CashState.NoteState |= NOTE_DEV_NONE;
		SYSPara1.NoteStat |= MBV_COMMERR_BIT;
	}

	//硬币器被禁能
	if(SYSPara.CoinEnable == 0) 
	{						   
	    SYSPara1.CoinsStat |= COIN_COMMERR_BIT;
		CashState.CoinState |= COIN_DEV_NONE;
	}

	//找零器是MDB设备，而硬币器不是MDB设备，则找零器状态设为无
	if((SYSPara.CoinEnable != MDB_COIN_EN) && (SYSPara.ChangerEnable == MDB_CHANGER_EN)) 
	{
	    CashState.ChangerState |= CHANGER_NONE;
	}
}

/*检测价格设置是否有0元的值*/
void CheckPricePara(void)	 
{
    uint8_t i,j;

	for(i = 0; i < MAXLAYER; i++) 
	{
	   if(SYSPara.LayEnable[i] == 0) 
	   {
		   continue;
	   }
	   for(j = 0; j < MAXCHANNEL; j++) 
	   {	      
		  if(Channels[i][j].ChannelStat == (DRV_CHNSET_BIT|CHANNELOK)) 
		  {		  
			  if(Channels[i][j].GoodsPrice == 0) 
			  {
		          SetDevStatus(DEV_ERR_BIT_PRICESET);				 //价格设置错误
		      }
		  }
	   }
	}
 }


/*检测Flash状态*/
void CheckFlashStat(void)
{
    if(SYSPara1.FlashStat > 0)
	{
	    SetDevStatus(DEV_ERR_BIT_FLASH);
	} 
}

/*检测RTC状态*/
void CheckRTCStat(void)
{
    if(SYSPara1.RTCStat > 0) {
	    //SetDevStatus(DEV_ERR_BIT_RTC);//120920 by cq DebugModify
	} 
}

/*
检测状态:每隔一段时间，自己判断一次各设备的工作情况，如果哪个故障了或因其它什么原因无法工作，将该设备置为故障等情况。如果可以再次工作了，将他的
状态再置为正常
再判断是否因为这个原因导致机器需要进入故障状态，将机器置为故障  
*/
void CheckMoneyDevStat(void)
{
	//120703 by cq MDBCompatible

     //是否需要将机器置为故障状态
     
	//121106 by cq BillSingleWork
	//121113 by cq Cashless
	if(isBillSingleWork() || SYSPara.ReaderEnable)
	{
		ClrDevStatus(DEV_ERR_BIT_PARASET);
	}
	else
	{
	     if(((CashState.CoinState & COIN_DEV_NONE) && ((CashState.NoteState & (NOTE_DEV_NONE|NOTE_DEV_PERR)) || (CashState.ChangerState & CHANGER_NONE))) 
		     || ((CashState.CoinState & COIN_DEV_PERR) && ((CashState.NoteState & (NOTE_DEV_NONE|NOTE_DEV_PERR|NOTE_DEV_ER)) || (CashState.ChangerState != CHANGER_OK)))
			 || ((CashState.CoinState & COIN_DEV_ER) && (CashState.NoteState & NOTE_DEV_PERR))
			 || ((SYSPara.ChangeServer == 0) && (CashState.ChangerState & CHANGER_NONE))) 
		 {	 
		     SetDevStatus(DEV_ERR_BIT_PARASET);
		 } 
		 else 
		 {
		     ClrDevStatus(DEV_ERR_BIT_PARASET);
		 }
	}

	 //是否需要置硬币器故障
	 //121106 by cq BillSingleWork
	 if(isBillSingleWork())
	 {
	     ClrDevStatus(DEV_ERR_BIT_COIN);
	 }
	 else if((CashState.CoinState & COIN_DEV_ER) 
	   && ((CashState.NoteState & (NOTE_DEV_NONE|NOTE_DEV_ER)) || (CashState.ChangerState != CHANGER_OK)))
	 {
		 SetDevStatus(DEV_ERR_BIT_COIN);
	 }
	 else 
	 {
	     ClrDevStatus(DEV_ERR_BIT_COIN);
	 }

	 //是否需要置纸币器故障
	 if((CashState.CoinState & COIN_DEV_NONE) && (CashState.NoteState & NOTE_DEV_ER)) 
	 {
	     SetDevStatus(DEV_ERR_BIT_NOTE);
	 }
	 else 
	 {
	     ClrDevStatus(DEV_ERR_BIT_NOTE);
	 } 

	 //是否需要置找零器硬币不足
	 if((CashState.CoinState & COIN_DEV_NONE) && (CashState.ChangerState & CHANGER_LOW)) 
	 {
	     SetDevStatus(DEV_ERR_BIT_COINLOW);
	 }

	 //是否需要置找零器故障
	 if((CashState.CoinState & COIN_DEV_NONE) && (CashState.ChangerState & CHANGER_ER)) 
	 {
	     SetDevStatus(DEV_ERR_BIT_CHANGER);
	 }

	 if(SYSPara.ChangeServer == 0) 
	 {
	     if(CashState.ChangerState & CHANGER_LOW) 
		 {
		     SetDevStatus(DEV_ERR_BIT_COINLOW);
		 } else 
		 {
		     ClrDevStatus(DEV_ERR_BIT_COINLOW);
		 }
		 
		 if(CashState.ChangerState & CHANGER_ER)
		 {
		     SetDevStatus(DEV_ERR_BIT_CHANGER);
	 	 } 
		 else 
		 {
			 ClrDevStatus(DEV_ERR_BIT_CHANGER);
		 }
	 } 
	 else 
	 {
	     if((CashState.ChangerState & CHANGER_LOW) == 0) 
		 {
			 ClrDevStatus(DEV_ERR_BIT_COINLOW);
		 }
		 if((CashState.ChangerState & CHANGER_ER) == 0) 
		 {
			 ClrDevStatus(DEV_ERR_BIT_CHANGER);
		 }
	 }

	//121106 by cq BillSingleWork
	if(isBillSingleWork())
	{
		ClrDevStatus(DEV_ERR_BIT_PARASET);
		ClrDevStatus(DEV_ERR_BIT_NOTE);
		ClrDevStatus(DEV_ERR_BIT_COIN);
		ClrDevStatus(DEV_ERR_BIT_COINLOW);
		ClrDevStatus(DEV_ERR_BIT_CHANGER);
	}

	//硬币器可以不用;by gzz 20130128
	if(SYSPara.CoinEnable == 0) 
	{
		//取消硬币器找零器故障码
		ClrDevStatus(DEV_ERR_BIT_PARASET);
		ClrDevStatus(DEV_ERR_BIT_NOTE);
		ClrDevStatus(DEV_ERR_BIT_COIN);
		ClrDevStatus(DEV_ERR_BIT_COINLOW);
		ClrDevStatus(DEV_ERR_BIT_CHANGER);
		//找零器状态设为正常
		CashState.ChangerState = CHANGER_OK; 
	}
}


//总的设备检查模块
void CheckVMCStat(void)
{  
	CheckMoneyDevStat();
	CheckFlashStat();
	CheckRTCStat();
}

/*获取最低面值的纸币暂存通道，大于这个面值的收币都可以暂存*/
void GetEscrowMinCh(void)
{
    uint8_t i;

	g_EscrowMinCh = 0;
	for(i = BILLCHMAX-1; i > 0; i--) 
	{
		if(SYSPara.BillValue[i] > 0) 
		{
			g_EscrowMinCh = i;
			break;
		}
	}

	if(SYSPara.NoteEscrow > 0) 
	{
		for(i = 0; i < BILLCHMAX; i++) 
		{
			if(SYSPara.NoteEscrowType[i] > 0) 
			{
				g_EscrowMinCh = i;
				break;
			}
		}
	}
}

/*获取纸币使能通道作为设置的使能位*/
void GetNoteEnBit(void)
{
    uint8_t i;
	
	CashTypeEnBit.BillTypeEN = 0;

	//121106 by cq MDBCompatible
	CashTypeEnBit.BillTypeEN = 0xff;

	//获取纸币使能通道
	for(i = 0; i < NOTE_GBA_NUM; i++)
	{			      
	   IncomeValue[NOTE_GBA_BASE+i] = SYSPara.BillValue[i];
	   if((SYSPara.BillValue[i] > 0) &&((SYSPara2.BillEnable>>i)&0x01))
	   {	   
	      CashTypeEnBit.BillTypeEN |= (1 << i);
		  //可以设置不收纸币;by gzz 20121219
		  //if(!((SYSPara2.BillEnable>>i)&0x01)) 
		  //{	   
		  //    CashTypeEnBit.BillTypeEN &= ~(1 << i);
		  //}
	   }
	}	
	Trace("\r\n **+enable=%#02x",SYSPara2.BillEnable);
	Trace("\r\n **+BillTypeEN=%#02x",CashTypeEnBit.BillTypeEN);
	
	if(CashTypeEnBit.BillTypeEN == 0)	
	{
	    if(SYSPara.GBAEnable == HR_NOTE_EN) 
		{
		    SYSPara1.NoteStat |= HR_VALUE_BIT;
		}  
		else 
		{
		    SYSPara1.NoteStat |= MBV_VALUE_BIT;
		}  
	}

	#if GBAHRDEVICE == 0 && MBVDEVICE == 0
	if(SYSPara.GBAEnable > 0) 
	{
	    SYSPara.NoteStat = 0x8000;
	}
	#endif
}

/*获取硬币使能通道作为设置的使能位*/
void GetCoinEnBit(void)
{
    uint8_t i;
	
	CashTypeEnBit.CoinTypeEN = 0;

	//121106 by cq MDBCompatible
	CashTypeEnBit.CoinTypeEN = 0xff; 
	
	//获取硬币使能通道
	for(i = 0; i < COIN_MDB_NUM; i++)
	{			  
	   if(SYSPara.CoinsValue[i] > 0) 
	   {
	       CashTypeEnBit.CoinTypeEN |= (1 << i);
	   }
	}
	if(CashTypeEnBit.CoinTypeEN == 0)	
	{
	    SYSPara1.CoinsStat |= COIN_VALUE_BIT;
	}	
}

/*检测是否手动置纸币器异常*/
void CheckNotePara(void)
{
    if(((SYSPara.GBAEnable == HR_NOTE_EN) && (SYSPara1.NoteStat & HR_VALUE_BIT))
	   || ((SYSPara.GBAEnable == MDB_NOTE_EN) && (SYSPara1.NoteStat & MBV_VALUE_BIT)))
	{
		CashTypeEnBit.BillType = 0;
	}
}

/*检测是否手动置硬币器异常*/
void CheckCoinPara(void)
{
	if(SYSPara1.CoinsStat & COIN_VALUE_BIT) 
	{
		CashTypeEnBit.CoinType = 0;
	} 
}

/*将设置的使能位导入当前的使能位*/
void EnableMoneyType(void) 
{	
	if(CashState.ChangerState == CHANGER_OK) 
	{
	    CashTypeEnBit.BillType = CashTypeEnBit.BillTypeEN;
	}
	else 
	{
	    CashTypeEnBit.BillType = 0;
	}
	CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;
}

/*禁能硬币器命令*/
void DisCoinDevCmd(void)
{
     CashTypeEnBit.CoinType = 0;
     CoinAcceptor_Enable(CashTypeEnBit.CoinType);	                 //使能
}

/*禁能纸币器命令*/
void DisNoteDevCmd(void)
{    
     CashTypeEnBit.BillType = 0;
     BillAcceptor_Enable(CashTypeEnBit.BillType);	                 //禁能
}

/*使能硬币器命令*/
void EnCoinDevCmd(void)
{
     if((CashState.CoinState == COIN_DEV_OK) || IsMaintainStat())
	 {
	 	//币满处理,原先相应通道币满后，会把对应通道禁能。现在不予禁能;by gzz 20121025
	     //CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN & (~CashTypeEnBit.CoinFullBit);
	     CashTypeEnBit.CoinType = CashTypeEnBit.CoinTypeEN;
		 CoinAcceptor_Enable(CashTypeEnBit.CoinType);	                 //使能
	 }
}

/*使能纸币器命令*/
void EnNoteDevCmd(void)
{
	//121106 by cq BillSingleWork
     //if(((CashState.ChangerState == CHANGER_OK) && (BillAcceptor_Available() == TRUE)) || IsMaintainStat())
	if((((isBillSingleWork()) || (CashState.ChangerState == CHANGER_OK))&& (BillAcceptor_Available() == TRUE)) || IsMaintainStat())
	 {				     					 
	     if(BillAcceptor_ErrJudge(BillAcceptor_Enable(CashTypeEnBit.BillTypeEN)) != TRUE) 
		 {	             //使能				     
			 CashTypeEnBit.BillType = CashTypeEnBit.BillTypeEN;
		 }
	 }
}
