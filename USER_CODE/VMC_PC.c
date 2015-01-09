#define  VMC_PC_C
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\config.h"
#include "global.h"
#include "itl.h"
#include "DebugTrace.h"
#include "Uart.h"
#include "string.h"
#include "VMC_PC.h"
#include "keypad.h"
#include "lcd.h"
#include "menu.h"
#include "Drv.h"
#include "lcd.h"
#include "RealTime.h"
#include "pcf8563t.h"



//------------------------------------------ 
extern uint16_t CurrentPayed=0;
extern uint16_t HardWareErr=0;
extern uint16_t CurrentDispenseCoin;
extern uint8_t SystemStatus;
extern struct HopperSet HopperSetList[HOPPERNUM];  //0存放最大面值,1存放第二大面值,依此类推
extern unsigned char OutGoods( unsigned char type );
extern uint8_t DisplayCurrentMoney( uint16_t Money );
//==========================================


unsigned char VPMsgBuf[255];

char *     Menu_ChannelOpt[2][19]=
{
	 {
	 "    你确定更改吗?",
	 "确认ENTER 返回CANCEL",
	 "\xd5\xfd在更改商品价格...", 
	 "商品价格更改失败!", 
	 "商品价格更改成功!",
	 "\xd5\xfd在更改商品余量...",
	 "商品余量更改失败!",
	 "商品余量更改成功!",
	 "    你确定添满吗?",
	 "\xd5\xfd在添满...",
	 "添满操作失败!",
	 "添满操作成功!",
	 "你确定添满此层架吗?",
	 "\xd5\xfd在添满当前层架...",
	 "层架添满操作失败!",
	 "层架添满操作成功!",
	 "    你确定同步吗?",
	 "\xd5\xfd在同步, 请稍等...",
	 "同步操作成功!",
	 },
	 {
	 "   Are you sure ?",
	 "   ENTER   CANCEL",
	  "Update Price...",
	 "Operate fail!",
	 "Operate successfully",
	 "Update sum...",
	 "Operate fail!",
	 "Operate successfully",
	 "   Are you sure ?",
	 "Update sum...",
	 "Operate fail!",
	 "Operate successfully",
	 "   Are you sure ?",
	 "Adding the tray...",
	 "Add the tray fail!",
	 "Operate successfully",
	 "   Are you sure ?",
	 "Please waiting...",
	 "Operate successfully",
	 }
};


struct VP_Mission sysVPMission;
struct GoodsMatrix sysGoodsMatrix[GOODSTYPEMAX];

void VPSerialInit( void )
{
	/*
    unsigned char data i;
	for( i=0; i<sizeof( SERAILPARAM ); i++ )
		ZhkSerialParam[i] = VPSerialParam[i];
	ZhkSerialInit();
    ES = 1;  //2009.03.02 added
	*/
	memset( VPMsgBuf, 0, sizeof(VPMsgBuf) );
}

//CHK and send the message
unsigned char VPBusTxMsg( void )
{
	unsigned char  i   = 0;
	//unsigned char  data len = 0;
	//unsigned char  data sum = 0;
	//unsigned char  DataString[255];
	
	//1.caculate the CHK
	//sysVPMission.send.datLen = sysVPMission.send.len - 5;
    sysVPMission.send.chk = 0;
	sysVPMission.send.chk += sysVPMission.send.sf;
	sysVPMission.send.chk += sysVPMission.send.len;
	sysVPMission.send.chk += sysVPMission.send.verFlag;
	sysVPMission.send.chk += sysVPMission.send.msgType;
	sysVPMission.send.chk += sysVPMission.send.sn;
    for( i=0; i<sysVPMission.send.datLen; i++  )
	{
		sysVPMission.send.chk += sysVPMission.send.msg[i];	
	}
	//2.send the message
	Uart3_SendByte( sysVPMission.send.sf );
	Uart3_SendByte( sysVPMission.send.len );
	Uart3_SendByte( sysVPMission.send.verFlag );
	Uart3_SendByte( sysVPMission.send.msgType );
	Uart3_SendByte( sysVPMission.send.sn );
    for( i=0; i<sysVPMission.send.datLen; i++  )
	{
		Uart3_SendByte( sysVPMission.send.msg[i] );
	}
	Uart3_SendByte( sysVPMission.send.chk/256 );
    Uart3_SendByte( sysVPMission.send.chk%256 );


	Trace( "\r\n S= XX");
	Trace( " %02x",sysVPMission.send.sf );
	Trace( " %02x",sysVPMission.send.len );
	Trace( " %02x",sysVPMission.send.verFlag );
	Trace( " %02x",sysVPMission.send.msgType );
	Trace( " %02x",sysVPMission.send.sn );
    for( i=0; i<sysVPMission.send.datLen; i++  )
	{
		Trace( " %02x",sysVPMission.send.msg[i] );
	}
	Trace( " %02x",sysVPMission.send.chk/256 );
    Trace( " %02x",sysVPMission.send.chk%256 );
/**/	

	return 1;
}


unsigned char VPBusFrameUnPack( void )
{
	unsigned char i=0, j=0, k=0, m=0;
	unsigned char len = 0;
	unsigned int  sum = 0;
	uint8_t	 length=0;
	unsigned char VPMsgBuf[255];

	
	
	memset(VPMsgBuf,0,sizeof(VPMsgBuf));
	
	/*
	while( !ZhkSerialIsRxBufNull() )
	{
		for( i=0; i<sizeof( VPMsgBuf )-1; i++ )
		{
			VPMsgBuf[i] = VPMsgBuf[i+1];
		}
		VPMsgBuf[sizeof(VPMsgBuf)-1] = ZhkSerialGetCh();
//#ifdef _DEBUG_TRACE
//		j ++ ;
//#endif
	}
	*/
	
	Trace( "\r\n L=");
	length = Uart3_Read(VPMsgBuf,100);
	Trace(" %02x",length);
	//Trace("----len=%d,str:%x,%x,%x",length,VPMsgBuf[0],VPMsgBuf[1],VPMsgBuf[2]);
	for(i = 0; i < length; i++)
		Trace(" %02x",VPMsgBuf[i]);


    if(length > 0)
	{ 
	    //Trace("----get str"); 
		for( i=0; i<=sizeof(VPMsgBuf)-7; i++ )
		{
		    //Trace("----ready confirm"); 
			//Trace("----str:%x,%x,%x,%x,%x,%x,%x",VPMsgBuf[0],VPMsgBuf[1],VPMsgBuf[2],VPMsgBuf[3],VPMsgBuf[4],VPMsgBuf[5],VPMsgBuf[6]);
		    //Check the SF
			if( VPMsgBuf[i] != VP_SF ) 
				continue;

			//Trace("==1,");
			//Check the len
			len = VPMsgBuf[i+1];
			if( !(len >=5) ) 
			    continue;
			
			//Trace("==2,");
			if( i+len+2 > sizeof( VPMsgBuf ) ) 
			    break;	 

			//Trace("==3,");
			//Check the CHK
			sum = 0;
			for( j=i; j<i+len+2-2; j++ )		
			{
				sum += VPMsgBuf[j];		
			}
			if( (VPMsgBuf[j] != sum/256)||(VPMsgBuf[j+1] != sum%256)  )
			{
				continue;
		    }

			
			//Trace("==4,");
			//Check the message type
	        if( !((VPMsgBuf[i+3]>=VP_MT_MIN_RECEIVE) && (VPMsgBuf[i+3]<=VP_MT_MAX_RECEIVE)) )   
	            continue;     

			
			//Trace("==5,");
			//Save the message
			sysVPMission.receive.sf      = VPMsgBuf[i];
			sysVPMission.receive.len     = VPMsgBuf[i+1];
	        sysVPMission.receive.verFlag = VPMsgBuf[i+2];
	 	    sysVPMission.receive.msgType = VPMsgBuf[i+3];
			sysVPMission.receive.sn      = VPMsgBuf[i+4];
	        sysVPMission.receive.datLen  = sysVPMission.receive.len - 5;
			for( m=0,k=i+5; k<i+5+sysVPMission.receive.datLen; m++,k++)
			{
				sysVPMission.receive.msg[m] = VPMsgBuf[k];	
			}
			sysVPMission.receive.chk = sysVPMission.receive.msg[k]*256 + sysVPMission.receive.msg[k+1];
			memset( VPMsgBuf, 0, sizeof(VPMsgBuf) );

			Trace( " VPBusFrameUnPack");

			
	        //Trace("----str over"); 
			return 1;
		}
	}	
	return 0;
}

