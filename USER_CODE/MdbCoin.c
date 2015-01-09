/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           MdbCoin.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        MDBÓ²±ÒÆ÷Çý¶¯³ÌÐò
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
#include "Mdbcoin.h"
#include "CoinAcceptor.h"
#include "mdb.h"

#include "Changer.h"//120401 by cq change structor


/*****************************************************************************
** Function name:	MdbCoin_PackAnalysis	
**
** Descriptions:	POLLÂÖÑ¯Ö®ºó£¬MDBÓ²±ÒÆ÷½ÓÊÕµÄ×´Ì¬Êý¾Ý°ü·ÖÎö
**						
**
** parameters:		buf:×´Ì¬Êý¾Ý»º´æ£¬len:×´Ì¬Êý¾Ý³¤¶È
** Returned value:	ÎÞ	
** 
*****************************************************************************/
void MdbCoin_PackAnalysis(uint8_t *buf, uint8_t len)
{
	uint8_t  i;
	
	for(i = 0; i < len; i++) 
	{       //report changer activity
         if(buf[i] & 0x80) 
		 {        //coins dispensed manually	      
			i++; //ºöÂÔµÚ2×Ö½Ú
		 } 
		 else if((buf[i] & 0xC0) == 0x40) 
		 {	 //coins deposited
            if(((buf[i] & 0x30) == 0x10) || ((buf[i] & 0x30) == 0)) //in tubes
		    {	 //in cashbox
			   CoinAcceptor_UpDateDeposit(COINACCEPTOR_STACKED,buf[i] & 0x0F);
			}         
			//CheckCoinsLeft();
			i++; //ºöÂÔµÚ2×Ö½Ú
         } 
		 else if((buf[i] & 0xE0) == 0x20) 
		 {	 //slug
		 }
		 else 
		 {									 //status
			switch(buf[i]) 
			{
	           case 0x01:			                 //escrow request				   
	           CoinAcceptor_UpDateDeposit(COINACCEPTOR_RETURN,0);
			   break;

	           case 0x02:			                 //changer pay out busy
	           break;

	           case 0x03:			                 //no credit
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {				   
				   CoinAcceptor_UpDateStat(COIN_NOCREDIT);
			   }
	           break;

	           case 0x04:			                 //defective tube sensor
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_SENSORERR);
			   }
	           break;

	           case 0x05:			                 //double arriver
	           break;

	           case 0x06:			                 //acceptor unplugged
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_UNPLUGGED);
			   }
	           break;

	           case 0x07:			                 //tube jam
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_TUBEJAM);
			   }
	           break;

	           case 0x08:			                 //rom chksum err
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_ROMCHKERR);
			   }
	           break;

	           case 0x09:			                 //coin routing err
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_ROUTINGERR);
			   }
	           break;

	           case 0x0A:			                 //changer busy
	           break;

	           case 0x0B:			                 //changer was reset
			   CoinAcceptor_SetRstStat();
	           break;

	           case 0x0C:			                 //coin jam
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_JAM);
			   }
	           break;

	           case 0x0D:		              	     //possible credited coin removal
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(COIN_REMOVAL);
			   }
	           break;

	           default:
			   if(CoinAcceptor_GetCoinLevel() < 3) 
			   {
				   CoinAcceptor_UpDateStat(buf[i]);
			   }
	           break;
  	        }
         }
    }
}
/*****************************************************************************
** Function name:	MdbCoin_Reset	
**
** Descriptions:	¸´Î»ÃüÁî
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_Reset(void)
{
  return Mdb_Session(COIN_RESET,NULL,0); 
}

/*****************************************************************************
** Function name:	MdbCoin_Setup	
**
** Descriptions:	»ñÈ¡ÅäÖÃÐÅÏ¢
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_Setup(void)
{
  return Mdb_Session(COIN_SETUP,NULL,0); 
}

/*****************************************************************************
** Function name:  MdbCoin_GetTubeStatus	
**
** Descriptions:   »ñÈ¡¸÷Í¨µÀÓ²±ÒÊý	
**						
**
** parameters:		
** Returned value: ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_GetTubeStatus(void)
{
  return Mdb_Session(TUBE_STATUS,NULL,0); 
}

/*****************************************************************************
** Function name:  MdbCoin_Enable	
**
** Descriptions:   Ê¹ÄÜÍ¶±ÒÍ¨µÀ	
**						
**
** parameters:	   CoinsTypeEn:Ê¹ÄÜÍ¶±ÒÍ¨µÀ£¬Î»Ä£Ê½£¬µÚnÎ»ÖÃ1ÔòµÚnÍ¨µÀÊ¹ÄÜ	
** Returned value: ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë		
** 
*****************************************************************************/
uint8_t MdbCoin_Enable(uint16_t CoinsTypeEn)
{
  uint8_t data[4];

  data[0] = CoinsTypeEn >> 8; 
  data[1] = CoinsTypeEn;
  data[2] = 0xFF;             
  data[3] = 0xFF;
  Trace("\r\n coinenable-%x,%x,%x,%x",data[0],data[1],data[2],data[3]);
  return Mdb_Session(COIN_TYPE,data,4); 
}

