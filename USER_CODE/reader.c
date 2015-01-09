
//#include "..\firmware\firmware.h"
#include "reader.h"
#include "task.h"

#include <stdarg.h>
#include "global.h"

ReaderParaStruct ReaderPara;



void Trace_R( uint8_t *format , ... )
{

#if 0

	va_list arg_ptr;
	uint8_t StringTemp[ 1024 ];
	uint16_t i;
	uint16_t len;

	//#ifdef _DEBUG_Trace_R
	va_start(arg_ptr, format);
	//len = vsprintf( StringTemp , format , arg_ptr );
	len = vsprintf( (char *)StringTemp , (const char *)format , arg_ptr );//120331 by cq disable warning
	va_end(arg_ptr);
	for( i = 0 ; i < len ; i ++ )
	{
		Uart0_SendByte( StringTemp[ i ] );
		//Uart0PutChar(StringTemp[ i ] );
	}
	//#endif
	
#endif
}

void Reader_SetComErr()
{
	ReaderPara.cComErr = 1;

	//ReaderPara.cComErr++;
	//if(ReaderPara.cComErr == 255)
		//ReaderPara.cComErr = 1;
}

void Reader_ClrComErr()
{
	ReaderPara.cComErr = 0;
}

uint8_t Reader_IsComErr()
{
	return ReaderPara.cComErr;

	//if(ReaderPara.cComErr > 10)
		//return 1;
	//else
		//return 0;
}

//if(Reader_IsComErr())
//SetDevStatus(DEV_ERR_BIT_READER);
//else
//ClrDevStatus(DEV_ERR_BIT_READER);


void Vmc_PostToReader(uint8_t cMsg)
{
	MsgBack.msgVTR = cMsg;
	OSMboxPost(mbVmcToCashless,&MsgBack.msgVTR);																								
}

void Reader_PostToVmc(uint8_t cMsg)
{
	MsgBack.msgRTV = cMsg;
	OSMboxPost(mbCashlessToVmc,&MsgBack.msgRTV);																								
}

uint16_t Reader_GetFundsMultiplyUnit()
{

	return (ReaderPara.iFundsAvailable * ReaderPara.iValueUnit);
}

uint16_t Reader_GetValueDivideUnit()
{
	if(ReaderPara.iValueUnit == 0)
		ReaderPara.iValueUnit = 1;
	
	Trace_R("     ReaderPara.iItemValue = %u", ReaderPara.iItemValue);

	return (ReaderPara.iItemValue / ReaderPara.iValueUnit);
}

void Reader_SetItemValue(uint16_t iItemValue)
{
	ReaderPara.iItemValue = iItemValue;	
}


uint8_t Reader_GetErrorCode()
{
	return ReaderPara.cErrorCode;
}


/*
void Reader_SetVmcSta(uint8_t cVmcSta)
{
	ReaderPara.cVmcSta = cVmcSta;
}

uint8_t Reader_GetVmcSta()
{
	return ReaderPara.cVmcSta;
}
*/


#define READER_SEND


