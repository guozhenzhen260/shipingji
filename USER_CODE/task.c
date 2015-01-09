

//#include "..\drivers\lcd.h"
//#include "..\config.h"

#include "reader.h"
#include "task.h"
#include "global.h"
#include "lcd.h"

//OS_EVENT	*semInit;
//OS_EVENT	*mbVmcToCashless;
//OS_EVENT	*mbCashlessToVmc;


//MessageBack  MsgBack;


#define DISP_BUF_SIZE 25
#define DISP_TIME_DLY 200 // 200 == 1s

void Disp_BeginSession()
{
	uint8_t str[DISP_BUF_SIZE] = {0};
	uint16_t iFunds;

	strcpy(str,"Begin Session...");
	WriteFull_LCD(LINE0,str,strlen(str));

	iFunds = Reader_GetFundsMultiplyUnit();
	sprintf(str,"Balance: %d.%02d", iFunds/100, iFunds%100);
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}


void Disp_VendApproved()
{	
	uint8_t str[DISP_BUF_SIZE] = {0};
	uint16_t iValue;
	
	strcpy(str,"Vend Approved...");
	WriteFull_LCD(LINE0,str,strlen(str));
	
	iValue = Reader_GetValueDivideUnit();
	sprintf(str,"Price: %d.%02d", iValue/100, iValue%100);
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}


void Disp_VendDenied()
{	
	uint8_t str[DISP_BUF_SIZE] = {0};
	uint16_t iValue;
	
	strcpy(str,"Vend Denied...");
	WriteFull_LCD(LINE0,str,strlen(str));
	
	iValue = Reader_GetValueDivideUnit();
	sprintf(str,"Price: %d.%02d", iValue/100, iValue%100);
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}


void Disp_EndSession()
{	
	uint8_t str[DISP_BUF_SIZE] = {0};
	
	strcpy(str,"End Session...");
	WriteFull_LCD(LINE0,str,strlen(str));
	
	strcpy(str,"See You Next Time");
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}

void Disp_MalfunctionError()
{
	uint8_t str[DISP_BUF_SIZE] = {0};
	
	//strcpy(str,"MalfunctionError...");
	strcpy(str,"Reader Malfunction...");
	WriteFull_LCD(LINE0,str,strlen(str));
	
	//strcpy(str,"Please Check Reader");	
	sprintf(str,"ErrorCode: %02x", Reader_GetErrorCode());
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}


void Disp_CmdOutOfSequence()
{
	uint8_t str[DISP_BUF_SIZE] = {0};
	
	strcpy(str,"Cmd Out Of Sequence");
	WriteFull_LCD(LINE0,str,strlen(str));
	
	strcpy(str,"Please Restart Vmc");
	WriteFull_LCD(LINE1,str,strlen(str));
	
	OSTimeDly(DISP_TIME_DLY);
}



//Reader_SetVmcSta(uint8_t cVmcSta)
//Reader_GetVmcSta()


/*


user plug in the card: 

RTV_BEGIN_SESSION 
balance = iFundsAvailable * moneyUnit; Reader_GetFundsMultiplyUnit();
display balance



user make choose:

hasnt goods: display hasnt goods
gmount not enough: display money not enough



all pass:

ItemValue = Price; Reader_SetItemValue(1);
cost = Price / moneyUnit; Reader_GetValueDivideUnit();
Reader_VendRequest

VTR_VEND_REQUEST   RTV_VEND_APPROVED 
out goods now, display success or failure
VTR_VEND_SUCCESS or VTR_VEND_FAILURE
and display balance

VTR_VEND_REQUEST   RTV_VEND_DENIED 
do not out goods, display reader denied
and display balance



the card out:

RTV_END_SESSION
display goodbye, return to the welcome

*/