/*****************************************************************************
** Function name:  MdbCoin_Disable		
**
** Descriptions:   ½ûÄÜÍ¶±Ò	
**						
**
** parameters:		
** Returned value: ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë		
** 
*****************************************************************************/
uint8_t MdbCoin_Disable(void)
{
  uint8_t data[4];

  data[0] = 0x00; 
  data[1] = 0x00;
  data[2] = 0xFF; 
  data[3] = 0xFF;

  return Mdb_Session(COIN_TYPE,data,4); 
}

/*****************************************************************************
** Function name:  MdbCoin_Dispense		
**
** Descriptions:   ³ö±Ò	
**						
**
** parameters:	   amount:³ö±ÒÊý type:³ö±ÒÍ¨µÀ	
** Returned value: ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë		
** 
*****************************************************************************/
uint8_t MdbCoin_Dispense(uint8_t amount, uint8_t type)
{
  uint8_t amount_type;

  amount_type = (amount << 4) | type;

  return Mdb_Session(COIN_DISPENSE, &amount_type, 1); 
}

/*****************************************************************************
** Function name:	MdbCoin_Expansion	
**
** Descriptions:	À©Õ¹ÃüÁî£¬ÓÃÓÚlevel3µÄ¸÷ÏîÃüÁî£¬ÀýÈç³ö±ÒÃüÁîµÈ
**						
**
** parameters:		subcommand:×ÓÃüÁî data[]:Êý¾Ý  count:Êý¾Ý³¤¶È
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_Expansion(uint8_t subcommand, uint8_t data[], uint8_t count)
{
  *data = subcommand;		   //Êý¾Ý´Ódata[1]¿ªÊ¼´æ´¢£¬Ê¹ÓÃÊ±×¢Òâ

  return  Mdb_Session(COIN_EXPANSION,data,count+1);
}

/*****************************************************************************
** Function name:	MdbCoin_GetID	
**
** Descriptions:	ÓÃÓÚlevel3,»ñÈ¡Ó²±ÒÆ÷ÕÒÁãÆ÷µÄÐÍºÅIDµÈµÈ
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_GetID(void)
{
  uint8_t data;

  return MdbCoin_Expansion(COIN_ID, &data, 0);
}

/*****************************************************************************
** Function name:	MdbCoin_FeatureEN	
**
** Descriptions:	ÓÃÓÚlevel3,µ±Ó²±ÒÆ÷Æô¶¯Ê±ºò£¬Èç¹ûÒÑ¾­²âµÃÖ§³Ölevel3Ä£Ê½£¬
**					µ÷ÓÃÕâ¸öº¯ÊýÓÃÓÚÊ¹ÄÜÀ©Õ¹ÃüÁî
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_FeatureEN(void)
{
  uint8_t data[5];

  data[1] = 0x00; 
  data[2] = 0x00; 
  data[3] = 0x00;
  data[4] = 0x07;

  return MdbCoin_Expansion(COIN_FEATURE_EN, data, 4);
}

/*****************************************************************************
** Function name:   MdbCoin_PayOut		
**
** Descriptions:   	ÓÃÓÚlevel3³ö±Ò
**						
**
** parameters:		amount:³ö±ÒÖµ
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_PayOut(uint8_t amount)
{
  uint8_t data[2];

  data[1] = amount; 

  return MdbCoin_Expansion(COIN_PAYOUT, data, 1);
}

/*****************************************************************************
** Function name:	MdbCoin_GetPaySt	
**
** Descriptions:	ÓÃÓÚlevel3»ñÈ¡³ö±ÒÇé¿ö
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë		
** 
*****************************************************************************/
uint8_t MdbCoin_GetPaySt(void)
{
  uint8_t data;

  return MdbCoin_Expansion(COIN_PAYOUT_ST, &data, 0);
}

/*****************************************************************************
** Function name:	MdbCoin_PayoutPoll	
**
** Descriptions:	ÓÃÓÚlevel3³ö±ÒÂÖÑ¯
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë		
** 
*****************************************************************************/
uint8_t MdbCoin_PayoutPoll(void)
{
  uint8_t data;

  return MdbCoin_Expansion(COIN_PAYOUT_POLL, &data, 0);
}

/*****************************************************************************
** Function name:	MdbCoin_Diagnostic	
**
** Descriptions:	ÓÃÓÚlevel3×´Ì¬Õï¶Ï
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_Diagnostic(void)
{
  uint8_t data;

  return MdbCoin_Expansion(COIN_DIAGNOSTIC, &data, 0);
}

/*****************************************************************************
** Function name:	MdbCoin_Poll	
**
** Descriptions:	ÆÕÍ¨ÂÖÑ¯
**						
**
** parameters:		
** Returned value:	ÕýÈ·ÏìÓ¦·µ»Ø½ÓÊÕÊý¾Ý³¤¶È£»·ñÔò·µ»Ø´íÎóÂë	
** 
*****************************************************************************/
uint8_t MdbCoin_Poll(void)
{
  return Mdb_Session(COIN_POLL,NULL,0); 
}







/*ÉÏµç¸´Î»MDBÓ²±ÒÆ÷*/
void MdbCoin_RstProc(void)
{
    uint8_t lenth;
	
	#if COINDEVICE			
	lenth = MdbCoin_Reset();			          //MDBÓ²±ÒÆ÷¸´Î»
	if(Mdb_ErrJudge(lenth) == TRUE) 
	{	
	   //¼ì²éÃüÁîÖ´ÐÐ½á¹û	£¬Èô³ö´í£¬ÔòMDBÓ²±ÒÆ÷²»ÔÚÏßÉÏ
	   //Coin´íÎó
	   CoinAcceptor_SetCommErr();		   		     
	} 
	#endif
}