uint8_t Reader_Reset()
{
	uint8_t len;
	
	Trace_R("\r\nMBV_RESET...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_RESET,NULL,0);	

	return len;
}


uint8_t Reader_SetupConfigData()
{
	uint8_t len;
	uint8_t ConfigData[5];

	Trace_R("\r\nMBV_SETUP CONFIGURATION DATA...");	
	Trace_R("\r\nW = XX - "); 
	ConfigData[0] = 0x00;//Configuration data
	ConfigData[1] = 0x01;//VMC Feature Level
	ConfigData[2] = 0x00;//Columns on Display
	ConfigData[3] = 0x00;//Rows on Display
	ConfigData[4] = 0x00;//Display Information
	len = Mdb_Session(MBV_R_SETUP,ConfigData,5);	

	return len;
}



uint8_t Reader_SetupMaxMinPrice()
{
	uint8_t len;
	uint8_t MaxMinPrice[5];
	
	Trace_R("\r\nMBV_SETUP MAX MIN PRICE...");	
	Trace_R("\r\nW = XX - "); 
	MaxMinPrice[0] = 0x01;//Max / Min prices
	MaxMinPrice[1] = 0xff;//Maximum Price – scaled
	MaxMinPrice[2] = 0xff;
	MaxMinPrice[3] = 0x00;//Minimum Price – scaled
	MaxMinPrice[4] = 0x00;
	len = Mdb_Session(MBV_R_SETUP,MaxMinPrice,5);	

	return len;
}

uint8_t Reader_Poll()
{
	uint8_t len;
	
	Trace_R("\r\nMBV_POLL...");	
	Trace_R("\r\nW = XX - "); 
	len = Mdb_Session(MBV_R_POLL,NULL,0); 

	return len;
}



uint8_t Reader_VendRequest(uint16_t ItemPrice, uint16_t ItemNumber)
{
	uint8_t len;
	uint8_t Request[5];
	
	Trace_R("\r\nVendRequest...");		
	Trace_R("\r\nW = XX - ");
	Request[0] = 0x00;//VEND REQUEST
	Request[1] = (uint8_t)ItemPrice >> 8;//Item Price - scaled交易出货单价
	Request[2] = (uint8_t)ItemPrice;
	Request[3] = (uint8_t)ItemNumber >> 8;;//Item Number交易出货数量
	Request[4] = (uint8_t)ItemNumber;
	len = Mdb_Session(MBV_R_VEND,Request,5);	

	return len;
}

uint8_t Reader_VendCancel()
{
	uint8_t len;
	uint8_t Cancel[1] = {0x01};//VEND CANCEL
	
	Trace_R("\r\nVendCancel...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_VEND,Cancel,1);	

	return len;
}

uint8_t Reader_VendSuccess(uint16_t ItemNumber)
{
	uint8_t len;
	uint8_t Success[3];
	
	Trace_R("\r\nVendSuccess...");		
	Trace_R("\r\nW = XX - ");
	Success[0] = 0x02;//VEND SUCCESS 
	Success[1] = (uint8_t)ItemNumber >> 8;//Item Number
	Success[2] = (uint8_t)ItemNumber;
	len = Mdb_Session(MBV_R_VEND,Success,3);	

	return len;
}

uint8_t Reader_VendFailure()
{
	uint8_t len;
	uint8_t Failure[1] = {0x03};//VEND FAILURE
	
	Trace_R("\r\nVendFailure...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_VEND,Failure,1);	

	return len;
}


uint8_t Reader_SessionComplete()
{
	uint8_t len;
	uint8_t SessionComplete[1] = {0x04};//SESSION COMPLETE
	
	Trace_R("\r\nSessionComplete...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_VEND,SessionComplete,1);	

	return len;
}



uint8_t Reader_ReaderDisable()
{
	uint8_t len;
	uint8_t Disable[1] = {0x00};
	
	Trace_R("\r\nReaderDisable...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_READER,Disable,1);	

	return len;
}

uint8_t Reader_ReaderEnable()
{
	uint8_t len;
	uint8_t Enable[1] = {0x01};
	
	Trace_R("\r\nReaderEnable...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_READER,Enable,1);	

	return len;
}

uint8_t Reader_ReaderCancel()
{
	uint8_t len;
	uint8_t Cancel[1] = {0x02};
	
	Trace_R("\r\nReaderCancel...");		
	Trace_R("\r\nW = XX - ");
	len = Mdb_Session(MBV_R_READER,Cancel,1);	

	return len;
}



#define READER_RECV


void Reader_UpdateConfigData(uint8_t * buf, uint8_t len)
{
	uint8_t i;	

	//R = 09 -	01 02 19 78 01 02 14 03 ae	   err = 00
	ReaderPara.cConfigData = buf[0];//01
	ReaderPara.cFeatureLevel = buf[1];//02
	ReaderPara.iCountryCode = ((((uint16_t)buf[2]) << 8) | buf[3]);//19+78 - Euro
	ReaderPara.cScaleFactor = buf[4];//01
	ReaderPara.cDecimalPlaces = buf[5];//02 - 0.01
	ReaderPara.cAppMaxRespTime = buf[6];//14 - 20s
	ReaderPara.cMixcellaneousOpt = buf[7];//03 - refunds and multivend
	
	ReaderPara.iValueUnit = 100;
	for(i = 0; i < ReaderPara.cDecimalPlaces; i++)
	{
		ReaderPara.iValueUnit /= 10;
	}
	ReaderPara.iValueUnit *= ReaderPara.cScaleFactor;
	//Price  /=  iValueUnit; - scaled
	
	Trace_R("     ReaderPara.iValueUnit = %u",ReaderPara.iValueUnit);
	
}


void Reader_BeginSession(uint8_t * buf, uint8_t len)
{
	//R = 03 -  03 13 88	

	ReaderPara.iFundsAvailable = ((((uint16_t)buf[1]) << 8) | buf[2]);//fffe ffff

	Trace_R("     iFundsAvailable = %u",ReaderPara.iFundsAvailable);
	
	if(ReaderPara.cFeatureLevel > 0x01)
	{
		//and so on ...
	}
}

void Reader_VendApproved(uint8_t * buf, uint8_t len)
{
	
	//Vend Amount - scaled
	ReaderPara.iVendAmount = ((((uint16_t)buf[1]) << 8) | buf[2]);//ffff

	Trace_R(" 	iVendAmount = %u",ReaderPara.iVendAmount);
}

void Reader_MalfunctionError(uint8_t * buf, uint8_t len)
{
	ReaderPara.cErrorCode = buf[1] & 0xF0;

	switch(ReaderPara.cErrorCode)
	{
		case 0x00: Trace_R("Payment media Error1"); break;
		case 0x10: Trace_R("Invalid Payment media1"); break;
		case 0x20: Trace_R("Tamper Error1"); break;
		case 0x30: Trace_R("Manufacturer Defined Error1"); break;
		case 0x40: Trace_R("Communications Error2"); break;
		case 0x50: Trace_R("Reader Requires Service2"); break;
		case 0x60: Trace_R("Unassigned2"); break;
		case 0x70: Trace_R("Manufacturer Defined Error2"); break;
		case 0x80: Trace_R("Reader Failure3"); break;
		case 0x90: Trace_R("Communications Error3"); break;
		case 0xA0: Trace_R("Payment media Jammed3"); break;
		case 0xB0: Trace_R("Manufacturer Defined Error"); break;
		case 0xC0: Trace_R("Refund error – internal reader credit lost"); break;
		default: Trace_R("Unassigned"); break;
	}
}

void Reader_UpdatePollMsg(uint8_t * buf, uint8_t len)
{

	switch(buf[0])
	{
		case RTV_JUST_RESET:		
			Trace_R("\r\nJustReset...");
			//do nothing ???
		break;
		
		case RTV_READER_CONFIG_INFO:
			Trace_R("\r\nReaderUpdateConfigData...");
			Reader_UpdateConfigData(buf,len);//3返回setup的信息，比如level,比例因子等
		break;
		
		case RTV_DISPLAY_REQUEST:
			Trace_R("\r\nDisplayRequest...");
			//for future use...
		break;
		
		case RTV_BEGIN_SESSION://6.poll到插卡信息，进入交易session			
			Trace_R("\r\nBeginSession...");
			Reader_BeginSession(buf,len);//得到卡内金额		
			ReaderPara.cReaderSta = READER_STA_SESSION_IDLE;
			Reader_PostToVmc(buf[0]);//display balance	将卡金额邮箱发送给vmc交易流程，开始进行交易				
		break;
		
		case RTV_SESSION_CANCEL_REQUEST://8情况一拔卡，取消交易session
			Trace_R("\r\nSessionCancelRequest...");
			ReaderPara.cSessionCancel = 1;//置1，告诉vmc取消交易session
		break;
		
		case RTV_VEND_APPROVED://8情况二卡金额足够，返回允许交易
			Trace_R("\r\nVendApproved...");	
			Reader_VendApproved(buf, len);
			Reader_PostToVmc(buf[0]);//allow dispense告诉vmc可以出货
		break;
		
		case RTV_VEND_DENIED:
			Trace_R("\r\nVendDenied...");
			Reader_PostToVmc(buf[0]);//belance not enough
		break;
		
		case RTV_END_SESSION://11结束本session,返回空闲使能状态	
			Trace_R("\r\nEndSession...");
			ReaderPara.cReaderSta = READER_STA_ENABLED;//返回使能状态
			Reader_PostToVmc(buf[0]);//display goodbye
		break;

		case RTV_CANCELLED:
			Trace_R("\r\nCancelled...");
			//reader will reset
		break;
		
		case RTV_PERIPHERAL_ID:
			Trace_R("\r\nPeripheralID...");
		break;
		
		case RTV_MALFUNCTION_ERROR:
			Trace_R("\r\nMalfunctionError...");
			Reader_MalfunctionError(buf, len);
			Reader_PostToVmc(buf[0]);//reader error
		break;
		
		case RTV_CMD_OUT_OF_SEQUENCE:
			Trace_R("\r\CmdOutOfSequence");
			Reader_PostToVmc(buf[0]);
		break;

		//level 1+...		
		
		//case 0x0D://Revalue Approved
		//break;
		//case 0x0E://Revalue Denied
		//break;
		//case 0x0F://Revalue Limit Amount
		//break;
		//case 0x10://User File Data
		//break;
		//case 0x11://Time/Data Request
		//break;
		//case 0x12://Data Entry Request
		//break;	
		
		default://...
			Trace_R("\r\nLevel 1+ ... buf[0] = %02x", buf[0]);
		break;
	}
}

#define READER_PROC


//Reader_PollProc()
//void Reader_Proc(Reader_SetupProc);
//void Reader_Proc(Reader_PollProc);
//void Reader_Proc(Reader_SessionCompleteProc);




void Reader_Proc(uint8_t (*fpReader_Proc)())
{
	uint8_t len;
	uint8_t i;	
	uint8_t buf[36];

	for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
		len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
	{
		len = fpReader_Proc();//.向串口发送相应指令
		if(Mdb_ErrJudge(len) == TRUE) 
		{
			Trace_R("\r\nMdb_ErrJudge...");
			Reader_SetComErr();
		} 
		else if(len>0 && len<0x80)
		{
			len &= 0x0F;
			Trace_R("\r\nbuf");
			Mdb_ReadBuf(buf,len);//从串口接收到对这条指令，读卡器回复的数据
			Trace_R("\r\nA = %02x - ",len);				
			for(i = 0; i < len; i++)
				Trace_R(" %02x",buf[i]);
	
			Reader_UpdatePollMsg(buf, len);
			
			//if(Reader_IsComErr())		
				Reader_ClrComErr();
		}
		else
		{
			//if(Reader_IsComErr())		
				Reader_ClrComErr();
		}
	}
	//while(len >= 0x80);
}


/*
void Reader_SetupProc()
{
	uint8_t len;
	uint8_t i;	
	uint8_t buf[36];

	buf[0] = 0;
	//do
	for(ReaderPara.iTimeDly = READER_TIMEDLY;  buf[0] != 0x01 || ReaderPara.iTimeDly != 0 ; )
	{
		len = Reader_SetupConfigData();
		
		if(Mdb_ErrJudge(len) == TRUE) 
		{
			Trace_R("\r\nMdb_ErrJudge...");
			Reader_SetComErr();
		} 
		else if(len>0 && len<0x80)
		{
			len &= 0x0F;
			Trace_R("\r\nbuf");
			Mdb_ReadBuf(buf,len);
			Trace_R("\r\nR = %02x - ",len);				
			for(i = 0; i < len; i++)
				Trace_R(" %02x",buf[i]);
			
			Reader_UpdatePollMsg(buf,len);
		}		
	}
	//while(len >= 0x80 || buf[0] != 0x01);	
	
}

void Reader_PollProc()
{
	uint8_t len;
	uint8_t i;	
	uint8_t buf[36];

	do
	{
		len = Reader_Poll();
		if(Mdb_ErrJudge(len) == TRUE) 
		{
			Trace_R("\r\nMdb_ErrJudge...");
			Reader_SetComErr();
		} 
		else if(len>0 && len<0x80)
		{
			len &= 0x0F;
			Trace_R("\r\nbuf");
			Mdb_ReadBuf(buf,len);
			Trace_R("\r\nA = %02x - ",len);				
			for(i = 0; i < len; i++)
				Trace_R(" %02x",buf[i]);
	
			Reader_UpdatePollMsg(buf, len);
		}	
	}
	while(len >= 0x80);
}

void Reader_SessionCompleteProc()
{
	uint8_t len;
	uint8_t i;	
	uint8_t buf[36];

	do
	{
		len = Reader_SessionComplete();
		if(Mdb_ErrJudge(len) == TRUE) 
		{
			Trace_R("\r\nMdb_ErrJudge...");
			Reader_SetComErr();
		} 
		else if(len>0 && len<0x80)
		{
			len &= 0x0F;
			Trace_R("\r\nbuf");
			Mdb_ReadBuf(buf,len);
			Trace_R("\r\nA = %02x - ",len);				
			for(i = 0; i < len; i++)
				Trace_R(" %02x",buf[i]);
	
			Reader_UpdatePollMsg(buf, len);
		}	
	}
	while(len >= 0x80);
}

*/


void Reader_PrintStat()
{
	switch(ReaderPara.cReaderSta)
	{
		case READER_STA_INIT:
			Trace_R("\r\nREADER_STA_INIT");
		break;
		case READER_STA_INACTIVE:
			Trace_R("\r\nREADER_STA_INACTIVE");
		break;
		case READER_STA_DISABLED:
			Trace_R("\r\nREADER_STA_DISABLED");
		break;
		case READER_STA_ENABLED:
			Trace_R("\r\nREADER_STA_ENABLED");
		break;
		case READER_STA_SESSION_IDLE:
			Trace_R("\r\nREADER_STA_SESSION_IDLE");
		break;
		case READER_STA_VEND:
			Trace_R("\r\nREADER_STA_VEND");
		break;
		case READER_STA_REVALUE:
			Trace_R("\r\nREADER_STA_REVALUE");
		break;
		case READER_STA_NAGATIVE_VEND:
			Trace_R("\r\nREADER_STA_NAGATIVE_VEND");
		break;		
		default:
			Trace_R("\r\nUNKNOW_STA");
		break;	
	}
}


uint8_t Reader_GetKey()
{
	uint8_t len;
	uint8_t i;	
	uint8_t buf[36];
	uint8_t key = 0;

	//key = Uart0GetCh();	

	//len = Uart0_GetStr(buf,36);	
	len = 0;
	
	if(len != 0)
		key = buf[0];

	if(key != 0)
		Trace_R("     key = %02x", key);

	if(0)
	//if(len != 0)
	{
		Trace_R("     len = %u, str =", len);
		for(i = 0; i < len; i++)
			Trace_R(" %02x",buf[i]);
	}

	switch(key)
	{
		case VTR_VEND_REQUEST:
			Trace_R("     VTR_VEND_REQUEST");
		break;
		
		case VTR_VEND_SUCCESS:
			Trace_R("     VTR_VEND_SUCCESS");
		break;
		
		case VTR_VEND_FAILURE:
			Trace_R("     VTR_VEND_FAILURE");
		break;
		
		case VTR_SESSION_COMPLETE:
			Trace_R("     VTR_SESSION_COMPLETE");
		break;
		
		case VTR_READER_DISABLE:
			Trace_R("     VTR_READER_DISABLE");
		break;
		
		case VTR_READER_ENABLE:
			Trace_R("     VTR_READER_ENABLE");
		break;
		
		case VTR_READER_CANCEL:
			Trace_R("     VTR_READER_CANCEL");
		break;		
		
		default:
		break;
	}

	return key;
}


void Reader_MainProc(uint8_t cMsg)
{
	uint8_t len;
	uint8_t i;	
	//uint8_t buf[36];
	//uint8_t key;
	
	Reader_PrintStat();
	
	switch(ReaderPara.cReaderSta)
	{
		case READER_STA_INIT://1处于初始状态
			
			for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
				len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
			{
				len = Reader_Reset();//复位操作
			}
			//while(len >= 0x80); 			
			ReaderPara.cReaderSta = READER_STA_INACTIVE;//复位成功，处于未激活状态
			
		break;
		
		case READER_STA_INACTIVE://2、处于未激活状态
			
			Reader_Proc(Reader_SetupConfigData);//进行配置setup操作
			Reader_Proc(Reader_Poll);//进行poll
			
			for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
				len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
			{		
				len = Reader_SetupMaxMinPrice();
			}
			//while(len >= 0x80);
			ReaderPara.cReaderSta = READER_STA_DISABLED;
			
		break;
		
		case READER_STA_DISABLED://4处于禁能状态
	
			//if vmc enable reader				
			if(cMsg == VTR_READER_ENABLE || ReaderPara.cReaderEnabel == 1)
			{
				ReaderPara.cReaderEnabel = 1;
				for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
					len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
				{
					len = Reader_ReaderEnable();
				}
				//while(len >= 0x80); 			
				ReaderPara.cReaderSta = READER_STA_ENABLED;
			}
			
		break;
		
		case READER_STA_ENABLED://5处于使能状态
	
			//vmc control
			if(cMsg == VTR_READER_DISABLE)//如果收到vmc叫他禁能的指令
			{
				ReaderPara.cReaderEnabel = 0;
				for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
					len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
				{
					len = Reader_ReaderDisable();
				}
				//while(len >= 0x80); 			
				ReaderPara.cReaderSta = READER_STA_DISABLED;
			}
	
			//vmc control				
			if(cMsg == VTR_READER_CANCEL)
			{
				for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
					len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
				{
					len = Reader_ReaderCancel();
				}
				//while(len >= 0x80); 			
				ReaderPara.cReaderSta = READER_STA_INIT;
			}
			
			//Reader_PollProc();
			
		break;
		
		case READER_STA_SESSION_IDLE://7进行session交易流程
	
			//vmc control
			//if(key == KEY_SESSION_COMPLETE || ReaderPara.cSessionCancel == 1)
			if(ReaderPara.cSessionCancel == 1)//情况一，拔卡，取消交易session
			{
				ReaderPara.cSessionCancel = 0;
				//Reader_SessionCompleteProc();	
				Reader_Proc(Reader_SessionComplete);//10本交易session结束
			}
			
			//Reader_PollProc();
	
			//vmc control		
			//121024 by cq Cashless
			if(ReaderPara.cReaderSta == READER_STA_SESSION_IDLE)//情况二，检测到需要购买请求
			{
				if(cMsg == VTR_VEND_REQUEST)
				{
					for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
						len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
					{
						//len = Reader_VendRequest(0, 0x01);
						len = Reader_VendRequest(Reader_GetValueDivideUnit(), 0x01);
					}
					//while(len >= 0x80); 			
					ReaderPara.cReaderSta = READER_STA_VEND;
				}				
			}
			
		break;
	
		case READER_STA_VEND://8开始进行出货操作
	
			//Reader_PollProc();
			
			//121024 by cq Cashless
			if(ReaderPara.cSessionCancel == 1)//中途拔卡
			{
				ReaderPara.cSessionCancel = 0;
				//Reader_SessionCompleteProc();	
				Reader_Proc(Reader_SessionComplete);
			}
	
			//vmc control
			if(cMsg == VTR_VEND_SUCCESS)//9情况一出货成功
			{
				for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
					len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
				{
					len = Reader_VendSuccess(0x01);//告诉读卡器出货成功，读卡器扣卡里面的钱
				}
				//while(len >= 0x80); 			
				ReaderPara.cReaderSta = READER_STA_SESSION_IDLE;//回到交易session流程7
			}				
	
			
			//vmc control
			if(cMsg == VTR_VEND_FAILURE)//9情况二出货失败
			{
				for(len = 0x80, ReaderPara.iTimeDly = READER_TIMEDLY; 
					len >= 0x80 && ReaderPara.iTimeDly != 0 ; )
				{
					len = Reader_VendFailure();//告诉读卡器出货失败，读卡器不扣卡里面的钱
				}
				//while(len >= 0x80); 			
				ReaderPara.cReaderSta = READER_STA_SESSION_IDLE;//回到交易session流程7
			}				
			
		break;
		
		case READER_STA_REVALUE:
		break;
		
		case READER_STA_NAGATIVE_VEND:
		break;
		
		default:
		break;
	}
	
	
	
	switch(ReaderPara.cReaderSta)
	{
		case READER_STA_ENABLED:
		case READER_STA_SESSION_IDLE:
		case READER_STA_VEND:
			//Reader_PollProc();			
			Reader_Proc(Reader_Poll);
		break;
		
		default:
		break;
	
	}
}


void Reader_InitPara()
{
	Trace_R("\r\nReader_InitPara");
	ReaderPara.cComErr = 0;
	ReaderPara.cReaderSta = READER_STA_INIT;
	ReaderPara.cReaderEnabel = 1;
	ReaderPara.cSessionCancel = 0;
	ReaderPara.iValueUnit = 0;
	ReaderPara.iItemValue = 0;
	ReaderPara.iFundsAvailable = 0;
}

void Reader_EndSessionHandle()
{
	ReaderPara.iItemValue = 0;
	ReaderPara.iFundsAvailable = 0;
}

void ReaderTest(void)
{

	uint8_t len;
	uint8_t i;	
	//uint8_t buf[36];
	uint8_t key;

	
	Trace_R("\r\nReaderTest...");	
	
	OSTimeDly(OS_TICKS_PER_SEC * 3);
	Reader_InitPara();

	do
	{
		//len = len - 1;
		len = 0;
	}
	while(len >= 0x80);

	
	key = Reader_GetKey();

	while(1)
	{	
		Reader_MainProc(key);		
	}
}