unsigned char VPMsgPackSend( unsigned char msgType, unsigned char flag, unsigned char snCtr )
{
    
    unsigned char i=0;
    unsigned int CurrentMoney=0;

	//if( !((msgType>=VP_TEXT_MSG)&&(msgType<=VP_MT_MAX_SEND)) )
	if(msgType>VP_MT_MAX_SEND)
		return VP_ERR_PAR;
    if((flag!=0)&&(flag!=1)) 
	    return VP_ERR_PAR;

	switch( msgType )
	{
		case VP_TEXT_MSG:
			{
				Trace("\r\nVP_TEXT_MSG");
			}
			break;
		case VP_ACK_RPT:
            {
				Trace("\r\nVP_ACK_RPT");
				sysVPMission.send.msgType = VP_ACK_RPT;
                sysVPMission.send.datLen = 0;
			}
			break;
		case VP_NAK_RPT:
		    {
				Trace("\r\nVP_NAK_RPT");
				sysVPMission.send.msgType = VP_NAK_RPT;
                sysVPMission.send.datLen = 0;
			}
			break;
		case VP_POLL:
		    {
				Trace("\r\nVP_POLL");
			    sysVPMission.send.msgType = VP_POLL;
                sysVPMission.send.datLen = 0;
			}
			break;
		case VP_STARTUP_RPT:
		    {
				Trace("\r\nVP_STARTUP_RPT");
				sysVPMission.send.msgType = VP_STARTUP_RPT;
                sysVPMission.send.datLen = 0;
			}
			break;
		case VP_VMC_SETUP:
		    {
				Trace("\r\nVP_VMC_SETUP");
			    sysVPMission.send.msgType = VP_VMC_SETUP;
                sysVPMission.send.datLen = 5;       //3,5
                sysVPMission.send.msg[0] = sysVPMission.proVer;
				sysVPMission.send.msg[1] = sysVPMission.columnNumSet;
				sysVPMission.send.msg[2] = sysVPMission.selectionNumSet;
                //Added the machine VER
                sysVPMission.send.msg[3] = VP_MAC_SET_H;
                sysVPMission.send.msg[4] = VP_MAC_SET_L;
                
			}
			break;
		case VP_PAYIN_RPT:
		    {
				Trace("\r\nVP_PAYIN_RPT");
			    sysVPMission.send.msgType = VP_PAYIN_RPT;
                sysVPMission.send.datLen = 5;      //3,5
				sysVPMission.send.msg[0] = sysVPMission.payInDev;
				sysVPMission.send.msg[1] = sysVPMission.payInMoney/256;
				sysVPMission.send.msg[2] = sysVPMission.payInMoney%256;
				//PAYIN_RPT协议中末尾添加两个字节
                //Total_Value：表示当前交易投币后，或者出货后，屏幕上显示的金额数 
                if(SYSPara.Decimal==1)
				{
				  	CurrentMoney = g_Amount * 10;
				}
				else if(SYSPara.Decimal==2)
				{
				  	CurrentMoney = g_Amount; 
				}		
				
                if(sysVPMission.payInDev == VP_DEV_ESCROWOUT)
                {
				  	CurrentMoney = (CurrentMoney - sysVPMission.payInMoney); 
				}
				else if( (sysVPMission.billHoldingFlag == 1)&&(sysVPMission.payInDev == VP_DEV_BILL) )
				{
				  	CurrentMoney = CurrentMoney; 
				}	
				else	
				{
				  	CurrentMoney = (CurrentMoney + sysVPMission.payInMoney); 
				}
					
                sysVPMission.send.msg[3] = CurrentMoney/256;
                sysVPMission.send.msg[4] = CurrentMoney%256;
			}
			break;
		case VP_PAYOUT_RPT:
		    {
				Trace("\r\nVP_PAYOUT_RPT");
			    sysVPMission.send.msgType = VP_PAYOUT_RPT;
				sysVPMission.send.datLen = 8;   
                sysVPMission.send.msg[0] = sysVPMission.payOutDev;
				sysVPMission.send.msg[1] = sysVPMission.payOutMoney1/256;
				sysVPMission.send.msg[2] = sysVPMission.payOutMoney1%256; 
                sysVPMission.send.msg[3] = sysVPMission.payOutMoney2/256;
				sysVPMission.send.msg[4] = sysVPMission.payOutMoney2%256;
				//PAYIN_RPT协议中末尾添加两个字节
                //Total_Value：表示当前交易投币后，或者出货后，屏幕上显示的金额数 	
                if(SYSPara.Decimal==1)
				{
				  	CurrentMoney = g_Amount * 10;
				}
				else if(SYSPara.Decimal==2)
				{
				  	CurrentMoney = g_Amount; 
				}	
                sysVPMission.send.msg[5] = CurrentMoney/256;
                sysVPMission.send.msg[6] = CurrentMoney%256;
				sysVPMission.send.msg[7] = sysVPMission.changeCmd;
				sysVPMission.changeCmd = 0;
			}
			break;
        case VP_COST_RPT://扣款操作;by gzz 20110823
		    {
				Trace("\r\nVP_COST_RPT");
			    sysVPMission.send.msgType = VP_COST_RPT;
				sysVPMission.send.datLen = 6;   
                sysVPMission.send.msg[0] = sysVPMission.costDev;
				sysVPMission.send.msg[1] = sysVPMission.costMoney/256;
				sysVPMission.send.msg[2] = sysVPMission.costMoney%256;                 
				//PAYIN_RPT协议中末尾添加两个字节
                //Total_Value：表示当前交易投币后，或者出货后，屏幕上显示的金额数 				
                sysVPMission.send.msg[3] = g_Amount/256;
                sysVPMission.send.msg[4] = g_Amount%256;
				sysVPMission.send.msg[5] = sysVPMission.costCmd;
				sysVPMission.costCmd = 0;
			}
			break;
		case VP_DEBT_RPT://上报欠条操作;by gzz 20110825
		    {
				Trace("\r\nVP_DEBT_RPT");
			    sysVPMission.send.msgType = VP_DEBT_RPT;
				sysVPMission.send.datLen = 4;   
                sysVPMission.send.msg[0] = sysVPMission.debtType;
				sysVPMission.send.msg[1] = sysVPMission.debtDev;
				sysVPMission.send.msg[2] = sysVPMission.debtMoney/256;
				sysVPMission.send.msg[3] = sysVPMission.debtMoney%256;   
			}
			break;			
		case VP_VENDOUT_RPT:
		    {
				Trace("\r\nVP_VENDOUT_RPT");
				sysVPMission.send.msgType = VP_VENDOUT_RPT;
                sysVPMission.send.datLen = 5;
				sysVPMission.send.msg[0] = sysVPMission.vendSta;
				sysVPMission.send.msg[1] = sysVPMission.vendColumn;
				sysVPMission.send.msg[2] = sysVPMission.vendType;
				sysVPMission.send.msg[3] = sysVPMission.vendCost/256;
				sysVPMission.send.msg[4] = sysVPMission.vendCost%256;
			}
			break;
		case VP_REQUEST: 
            {
				Trace("\r\nVP_REQUEST");
				sysVPMission.send.msgType = VP_REQUEST;
				sysVPMission.send.datLen = 1;
				sysVPMission.send.msg[0] = sysVPMission.request;
			}
			break;
		case VP_ADMIN_RPT:
		    {
				Trace("\r\nVP_ADMIN_RPT");
			    sysVPMission.send.msgType =  VP_ADMIN_RPT;

				if( sysVPMission.ADM_Type == VP_ADMIN_COL_PRICE )
				{
				    sysVPMission.send.datLen = 4;
					sysVPMission.send.msg[0] = sysVPMission.ADM_Type;
					sysVPMission.send.msg[1] = sysVPMission.ADM_Dat[0];
					sysVPMission.send.msg[2] = sysVPMission.ADM_Dat[1];	
                    sysVPMission.send.msg[3] = sysVPMission.ADM_Dat[2];	
				}
				else if( sysVPMission.ADM_Type == VP_ADMIN_GOODSADDCOL )
				{
					sysVPMission.send.datLen = 3;
					sysVPMission.send.msg[0] = sysVPMission.ADM_Type;
					sysVPMission.send.msg[1] = sysVPMission.ADM_Dat[0];
					sysVPMission.send.msg[2] = sysVPMission.ADM_Dat[1];
				}
                else if( sysVPMission.ADM_Type == VP_ADMIN_GOODSADDSLAVE )
				{
					sysVPMission.send.datLen = 3;
					sysVPMission.send.msg[0] = sysVPMission.ADM_Type;
					sysVPMission.send.msg[1] = sysVPMission.ADM_Dat[0];
					sysVPMission.send.msg[2] = sysVPMission.ADM_Dat[1];
				}
				else if( sysVPMission.ADM_Type == VP_ADMIN_GOODSADDTRAY ) //Add the tray's goods
				{
				    sysVPMission.send.datLen = 2;
					sysVPMission.send.msg[0] = sysVPMission.ADM_Type;
					sysVPMission.send.msg[1] = sysVPMission.ADM_Dat[0];
				}
				else  //Add all columns' goods, chage, get bill
				{
				    sysVPMission.send.datLen = 1;
					sysVPMission.send.msg[0] = sysVPMission.ADM_Type;
				}
				
			}
			break;
        case VP_ACTION_RPT:
		    {
				Trace("\r\nVP_ACTION_RPT");
				sysVPMission.send.msgType = VP_ACTION_RPT;
				if( sysVPMission.ACT_DatLen >= sizeof(sysVPMission.ACT_Dat) )
				    sysVPMission.ACT_DatLen = sizeof(sysVPMission.ACT_Dat);
				sysVPMission.send.datLen  = sysVPMission.ACT_DatLen + 4;
				sysVPMission.send.msg[0] = sysVPMission.ACT_Type;
				sysVPMission.send.msg[1] = sysVPMission.ACT_Money/256;
				sysVPMission.send.msg[2] = sysVPMission.ACT_Money%256;
				sysVPMission.send.msg[3] = sysVPMission.ACT_DatLen;
                for( i=0; i<sysVPMission.ACT_DatLen; i++ )
				{
					sysVPMission.send.msg[i+4] = sysVPMission.ACT_Dat[i];
				}
			}
			break;
        case VP_BUTTON_RPT:
		    {
				Trace("\r\nVP_BUTTON_RPT");
				sysVPMission.send.msgType = VP_BUTTON_RPT;
				sysVPMission.send.datLen = 2;
				sysVPMission.send.msg[0] = sysVPMission.BUT_Type;
				sysVPMission.send.msg[1] = sysVPMission.BUT_Value;
			}
			break;
		case VP_STATUS_RPT:
		    {
				Trace("\r\nVP_STATUS_RPT");
			    sysVPMission.send.msgType = VP_STATUS_RPT;
                //sysVPMission.send.datLen  = 15;
				sysVPMission.send.datLen  = 17;
				sysVPMission.send.msg[0]  = sysVPMission.STA_CoinA;
				sysVPMission.send.msg[1]  = sysVPMission.STA_ChangeNum1/256;
				sysVPMission.send.msg[2]  = sysVPMission.STA_ChangeNum1%256;
				sysVPMission.send.msg[3]  = sysVPMission.STA_ChangeNum2/256;
				sysVPMission.send.msg[4]  = sysVPMission.STA_ChangeNum2%256;
				sysVPMission.send.msg[5]  = sysVPMission.STA_BillA;
				sysVPMission.send.msg[6]  = sysVPMission.STA_Oz;
				sysVPMission.send.msg[7]  = sysVPMission.STA_VMC;
				sysVPMission.send.msg[8]  = sysVPMission.STA_Ht;
				sysVPMission.send.msg[9]  = sysVPMission.STA_Rrsw;
				sysVPMission.send.msg[10] = sysVPMission.STA_Outdev;
				sysVPMission.send.msg[11] = sysVPMission.STA_Tep1;
				sysVPMission.send.msg[12] = sysVPMission.STA_Tep2;
				sysVPMission.send.msg[13] = sysVPMission.STA_ICCard;
				sysVPMission.send.msg[14] = sysVPMission.STA_Bank;
				//
                sysVPMission.send.msg[15] = sysVPMission.E_Tem1;
				sysVPMission.send.msg[16] = sysVPMission.E_Tem2;
			}
			break;
        case VP_COLUMNSTA_RPT:
            {
				Trace("\r\nVP_COLUMNSTA_RPT");
                sysVPMission.send.msgType = VP_COLUMNSTA_RPT;
                sysVPMission.send.datLen  = COLUMN_NUM_SET;    
                //
            }
            break;
        case VP_CARD_RPT:
			Trace("\r\nVP_CARD_RPT");
		    /*
            {
                sysVPMission.send.msgType = VP_CARD_RPT;  
                sysVPMission.send.datLen  = sysBCMission.msgLen + 1;
                sysVPMission.send.msg[0]  = 1;       
                for( i=0; i<sysBCMission.msgLen; i++ )
                {
                	sysVPMission.send.msg[i+1] = sysBCMission.msgBuf[i];    
                }
            }
			*/
            break;
		case VP_VENDOUT_REQ:	
			{
				Trace("\r\nVP_VENDOUT_REQ");
                sysVPMission.send.msgType = VP_VENDOUT_REQ;
                sysVPMission.send.datLen  = 3;    
				sysVPMission.send.msg[0]  = sysVPMission.vendReqColumn;
				sysVPMission.send.msg[1]  = sysVPMission.vendReqCost/256;
				sysVPMission.send.msg[2]  = sysVPMission.vendReqCost%256;
                //
            }
			break;
		default: 
			break;
	}
	//
	sysVPMission.send.sf  = VP_SF;
	//
    sysVPMission.send.len = sysVPMission.send.datLen + 5;
	//
    sysVPMission.send.verFlag = VP_PROTOCOL_VER;
	if( flag == 1 )
	{
	    sysVPMission.send.verFlag += VP_PROTOCOL_ACK;
	}
	else
	{
		sysVPMission.send.verFlag += VP_PROTOCOL_NAK;
	}
	//
	if( snCtr == 1 )
	{
	    if( sysVPMission.sysStatus != 0 )
		{
			sysVPMission.send.sn += 1;
    	}
		else
		{
			sysVPMission.sysStatus |= VPM_STA_FIRSTCOM;
		}
	}
    sysVPMission.comLock = 1;
    VPBusTxMsg();
    sysVPMission.comLock = 0;
	return VP_ERR_NULL;
}

//----------------------------------------------------------------------
//PC--VMC command mission
unsigned char VP_CMD_GoodsPar( void )
{
    unsigned char i = 0;
    unsigned char j = 0;
	//unsigned char GoodsRev;
	unsigned char GoodsRev = GOODSTYPEMAX;	//120827 by cq PCcommunication
	unsigned int  GoodsPrice;
	GoodsRev = sysVPMission.receive.datLen / 3;
    //if( sysVPMission.receive.datLen == (GOODSTYPEMAX+GOODSTYPEMAX*2)  )
	//{
        
	    if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
        {
	    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
        }
		//----------------------------------
		//  1....17,   19...               54
		//SP1....SP18, SP1_Price...SP18_Price

		//01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12
        for( i=0; i<GOODSTYPEMAX; i++)			
		{
			sysGoodsMatrix[i].GoodsType = sysVPMission.receive.msg[i];
            //sysVPMission.selItem[i]     = sysVPMission.receive.msg[i];
            //Trace("\r\ngoodstype=%d",sysGoodsMatrix[i].GoodsType);
		}

		//00 64   00 c8   01 2c   01 90 
		//00 64   00 64   1e 14   22 60 
		//03 de   07 4e   0b 36   0f 1e 
		//13 06   16 ee   1b 4e   03 de 
		//23 1e   27 06              0a bc 
		for( i=0, j=GOODSTYPEMAX; i<GOODSTYPEMAX; i++,j+=2 )
		{
			if(SYSPara.Decimal==1)
			{
			  	GoodsPrice = (sysVPMission.receive.msg[j]*256+sysVPMission.receive.msg[j+1]) / 10;
			}
			else if(SYSPara.Decimal==2)
			{
			  	GoodsPrice = sysVPMission.receive.msg[j]*256+sysVPMission.receive.msg[j+1]; 
			}
			sysGoodsMatrix[i].Price  = GoodsPrice; 
            //sysVPMission.selPrice[i] = sysVPMission.receive.msg[j]*256+sysVPMission.receive.msg[j+1];  
            //Channels[NowLayerNo][NowCHNO].GoodsPrice	= sysVPMission.receive.msg[j]*256+sysVPMission.receive.msg[j+1]; 
            //Trace("\r\ngoodsprice=%d",sysGoodsMatrix[i].Price);
		}

		sysVPMission.sysStatus |= VPM_STA_GOODSPAR;
	//}
	//else
	//{
	//    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 //1, 0
	//	return VP_ERR_PAR;
	//}
    return VP_ERR_NULL;	
}