uint8_t Vmc_HandleReaderMsg(uint8_t cMsg)
{
	uint8_t cSta = MVC_IDLE;
	
	switch(cMsg)
	{
		//case RTV_JUST_RESET:		
		//break;		
		//case RTV_READER_CONFIG_INFO:
		//break;		
		//case RTV_DISPLAY_REQUEST:
		//break;
		
		case RTV_BEGIN_SESSION: 		
			//Disp_BeginSession();
			//Reader_SetVmcSta(MVC_SALE);
			g_Amount = Reader_GetFundsMultiplyUnit();
			if(IsMoneyInEN()) //如果投币器状态显示是使能的，予以禁能
		    {
			   SetTouBiStat(CMD_ALL_DIS);  //禁能投币  
			   DisableMoneyIn();//如果投币器状态显示设为禁能
		    }
			cSta = MVC_SALE;
		break;
		
		//case RTV_SESSION_CANCEL_REQUEST:
		//break;
		
		case RTV_VEND_APPROVED:
			//Disp_VendApproved();
			//Reader_SetVmcSta(MVC_CHUHUO);			
			cSta = MVC_CHUHUO;
		break;
		
		case RTV_VEND_DENIED:
			//Disp_VendDenied();
			//Reader_SetVmcSta(MVC_SALE);			
			cSta = MVC_SALE;
		break;
		
		case RTV_END_SESSION:	
			//Disp_EndSession();
			Reader_EndSessionHandle();
			//Reader_SetVmcSta(MVC_PAYOUT);			
			g_Amount = 0;
			//cSta = MVC_PAYOUT;
			cSta = MVC_SAVEDAT;
			//cSta = MVC_BEGIN;
		break;

		//case RTV_CANCELLED:
		//break;
		
		//case RTV_PERIPHERAL_ID:
		//break;
		
		case RTV_MALFUNCTION_ERROR:
			//Disp_MalfunctionError();
			//Reader_SetVmcSta(uint8_t);			
			//cSta = MVC_BEGIN;
		break;
		
		case RTV_CMD_OUT_OF_SEQUENCE:
			//Disp_CmdOutOfSequence();
			//Reader_SetVmcSta(uint8_t);			
			//cSta = MVC_BEGIN;
		break;

		default://...
			Trace_R("\r\nGet Reader Other cMsg = %02x", cMsg);
		break;
	}
	
	return cSta;
}



uint32_t * Vmc_PendFromReader(uint8_t * SaleStat, uint8_t SaleStat_2 ,uint16_t iTime)
{
	uint8_t cSta,cMsg,err;
	uint32_t *PKeyInput;

	if(SYSPara.ReaderEnable)
	{
		PKeyInput = OSMboxPend(mbCashlessToVmc,iTime,&err);
		
		if (err == OS_NO_ERR)
		{
			cMsg = (uint8_t)*PKeyInput;
			cSta = Vmc_HandleReaderMsg(cMsg);
			if(cSta != MVC_IDLE)
				*SaleStat = cSta;
		}
		else
			*SaleStat = SaleStat_2;
	}

	return PKeyInput;
}

void TaskBTest(void)
{
	uint8_t i,err,cMsg,len,key;
	uint32_t *lpMsg;

	while(1)
	{
		lpMsg = OSMboxPend(mbCashlessToVmc,OS_TICKS_PER_SEC / 10,&err);

		if (err == OS_NO_ERR)
		{
			cMsg = (uint8_t)*lpMsg;
			Vmc_HandleReaderMsg(cMsg);		
		}
		
		key = Reader_GetKey();
		
		if(key != 0)
		{
			//VTR_VEND_REQUEST:
			//VTR_VEND_SUCCESS:
			//VTR_VEND_FAILURE:
			//VTR_SESSION_COMPLETE:
			//VTR_READER_DISABLE:
			//VTR_READER_ENABLE:			
			//VTR_READER_CANCEL:
				
			Vmc_PostToReader(key);
		}
	}

}


/*
	else if(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) 
	{								   
		SaleStat = ;	
		
	Vmc_PostToReader(VTR_VEND_REQUEST); 

	//Pend...


	else 
	{														 
	   g_Amount -= Channels[x][y].GoodsPrice;					   
	
	Vmc_PostToReader(VTR_VEND_SUCCESS); //Reader_SetVmcSta(MVC_SALE);
	Vmc_PostToReader(VTR_VEND_FAILURE); //Reader_SetVmcSta(MVC_SALE);

*/



void TaskCTest(void)
{
	uint8_t i,err,cMsg,len,key;
	uint32_t *lpMsg;
	
	OSTimeDly(OS_TICKS_PER_SEC * 3);
	
	Reader_InitPara();
	
	while(1)
	{
		lpMsg = OSMboxPend(mbVmcToCashless,OS_TICKS_PER_SEC / 10,&err);

		if (err == OS_NO_ERR)  
			cMsg = (uint8_t)*lpMsg;
		else
			cMsg = 0;
		
		Reader_MainProc(cMsg);
	}

}