/*MDBÓ²±ÒÆ÷³õÊ¼»¯¹ý³Ì*/
void MdbCoin_InitProc(void)
{
   uint8_t buf[36],lenth,j;
   CoinParaStruct cpara;

   //uint8_t Decimal_1 = 1;//120703 by cq MDBCompatible
   
   if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
   {
		g_ChangeMinCh = 0;
		for(j = 0; j < COIN_MDB_NUM; j++) 
	    {
		   if(SYSPara.CoinsValue[j] > 0)
		   {
			   g_ChangeMinCh = j;
			   break;
		   }
		}
   }
   
   MdbCoin_RstProc();
   Trace("\r\n coin1");

   if(CoinAcceptor_CommErr() == FALSE) 
   {
	   OSTimeDly(OS_TICKS_PER_SEC / 10);	   	   
	   
	   lenth = MdbCoin_Setup();			         //»ñÈ¡ÅäÖÃÐÅÏ¢	    
	   if(Mdb_ErrJudge(lenth) != TRUE) 
	   {
	   	  Trace("\r\n coin2");
	      //Í¨µÀjÓ²±ÒÃæÖµ=CoinValue[j]*CoinScale/CoinDecimal
	      Mdb_ReadBuf(buf,lenth);
		  
		  cpara.Level = buf[0];
		  //cpara.Level = 2;
		  //cpara.Scale = buf[3];

		  //120703 by cq MDBCompatible
		  /*
		  cpara.Decimal = 1;
		  for(j = 0; j < buf[4]; j++) 
		  {
		     cpara.Decimal *= 10;

			 //Decimal_1 *= 10;//120703 by cq MDBCompatible
		  }
		  */
		  cpara.Decimal = 100;
		  for(j = 0; j < buf[4]; j++) 
		  {
			 cpara.Decimal /= 10;
		  }
		  
		  cpara.Scale = buf[3] * cpara.Decimal;
		  
		  cpara.Routing	= (((uint16_t)buf[5]) << 8) | buf[6];
	      g_CoinMinValue = 0xFFFF;


		for(j = 0; j < 7; j++) 
		  Trace("\r\nbuf[0+0x%x] = 0x%x", j,buf[j]);

		  
		  for(j = 0; j < COIN_MDB_NUM; j++) 
		  {
			  Trace("\r\nbuf[7+0x%x] = 0x%x", j,buf[7+j]);
		  
			 if(buf[7+j] == 0xFF)
			 {
			     break;
			 }

			 //120703 by cq MDBCompatible
			 IncomeValue[COIN_MDB_BASE+j] = buf[7+j] * cpara.Scale;
			 //IncomeValue[COIN_MDB_BASE+j] = buf[7+j] * cpara.Scale * 10 / Decimal_1 ;
			 //IncomeValue[COIN_MDB_BASE+j] = buf[7+j] * cpara.Scale * cpara.Decimal;

			 //121106 by cq MDBCompatible
			  SYSPara.CoinsValue[j] = buf[7+j] * cpara.Scale;
			 
			 OutcomeValue[CHANGE_COIN_BASE+j] = IncomeValue[COIN_MDB_BASE+j];
			 if((OutcomeValue[CHANGE_COIN_BASE+j] > 0) && (OutcomeValue[CHANGE_COIN_BASE+j] < g_CoinMinValue)) 
			 {
			     g_CoinMinValue = OutcomeValue[CHANGE_COIN_BASE+j];
			 }

			 //120703 by cq MDBCompatible
			 
			 /*
			 if((CashTypeEnBit.CoinTypeEN & (1 << j)) && ((buf[7+j] * cpara.Scale) != SYSPara.CoinsValue[j])) 
			 {

				Trace("\r\n(CashTypeEnBit.CoinTypeEN & (1 << j)) && ((buf[7+j] * cpara.Scale) != SYSPara.CoinsValue[j])");
			 
			    CoinAcceptor_SetValueErr();
				CashTypeEnBit.CoinType = 0;
			 }
			 */

			 
		  }

		  //120703 by cq MDBCompatible
		/*
		  
		  for(;j < COIN_MDB_NUM; j++) 
		  {
		     if(SYSPara.CoinsValue[j] > 0)
			 {
				 Trace("\r\nif(SYSPara.CoinsValue[j] > 0)");
				 
			     CoinAcceptor_SetValueErr();
				 CashTypeEnBit.CoinType = 0;
			 }
		  }
		  */

		  
		  if(g_CoinMinValue == 0xFFFF) 
		  {
		      g_CoinMinValue = 1;
		  }

		  CoinAcceptor_SetCoinPara(cpara);
	   }
	
	   if(CoinAcceptor_GetCoinLevel() >= 3) 
	   {
	      //OSTimeDly(OS_TICKS_PER_SEC / 10);
	      //lenth = MdbCoin_GetID();			 //»ñÈ¡IDÐÅÏ¢
	
	      OSTimeDly(OS_TICKS_PER_SEC / 10);
	      lenth = MdbCoin_FeatureEN();		     //Ê¹ÄÜÀ©Õ¹Î»
	   }
	
	   OSTimeDly(OS_TICKS_PER_SEC / 10);
	   CashTypeEnBit.CoinFullBit = MdbCoin_CheckTubeStat(&g_CoinLeft);		         //»ñÈ¡¸÷Í¨µÀÓ²±ÒÊý   
	
	   if(g_CoinLeft > SYSPara.MoneyMaxin) 
	   {
	      g_CoinLeft = SYSPara.MoneyMaxin;
	   }
	
	 //121106 by cq BillSingleWork
	//if(Changer_LowStat() == TRUE) 
	 if((!isBillSingleWork()) && (Changer_LowStat() == TRUE))
	   {
		   CashTypeEnBit.BillType = 0;
	   }	
	   //»ñÈ¡Ó²±ÒÊ¹ÄÜÍ¨µÀ
	   for(j = 0; j < COIN_MDB_NUM; j++)
	   {			  
		   if(SYSPara.CoinsValue[j] > 0) 
		   {
		        CashTypeEnBit.CoinTypeEN |= (1 << j);
		   }
	   }	
	   Trace("\r\n coin3-%d,%d,%d,%d,%d,%d,%d,%d",SYSPara.CoinsValue[0],SYSPara.CoinsValue[1],SYSPara.CoinsValue[2],SYSPara.CoinsValue[3],SYSPara.CoinsValue[4],SYSPara.CoinsValue[5],SYSPara.CoinsValue[6],SYSPara.CoinsValue[7]);
   }
}