unsigned char VP_CMD_ColumnPar( void )
{
    unsigned char i = 0;
	unsigned char j;
	unsigned char x = 0;
	unsigned char y = 0;
	uint8_t   channel,ChannelX,ChannelY;
	
	
	j = sysVPMission.receive.datLen / 2;
	//if( sysVPMission.receive.datLen == COLUMN_NUM_SET*2 )
	//{
        if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
        {
	    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
        }
		//-------------------------------------------------
		//1...       48  49...        96
		//colNum_1...48  colGoods_1...48
		//for( i=0, j=COLUMN_NUM_SET; i<COLUMN_NUM_SET; i++, j++ )
		//{
			//GoodsWaySetVal[i].GoodsCurrentSum = sysVPMission.receive.msg[i];
			//GoodsWaySetVal[i].GoodsType = sysVPMission.receive.msg[j];
			//Channels[NowLayerNo][NowCHNO].GoodsNum	= sysVPMission.receive.msg[i];			
		//}
		/*
		14 14 14 14 14 14 14 14 \\ 1~48 num
		14 14 14 14 14 14 14 14 
		14 14 14 14 14 14 14 14 
		14 14 14 14 14 14 14 14 
		14 14 14 14 14 14 14 14 
		00 00 00 00 00 00 00 00 
		
		01 02 03 02 02 01 03 01 \\ 1~48 type
		09 0a 01 02 03 04 05 06 
		07 08 09 0a 01 02 03 04 
		05 06 07 08 09 0a 01 02 
		03 04 05 06 07 08 09 0a 
		00 00 00 00 00 00 00 00 
		*/

		i = 0;
		j = 48;		//120620 by cq PCcommunication
		
		for(x = 0; x<MAXLAYER_6;x++)	  
		{			
			for(y=0; y<MAXCHANNEL;y++) 
			{
				channel = x*10+y+11;
				if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
				{
					Channels[ChannelX][ChannelY].GoodsNum = sysVPMission.receive.msg[i++];//货道存量
					Channels[ChannelX][ChannelY].GoodsCode = sysVPMission.receive.msg[j++];//商品id
				}			    
			}
		}
		sysVPMission.sysStatus |= VPM_STA_COLUMNPAR;
	//}
	//else
	//{
	//	VPMsgPackSend( VP_NAK_RPT, 0, 0  );  //1,0
	//	return VP_ERR_PAR;
	//}
    return VP_ERR_NULL;	
}

unsigned char VP_CMD_Init_OK( void )
{
    volatile unsigned char i = 0;
	volatile unsigned char j = 0;
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
	{
    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
    }
    sysVPMission.sysStatus |= VPM_STA_INITOK;
	return VP_ERR_NULL;	
}


//120620 by cq PCcommunication
unsigned char VP_CMD_Vendout( void )
{
	unsigned char i = 0;
	volatile unsigned char j = 0;
	//unsigned int	sel = 0xffff;
	//unsigned char col = 0xff;
	unsigned int  moneyBuf = 0;
	//unsigned char dspUpdate = 0;
	unsigned char x = 0;
	unsigned char y = 0;

	unsigned char  channel,ChannelX,ChannelY;
	unsigned char key = 0xff;
	unsigned char breakFlag = 0;

	//static unsigned char loopTemp = 0;

	Trace("\r\n1.VP_CMD_Vendout");

	// L= 0b e7 09 82 83 5f 	
	//01 
	//03 
	//00 64 	
	//02 bc VPBusFrameUnPack

	// L= 0b e7 09 82 83 13 
	//01 
	//07 
	//00 64 
	//02 74 VPBusFrameUnPack
	
	
	//3.Check the column's status
	
	//4.ACK
	/**/
	sysVPMission.vendGoods = sysVPMission.receive.msg[0];
	sysVPMission.vendType  = 0;
	sysVPMission.vendCost  = 0;
	//sysVPMission.vendColumn = col;	
	//key = sysVPMission.vendGoods;

		
	//for(x = 0; x<MAXLAYER_6;x++)	  
	//	for(y=0; y<MAXCHANNEL;y++)			
	//		Trace(" [%x][%x] = %d,",x,y,Channels[x][y].GoodsCode);

	//Trace("\r\n Channels[x][y].GoodsCode");
	Trace("\r\n");

	/*
	for(x = 0; x<MAXLAYER_6;x++)	  
	{					
		for(y=0; y<MAXCHANNEL;y++) 
		{  
			channel = x*10+y+11;
			if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
			{	
				Trace(" [%x][%x] = %d,",ChannelX,ChannelY,Channels[ChannelX][ChannelY].GoodsCode);
				//if(Channels[ChannelX][ChannelY].GoodsCode == key)
				if( (Channels[ChannelX][ChannelY].GoodsCode == key)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
				{
					breakFlag = 1;						
					break;
				}				
			} 
		}	

		if(breakFlag)
			break;
	}	
	*/

		
	sysVPMission.vendColumn2 = sysVPMission.receive.msg[0];
	Trace(" channel=%02x,",sysVPMission.vendColumn2);


	/*
	//---------------------------------------------------
	//2011.7.19 changed for 
	//moneyBuf = CurrentPayed;	  
	if(SYSPara.Decimal==1)
	{
		moneyBuf = g_Amount * 10;
	}
	else if(SYSPara.Decimal==2)
	{
		moneyBuf = g_Amount; 
	}	

	
	Trace(" moneyBuf=%d,",moneyBuf);

	if( sysVPMission.vendCost > moneyBuf )
	{
		VPMsgPackSend( VP_NAK_RPT, 0, 0  );  
		
		Trace(" VP_ERR_PAR,");
		return VP_ERR_PAR;			
	}
	*/
	
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
	{
	
		Trace(" VP_ACK_RPT,");
		VPMsgPackSend( VP_ACK_RPT, 0, 0 );
	}
	
	//5.Vendout    
	sysVPMission.vendCmd = 1;	
	OSMboxAccept(g_PCCMDMsg);
	MsgBack.CmdPCType = VP_VENDOUT_IND;

	OSMboxPost(g_PCCMDMsg,&MsgBack.CmdPCType);	
	Trace("\r\n Post VP_VENDOUT_IND,%d",sysVPMission.vendColumn2);

	return VP_ERR_NULL; 
}



//添加扣款函数;by gzz 20110823
unsigned char VP_CMD_Cost( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;
	unsigned int CurrentMoney=0;

    //1.Check the data
    if( sysVPMission.receive.datLen != 4  )
	{
	    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}
    sysVPMission.costDev = sysVPMission.receive.msg[0];
	sysVPMission.costMoney = sysVPMission.receive.msg[1]*256 + sysVPMission.receive.msg[2];
	//2.得到当前投入的金额
	
	//3.用户投币金额小于扣款金额时，返回NAK_RPT   
	if(SYSPara.Decimal==1)
	{
	  	CurrentMoney = g_Amount * 10;
	}
	else if(SYSPara.Decimal==2)
	{
	  	CurrentMoney = g_Amount; 
	}	
    if( (sysVPMission.costDev != 0) || (sysVPMission.costMoney > CurrentMoney ) )
	{
    	VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}

	//2.ACK
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
	{
    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
	}

    //3.Set command flag
	sysVPMission.costCmd = sysVPMission.receive.msg[3];
	OSMboxAccept(g_PCCMDMsg);
	MsgBack.CmdPCType = VP_COST_IND;
	OSMboxPost(g_PCCMDMsg,&MsgBack.CmdPCType);	
    return VP_ERR_NULL;	
}

unsigned char VP_CMD_Reset( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;
    
	//1.Check the data
    if( sysVPMission.receive.datLen != 1  )
	{
	    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}
    sysVPMission.resetDev = sysVPMission.receive.msg[0];
    //2.ACK
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
	{
    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
	}
    //3.Reset action......
	//sysVPMission.resetCmd = 1;	
	Trace("\r\n Send ResetIND");
	OSMboxAccept(g_PCCMDMsg);
	MsgBack.CmdPCType = VP_RESET_IND;
	OSMboxPost(g_PCCMDMsg,&MsgBack.CmdPCType);	
    return VP_ERR_NULL;	
}

void GameLedOn( void )
{
    /*
	ZhkHardSramSelect();
	EA = 0;
	ZhkHardIOOutputReg |= BitPowerCtrl;
	ZhkHardNullSelect();
	P0 = ZhkHardIOOutputReg;
	ZhkHardIOOutputSelect();
	ZhkHardOutputLock();
	EA = 1;
    //Trace( "ZhkHardIOOutputReg=%bxH\n" , ZhkHardIOOutputReg );
    //Trace( "P0State=%bxH\n" , P0State );
	*/
}


void GameLedOff( void )
{
    /*
	ZhkHardSramSelect();
	EA = 0;
	ZhkHardIOOutputReg &= ~BitPowerCtrl;
	ZhkHardNullSelect();
	P0 = ZhkHardIOOutputReg;
	ZhkHardIOOutputSelect();
	ZhkHardOutputLock();
	EA = 1;
    //Trace( "ZhkHardIOOutputReg=%bxH\n" , ZhkHardIOOutputReg );
    //Trace( "P0State=%bxH\n" , P0State );
	*/
}

unsigned char VP_CMD_Control( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;
    
	//1.Check the data
    //if( sysVPMission.receive.datLen != 2  )
	//{
	//    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
	//	return VP_ERR_PAR;
	//}
	if( !((sysVPMission.receive.msg[0]>=1)&&(sysVPMission.receive.msg[0]<=17))   )
	{
	    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}
    sysVPMission.ctrType  = sysVPMission.receive.msg[0];
	sysVPMission.ctrValue = sysVPMission.receive.msg[1];
    //Judge the device status
    //if( ((sysVPMission.ctrType==VP_DEV_CTR_LED1)||(sysVPMission.ctrType==VP_DEV_CTR_COMPRESSOR1))&&((SystemParameter.ACDCModule==0)||(DeviceStatus.ACDCModule!=0)) )
    //{
    //	VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
	//	return VP_ERR_PAR;
    //}
    //2.ACK	
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
    {
		VPMsgPackSend( VP_ACK_RPT, 0, 0 );
	}
    //3.Action......
	if( sysVPMission.ctrType == VP_DEV_GAME_LED )
	{
    	if( sysVPMission.ctrValue == 0 )
		{
			GameLedOff();
		}
		else
		{
			GameLedOn();
		}
	}
	else if( sysVPMission.ctrType == VP_DEV_CTR_LED1 )
	{
		if( sysVPMission.ctrValue == 0 )
		{
			sysVPMission.ACDCLedCtr = 0;
		}
		else
		{
			sysVPMission.ACDCLedCtr = 1;
		}
		//ACDCMission();
	}
	else if( sysVPMission.ctrType == VP_DEV_CTR_COMPRESSOR1 )
	{
	    if( sysVPMission.ctrValue == 0 )
		{
			sysVPMission.ACDCCompressorCtr = 0;
		}
		else
		{
			sysVPMission.ACDCCompressorCtr = 1;
		}
		//ACDCMission();    
	}
	else if( sysVPMission.ctrType == VP_DEV_RTC )
	{
	    TranData.Time.YEAR	= sysVPMission.receive.msg[0] * 256 + sysVPMission.receive.msg[1];
		RTCn_SetTime (RTCN_TYPE_YEAR, (uint32_t)(TranData.Time.YEAR));
		TranData.Time.MONTH	= sysVPMission.receive.msg[2];
		RTCn_SetTime (RTCN_TYPE_MONTH,(uint32_t)(TranData.Time.MONTH));
		TranData.Time.DOM	= sysVPMission.receive.msg[3];
		RTCn_SetTime (RTCN_TYPE_DATE,(uint32_t)(TranData.Time.DOM));
		TranData.Time.HOUR	= sysVPMission.receive.msg[4];
		RTCn_SetTime (RTCN_TYPE_HOUR, (uint32_t)(TranData.Time.HOUR));
		TranData.Time.MIN	= sysVPMission.receive.msg[5];
		RTCn_SetTime (RTCN_TYPE_MIN, (uint32_t)(TranData.Time.MIN));
	}
    return VP_ERR_NULL;	
}

unsigned char VP_CMD_GetStatus( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;

	//1.Check the data
    if( sysVPMission.receive.datLen != 0  )
	{
	    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK  )
	{
		VPMsgPackSend( VP_ACK_RPT, 0, 0  );
	}
	//2.
    VPMission_Status_RPT();
    return VP_ERR_NULL;	
}