/*¼ì²éMDBÓ²±ÒÆ÷Ê£ÓàÓ²±ÒÇé¿ö*/
uint16_t MdbCoin_CheckTubeStat(uint32_t *coinleft)
{
    uint8_t  lenth,j;
	uint8_t  buf[20];
	uint32_t cintube = g_CoinLeft;
	uint16_t tfstat = 0;
	
	lenth = MdbCoin_GetTubeStatus();			     //»ñÈ¡¸÷Í¨µÀÓ²±ÒÊý
	if(Mdb_ErrJudge(lenth) != TRUE) 
	{
	   Mdb_ReadBuf(buf,lenth);
	   if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
	   {
		   cintube = 0;			                 //¼ÆËãTUBEÖÐÓ²±Ò×ÜÖµ
		   for(j = 0; j < COIN_MDB_NUM; j++) 
		   {
		      g_CoinTypeNum[j] = buf[2+j];
			  cintube += g_CoinTypeNum[j] * SYSPara.CoinsValue[j];	//»ñÈ¡×ÜÓ²±ÒÖµ
		   }
		   g_coin1Num = buf[2];
		   g_coin2Num = buf[3];
	   }					   					    

	   tfstat = ((uint16_t)(buf[0]) << 8) | buf[1];
	}
	*coinleft = cintube;
	
	return tfstat;   
}

#if 0
/*Level3 MDBÓ²±ÒÆ÷³ö±Òº¯Êý*/
/*²ÎÊý MoneyPay:³ö±ÒÖµ*/
void CoinPayout3(uint32_t MoneyPay) 
{
    uint8_t   buf[36],j,lenth;
	uint8_t   PayoutBusy,Outmin;
	uint32_t  temp,MoneyOut,MoneyAll;
		
	temp = MoneyPay;
	TimerDly.TimeDly = (temp/g_CoinMinValue/2+30) * 100;
	while(TimerDly.TimeDly > 0) {
		MoneyAll = 0;			                 //¼ÆËãTUBEÖÐÓ²±Ò×ÜÖµ
	    Outmin = COIN_MDB_NUM;
		for(j = 0; j < COIN_MDB_NUM; j++) {	    
		   //MoneyAll += g_CoinTypeNum[j] * SYSPara.CoinsValue[j];	//»ñÈ¡×ÜÓ²±ÒÖµ
		   MoneyAll += g_CoinTypeNum[j] * OutcomeValue[CHANGE_COIN_BASE+j];
		   if((g_CoinTypeNum[j] > 0) && (j < Outmin)) {
			   Outmin = j;
		   }
	    }					 				
	 					 
	    if(MoneyAll > temp)	{
		    MoneyAll = temp;					//³ö±ÒÖµ
		}

		if(MoneyAll > CoinScale*255) {
		    MoneyAll = CoinScale * 255;
		}

		if((MoneyAll == 0) || (MoneyAll < OutcomeValue[CHANGE_COIN_BASE+Outmin])) {
		    if((MsgBack.pBackMsg == 0) && (SYSPara.ChangerEnable == MDB_CHANGER_EN)) {
			    OSTimeDly(OS_TICKS_PER_SEC);
			}
			break;
		}

		OSTimeDly(OS_TICKS_PER_SEC / 10);					    		   			 		                
		 
		MdbCoin_PayOut(MoneyAll / CoinScale);	     //MDBÕÒÁãÖ¸Áî
		
		PayoutBusy = 1;	  
        while((TimerDly.TimeDly > 0) && (PayoutBusy == 1)) {
           OSTimeDly(OS_TICKS_PER_SEC / 10);

           lenth = MdbCoin_PayoutPoll();		     //ÂÖÑ¯³ö±ÒÇé¿ö
           if(Mdb_ErrJudge(lenth) != TRUE) {
              SYSPara.CoinsStat &= ~COIN_COMMERR_BIT;
			  if(lenth == 0) {				     //³ö±ÒÍê³ÉÍË³öÑ­»·
                 PayoutBusy = 0;
              }
           } else {
		      SYSPara.CoinsStat |= COIN_COMMERR_BIT;
			  if(SYSPara.ChangerEnable == MDB_CHANGER_EN) {
			      CashState.ChangerState |= CHANGER_ER;
			  }
			  CashState.CoinState |= COIN_DEV_ER;						  
	          break;
		   }
        }

        if(CashState.CoinState & COIN_DEV_ER) {
		    break;
		}
		
		do {
			OSTimeDly(OS_TICKS_PER_SEC / 10);					 
            lenth = MdbCoin_GetPaySt();			         //²éÑ¯³ö±ÒÇé¿ö
		} while((lenth == 0) && (TimeDly > 0));

	    if((Mdb_ErrJudge(lenth) != TRUE) && (lenth > 0)) {		                						   
		   Mdb_ReadBuf(buf,lenth);
		   MoneyOut = 0;						   
		   for(j = 0; (j < COIN_MDB_NUM) && (j < lenth); j++) {
			  if(BeSaleStat()) {
				  TranData.OutcomeCH[CHANGE_COIN_BASE+j] += buf[j];
			      TranTotal.OutcomeCH[CHANGE_COIN_BASE+j] += buf[j];
			  }
		      //MoneyOut += SYSPara.CoinsValue[j] * buf[j];	//Êµ¼Ê³ö±ÒÖµ
			  MoneyOut += OutcomeValue[CHANGE_COIN_BASE+j] * buf[j];

			  if(g_CoinTypeNum[j] >= buf[j]) {
			      g_CoinTypeNum[j] -= buf[j];
			  } else {
			      g_CoinTypeNum[j] = 0;
			  }							  
           }
		   if(BeSaleStat()) {
			   TranTotal.TotalChange += MoneyOut;
		   }
	 	   MsgBack.pBackMsg += MoneyOut;
		   temp -= MoneyOut;
		   
		   if(MoneyOut == 0) {
		       break;
		   }					 				           						
        } else {
		   break;
		}					
     }
}
#endif


/*Level3 MDBÓ²±ÒÆ÷³ö±Òº¯Êý*/
/*²ÎÊý MoneyPay:³ö±Ò½ð¶î*/
void MdbCoin_Payout3(uint32_t MoneyPay) 
{
    uint8_t   buf[36],j,lenth;
	uint8_t   PayoutBusy,Outmin,coinscale;
	uint32_t  temp,MoneyOut,MoneyAll;

		
	coinscale = CoinAcceptor_GetCoinScale();
	temp = MoneyPay;
	
	Trace("\r\nMdbCoin_Payout3 coinscale = %u temp = %ld",coinscale,temp);
	
	TimerDly.TimeDly = (temp/g_CoinMinValue/2+30) * 100;
	while(TimerDly.TimeDly > 0) 
	{
		Trace("\r\nTimerDly.TimeDly = %d", TimerDly.TimeDly);
	
		MoneyAll = 0;			                 //¼ÆËãTUBEÖÐÓ²±Ò×ÜÖµ
	    Outmin = COIN_MDB_NUM;
		for(j = 0; j < COIN_MDB_NUM; j++) 
		{	    
		   //MoneyAll += g_CoinTypeNum[j] * SYSPara.CoinsValue[j];	//»ñÈ¡×ÜÓ²±ÒÖµ
		   MoneyAll += g_CoinTypeNum[j] * OutcomeValue[CHANGE_COIN_BASE+j];

		   Trace("\r\ng_CoinTypeNum[%d] = %d, OutcomeValue[CHANGE_COIN_BASE+%d] = %d", j,g_CoinTypeNum[j],j,OutcomeValue[CHANGE_COIN_BASE+j]);

		   if((g_CoinTypeNum[j] > 0) && (j < Outmin)) 
		   {
			   Outmin = j;
		   }
	    }

		Trace("\r\nMoneyAll_1 = %ld", MoneyAll);
	 					 
	    if(MoneyAll > temp)	
		{
		    MoneyAll = temp;					//³ö±ÒÖµ
		}

		
		Trace("\r\nMoneyAll_2 = %ld", MoneyAll);
				
		if(MoneyAll > coinscale*255) 
		{
		    MoneyAll = coinscale * 255;
		}
		
		Trace("\r\nMoneyAll_3 = %ld", MoneyAll);

		Trace("\r\nOutcomeValue[CHANGE_COIN_BASE+%u] = %d  Outmin = %u",Outmin,OutcomeValue[CHANGE_COIN_BASE+Outmin],Outmin);

		if((MoneyAll == 0) || (MoneyAll < OutcomeValue[CHANGE_COIN_BASE+Outmin])) 
		{
		    if(temp == MoneyPay) 
			{			   //Ã»ÓÐ³ö±Ò
			    OSTimeDly(OS_TICKS_PER_SEC);
			}
			break;
		}

		OSTimeDly(OS_TICKS_PER_SEC / 10);	

		
		Trace("\r\nMoneyAll_4 = %ld, coinscale = %u", MoneyAll,coinscale);
		 
		MdbCoin_PayOut(MoneyAll / coinscale);	     //MDBÕÒÁãÖ¸Áî

		Trace("\r\n-a=");
		
		PayoutBusy = 1;	  
        while((TimerDly.TimeDly > 0) && (PayoutBusy == 1)) 
		{
			Trace("-b=");
		
           OSTimeDly(OS_TICKS_PER_SEC / 10);

           lenth = MdbCoin_PayoutPoll();		     //ÂÖÑ¯³ö±ÒÇé¿ö
           if(Mdb_ErrJudge(lenth) != TRUE)
		   {
			   Trace("-c=");
		   
              SYSPara1.CoinsStat &= ~COIN_COMMERR_BIT;
			  if(lenth == 0) 
			  {				     //³ö±ÒÍê³ÉÍË³öÑ­»·
                 PayoutBusy = 0;
              }
           } 
		   else
		   {
			   Trace("-d=");
		   
		      CoinAcceptor_SetCommErr();;
			  Changer_SetCommErr(0);					  
	          break;
		   }
        }

		
		Trace("-e=");

        if(CashState.CoinState & COIN_DEV_ER) 
		{
		    break;
		}

		
		Trace("-f=");
		
		do 
		{
			Trace("-g=");
		
			OSTimeDly(OS_TICKS_PER_SEC / 10);					 
            lenth = MdbCoin_GetPaySt();			         //²éÑ¯³ö±ÒÇé¿ö
		} 
		while((lenth == 0) && (TimerDly.TimeDly > 0));

		
		Trace("-h=");

	    if((Mdb_ErrJudge(lenth) != TRUE) && (lenth > 0)) 
		{		                		
			Trace("-i=");
		
		   Mdb_ReadBuf(buf,lenth);
		   MoneyOut = 0;	
		   //Trace("\r\n buf[0]=%x,buf[1]=%x,buf[2]=%x,buf[3]=%x,buf[4]=%x,buf[5]=%x,buf[6]=%x,buf[7]=%x",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
		   for(j = 0; (j < COIN_MDB_NUM) && (j < lenth); j++) 
		   {
			   Trace("-j=");
		   
			  Changer_UpDatepBuf(buf[j],j);
			  MoneyOut += OutcomeValue[CHANGE_COIN_BASE+j] * buf[j];
			  

			  if(g_CoinTypeNum[j] >= buf[j]) 
			  {
			      g_CoinTypeNum[j] -= buf[j];

				  
				  Trace("-k=");
			  } 
			  else 
			  {
			      g_CoinTypeNum[j] = 0;

				  
				  Trace("-l=");
			  }							  
           }
		   temp -= MoneyOut;

		   
		   Trace("-m=");
		   
		   if(MoneyOut == 0) 
		   {
		   
			   Trace("-n=");
		       break;
		   }					 				           						
        }
		else 
		{        
		
			Trace("-o=");
		   break;
		}		
		Trace("-p=");
    }	
	Trace("-q=");
}