unsigned char VP_CMD_Payout( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;

    //1.Check the data
    //if( sysVPMission.receive.datLen != 3  )
	//{
		//Trace("\r\none nak");
	 //   VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
	//	return VP_ERR_PAR;
	//}

	//L= 0a e7 08 82 89 6b 
	//00 
	//02 58 
	//02 bf VPBusFrameUnPack
	
    sysVPMission.changeDev   = sysVPMission.receive.msg[0];
	sysVPMission.changeMoney = sysVPMission.receive.msg[1]*256 + sysVPMission.receive.msg[2];

	if( (sysVPMission.changeDev != 0) || (sysVPMission.changeMoney > g_CoinLeft ) || (SYSPara1.CoinsStat!=0) )
	{
		//Trace("\r\ntwo nak");
    	VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}

	//2.ACK
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK )
	{
    	VPMsgPackSend( VP_ACK_RPT, 0, 0 );
	}

    //3.Set command flag
	sysVPMission.changeCmd = sysVPMission.receive.msg[3];
	OSMboxAccept(g_PCCMDMsg);
	
	MsgBack.CmdPCType = VP_PAYOUT_IND; 
	Trace("\r\n Post VP_PAYOUT_IND");
	
	OSMboxPost(g_PCCMDMsg,&MsgBack.CmdPCType);	
    return VP_ERR_NULL;	
}


unsigned char VP_CMD_GetColumnSta( void )
{
    volatile unsigned char i = 0;
    volatile unsigned char j = 0;

	//1.Check the data
    if( sysVPMission.receive.datLen != 0  )
	{
	    VPMsgPackSend( VP_NAK_RPT, 0, 0  );	 
		return VP_ERR_PAR;
	}
	if( sysVPMission.receive.verFlag & VP_PROTOCOL_ACK  )
	{
		//Trace("\r\nHUODAO_IND,return ack");
		VPMsgPackSend( VP_ACK_RPT, 0, 0  );
	}
	//2.
    VPMission_ColumnSta_RPT();
    return VP_ERR_NULL;	
}


//-----------------------------------------------------------------------------
unsigned char UpdateSelectionLed_GoodsSta( void )
{
	uint8_t  i = 0;

    sysVPMission.sel1ErrLed = 0;
    sysVPMission.sel1ReadyLed = 0;
	sysVPMission.sel2ErrLed = 0;
    sysVPMission.sel2ReadyLed = 0;
	sysVPMission.sel3ErrLed = 0;
    sysVPMission.sel3ReadyLed = 0;

	for( i=0; i<KEYEXTRAVAL; i++ )
    {
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel1ErrLed |= 1<<i;
		}
    	else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
        {
        	sysVPMission.sel1ErrLed |= 1<<i;
        }
        else
        {
            sysVPMission.sel1ReadyLed |= 1<<i;
        }
    }
    for( i=KEYEXTRAVAL; i<KEYEXTRAVAL*2; i++ )
    {
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
		}
    	else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
        {
        	sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
        }
        else
        {
            sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL);
        }
    }
	for( i=KEYEXTRAVAL*2; i<KEYEXTRAVAL*3; i++ )
    {
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
		}
    	else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
        {
        	sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
        }
        else
        {
            sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2);
        }
    }
/*
	//111109 by cq 五角货不卖
	for( i=0; i<KEYEXTRAVAL; i++ )
	{
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;//有亮灯
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;
	}
	
	if( i >= KEYEXTRAVAL)
		g_Goods5jLeft = 1;
	else
		g_Goods5jLeft = 0;
*/	
    return VP_ERR_NULL;
}

unsigned char UpdateSelLed_Trade( void )
{   
    uint8_t  i = 0;
    
	//----------------------------------------------------------------
	//2011.5.19 added for the bill holding status judgement
    if( sysITLMission.billHoldingFlag == 1 )
	{
	    //if( (CurrentPayed+sysITLMission.billHoldingValue) <= 0 )
		{
			return VP_ERR_NULL;
		}
	}
	//else if( CurrentPayed <= 0 )
	//{
	//	return VP_ERR_NULL;	
	//}
    //=================================================================

    //sysVPMission.sel1ErrLed = 0;
    sysVPMission.sel1ReadyLed = 0;
	//sysVPMission.sel2ErrLed = 0;
    sysVPMission.sel2ReadyLed = 0;
	//sysVPMission.sel3ErrLed = 0;
    sysVPMission.sel3ReadyLed = 0;

	for( i=0; i<KEYEXTRAVAL; i++ )
    {
    
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel1ErrLed |= 1<<i;
		}		
        else if( sysITLMission.billHoldingFlag == 1 )
        {
            //if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( (CurrentPayed+sysITLMission.billHoldingValue) >= sysGoodsMatrix[i].Price ) )
			if( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel1ReadyLed |= 1<<i;
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel1ErrLed |= 1<<i;
	        }
        }
        else
        {
	    	//if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( CurrentPayed >= sysGoodsMatrix[i].Price ) )
			if ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel1ReadyLed |= 1<<i;
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel1ErrLed |= 1<<i;
	        }
        }
        
    }

    for( i=KEYEXTRAVAL; i<KEYEXTRAVAL*2; i++ )
    {
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
		}		
        else if( sysITLMission.billHoldingFlag == 1 )
		{
		    //if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( (CurrentPayed+sysITLMission.billHoldingValue) >= sysGoodsMatrix[i].Price ) )
	        if( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
			{
	        	sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL);
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	        }
		}
	    else
		{
	    	//if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( CurrentPayed >= sysGoodsMatrix[i].Price ) )
	        if( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
			{
	        	sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL);
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	        }
		}
    }

	for( i=KEYEXTRAVAL*2; i<KEYEXTRAVAL*3; i++ )
    {
		//111109 by cq 五角货不卖
		if ((sysVPMission.g_Coin5jLittle == 1) && (sysVPMission.g_Goods5jFlag[i] == 1) && (sysVPMission.SystemState == 0))
		{
			sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
		}		
        else if( sysITLMission.billHoldingFlag == 1 )
		{
			//if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( (CurrentPayed+sysITLMission.billHoldingValue) >= sysGoodsMatrix[i].Price ) )
	        if ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
			{
	        	sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2);
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	        }
		}
	    else
		{
	    	//if( ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE ) && ( CurrentPayed >= sysGoodsMatrix[i].Price ) )
	        if ( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
			{
	        	sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2);
	        }
            //by gzz 20110611 
            else if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
	        {
	        	sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	        }
		}
    }
	/*
	//111109 by cq 五角货不卖
	for( i=0; i<KEYEXTRAVAL; i++ )
	{
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;//有亮灯
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;
		if(sysVPMission.sel1ErrLed && 0x3f);
		else break;
	}
	
	if( i >= KEYEXTRAVAL)
		g_Goods5jLeft = 1;
	else
		g_Goods5jLeft = 0;
	*/
    return VP_ERR_NULL;
    
}

unsigned char UpdateGoodsMatrixStatus( unsigned char goodsNum )
{
    uint8_t  i = 0;
	uint8_t  j = 0;
	uint8_t  k = 0;
	uint8_t  oldOne = 0;
	uint8_t  position = 0;

	//-------------------------------------------------------------------------------------------------------------------
	//Update the information for vending
	if( goodsNum == 0xff )
	{
		for( i=0; i<GOODSTYPEMAX; i++)
		{
		    //-------------------------------------------------
		    //oldOne = sysGoodsMatrix[i].NextColumn;
			oldOne = sysGoodsMatrix[i].ColumnMatrix[0];
			for( k=0; k<sysGoodsMatrix[i].ColumnNum; k++ )
			{
				if( sysGoodsMatrix[i].NextColumn == sysGoodsMatrix[i].ColumnMatrix[k] )
				{
					oldOne = k;
					break;
				}
			}
			//=================================================
			if( oldOne >= sysGoodsMatrix[i].ColumnNum-1 )
			{
			    //1.find in all stage
				sysGoodsMatrix[i].NextColumn = 0xff;
		  		for( j=0; j<sysGoodsMatrix[i].ColumnNum; j++)
				{
				    k = sysGoodsMatrix[i].ColumnMatrix[j];
					//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
					//{
					//	sysGoodsMatrix[i].NextColumn = k;
					//	break;
					//}
				}	
			}
			else
			{
			    //2.find in ascending order stage
				sysGoodsMatrix[i].NextColumn = 0xff;
				position = oldOne+1;
		  		for( j=position; j<sysGoodsMatrix[i].ColumnNum; j++)
				{
				    k = sysGoodsMatrix[i].ColumnMatrix[j];
					//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
					//{
					//	sysGoodsMatrix[i].NextColumn = k;
					//	break;
					//}
				}
				//3.find in descending order stage
				if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
				{
					sysGoodsMatrix[i].NextColumn = 0xff;
			  		for( j=0; j<sysGoodsMatrix[i].ColumnNum; j++)
					{
					    k = sysGoodsMatrix[i].ColumnMatrix[j];
						//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
						//{
						//	sysGoodsMatrix[i].NextColumn = k;
						//	break;
						//}
					}	
				}
			}
		    
		}
		
    }

	//
	if( (goodsNum>=0)&&(goodsNum<GOODSTYPEMAX) )
	{    
	    i = goodsNum;
		//-------------------------------------------------
	    //oldOne = sysGoodsMatrix[i].NextColumn;
		oldOne = sysGoodsMatrix[i].ColumnMatrix[0];
		for( k=0; k<sysGoodsMatrix[i].ColumnNum; k++ )
		{
			if( sysGoodsMatrix[i].NextColumn == sysGoodsMatrix[i].ColumnMatrix[k] )
			{
				oldOne = k;
				break;
			}
		}
		//=================================================
		if( oldOne >= sysGoodsMatrix[i].ColumnNum-1 )
		{
		    //1.find in all stage
			sysGoodsMatrix[i].NextColumn = 0xff;
	  		for( j=0; j<sysGoodsMatrix[i].ColumnNum; j++)
			{
			    k = sysGoodsMatrix[i].ColumnMatrix[j];
				//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
				//{
				//	sysGoodsMatrix[i].NextColumn = k;
				//	break;
				//}
			}	
		}
		else
		{
		    //2.find in ascending order stage
			sysGoodsMatrix[i].NextColumn = 0xff;
			position = oldOne+1;
	  		for( j=position; j<sysGoodsMatrix[i].ColumnNum; j++)
			{
			    k = sysGoodsMatrix[i].ColumnMatrix[j];
				//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
				//{
				//	sysGoodsMatrix[i].NextColumn = k;
				//	break;
				//}
			}
			//3.find in descending order stage
			if( sysGoodsMatrix[i].NextColumn == GOODS_MATRIX_NONE )
			{
				sysGoodsMatrix[i].NextColumn = 0xff;
		  		for( j=0; j<sysGoodsMatrix[i].ColumnNum; j++)
				{
				    k = sysGoodsMatrix[i].ColumnMatrix[j];
					//if( /*(GoodsWaySetVal[k].Price > 0) && */(GoodsWaySetVal[k].GoodsCurrentSum > 0) && (GoodsWaySetVal[k].WayState & 0x01) && ( (GoodsWaySetVal[k].WayState & 0x0A ) == 0x00 ) )  //0x01, 0x02
					//{
					//	sysGoodsMatrix[i].NextColumn = k;
					//	break;
					//}
				}	
			}
		}
		//--------------------------------------------------------------------------------------
		for( i=0; i<GOODSTYPEMAX; i++ )
	    {
	    	if( sysGoodsMatrix[i].GoodsType == sysGoodsMatrix[goodsNum].GoodsType )
			{
				sysGoodsMatrix[i].NextColumn = sysGoodsMatrix[goodsNum].NextColumn;
				//
			}
	    }
	}
    //------------------------------------------------------------------------------------------
    /*
	for( i=0; i<GOODSTYPEMAX-1; i++ )
	{
		for( j=i+1; j<GOODSTYPEMAX; j++ )
		{
			if( sysGoodsMatrix[i].GoodsType == sysGoodsMatrix[j].GoodsType )
			{
                if( sysGoodsMatrix[i].NextColumn != GOODS_MATRIX_NONE )
                {
					sysGoodsMatrix[j].NextColumn = sysGoodsMatrix[i].NextColumn;
				  //sysGoodsMatrix[j].ColumnNum  = sysGoodsMatrix[i].ColumnNum;
                }
                else if( sysGoodsMatrix[j].NextColumn != GOODS_MATRIX_NONE )
                {
                    sysGoodsMatrix[i].NextColumn = sysGoodsMatrix[j].NextColumn;
                }
			}
		}
	}
    */
    
    //------------------------------------------------------------------------------------------
	for( i=0; i<GOODSTYPEMAX; i++)
	{
		//Trace("\n sysGoodsMatrix[%bu].GoodsType = %u", i, sysGoodsMatrix[i].GoodsType );
		//Trace("\n sysGoodsMatrix[%bu].ColumnNumber = %bu", i, sysGoodsMatrix[i].ColumnNumber );
		//Trace("\n sysGoodsMatrix[%bu].NextColumn = %u", i, sysGoodsMatrix[i].NextColumn );
		for( j=0; j<sysGoodsMatrix[i].ColumnNum; j++ )
		{
			//Trace("\n sysGoodsMatrix[%bu].ColumnMatrix[%bu] = %u", i, j, sysGoodsMatrix[i].ColumnMatrix[j] );
		}
	}
    
    //Update the selection led status
	//2011.5.19 added judgement for the trade mode
    if( (CurrentPayed+sysITLMission.billHoldingValue) == 0 )
	{
		UpdateSelectionLed_GoodsSta();
	}
    return VP_ERR_NULL;
}