#if 0
/*Level2 MDBÓ²±ÒÆ÷³ö±Òº¯Êý*/
/*²ÎÊý MoneyPay:³ö±ÒÖµ*/
void CoinPayout2(uint32_t MoneyPay)
{
    uint8_t   buf[36],i,j,lenth;
	uint8_t   PayoutBusy,num;
	uint16_t  numall;	
	uint32_t  MoneyOut,MoneyAll;
	
	MoneyAll = MoneyPay;                     //×Ü³ö±ÒÖµ

	TimeDly = (MoneyAll/g_CoinMinValue/2+30) * 100;
	for(j = COIN_MDB_NUM-1; TimerDly.TimeDly > 0; j--) {
	   MoneyOut = 0;  
	   /*if(((CashTypeEnBit.CoinTypeEN & (1 << j)) == 0) || (SYSPara.CoinsValue[j] == 0)) {
	      if(j > g_ChangeMinCh) {		 
		     continue;
		  } else {
		     break;
		  }
	   }*/

	   if(OutcomeValue[CHANGE_COIN_BASE+j] == 0) {
	       continue;
	   }

	   //numall = (uint8_t)(MoneyAll/SYSPara.CoinsValue[j]);
	   numall = (uint8_t)(MoneyAll/OutcomeValue[CHANGE_COIN_BASE+j]);
	   
	   while(TimerDly.TimeDly > 0) {
		   if(numall > g_CoinTypeNum[j]) {
		      num = g_CoinTypeNum[j];
		   } else {
		      num = numall;
		   }
		   if(num > 15) {
		       num = 15;
		   }

		   if(num == 0) {
		       break;
		   }
	   
	      MdbCoin_Dispense(num, j);					  					      
		
		  PayoutBusy = 1;	  
          while((TimerDly.TimeDly > 0) && (PayoutBusy == 1)) {
             OSTimeDly(OS_TICKS_PER_SEC / 10);

             lenth = MdbCoin_Poll();		     //ÂÖÑ¯³ö±ÒÇé¿ö
             if(Mdb_ErrJudge(lenth) != TRUE) {
                Mdb_ReadBuf(buf,lenth);
				SYSPara.CoinsStat &= ~COIN_COMMERR_BIT;
				for(i = 0; i < lenth; i++) {
                   if(buf[i] == 0x02) {	 //PayOut Busy
				      break;
				   }									
                }
			    if(i == lenth) {
			       PayoutBusy = 0;		     //³ö±ÒÍê³ÉÍË³öÑ­»·
			    }
             } else {
	            SYSPara.CoinsStat |= COIN_COMMERR_BIT;
				if(SYSPara.ChangerEnable == MDB_CHANGER_EN) {
				    CashState.ChangerState |= CHANGER_ER;
				}
				CashState.CoinState |= COIN_DEV_ER;
				break;
	         }
          } 
	   
		  if(((CashState.CoinState & COIN_DEV_ER) == 0)&& (TimerDly.TimeDly > 0)) {
			  if(BeSaleStat()) {
				  TranData.OutcomeCH[CHANGE_COIN_BASE+j] += num;
			      TranTotal.OutcomeCH[CHANGE_COIN_BASE+j] += num;
			  }
		      //MoneyOut += num * SYSPara.CoinsValue[j];  //Êµ¼Ê³ö±ÒÖµ
		      //MoneyAll -= num * SYSPara.CoinsValue[j];			  
			  MoneyOut += num * OutcomeValue[CHANGE_COIN_BASE+j];  //Êµ¼Ê³ö±ÒÖµ
		      MoneyAll -= num * OutcomeValue[CHANGE_COIN_BASE+j];						  

			  OSTimeDly(OS_TICKS_PER_SEC / 10);	
			  g_CoinTypeNum[j] -= num;						  
			  numall -= num;
		  }	else {
		      break;
		  }				      
	   }

	   MsgBack.pBackMsg += MoneyOut;
	   if(BeSaleStat()) {
		   TranTotal.TotalChange += MoneyOut;
	   }
	   
	   if((TimerDly.TimeDly == 0) || (CashState.CoinState & COIN_DEV_ER)) {					      
		  break;
	   }					   

	   if((j == 0/*g_ChangeMinCh*/) || (MoneyAll == 0)) {					      
		  if((MsgBack.pBackMsg == 0) && (SYSPara.ChangerEnable == MDB_CHANGER_EN)) {
			  OSTimeDly(OS_TICKS_PER_SEC);
		  }
		  break;
	   }
	}
}
#endif