unsigned char VP_Update_ColumnGoodsPar( void )
{
    unsigned char x = 0;
	unsigned char y = 0;
	unsigned char i = 0;
	uint8_t   channel,ChannelX,ChannelY;

	//1.Update the storage status	
	for(x = 0; x<MAXLAYER_6;x++)	  
	{
		for(y=0; y<MAXCHANNEL;y++) 
		{
		    if((Channels[x][y].GoodsNum	== 0 )&&((Channels[x][y].ChannelStat& 0x03)==CHANNELOK))
			{
				Channels[x][y].ChannelStat	&= DRV_CHNSET_BIT;
				Channels[x][y].ChannelStat 	|= CHANNELOVER;					
			}
			else if((Channels[x][y].GoodsNum	!= 0 )&&((Channels[x][y].ChannelStat& 0x03)==CHANNELOVER))
			{
				Channels[x][y].ChannelStat	&= DRV_CHNSET_BIT;
				Channels[x][y].ChannelStat 	|= CHANNELOK;					
			}	
		}
	}
	//2.Update the goods matrix and column's price
	for(x = 0; x<MAXLAYER_6;x++)	  
	{		
		for(y=0; y<MAXCHANNEL;y++) 
		{			
			channel = x*10+y+11;
			if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
			{
				for( i=0; i<GOODSTYPEMAX; i++ )
				{			
					if( Channels[ChannelX][ChannelY].GoodsCode == sysGoodsMatrix[i].GoodsType )
					{
		            	Channels[ChannelX][ChannelY].GoodsPrice = sysGoodsMatrix[i].Price;
					}
				}
			}			
		}
	}
	return VP_ERR_NULL;
}
//=======================================================================

//-----------------------------------------------------------------------
//VMC--PC command mission
unsigned char VPMission_Startup_RPT( void )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;

    retry = VP_COM_RETRY;
TAB_VPM_STARPT_RETRY:
	flag = VPMsgPackSend( VP_STARTUP_RPT, 1, 1);
	//Trace("++VPMsgPackSend");
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{	
		//Trace("++VPBusFrameUnPack,%d",i);
		if( VPBusFrameUnPack() )
		{
		    //Trace("++goto ok");
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_STARPT_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_STARPT_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//
        return VP_ERR_COM;
	}
TAB_VPM_STARPT_COM_OK:
	Trace("++now ok");
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}

unsigned char VPMission_Setup_RPT( void )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;

    retry = VP_COM_RETRY;
TAB_VPM_SETUP_RETRY:
    //-------------------------------------------
	//Fixed by VMC or controlled by PC?
    sysVPMission.proVer = VP_PRO_VER;

	//120827 by cq PCcommunication
	//sysVPMission.columnNumSet = 24;
	//sysVPMission.selectionNumSet = 12;
	sysVPMission.columnNumSet = COLUMN_NUM_SET;//48
	sysVPMission.selectionNumSet = GOODSTYPEMAX;//18
	
	//Added the machine VER
    //sysVPMission.send.msg[3] = VP_MAC_SET_H;
    //sysVPMission.send.msg[4] = VP_MAC_SET_L;
	//===========================================
	flag = VPMsgPackSend( VP_VMC_SETUP, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_SETUP_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_SETUP_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//
        return VP_ERR_COM;
	}
TAB_VPM_SETUP_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}


unsigned char VPMission_Payin_RPT( unsigned char dev, unsigned int money )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;

	Trace("\r\nVPMission_Payin_RPT");
    
	//
    retry = VP_COM_RETRY;
TAB_VPM_PAYIN_RETRY:
    //-------------------------------------------
    sysVPMission.payInDev = dev;
	sysVPMission.payInMoney = money;
	//===========================================
	flag = VPMsgPackSend( VP_PAYIN_RPT, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_PAYIN_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_PAYIN_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//3 次PC仍无响应，则跳过该消息，同时打印“PAYIN_RPT Err, device=类型,val=金额”
        
        return VP_ERR_COM;
	}
TAB_VPM_PAYIN_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}


unsigned char VPMission_Payout_RPT( unsigned char dev, unsigned int money1, unsigned int money2 )
{
    unsigned char retry = 0;
    volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//
    retry = VP_COM_RETRY;
TAB_VPM_PAYOUT_RETRY:
    //-------------------------------------------
    sysVPMission.payOutDev = dev;
	sysVPMission.payOutMoney1 = money1;
	sysVPMission.payOutMoney2 = money2;
	//===========================================
	flag = VPMsgPackSend( VP_PAYOUT_RPT, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_PAYOUT_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_PAYOUT_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_PAYOUT_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}
///*
//发送pc扣款后金额;by gzz 20110823
unsigned char VPMission_Cost_RPT( unsigned char dev, unsigned int money )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//
    retry = VP_COM_RETRY;
TAB_VPM_COST_RETRY:  
    //-------------------------------------------
    sysVPMission.costDev = dev;
	sysVPMission.costMoney = money;
	//===========================================
	flag = VPMsgPackSend( VP_COST_RPT, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_COST_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_COST_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_COST_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}

//当上报欠款时，上报pc端;by gzz 20110825
unsigned char VPMission_Debt_RPT( unsigned char type, unsigned char dev, unsigned int money )
{
    volatile unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//
    retry = VP_COM_RETRY;
//TAB_VPM_DEBT_RETRY:  
    //-------------------------------------------
    sysVPMission.debtType = type;
    sysVPMission.debtDev = dev;
	sysVPMission.debtMoney = money;
	//===========================================
	flag = VPMsgPackSend( VP_DEBT_RPT, 0, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}	
    /*
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_DEBT_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_DEBT_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_DEBT_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
    */
	return VP_ERR_NULL;
}

unsigned char VPMission_Vendout_RPT( unsigned char status, unsigned char column, unsigned char type, unsigned int cost )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//--------------------------------------------
	/*
	#ifndef MACHINE_SET_VMC_PC
	   return VP_ERR_NULL;
	#endif
	*/
    //============================================

    retry = VP_COM_RETRY;
TAB_VPM_VENDOUT_RETRY:
    //-------------------------------------------
    sysVPMission.vendSta = status;
	sysVPMission.vendColumn = column+1;
	sysVPMission.vendType = type;
	sysVPMission.vendCost = cost;
	//===========================================
	flag = VPMsgPackSend( VP_VENDOUT_RPT, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_VENDOUT_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_VENDOUT_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_VENDOUT_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}


unsigned char VPMission_Vendout_REQ( unsigned char column, unsigned int cost )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//--------------------------------------------
	/*
	#ifndef MACHINE_SET_VMC_PC
	   return VP_ERR_NULL;
	#endif
	*/
    //============================================

    retry = VP_COM_RETRY;
TAB_VPM_VENDOUTREQ_RETRY:
    //-------------------------------------------
	sysVPMission.vendReqColumn = column+1;
	sysVPMission.vendReqCost = cost;
	//===========================================
	flag = VPMsgPackSend( VP_VENDOUT_REQ, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_VENDOUTREQ_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_VENDOUTREQ_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_VENDOUTREQ_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		case  VP_ACK:
			return VP_ERR_NULL;
			//break;
		default:
			return VP_ERR_COM;
		    //break;
	}	
}



unsigned char VPMission_Request( unsigned char type )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
    retry = VP_COM_RETRY;
TAB_VPM_REQUEST_RETRY:
    //-------------------------------------------
    sysVPMission.request = type;
	//===========================================
	flag = VPMsgPackSend( VP_REQUEST, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_REQUEST_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_REQUEST_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_REQUEST_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	return VP_ERR_NULL;
}

//unsigned char VPMission_Admin_RPT( unsigned char type, unsigned char column, unsigned char num )
unsigned char VPMission_Admin_RPT( unsigned char type, unsigned char column, unsigned int num )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//
    retry = VP_COM_RETRY;
TAB_VPM_ADMIN_RETRY:
    //-------------------------------------------
	if( type == VP_ADMIN_COL_PRICE )    //price
	{
		sysVPMission.ADM_Type   = type;
		sysVPMission.ADM_Dat[0] = column;
		sysVPMission.ADM_Dat[1] = num/256;
        sysVPMission.ADM_Dat[2] = num%256;
	}
	else  //the single column's goods, all columns' goods, or the tray columns'goods
	{
    	sysVPMission.ADM_Type   = type;
		sysVPMission.ADM_Dat[0] = column;
		sysVPMission.ADM_Dat[1] = num;
	}
	//===========================================
	flag = VPMsgPackSend( VP_ADMIN_RPT, 1, 1);
	Trace("\r\n###1.1");
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	Trace("\r\n###1.2");
	while( sysVPMission.msTimer2 )
	{
		Trace("\r\n###msTimer2=%d",sysVPMission.msTimer2);
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_ADMIN_COM_OK;
		}
	}
	Trace("\r\n###1.3");
	if( retry-- )
	{
		Trace("\r\n###retry=%d",retry);
		goto TAB_VPM_ADMIN_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//?
        return VP_ERR_COM;
	}
TAB_VPM_ADMIN_COM_OK:
    switch( sysVPMission.receive.msgType )
	{   
        case VP_NAK:
            return VP_NAK;
        	//break;
		case VP_ACK:
		    return VP_ACK;
		    //break;
		default:
		    break;
	}	
	return VP_ERR_NULL;

}

//For V2
unsigned char VPMission_Action_RPT( unsigned char type, unsigned int value, unsigned char len, unsigned char *dat )
{
    volatile unsigned char i = 0;
    type = 0;
	value = 0;
	len = 0;
    i = *dat;
    
	i=type;
	i=value;
	i=len;
	i=*dat;  
	return VP_ERR_NULL;
}

unsigned char VPMission_Button_RPT( unsigned char type, unsigned char value )
{
    volatile unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;
    
	//
    retry = VP_COM_RETRY;
//TAB_VPM_BUTTON_RETRY:
    //-------------------------------------------
    sysVPMission.BUT_Type   = type;
	sysVPMission.BUT_Value  = value;
    if(type==VP_BUT_GAME)//游戏按键蜂鸣器响;by gzz 20110721
    {   
    	//Beep();
    }
	//===========================================
    //1-0: button message, not need ACK
	flag = VPMsgPackSend( VP_BUTTON_RPT, 0, 1);   
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
    /*
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
			goto TAB_VPM_BUTTON_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_BUTTON_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//-----------------------------------
		//?
        
        return VP_ERR_COM;
	}
TAB_VPM_BUTTON_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
    */
	return VP_ERR_NULL;

}