/*Level2 MDBÓ²±ÒÆ÷³ö±Òº¯Êý*/
/*²ÎÊý MoneyPay:³ö±ÒÖµ*/
void MdbCoin_Payout2(uint32_t MoneyPay)
{
    uint8_t   buf[36],i,j,lenth;
	uint8_t   PayoutBusy,num;
	uint16_t  numall;	
	uint32_t  MoneyAll;
	
	MoneyAll = MoneyPay;                     //×Ü³ö±ÒÖµ

	TimerDly.TimeDly = (MoneyAll/g_CoinMinValue/2+30) * 100;
	for(j = COIN_MDB_NUM-1; TimerDly.TimeDly > 0; j--) 
	{
	   /*if(((CashTypeEnBit.CoinTypeEN & (1 << j)) == 0) || (SYSPara.CoinsValue[j] == 0)) {
	      if(j > g_ChangeMinCh) {		 
		     continue;
		  } else {
		     break;
		  }
	   }*/

	   if(OutcomeValue[CHANGE_COIN_BASE+j] == 0) 
	   {
	       continue;
	   }

	   //numall = (uint8_t)(MoneyAll/SYSPara.CoinsValue[j]);
	   numall = (uint8_t)(MoneyAll/OutcomeValue[CHANGE_COIN_BASE+j]);
	   
	   while(TimerDly.TimeDly > 0) 
	   {
		   if(numall > g_CoinTypeNum[j])
		   {
		      num = g_CoinTypeNum[j];
		   } 
		   else 
		   {
		      num = numall;
		   }
		   if(num > 15) 
		   {
		       num = 15;
		   }

		   if(num == 0) 
		   {
		       break;
		   }
	   
	      MdbCoin_Dispense(num, j);					  					      
		
		  PayoutBusy = 1;	  
          while((TimerDly.TimeDly > 0) && (PayoutBusy == 1)) 
		  {
             OSTimeDly(OS_TICKS_PER_SEC / 10);

             lenth = MdbCoin_Poll();		     //ÂÖÑ¯³ö±ÒÇé¿ö
             if(Mdb_ErrJudge(lenth) != TRUE) 
			 {
                Mdb_ReadBuf(buf,lenth);
				SYSPara1.CoinsStat &= ~COIN_COMMERR_BIT;
				for(i = 0; i < lenth; i++) 
				{
                   if(buf[i] == 0x02) 
				   {	 //PayOut Busy
				      break;
				   }									
                }
			    if(i == lenth) 
				{
			       PayoutBusy = 0;		     //³ö±ÒÍê³ÉÍË³öÑ­»·
			    }
             } 
			 else
			 {
	            CoinAcceptor_SetCommErr();
				Changer_SetCommErr(0);
				break;
	         }
          } 
	   
		  if((Changer_CommErr(0) == FALSE) && (TimerDly.TimeDly > 0)) 
		  {
			  Changer_UpDatepBuf(num,j);						  

			  OSTimeDly(OS_TICKS_PER_SEC / 10);	
			  g_CoinTypeNum[j] -= num;						  
			  numall -= num;
		  }	
		  else 
		  {
		      break;
		  }				      
	   }
	   
	   if((TimerDly.TimeDly == 0) || (Changer_CommErr(0) == TRUE)) 
	   {					      
		   break;
	   }					   

	   if((j == 0/*g_ChangeMinCh*/) || (MoneyAll == 0)) 
	   {					      
		  if(MoneyAll == MoneyPay) 
		  {
			  OSTimeDly(OS_TICKS_PER_SEC);
		  }
		  break;
	   }
	}
}