unsigned char VPMission_Status_RPT( void )
{
    volatile unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;
	uint16_t status;
    
    retry = VP_COM_RETRY;
	//------------------------------------------------------
    //1.硬币器状态cc_st	
	if(SYSPara.CoinEnable == 0)
	{
		sysVPMission.STA_CoinA = 3;
		//Trace("\r\nCoinDisable");
	}
	else
	{
		if (SYSPara1.CoinsStat == 0)
		{
			sysVPMission.STA_CoinA = 0;
			//Trace("\r\nCoinState=0");
		}
		else
		{
			sysVPMission.STA_CoinA = 2;
			//Trace("\r\nCoinErr");
		}	
	}

    //2.机器中当前可用于找零的5角硬币量cnt5j   
    if((SYSPara.CoinEnable > 0)&&(SYSPara1.CoinsStat == 0))
    {
    	sysVPMission.STA_ChangeNum1 = g_coin1Num;
    }
	else
	{		
		sysVPMission.STA_ChangeNum1 = 0;
	}
	
    //3.机器中当前可用于找零的1元硬币量cnt1y 
    if((SYSPara.CoinEnable > 0)&&(SYSPara1.CoinsStat == 0))
    {
    	sysVPMission.STA_ChangeNum2 = g_coin2Num;
    }
	else
	{		
		sysVPMission.STA_ChangeNum2 = 0;
	}	
	//Trace("\r\nChangeNum=0");

    
    //4.纸币器状态bv_st	
	if(SYSPara.GBAEnable == 0)
	{
		sysVPMission.STA_BillA = 3;
		//Trace("\r\nNoteDisable");
	}
	else
	{
		if (SYSPara1.NoteStat == 0)
		{
			sysVPMission.STA_BillA = 0;
			//Trace("\r\nNotestate=0");
		}
		else
		{
			sysVPMission.STA_BillA = 2;
			//Trace("\r\nNoteErr");
		}	
	}

	//5.臭氧/紫外线消毒设备状态
	sysVPMission.STA_Oz = 3;

    //5.找零设备状态ch1_st,ch2_st	
	if(SYSPara.CoinEnable == 0)
	{
		sysVPMission.STA_Changer1 = 3;
		//Trace("\r\nChange1Disable");
	}
	else
	{
		if (SYSPara1.CoinsStat == 0)
		{
			sysVPMission.STA_Changer1 = 0;
			//Trace("\r\nChange1state=0");
		}
		else
		{
			sysVPMission.STA_Changer1 = 2;
			//Trace("\r\nChange1stateErr");
		}	
	}

	if(SYSPara.CoinEnable == 0)
	{
		sysVPMission.STA_Changer2 = 3;
		//Trace("\r\nChange2Disable");
	}
	else
	{
		if (SYSPara1.CoinsStat == 0)
		{
			sysVPMission.STA_Changer2 = 0;
			//Trace("\r\nChange2State=0");
		}
		else
		{
			sysVPMission.STA_Changer2 = 2;
			//Trace("\r\nChange2StateErr");
		}	
	}
	
    
    //6.主控器状态vmc_st
	//只有机器因故障而不能工作后，才上报故障信息;by gzz 20110902
	if(IsDeviceOK())
	{
		sysVPMission.STA_VMC = 0;
		//Trace("\r\nVMC=0");
	}	
	else
    {
	    sysVPMission.STA_VMC = 2;
		//Trace("\r\nVMCErr");
	}
   
	
	//7.加热设备
	sysVPMission.STA_Ht = 0;

	//8.除味设备
	sysVPMission.STA_Rrsw = 3;

	//9.出货设备
	sysVPMission.STA_Outdev = 0;
	
    //7.未定义1,未定义2,未定义3状态
    sysVPMission.STA_Tep1 = 3;
	sysVPMission.STA_Tep2 = 3;
    sysVPMission.STA_Bank   = 3;
	//Trace("\r\ntep1,tep2,bank,bus,mobile=3");
	

    //8.公交一卡通gj	
	sysVPMission.STA_Bus = 3;

	//9.手机支付模块sj
	sysVPMission.STA_MobilePOS = 3;
    
    //10.出货检测
    if(SYSPara.GOCCtr == 0)
    {
    	sysVPMission.STA_ICCard = 3;
		Trace("\r\ngocDisable");
    }
	else
	{
		status = GetDevStatus();
		if(status & DEV_ERR_BIT_GOODSCHK) 
		{
			sysVPMission.STA_ICCard = 2;
			//Trace("\r\nGocErr");
		}
		else 
		{
			sysVPMission.STA_ICCard = 0;
			//Trace("\r\nGoc=0");
		} 	
	}
	
	
	//11.货仓1温度tem1，货仓2温度tem2	
	sysVPMission.E_Tem1 = 0xfe;
	sysVPMission.E_Tem2 = 0xfe;
	//Trace("\r\ntemp1,temp2=fe");

	//==============================================================
//TAB_VPM_STATUS_RETRY:
    flag = VPMsgPackSend( VP_STATUS_RPT, 0, 1);  //1-0, not need ACK
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	/*
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_STATUS_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_STATUS_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//? 
        return VP_ERR_COM;
	}
TAB_VPM_STATUS_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	*/
	return VP_ERR_NULL;

}

unsigned char VPMission_ColumnSta_RPT( void )
{
    volatile unsigned char retry = 0;
	unsigned char i = 0;
	unsigned char x = 0;
	unsigned char y = 0;
	unsigned char flag = 0;  
	
    retry = VP_COM_RETRY;
	//--------------------------------------------	
	memset(sysVPMission.send.msg,0,sizeof(sysVPMission.send.msg));
    //for(i=0; i<COLUMN_NUM_SET; i++)
	//{
        //if( !(GoodsWaySetVal[i].WayState & 0x01) )//故障
        //{
		//    sysVPMission.send.msg[i] |= 0x40;   //Error flag Added by Andy 2011.5.5	
        //}
		//if( GoodsWaySetVal[i].WayState & 0x0A )//故障
 		//{
		//	sysVPMission.send.msg[i] |= 0x40;   //Error flag 
		//}
        //if( ( GoodsWaySetVal[i].WayState & 0x04 )&&( GoodsWaySetVal[i].GoodsCurrentSum >= 1 )  )//缺货
        //{
        //    sysVPMission.send.msg[i] |= 0x80;    
        //}
		//sysVPMission.send.msg[i] += GoodsWaySetVal[i].GoodsCurrentSum & 0x3F; 
	//}
	for(x = 0; x<MAXLAYER;x++)	  
	{
		for(y=0; y<MAXCHANNEL;y++) 
		{	
			if(!((Channels[x][y].ChannelStat& 0x03) & CHANNELOK))//未测试
			{
	            sysVPMission.send.msg[i] |= 0x40;    
	        }
			if((Channels[x][y].ChannelStat& 0x03) == CHANNELHARDERR)//故障
		    {
	            sysVPMission.send.msg[i] |= 0x40;    
	        }
			
		    //if((Channels[x][y].ChannelStat& 0x03) == CHANNELOVER)//缺货
		    //{
	        //    sysVPMission.send.msg[i] |= 0x80;    
	        //}
			
			
			sysVPMission.send.msg[i] += Channels[x][y].GoodsNum;
			
			//sysVPMission.send.msg[i] = Channels[x][y].GoodsNum;			
			//Trace("\r\n current,lay=%d,column=%d,GoodsCode=%d,GoodsPrice=%d,ChannelStat=%x,GoodsNum=%d,SuccessNum=%d,DoubtNum=%d,TestNum=%d,msg=%d",x,y,Channels[x][y].GoodsCode,Channels[x][y].GoodsPrice,Channels[x][y].ChannelStat,Channels[x][y].GoodsNum,Channels[x][y].SuccessNum,Channels[x][y].DoubtNum,Channels[x][y].TestNum,Channels[x][y].TestNum,sysVPMission.send.msg[i]);
			i++;
			
		}
	}

	/*
	Trace("\r\n column=%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,",
			Channels[0][0].ChannelStat,sysVPMission.send.msg[1],sysVPMission.send.msg[2],sysVPMission.send.msg[3],sysVPMission.send.msg[4],sysVPMission.send.msg[5],sysVPMission.send.msg[6],
			sysVPMission.send.msg[7],sysVPMission.send.msg[8],sysVPMission.send.msg[9],sysVPMission.send.msg[10],sysVPMission.send.msg[11],sysVPMission.send.msg[12],sysVPMission.send.msg[13],
			sysVPMission.send.msg[14],sysVPMission.send.msg[15],sysVPMission.send.msg[16],sysVPMission.send.msg[17],sysVPMission.send.msg[18],sysVPMission.send.msg[19],sysVPMission.send.msg[20],
			sysVPMission.send.msg[21],sysVPMission.send.msg[22],sysVPMission.send.msg[23],sysVPMission.send.msg[24],sysVPMission.send.msg[25],sysVPMission.send.msg[26],sysVPMission.send.msg[27],
			sysVPMission.send.msg[28],sysVPMission.send.msg[29],sysVPMission.send.msg[30],sysVPMission.send.msg[31],sysVPMission.send.msg[32],sysVPMission.send.msg[33],sysVPMission.send.msg[34],
			sysVPMission.send.msg[35],sysVPMission.send.msg[36],sysVPMission.send.msg[37],sysVPMission.send.msg[38],sysVPMission.send.msg[39],sysVPMission.send.msg[40],sysVPMission.send.msg[41],
			sysVPMission.send.msg[42],sysVPMission.send.msg[43],sysVPMission.send.msg[44],sysVPMission.send.msg[45],sysVPMission.send.msg[46],sysVPMission.send.msg[47]);
    
    //Trace("\r\n 11 current,lay=%d,column=%d,GoodsCode=%d,GoodsPrice=%d,ChannelStat=%d,GoodsNum=%d,SuccessNum=%d,DoubtNum=%d,TestNum=%d",NowLayerNo,NowCHNO,Channels[NowLayerNo][NowCHNO].GoodsCode,Channels[NowLayerNo][NowCHNO].GoodsPrice,Channels[NowLayerNo][NowCHNO].ChannelStat,Channels[NowLayerNo][NowCHNO].GoodsNum,Channels[NowLayerNo][NowCHNO].SuccessNum,Channels[NowLayerNo][NowCHNO].DoubtNum,Channels[NowLayerNo][NowCHNO].TestNum,Channels[NowLayerNo][NowCHNO].TestNum);
	
    //Trace("\r\n 11col,GoodsCode=%d,GoodsPrice=%d,ChannelStat=%d,GoodsNum=%d,SuccessNum=%d,DoubtNum=%d,TestNum=%d",Channels[1][1].GoodsCode,Channels[1][1].GoodsPrice,Channels[1][1].ChannelStat,Channels[1][1].GoodsNum,Channels[1][1].SuccessNum,Channels[1][1].DoubtNum,Channels[1][1].TestNum,Channels[1][1].TestNum);
	
	Trace("\r\n msg=%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,",
		sysVPMission.send.msg[0],sysVPMission.send.msg[1],sysVPMission.send.msg[2],sysVPMission.send.msg[3],sysVPMission.send.msg[4],sysVPMission.send.msg[5],sysVPMission.send.msg[6],
		sysVPMission.send.msg[7],sysVPMission.send.msg[8],sysVPMission.send.msg[9],sysVPMission.send.msg[10],sysVPMission.send.msg[11],sysVPMission.send.msg[12],sysVPMission.send.msg[13],
		sysVPMission.send.msg[14],sysVPMission.send.msg[15],sysVPMission.send.msg[16],sysVPMission.send.msg[17],sysVPMission.send.msg[18],sysVPMission.send.msg[19],sysVPMission.send.msg[20],
		sysVPMission.send.msg[21],sysVPMission.send.msg[22],sysVPMission.send.msg[23],sysVPMission.send.msg[24],sysVPMission.send.msg[25],sysVPMission.send.msg[26],sysVPMission.send.msg[27],
		sysVPMission.send.msg[28],sysVPMission.send.msg[29],sysVPMission.send.msg[30],sysVPMission.send.msg[31],sysVPMission.send.msg[32],sysVPMission.send.msg[33],sysVPMission.send.msg[34],
		sysVPMission.send.msg[35],sysVPMission.send.msg[36],sysVPMission.send.msg[37],sysVPMission.send.msg[38],sysVPMission.send.msg[39],sysVPMission.send.msg[40],sysVPMission.send.msg[41],
		sysVPMission.send.msg[42],sysVPMission.send.msg[43],sysVPMission.send.msg[44],sysVPMission.send.msg[45],sysVPMission.send.msg[46],sysVPMission.send.msg[47]);
	*/

	
	//============================================
//TAB_VPM_COLUMNSTA_RETRY:	
	flag = VPMsgPackSend( VP_COLUMNSTA_RPT, 0, 1);   //Not need ACK
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	/*
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
			goto TAB_VPM_COLUMNSTA_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_COLUMNSTA_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//   
        return VP_ERR_COM;
	}
TAB_VPM_COLUMNSTA_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
		default:
		    break;
	}
	*/
	//Trace("\r\nHUODAO_RPT,return ack");	
	return VP_ERR_NULL;

}

unsigned char VPMission_Card_RPT( void )
{
    volatile unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;  
    retry = VP_COM_RETRY;
    //1-0: button message, not need ACK
	flag = VPMsgPackSend( VP_CARD_RPT, 0, 1);   
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	return VP_ERR_NULL;

}

unsigned char VPMission_Poll( void )
{
    unsigned char retry = 0;
	volatile unsigned char i = 0;
	unsigned char flag = 0;

    retry = VP_COM_RETRY;
TAB_VPM_P_RETRY:
	flag = VPMsgPackSend( VP_POLL, 1, 1);
    if( flag != VP_ERR_NULL )
    {
		return VP_ERR_PAR;
	}
	sysVPMission.msTimer2 = VP_TIME_OUT;
	while( sysVPMission.msTimer2 )
	{
		if( VPBusFrameUnPack() )
		{
		    sysVPMission.comErrNum = 0;
			goto TAB_VPM_P_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_VPM_P_RETRY;
	}
	else
	{
		sysVPMission.comErrNum++;
		//
        return VP_ERR_COM;
	}
TAB_VPM_P_COM_OK:
    switch( sysVPMission.receive.msgType )
	{
	    case VP_INITIAL_OK:
			Trace("\r\nVP_INITIAL_OK IND");
		    VP_CMD_Init_OK();
			break;
		case VP_VENDOUT_IND:			
			Trace("\r\nVP_VENDOUT_IND");
		    VP_CMD_Vendout();
		    break;
		case VP_RESET_IND:
			Trace("\r\nVP_RESET_IND");
		    VP_CMD_Reset();
		    break;
		case VP_CONTROL_IND:
			Trace("\r\nVP_CONTROL_IND");
		    VP_CMD_Control();
		    break;
		case VP_GET_STATUS:
			Trace("\r\nVP_GET_STATUS");
		    VP_CMD_GetStatus();
		    break;
		case VP_HOUDAO_IND:
			Trace("\r\nVP_HOUDAO_IND");
		    VP_CMD_ColumnPar();	
			if( sysVPMission.sysStatus & VPM_STA_PCPAROK )
			{
				VP_Update_ColumnGoodsPar();
			}
		    break;
		case VP_POSITION_IND:
			Trace("\r\nVP_POSITION_IND");
		    VP_CMD_GoodsPar();	
			if( sysVPMission.sysStatus & VPM_STA_PCPAROK )
			{
				Trace("\r\nVP_Update_ColumnGoodsPar");
				VP_Update_ColumnGoodsPar();
			}
			break;
	    case VP_PAYOUT_IND:
			Trace("\r\nVP_PAYOUT_IND");
		    VP_CMD_Payout();
		    break;
        case VP_GET_COLUMNSTA:
			Trace("\r\nVP_GET_COLUMNSTA");
            VP_CMD_GetColumnSta();
            break;
        case VP_COST_IND://添加扣款函数;by gzz 20110823
			Trace("\r\nVP_COST_IND");
		    VP_CMD_Cost();
		    break;
		default:
		    break;
	}
	return VP_ERR_NULL;
}

unsigned char VPMission_Init( void )
{
    //unsigned char i = 0;
	volatile unsigned char j = 0;
	unsigned char result = 0;
   
	//1.startup
TAB_VPM_INIT_STARTUP:
	do
	{
    	result = VPMission_Startup_RPT();
	}while( result != VP_ERR_NULL );
	//Trace("\r\n Startup");
    
	//2.setup
	result = VPMission_Setup_RPT();
	if( result != VP_ERR_NULL ) goto TAB_VPM_INIT_STARTUP;
    //Trace("\r\nSetup");
    
    while( 1 )
    {
	    
    	result = VPMission_Poll();
		Trace("\r\nPoll");
        if( result != VP_ERR_NULL ) goto TAB_VPM_INIT_STARTUP;
        if( (sysVPMission.sysStatus & VPM_STA_INITOK)&&(sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&(sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
        {
        	VP_Update_ColumnGoodsPar();
        	sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
        	sysVPMission.sysStatus &= ~VPM_STA_GOODSPAR;
            sysVPMission.sysStatus &= ~VPM_STA_INITOK;
        	sysVPMission.VPMode = 1;
			Trace("\r\nUpdate_Column");
            break;
        }

    }
    sysVPMission.resetCmd = 0;
    

    sysVPMission.dspTimer1  = VP_DSP_TIME1;
	sysVPMission.sysStatus |= VPM_STA_PCPAROK;
    
	return VP_ERR_NULL;
}

//Flag=1更新主机参数，Flag=2更新从机参数;by gzz 20110509
unsigned char VPAddSingleColGoods( unsigned char col, unsigned char num, unsigned char oldNum, unsigned char Flag )
{   
    volatile uint8_t  i    = 0;
	volatile uint8_t  key  = 0;
	uint8_t  flag = 0;
	uint8_t *NowKey;
	uint8_t err=0;

	WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][0],strlen(Menu_ChannelOpt[SYSPara.Language][0]));
    WriteFull_LCD(LINE1,Menu_ChannelOpt[SYSPara.Language][1],strlen(Menu_ChannelOpt[SYSPara.Language][1]));

	//120621 by cq KeySlowly
	//NowKey = OSMboxPend(g_KeyMsg,0,&err);		//等待键盘输入
    NowKey = OSQPend(g_KeyMsg,0,&err);		//等待键盘输入
    
	if (err ==OS_NO_ERR) 
	{
		if(*NowKey == KEY_CANCEL) 
			return 0;
   	}
	
	ClearScreen();
    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][5],strlen(Menu_ChannelOpt[SYSPara.Language][5]));

    OSTimeDly(OS_TICKS_PER_SEC / 5);
	
    while( 1 )
    {
		if(Flag == 1)
			flag = VPMission_Admin_RPT( VP_ADMIN_GOODSADDCOL, col+1, num );
		else if(Flag == 2)
			flag = VPMission_Admin_RPT( VP_ADMIN_GOODSADDSLAVE, col+1, num );

		if( (flag==VP_ERR_NULL)||(flag==VP_ACK)||(flag==VP_NAK) ) break;
        if( flag == VP_ERR_COM ) break;
        OSTimeDly(OS_TICKS_PER_SEC / 2);
	}

    if( flag == VP_ERR_COM )
	{	    
        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][6],strlen(Menu_ChannelOpt[SYSPara.Language][6]));
	}
	else if( sysVPMission.receive.msgType == VP_ACK )
	{
        //by gzz 20120223
	    VPMission_Request(1);
	    sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
	    while( 1 )
	    {
			flag = VPMission_Poll();
			if( flag == VP_ERR_COM ) break;
			if( /*(sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&*/ (sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
		    {			
		    	VP_Update_ColumnGoodsPar();
				break;
		    }
	       OSTimeDly(OS_TICKS_PER_SEC / 2);
		}
			    
		if(Flag == 1)
		{	    
	        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][7],strlen(Menu_ChannelOpt[SYSPara.Language][7]));
		}
		return 1;
    }
	else
	{	    
        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][6],strlen(Menu_ChannelOpt[SYSPara.Language][6]));
	}
	return 0;

}

unsigned char VPChangeColPrice( unsigned char col, unsigned int price, unsigned int oldPrice )
{   
    volatile uint8_t  i    = 0;
	volatile uint8_t  key  = 0;
	uint8_t  flag = 0;
	uint8_t *NowKey;
	uint8_t err=0;

    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][0],strlen(Menu_ChannelOpt[SYSPara.Language][0]));
    WriteFull_LCD(LINE1,Menu_ChannelOpt[SYSPara.Language][1],strlen(Menu_ChannelOpt[SYSPara.Language][1]));

	//120621 by cq KeySlowly
	//NowKey = OSMboxPend(g_KeyMsg,0,&err);		//等待键盘输入
    NowKey = OSQPend(g_KeyMsg,0,&err);		//等待键盘输入
    
   	if (err ==OS_NO_ERR) 
	{
		if(*NowKey == KEY_CANCEL) 
			return 0;
   	}
	
	ClearScreen();
    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][2],strlen(Menu_ChannelOpt[SYSPara.Language][2]));

    OSTimeDly(OS_TICKS_PER_SEC / 5);
    while( 1 )
    {
    	//Trace("\r\n+++++++++NOW VPMission_Admin_RPT"); 
		flag = VPMission_Admin_RPT( VP_ADMIN_COL_PRICE, col+1, price );
        if( (flag==VP_ERR_NULL)||(flag==VP_ACK)||(flag==VP_NAK) ) break;
        if( flag == VP_ERR_COM ) break;
        OSTimeDly(OS_TICKS_PER_SEC / 2);
	}  
 
	if( flag == VP_ERR_COM )
	{	    
        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][3],strlen(Menu_ChannelOpt[SYSPara.Language][3]));
	}
    else if( sysVPMission.receive.msgType == VP_ACK )
	{
	    //Trace("\r\n+++++++++NOW VPMission_Request"); 
        VPMission_Request(1);
	    sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
	    sysVPMission.sysStatus &= ~VPM_STA_GOODSPAR;
	    while( 1 )
	    {
	        //Trace("\r\n+++++++++NOW VPMission_Poll"); 
			VPMission_Poll();
			
			if( (sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&(sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
		    {
		    	//Trace("\r\n+++++++++VPM_STA_GOODSPAR,VPM_STA_COLUMNPAR"); 
		    	VP_Update_ColumnGoodsPar();
			 	break;				
		    }
	        OSTimeDly(OS_TICKS_PER_SEC / 2);
		}
			    
		WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][4],strlen(Menu_ChannelOpt[SYSPara.Language][4]));
		return 1;
    }
	else
	{	    
        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][3],strlen(Menu_ChannelOpt[SYSPara.Language][3]));
	}
    OSTimeDly(OS_TICKS_PER_SEC * 3);	
	return 0;

}

unsigned char VPAddAllColGoods( void )
{   
    volatile uint8_t  i    = 0;
	volatile uint8_t  key  = 0;
	uint8_t  flag = 0;
		
	ClearScreen();
    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][9],strlen(Menu_ChannelOpt[SYSPara.Language][9]));

    OSTimeDly(OS_TICKS_PER_SEC / 5);
			
    VPMission_Admin_RPT( VP_ADMIN_GOODSADDALL, 0, 0);
    VPMission_Request(1);
    sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
    while( 1 )
    {
		flag = VPMission_Poll();

		if( flag == VP_ERR_COM ) break;

		if( /*(sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&*/ (sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
	    {			
	    	VP_Update_ColumnGoodsPar();
			break;
	    }
        OSTimeDly(OS_TICKS_PER_SEC / 2);
	}

    if( flag == VP_ERR_COM  )
	{	    
        WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][10],strlen(Menu_ChannelOpt[SYSPara.Language][10]));
	}
	else
	{	 
		WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][11],strlen(Menu_ChannelOpt[SYSPara.Language][11]));
		return 1;	        
	}
    OSTimeDly(OS_TICKS_PER_SEC);
		
	return 0;

}

//填满层架货道;by gzz 20110429
unsigned char VPAddLayerColGoods( void )
{
	//uint8_t  i    = 0;
	//uint8_t  key  = 0;
	uint8_t  flag = 0;
	//uint8_t  RackNo = 0;
    //uint8_t  Tempstr[8];
    //uint8_t  len = 0;
	  		
	if(	SYSPara.LayEnable[NowLayerNo] == 0x01)
	{	    		
		ClearScreen();
	    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][13],strlen(Menu_ChannelOpt[SYSPara.Language][13]));

	    OSTimeDly(OS_TICKS_PER_SEC / 5);		
	    //--------------------------------------------------------------
	    VPMission_Admin_RPT( VP_ADMIN_GOODSADDTRAY, NowLayerNo+1, 0);
	    //sysVPMission.requestFlag = 1;
		VPMission_Request(1);
	    sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
	    while( 1 )
	    {
			flag = VPMission_Poll();
			if( flag == VP_ERR_COM ) break;
			if( /*(sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&*/ (sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
		    {
		    	VP_Update_ColumnGoodsPar();
				break;
		    }
	        OSTimeDly(OS_TICKS_PER_SEC / 2);
		}
		//==============================================================
		if( flag == VP_ERR_COM  )
		{
			WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][14],strlen(Menu_ChannelOpt[SYSPara.Language][14]));
		}
		else
		{
			WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][15],strlen(Menu_ChannelOpt[SYSPara.Language][15]));
			return 1;						
		}			
	}
    OSTimeDly(OS_TICKS_PER_SEC);
	return 0;

}