#if 0
/*MDBÓ²±ÒÆ÷ÕÒ±ÒÃüÁî*/
/*²ÎÊý MoneyPay:³ö±ÒÖµ*/
void CoinPayoutCmd(uint32_t MoneyPay)
{    
     uint8_t   buf[36],j,lenth;	
	 
	 #if COINDEVICE
	 OSTimeDly(OS_TICKS_PER_SEC / 10);
	 
	 lenth = MdbCoin_GetTubeStatus();			         //»ñÈ¡²¢¸üÐÂÓ²±ÒÊý
     if(Mdb_ErrJudge(lenth) != TRUE) {
         Mdb_ReadBuf(buf,lenth);
		 for(j = 0; j < COIN_MDB_NUM; j++) {
             g_CoinTypeNum[j] = buf[2+j];
         }
     }
	 	 				 
	 MsgBack.pBackMsg = 0;
	 
	 if(CoinLevel >= 3)	 {//LEVEL3
	    CoinPayout3(MoneyPay);						 
	 } else {//LEVEL2
	    CoinPayout2(MoneyPay);														 			        
	 } //LEVEL2

	 OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

     if(MsgBack.pBackMsg > 0) {
		 OSTimeDly(OS_TICKS_PER_SEC / 10);
	     //CheckCoinsLeft();
	 }	             
     #endif
}
#endif

void MdbCoin_PayoutProc(uint32_t MoneyPay)
{    
     uint8_t   buf[36],j,lenth;	

	 /*

	 
	 NRI
	  
	  case MDB_CHANGER_EN: moneyout = 10
	  MdbCoin_PayoutProc-3=
	  -3=
	  -3=
	  -3=
	  -4=
	  -5=
	  -6=
	  -8=inState = 0x1
	 
	 case MDB_CHANGER_EN: moneyout = 10
	 MdbCoin_PayoutProc
	 -3=
	 buf[2+0x0] = 0x0
	 -3=
	 buf[2+0x1] = 0x0
	 -3=
	 buf[2+0x2] = 0x20 //32
	 -3=
	 buf[2+0x3] = 0x0
	 -3=
	 buf[2+0x4] = 0x0
	 -3=
	 buf[2+0x5] = 0x0
	 -3=
	 buf[2+0x6] = 0x0
	 -3=
	 buf[2+0x7] = 0x0
	 -4=
	 -5=
	 -6=
	 -8=.CoinState = 0x0


	  

	 MEI
	 case MDB_CHANGER_EN: moneyout = 10
	MdbCoin_PayoutProc	
	-3=
	-3=
	-3=
	-3=
	-4=
	-5=
	-6=
	-8=ate = 0x0

	 case MDB_CHANGER_EN: moneyout = 10
	 MdbCoin_PayoutProcf[0+0x1] = 0x0
	 -3=
	 buf[2+0x0] = 0x4  //4
	 -3=
	 buf[2+0x1] = 0x10 //16
	 -3=
	 buf[2+0x2] = 0x0
	 -3=
	 buf[2+0x3] = 0x0
	 -3=
	 buf[2+0x4] = 0x0
	 -3=
	 buf[2+0x5] = 0x0
	 -3=
	 buf[2+0x6] = 0x0
	 -3=
	 buf[2+0x7] = 0x0
	 -4=
	 -5=
	 -6=
	 -8=tate = 0x0


	 */

	 Trace("\r\nMdbCoin_PayoutProc");
	 
	 #if COINDEVICE
	 OSTimeDly(OS_TICKS_PER_SEC / 10);
	 
	 lenth = MdbCoin_GetTubeStatus();			         //»ñÈ¡²¢¸üÐÂÓ²±ÒÊý

	Trace("\r\n-1=");
	 if(Mdb_ErrJudge(lenth) != TRUE) 
	 {
	 
	 Trace("\r\n-2=");
	 
         Mdb_ReadBuf(buf,lenth);
		 
		 for(j = 0; j < 2; j++) 
		   Trace("\r\nbuf[0+0x%x] = 0x%x", j,buf[j]);

		 
		 for(j = 0; j < COIN_MDB_NUM; j++) 
		 {
			 Trace("\r\n-3=");
             g_CoinTypeNum[j] = buf[2+j];
			 
			 Trace("\r\nbuf[2+0x%x] = 0x%x", j,buf[2+j]);
         }
     }	 	 				 

	 Trace("\r\n-4=");
	 if(CoinAcceptor_CommErr() == FALSE) 
	 {
	 
	 Trace("\r\n-5=");
		 if(CoinAcceptor_GetCoinLevel() >= 3)	 
		 {//LEVEL3
			 Trace("\r\n-6=");
		    MdbCoin_Payout3(MoneyPay);						 
		 } 
		 else 
		 {//LEVEL2
			 Trace("\r\n-7=");
		    MdbCoin_Payout2(MoneyPay);														 			        
		 } //LEVEL2	  
	 }           
     #endif

	 
	 Trace("\r\n-8=");
}

  
/******************************************************************************
**                            End Of File
******************************************************************************/