//by gzz 20110506
//	1补硬币，2取纸币
unsigned char VPAddChanges( uint8_t  testDev )
{  
    uint8_t  i    = 0;
	uint8_t  key  = 0;
	uint8_t  flag = 0;
	uint8_t  DevStr[10];
	uint8_t  str[20];
	uint8_t  len = 0;
	uint8_t *NowKey;
	uint8_t err=0;

	
	if(testDev == 1)
	{
		if(SYSPara.Language==0)
		{
			sprintf( DevStr, "%s", "补硬币" );
		}
		else
		{
			sprintf( DevStr, "%s", "Coin" );
		}			
	}
	else if(testDev == 2)
	{
		if(SYSPara.Language==0)
		{
			sprintf( DevStr, "%s", "取纸币" );
		}
		else
		{
			sprintf( DevStr, "%s", "Cash" );
		}				
	}

	ClearScreen();
	if(SYSPara.Language==0)
	{
		len = sprintf( str, "  你确定%s吗?", DevStr ); 
		WriteFull_LCD(LINE0,str, len);		
		WriteFull_LCD(LINE1,Menu_ChannelOpt[SYSPara.Language][1],strlen(Menu_ChannelOpt[SYSPara.Language][1]));
	}
	else
	{
		WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][0],strlen(Menu_ChannelOpt[SYSPara.Language][0]));
		WriteFull_LCD(LINE1,Menu_ChannelOpt[SYSPara.Language][1],strlen(Menu_ChannelOpt[SYSPara.Language][1]));
	}


												
	
	//120621 by cq KeySlowly
	//NowKey = OSMboxPend(g_KeyMsg,0,&err);		//等待键盘输入
    NowKey = OSQPend(g_KeyMsg,0,&err);		//等待键盘输入

	if (err ==OS_NO_ERR) 
	{
		if(*NowKey == KEY_CANCEL) 
			return 0;
	}
	
	ClearScreen();

	if(SYSPara.Language==0)
	{
		len = sprintf( str, "  \xd5\xfd在%s", DevStr );
		WriteFull_LCD(LINE0,str, len);	
	}
	else
	{
		WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][17],strlen(Menu_ChannelOpt[SYSPara.Language][17]));		
	}
    OSTimeDly(OS_TICKS_PER_SEC / 5);	
    while( 1 )
    {
		if(testDev == 1)
		{
			flag = VPMission_Admin_RPT( VP_ADMIN_CHANGEADD, 0, 0 );
		}
		else if(testDev == 2)
		{
			flag = VPMission_Admin_RPT( VP_ADMIN_GETBILL, 0, 0 );
		}
		if( (flag==VP_ERR_NULL)||(flag==VP_ACK)||(flag==VP_NAK) ) break;
		if( flag == VP_ERR_COM ) break;
        OSTimeDly(OS_TICKS_PER_SEC / 2);
	}

	if( flag == VP_ERR_COM )
    {
		if(SYSPara.Language==0)
		{
			len = sprintf( str, "%s失败!", DevStr );
			WriteFull_LCD(LINE0,str, len);	
		}
		else
		{
			WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][3],strlen(Menu_ChannelOpt[SYSPara.Language][3]));		
		}
	}
    else if( sysVPMission.receive.msgType == VP_ACK )
	{		
		if(SYSPara.Language==0)
		{
			len = sprintf( str, "%s成功!", DevStr );
			WriteFull_LCD(LINE0,str, len);	
		}
		else
		{
			WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][4],strlen(Menu_ChannelOpt[SYSPara.Language][4]));		
		}
        if(testDev == 1)
		{				
            //TradeCounter.CoinSum5jBack = TradeCounter.CoinSum5j;			
			//TradeCounter.CoinSum5jID++;
            //TradeCounter.CoinSum1yBack = TradeCounter.CoinSum1y;			
			//TradeCounter.CoinSum1yID++;
           
            //TradeCounter.Hopper1SumBack = TradeCounter.Hopper1Sum;			
			//TradeCounter.Hopper1SumID++;
            //TradeCounter.Hopper2SumBack = TradeCounter.Hopper2Sum;			
			//TradeCounter.Hopper2SumID++;
            //TradeCounter.Hopper3SumBack = TradeCounter.Hopper3Sum;			
			//TradeCounter.Hopper3SumID++;
		}
		else if(testDev == 2)
		{
			//TradeCounter.CashSumBack = TradeCounter.CashSum;			
			//TradeCounter.CashSumID++;
		}    
        //SaveTradeParam();//by gzz 20110610
    }
	else
	{
		if(SYSPara.Language==0)
		{
			len = sprintf( str, "%s失败!", DevStr );
			WriteFull_LCD(LINE0,str, len);	
		}
		else
		{
			WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][3],strlen(Menu_ChannelOpt[SYSPara.Language][3]));		
		}
	}
    OSTimeDly(OS_TICKS_PER_SEC * 3);		
	return 0;
  
}

unsigned char VPSynGoodsCol( void )
{   
    volatile uint8_t  i = 0;
	volatile uint8_t  key = 0;
    uint8_t  flag = 0;
	uint8_t *NowKey;
	uint8_t err=0;

	ClearScreen();
	WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][16],strlen(Menu_ChannelOpt[SYSPara.Language][16]));
    WriteFull_LCD(LINE1,Menu_ChannelOpt[SYSPara.Language][1],strlen(Menu_ChannelOpt[SYSPara.Language][1]));

	//120621 by cq KeySlowly
	//NowKey = OSMboxPend(g_KeyMsg,0,&err);		//等待键盘输入
    NowKey = OSQPend(g_KeyMsg,0,&err);		//等待键盘输入

   	if (err ==OS_NO_ERR) 
	{
		if(*NowKey == KEY_CANCEL) 
			return 0;
   	}
	
	ClearScreen();
    WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][17],strlen(Menu_ChannelOpt[SYSPara.Language][17]));

    OSTimeDly(OS_TICKS_PER_SEC / 5);
	
    while( 1 )
	{
		 flag = VPMission_Admin_RPT( VP_ADMIN_GOODSCOLUMN, 0, 0);
		 if( flag == VP_ACK )  break;
         OSTimeDly(OS_TICKS_PER_SEC);
	}
    //sysVPMission.requestFlag = 1;
	VPMission_Request(1);
    sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
    sysVPMission.sysStatus &= ~VPM_STA_GOODSPAR;
    while( 1 )
    {
		VPMission_Poll();
		if( (sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&(sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
	    {
			VP_Update_ColumnGoodsPar();
			break;
	    }
        OSTimeDly(OS_TICKS_PER_SEC / 2);
	}    
	WriteFull_LCD(LINE0,Menu_ChannelOpt[SYSPara.Language][18],strlen(Menu_ChannelOpt[SYSPara.Language][18]));
    OSTimeDly(OS_TICKS_PER_SEC * 2);		
	return 0;

}

unsigned char VPCountCoin( unsigned char hopperNum )
{   
    uint8_t  i = 0;
	volatile uint8_t  key = 0;
    volatile uint8_t  flag = 0;
	uint8_t  ret = 0;
	uint8_t  coin1flag = 0; 
    uint16_t   coinNum   = 0;
    volatile uint16_t   coinMoney = 0;
    uint16_t   coinPrice = 0;
	volatile uint8_t  str[20];
	uint8_t  len = 0;
    //struct DEVICE  *HopperDev;
    uint8_t  *devHopperSta;

	//if( !((hopperNum>=1)&&(hopperNum<=2)) )  return 0;
    
	if( hopperNum == 1 )
	{
		//HopperDev = &Hopper1;
        //coinPrice = SystemParameter.HopperCoinPrice1;
        //devHopperSta = &DeviceStatus.ChangeUnit1;	
	}
	else if( hopperNum == 2 )
	{
	    //HopperDev = &Hopper2;
        //coinPrice = SystemParameter.HopperCoinPrice2;
        //devHopperSta = &DeviceStatus.ChangeUnit1;	
	}
	else
	{
		return 0;
	}

#ifdef _CHINA_
	//DisplayStr( 0, 0, 1, "  你确定清点吗?", 15 );   
	//DisplayStr( 0, 1, 1, "确定ENTER 取消CANCEL", 20 );	
#else		
	//DisplayStr( 0, 0, 1, "   Are you sure?", 16 );  
	//DisplayStr( 0, 1, 1, "   ENTER   CANCEL", 17 );	
#endif
													
	//while( 1 )
	//{
		//key = GetKey();
		//if( ( key == KEY_CANCEL ) || (key == KEY_SUBMIT) )
		//	break;
		//OSTimeDly(OS_TICKS_PER_SEC / 10);	
	//}
	//if( key == KEY_CANCEL ) return 0;
	
	//Display the message
//#ifdef _CHINA_
	//DisplayStr( 0, 0, 1, "\xd5\xfd在清点, 请稍等...", 19 );    
	//ClearDisplayLine( 2 );	
//#else		
	//DisplayStr( 0, 0, 1, "Please waiting...", 17 );    
	//ClearDisplayLine( 2 );
//#endif
    //WaitForWork( 1000, NULL );
    //if( HopperOutCoin( 1000, hopperNum, HopperDev ) == 0 )		
	{   
        OSTimeDly(OS_TICKS_PER_SEC);
		//查询出币结果	
		ret = 0;
		while( ret < 2 )
		{
			//if( GetHardWareDeviceState( hopperNum, HopperDev ) != 1 ) //更新状态成功
			{
				//if( HopperDev->State &  0x50 )   //红外线传感有问题或系统故障
				//	coin1flag |= 4;              //硬件故障			
				//if( HopperDev->State &  0x20 )   //实际无币或光测无币
				//	coin1flag |= 2;              //币不足
				//--------------------------------------------------------
				//if( HopperDev->State & 0x08 )    //Empty
				//    coin1flag |= 8;
				//========================================================
				//coinNum = HopperGetCoinOut( hopperNum );
                coinMoney = coinNum*coinPrice;
				Trace( "\n outMoney1 = %d", coinNum );		
				ret = 0;
				break;
			}
			//else    //查询状态超时
			//{
			//	ret++;
			//	OSTimeDly(OS_TICKS_PER_SEC);	
			//}
		}
		if( ret >= 2 )  coin1flag |= 4;
	}
	//else
	//{
	//	coin1flag |= 4;
	//}

    //update the hopper status
    for( i = 0; i < 3; i ++ )
	{
		//if( HopperSetList[i].HopperIndex == hopperNum)					
		{
			if( coin1flag & 4 )	
			{
				//DeviceStatus.ChangeUnit1 = 1;
                *devHopperSta = 1;
				//HopperSetList[i].HopperState = 2;
			}
			else if( coin1flag & 8 )                 //Empty
			{
			    //DeviceStatus.ChangeUnit1 = 4;      //Empty
                *devHopperSta = 4;
				//HopperSetList[i].HopperState = 2;
			}
			else if( coin1flag & 2 ) 
			{
				//DeviceStatus.ChangeUnit1 = 2;
                *devHopperSta = 2; 
				//HopperSetList[i].HopperState = 2;
			}
			break;
		}			
	}

	//display the counting result		
#ifdef _CHINA_
	//DisplayStr( 0, 0, 1, "清点完毕，请核对!", 17 );
	////DisplayStr( 0, 1, 1, "硬币金额: ", 10 );
#else
	//DisplayStr( 0, 0, 1, "Pls check the result", 20 );
	////DisplayStr( 0, 1, 1, "Coin:", 5 );
#endif
		
    //if( flag == 1 )
	{	
	    /*
		switch( SystemParameter.curUnit )
		{
			case 1:
		#ifdef _CHINA_
				len = sprintf( str, "清点金额: %u", coinMoney );	
		#else
				len = sprintf( str, "Payout: %u", coinMoney );		
		#endif	
			break;				
			case 10:
		#ifdef _CHINA_
				len = sprintf( str, "清点金额: %u.%u", coinMoney/SystemParameter.curUnit, coinMoney%SystemParameter.curUnit );	
		#else
				len = sprintf( str, "Payout: %u.%u", coinMoney/SystemParameter.curUnit, coinMoney%SystemParameter.curUnit );		
		#endif	
			break;
			case 100:
		#ifdef _CHINA_
				len = sprintf( str, "清点金额: %u.%02u", coinMoney/SystemParameter.curUnit, coinMoney%SystemParameter.curUnit );	
		#else
				len = sprintf( str, "Payout: %u.%02u", coinMoney/SystemParameter.curUnit, coinMoney%SystemParameter.curUnit );		
		#endif	
			break;
			default:
				len = 0;
				memset( str, 0, sizeof( str ) );
			break;
		}
		*/
		if( len != 0 )
		{
			//DisplayStr( 0, 1, 1, str, len );
		}
	}
    //
	//while( 1 )
	//{
		//key = GetKey();		
		//if( ( key  == KEY_SUBMIT ) || ( key  == KEY_CANCEL ) )
	    //{
		//	break;
	    //}
    //}
	return 0;

}









