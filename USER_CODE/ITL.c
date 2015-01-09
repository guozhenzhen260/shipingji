/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           ITL.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        ITL纸币器驱动程序
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
#include "BillAcceptor.h"
#include "itl.h"

//#define  ITL_DEBUG



unsigned char ITLMsgBuf[255];  //255

//SERAIL1PARAM  code ITLSerialParam =
//{
//	8, VERIFY_NULL, 1, 0x00, 0x96, 0x00
//};

struct ITL_Mission sysITLMission;


//const unsigned int code CrcTable[8*32]=
//crc校验表
const unsigned int CrcTable[8*32]=
{
   /*
   0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,
   0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
   0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,
   0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
   0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,
   0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
   0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,
   0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
   0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,
   0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
   0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,
   0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
   0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,
   0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
   0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,
   0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
   0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,
   0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
   0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,
   0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
   0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,
   0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
   0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,
   0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
   0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,
   0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
   0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,
   0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
   0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,
   0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
   0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,
   0x8213,0x0216,0x021C,0x8219,0x0280,0x820D,0x8207,0x0202,
   */
   0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,
   0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
   0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,
   0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
   0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,
   0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
   0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,
   0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
   0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,
   0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
   0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,
   0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
   0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,
   0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
   0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,
   0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
   0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,
   0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
   0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,
   0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
   0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,
   0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
   0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,
   0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
   0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,
   0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
   0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,
   0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
   0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,
   0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
   0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,
   0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
 };



//-----------------------------------------------------------
//更新crc校验值
void ITLUpdateCrc( const unsigned char num )
{
    unsigned char addr = 0;
    addr = ( num ^ sysITLMission.crcH );
    sysITLMission.crcH = (unsigned char)(CrcTable[addr]>>8)^sysITLMission.crcL;
    sysITLMission.crcL = (unsigned char)(CrcTable[addr] & 0x00FF );
}

//-----------------------------------------------------------
//重置crc校验值
void ITLResetCrc( void )
{
    sysITLMission.crcL = 0xFF;
    sysITLMission.crcH = 0xFF;
}

//使能纸币器通道
/* 
void ITLChnCtr()
{
    sysITLMission.chnCtr1 = 0;
	sysITLMission.chnCtr2 = 0;
    
    if( SystemParameter.billValue[0] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x01;
	}
	if( SystemParameter.billValue[1] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x02;
	}
	if( SystemParameter.billValue[2] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x04;
	}
	if( SystemParameter.billValue[3] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x08;
	}
	if( SystemParameter.billValue[4] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x10;
	}
	if( SystemParameter.billValue[5] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x20;
	}
	if( SystemParameter.billValue[6] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x40;
	}
	if( SystemParameter.billValue[7] != 0   )
	{
		sysITLMission.chnCtr1 |= 0x80;
	}
    //---------------------------------------------------------
	#ifdef ITL_DEBUG
    	sysITLMission.chnCtr1 = 0xff;
		sysITLMission.chnCtr2 = 0xff;
	#endif
    //sysITLMission.chnCtr1 = 0xff;
	//sysITLMission.chnCtr2 = 0xff;
    //=========================================================

}
*/ 
//纸币器通道控制
void ITLChnCtr(uint16_t billtype)
{
    sysITLMission.chnCtr1 = 0;
	sysITLMission.chnCtr2 = 0;
	sysITLMission.chnCtr1 = billtype;
	sysITLMission.chnCtr2 = billtype;
}


/*
void ITLSerialInit( void )
{	
	unsigned char i=0;
    
	//DelayMs(1);
	ZhkHardSerialSwitch( 0 );
	//DelayMs(1);
	for( i=0; i<sizeof( SERAIL1PARAM ); i ++ )
	{
		ZhkSerial1Param[ i ] = ITLSerialParam[ i ];
	}
	ZhkSerial1Init();
    memset( ITLMsgBuf, 0, sizeof(ITLMsgBuf) );   
}
*/

	  
//Create the CHK and send the message
unsigned char ITLBusTxMsg( void )
{
	unsigned char i = 0;

	Uart2_ClrBuf();
    Uart2_SetEvbMode();
    Uart2_SetParity(PARITY_DIS);

	//1.Create the CHK
    ITLResetCrc();
    ITLUpdateCrc( sysITLMission.send.seq );   //
	ITLUpdateCrc( sysITLMission.send.len );	
	for( i=0; i<sysITLMission.send.len; i++ )
	{
    	ITLUpdateCrc( sysITLMission.send.msg[i] );
	}
    //--------------------------------------------
    #ifdef ITL_DEBUG
    	if( (sysITLMission.send.msg[0]==ITL_CMD_SETINHIBITS)&&(sysITLMission.send.seq==0x80) )
        {
        	sysITLMission.crcL = 0x25;
            sysITLMission.crcH = 0xa4;	
        }
        else if( (sysITLMission.send.msg[0]==ITL_CMD_ENABLE)&&(sysITLMission.send.seq==0x00) )
        {
            sysITLMission.crcL = 0x3C;
            sysITLMission.crcH = 0x08;
        }
        else if( (sysITLMission.send.msg[0]==ITL_CMD_POLL)&&(sysITLMission.send.seq==0x00) )
        {
            sysITLMission.crcL = 0x11;
            sysITLMission.crcH = 0x88;
        }
        else if( (sysITLMission.send.msg[0]==ITL_CMD_POLL)&&(sysITLMission.send.seq==0x80) )
        {
            sysITLMission.crcL = 0x12;
            sysITLMission.crcH = 0x02;
        }
    #endif
    //============================================
	sysITLMission.send.crcL = sysITLMission.crcL;
	sysITLMission.send.crcH = sysITLMission.crcH;
	//2.Send the message
    Uart2_SendByte( sysITLMission.send.stx );
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	Uart2_SendByte( sysITLMission.send.seq );
    OSTimeDly(OS_TICKS_PER_SEC / 10);
    Uart2_SendByte( sysITLMission.send.len );
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	if( sysITLMission.send.len == ITL_MSG_STX )
	{
		Uart2_SendByte( sysITLMission.send.len );
		OSTimeDly(OS_TICKS_PER_SEC / 10);
	}

	for( i=0; i<sysITLMission.send.len; i++ )
	{
		Uart2_SendByte( sysITLMission.send.msg[i] );
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		if( sysITLMission.send.msg[i] == ITL_MSG_STX )
		{
			Uart2_SendByte( sysITLMission.send.msg[i] );
			OSTimeDly(OS_TICKS_PER_SEC / 10);
		}
	}

	Uart2_SendByte( sysITLMission.send.crcL );
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	if( sysITLMission.send.crcL == ITL_MSG_STX )
	{
	    Uart2_SendByte( sysITLMission.send.crcL );
		OSTimeDly(OS_TICKS_PER_SEC / 10);
	}
    
	Uart2_SendByte( sysITLMission.send.crcH );
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	if( sysITLMission.send.crcH == ITL_MSG_STX )
	{
	    Uart2_SendByte( sysITLMission.send.crcH );
		OSTimeDly(OS_TICKS_PER_SEC / 10);
	}
    
    return 1;
}

//msgType, seq 
//发送数据包
unsigned char ITLMsgPackSend( unsigned char msgType, unsigned char flag )
{

    //unsigned char i = 0;

    if( !((msgType>=ITL_CMD_MIN)&&(msgType<=ITL_CMD_MAX)) )
		return ITL_ERR_PAR;

    if((flag!=0)&&(flag!=1)) 
	    return ITL_ERR_PAR;

	sysITLMission.send.stx = ITL_MSG_STX;
	if( flag == 1 )
	{
		if( sysITLMission.send.seq == ITL_MSG_SEQ_0 )
		    sysITLMission.send.seq = ITL_MSG_SEQ_1;
		else 
		    sysITLMission.send.seq = ITL_MSG_SEQ_0;
	}

    switch( msgType )
	{
	    case ITL_CMD_RESET:
			{
				sysITLMission.send.len = 1;
				sysITLMission.send.msg[0] = ITL_CMD_RESET;
			}
			break;
		case ITL_CMD_SETINHIBITS:
		    {
			    sysITLMission.send.len = 3;
				sysITLMission.send.msg[0] = ITL_CMD_SETINHIBITS;
				sysITLMission.send.msg[1] = sysITLMission.chnCtr1;
				sysITLMission.send.msg[2] = sysITLMission.chnCtr2;
			}
		    break;
        case ITL_CMD_DSPON:
		    {
			    sysITLMission.send.len = 1;
				sysITLMission.send.msg[0] = ITL_CMD_DSPON;
			}
		    break;
		case ITL_CMD_DSPOFF:
		    {
			    sysITLMission.send.len = 1;
				sysITLMission.send.msg[0] = ITL_CMD_DSPOFF;
			}
		    break;
		case ITL_CMD_SETUP:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_SETUP;
			}
		    break;
		case ITL_CMD_POLL:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_POLL;
			}
		    break;
		case ITL_CMD_REJECT:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_REJECT;
			}
		    break;
		case ITL_CMD_DISABLE:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_DISABLE;
			}
		    break;
        case ITL_CMD_ENABLE:
		    {
				 sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_ENABLE;
			}
		    break;
		case ITL_CMD_SYN:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_SYN;
			}
		    break;
        case ITL_CMD_HOLD:
		    {
			     sysITLMission.send.len = 1;
				 sysITLMission.send.msg[0] = ITL_CMD_HOLD;
			}
		    break;
		default: break;
	}
    ITLBusTxMsg();
    OSTimeDly(OS_TICKS_PER_SEC / 10);
    return ITL_ERR_NULL;
}


unsigned char ITLBusUnpack( void )
{
	uint8_t  i=0, j=0, k=0;
	uint8_t  len = 0;
	uint8_t  seq = 0;
	volatile uint8_t  position = 0;
	uint8_t  newMsg[255];
	uint8_t	 length=0;
		
	//return 1;
    //-------------------------------------------------
	/*
    #ifdef ITL_DEBUG
        return 1;
    #endif
	*/
    //=================================================

	//while( !ZhkSerial1IsRxBufNull() )
	//{
	//	for( i=0; i<sizeof( ITLMsgBuf )-1; i++ )
	//	{
	//		ITLMsgBuf[i] = ITLMsgBuf[i+1];
	//	}
	//	ITLMsgBuf[sizeof(ITLMsgBuf)-1] = ZhkSerial1GetCh();
	//}
	length = Uart2_Read(ITLMsgBuf,20);

    if(length > 0)
	{ 
		for( i=0; i<=sizeof(ITLMsgBuf)-6; i++ )
		{
		    //Check the SF
			if( ITLMsgBuf[i] != ITL_MSG_STX ) 
				continue;
			//Check the len
			seq = ITLMsgBuf[i+1];
			if( !((seq==ITL_MSG_SEQ_0)||(seq==ITL_MSG_SEQ_1)) ) 
			    continue;
			len = ITLMsgBuf[i+2];
			if( i+len+5 > sizeof( ITLMsgBuf ) ) 
			    break;
	
	        //-------------------------------------------------
			/*
			#ifdef ITL_DEBUG
	            return 1;
			#endif
			*/
	        //=================================================
	
		    //Delete the extral "0x7F"
			memset( newMsg, 0, sizeof(newMsg) );
	        position = i;
			k = 0;
			newMsg[k++] = ITLMsgBuf[i];
			newMsg[k++] = ITLMsgBuf[i+1];
	        for( j=i+2; j<sizeof(ITLMsgBuf); j++)
			{
	        	newMsg[k++] = ITLMsgBuf[j];
				if( ITLMsgBuf[j+1] == ITL_MSG_STX )
				{
					j += 1;
				}
			}
			//Check the CHK
			ITLResetCrc();
	      //ITLUpdateCrc( newMsg[0] );
	        ITLUpdateCrc( newMsg[1] );   //
		    ITLUpdateCrc( newMsg[2] );   //	
		    for( j=3; j<len+3; j++ )
		    {
	    		ITLUpdateCrc( newMsg[j] );
		    }
			if( (sysITLMission.crcL != newMsg[j]) || (sysITLMission.crcH != newMsg[j+1]) )
				continue;
			//Check the message type
	        if( !( (newMsg[2]>=1)&&((newMsg[3]>=ITL_ACK_MIN)&&(newMsg[3]<=ITL_ACK_MAX)) ) )   
	            continue;     
			//Save the message
			sysITLMission.receive.stx = newMsg[0];
			sysITLMission.receive.seq = newMsg[1];
			sysITLMission.receive.len = newMsg[2];
			for( k=0,j=3; j<sysITLMission.receive.len+3; k++,j++ )
			{
				sysITLMission.receive.msg[k] = newMsg[j];	
			}
	        sysITLMission.receive.crcL = newMsg[j];
			sysITLMission.receive.crcH = newMsg[j+1];
			memset( ITLMsgBuf, 0, sizeof(ITLMsgBuf) );
			return 1;
		}
	}	
	return 0;
			
}


unsigned char ITLMission_Syn( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;
    
	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Syn_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_SYN, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Syn_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Syn_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Syn_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}


unsigned char ITLMission_Setup( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;
    
	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Setup_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_SETUP, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Setup_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Setup_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Setup_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;
	

}

unsigned char ITLMission_SetInhibit(uint16_t billtype)
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;
  
	//1.
    //ITLSerialInit();
	//2.
    ITLChnCtr(billtype);
    retry = ITL_COM_RETRY;
TAB_ITLM_SetInhibit_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_SETINHIBITS, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_SetInhibit_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_SetInhibit_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_SetInhibit_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}


unsigned char ITLMission_Reset( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Reset_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_RESET, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Reset_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Reset_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Reset_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}

unsigned char ITLMission_Dspon( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Dspon_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_DSPON, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Dspon_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Dspon_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Dspon_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}

unsigned char ITLMission_Dspoff( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Dspoff_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_DSPOFF, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Dspoff_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Dspoff_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Dspoff_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}

unsigned char ITLMission_Hold( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Hold_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_HOLD, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Hold_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Hold_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Hold_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}

//111223 by cq ITL纸币器
unsigned char ITLMission_Enable(uint16_t billtype)
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;
  
	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Enable_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_ENABLE, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Enable_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Enable_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Enable_COM_OK:
    sysITLMission.enableFlag = 1;
	sysITLMission.billESta = sysITLMission.receive.msg[0];
    //DeviceStatus.BillAccepter=0;//by gzz 20120215  
    return ITL_ERR_NULL;

}


unsigned char ITLMission_Disable( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

    
	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Disable_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_DISABLE, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Disable_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Disable_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Disable_COM_OK:
    sysITLMission.enableFlag = 0;
	sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;
}

unsigned char ITLMission_Reject( void )
{    
    unsigned char retry = 0;
	//unsigned char i = 0;
	unsigned char flag = 0;

	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_Reject_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_REJECT, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_Reject_COM_OK;
		}
	}
	if( retry-- )
	{
		goto TAB_ITLM_Reject_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
        return ITL_ERR_COM;
	}
TAB_ITLM_Reject_COM_OK:
    sysITLMission.billESta = sysITLMission.receive.msg[0];
    return ITL_ERR_NULL;

}


//轮询纸币器
unsigned char ITLMission_Poll( void )
{   
    unsigned char retry = 0;
	unsigned char i = 0;
	unsigned char flag = 0;
    
	//1.
    //ITLSerialInit();
	//2.
    retry = ITL_COM_RETRY;
TAB_ITLM_P_RETRY:
	flag = ITLMsgPackSend( ITL_CMD_POLL, 1);
    if( flag != ITL_ERR_NULL )
    {
		return ITL_ERR_PAR;
	}
	sysITLMission.msTimer2 = ITL_TIME_OUT;
	while( sysITLMission.msTimer2 )   //sysVPMission.msTimer2
	{
		if( ITLBusUnpack() )
		{
		    sysITLMission.comErrNum = 0;
			goto TAB_ITLM_P_COM_OK;
		}
		
		//111223 by cq ITL纸币器
        //------------------------------------------------------------------
        //if( IfGameKeyOn() )
		//{
			//VPMission_Button_RPT( VP_BUT_GAME, VP_BUT_NUMBER );
	        //Beep();
		//}
        //===================================================================
	}

	if( retry-- )
	{
		goto TAB_ITLM_P_RETRY;
	}
	else
	{
		sysITLMission.comErrNum++;
		if( sysITLMission.comErrNum >= ITL_TIME_OUT_NUM )
		{
			//DeviceStatus.BillAccepter |= 0x01;
		}
        return ITL_ERR_COM;
	}
TAB_ITLM_P_COM_OK:
    //Check the the bill status
	sysITLMission.billSta  = ITL_BILL_NULL;
    sysITLMission.billESta = ITL_BILL_NULL;
	sysITLMission.status   = ITL_DEV_NULL;
    for( i=0; i<sysITLMission.receive.len; i++ )
	{
		if( sysITLMission.receive.msg[i] == ITL_ACK_READ )
		{
			if( (sysITLMission.receive.msg[i+1]>=1)&&(sysITLMission.receive.msg[i+1]<=8) )
			{
				sysITLMission.billSta  |= ITL_BILL_READ;
				sysITLMission.billChn   = sysITLMission.receive.msg[i+1];
				//sysITLMission.billValue = SystemParameter.billValue[sysITLMission.billChn-1];
			}
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_STACKING )
		{
			sysITLMission.billSta |= ITL_BILL_STACKING;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_CREDIT )
		{
			if( (sysITLMission.receive.msg[i+1]>=1)&&(sysITLMission.receive.msg[i+1]<=8) )
			{
				sysITLMission.billSta  |= ITL_BILL_CREDIT;
				sysITLMission.billChn   = sysITLMission.receive.msg[i+1];
				//sysITLMission.billValue = SystemParameter.billValue[sysITLMission.billChn-1];
			}
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_STACKED )
		{
			sysITLMission.billSta  |= ITL_BILL_STACKED;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_REJECTING )
		{
			sysITLMission.billSta  |= ITL_BILL_REJECTING;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_REJECTED )
		{
			sysITLMission.billSta  |= ITL_BILL_REJECTED;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_SJAM )
		{
			sysITLMission.billSta  |= ITL_BILL_SJAM;   //ERR
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_UJAM )
		{
			sysITLMission.billSta  |= ITL_BILL_UJAM;  //ERR
		}
		//
		else if( sysITLMission.receive.msg[i] == ITL_ACK_RESET )
		{
		    sysITLMission.status |= ITL_DEV_RESET;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_DISABLED )
		{
		    sysITLMission.status |= ITL_DEV_DISABLED;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_NCFFAR )
		{
		    sysITLMission.status |= ITL_DEV_NCFFAR;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_NCICBAR )
		{
		    sysITLMission.status |= ITL_DEV_NCICBAR;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_CBREM )
		{
		    sysITLMission.status |= ITL_DEV_CBREM;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_CBREP )
		{
		    sysITLMission.status |= ITL_DEV_CBREP;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_STAFULL )
		{
		    sysITLMission.status |= ITL_DEV_STAFULL;
		}
		else if( sysITLMission.receive.msg[i] == ITL_ACK_FATTN )
		{
		    sysITLMission.status |= ITL_DEV_FATTN;
		}
        else if( sysITLMission.receive.msg[i] == ITL_ACK_FAIL )
		{
		    sysITLMission.status |= ITL_DEV_FAIL;
		}
        //Andy added 2011.4.15.
        else if( sysITLMission.receive.msg[i] == ITL_ACK_CMDAPL )
        {
        	sysITLMission.billESta = ITL_ACK_CMDAPL;
        }
        else if( sysITLMission.receive.msg[i] == ITL_ACK_OK )
        {
        	sysITLMission.billESta = ITL_ACK_OK;
        }

	} 
    return ITL_ERR_NULL;
}


/*****************************************************************************
** Function name:	Gbahr1_PackAnalysis	
**
** Descriptions:	是POLL轮询之后，ITL纸币器接收的状态数据包分析
**						
**
** parameters:		buf:状态数据缓存，len:状态数据长度
** Returned value:	无	
** 
*****************************************************************************/
void ITLNote_PackAnalysis(uint8_t *buf, uint8_t len)
{
	 //uint16_t status = HR_LCBPRESENT_BIT;
	 uint8_t moneych;

	 moneych = sysITLMission.billChn;
	 //收到纸币状态	 stacked	
	 if( ( ( sysITLMission.billSta & ITL_BILL_STACKED )||( sysITLMission.billSta & ITL_BILL_CREDIT ) )&&( sysITLMission.billStaCtr & ITL_BILL_READ ))    		     
	 {
		 BillAcceptor_ClrInValid();
		 moneych -= 1;
		 BillAcceptor_UpdateAcpt(BILLACCEPTOR_STACKED, moneych);	
	 } 
	 else if( sysITLMission.billSta & ITL_BILL_REJECTED ) 	     //退币状态	 returned	
	 {
		 BillAcceptor_ClrInValid();
		 moneych -= 1;
		 BillAcceptor_UpdateAcpt(BILLACCEPTOR_RETURNED, moneych);
	 } 
		 
	 //if(status != HR_LCBPRESENT_BIT) 
	 //{
	//	BillAcceptor_UpDateStat(status);
	 //}	      	  
}


unsigned char ITLMission_Init_1(uint16_t billtype)
{
    //ITLMission_Reset();
    //DelayMs( ITL_TIME_CMD_INTEVEL );          
    //ITLMission_Syn();
    //DelayMs( ITL_TIME_CMD_INTEVEL );
    ITLMission_Disable();
    OSTimeDly(OS_TICKS_PER_SEC / 50);
    ITLMission_SetInhibit(billtype);
    OSTimeDly(OS_TICKS_PER_SEC / 50);
    ITLMission_Dspon();
    OSTimeDly(OS_TICKS_PER_SEC / 50);
	ITLMission_Enable(billtype);
    OSTimeDly(OS_TICKS_PER_SEC / 50);
    return ITL_ERR_NULL;

	//MDBMission_Bill_Init();
    //return ITL_ERR_NULL;
}

unsigned char ITLMission_Init(uint16_t billtype)
{
    ITLMission_Reset();
    OSTimeDly(OS_TICKS_PER_SEC / 10);         //30000,10000,4000,3000
    //ITLMission_Syn();
    //DelayMs( ITL_TIME_CMD_INTEVEL );
    ITLMission_SetInhibit(billtype);
    OSTimeDly(OS_TICKS_PER_SEC / 10);
    ITLMission_Dspon();
    OSTimeDly(OS_TICKS_PER_SEC / 10);
	ITLMission_Enable(billtype);
    OSTimeDly(OS_TICKS_PER_SEC / 10);
    return ITL_ERR_NULL;

	//MDBMission_Bill_Init();
    //return ITL_ERR_NULL;
}

//暂存的纸币压钞
unsigned char ITLMission_HB_Stack( void )
{
    volatile unsigned char flag = 0;

	if( sysITLMission.billHoldingFlag == 1 )
	{
	    flag = ITLMission_Poll(); 
        sysITLMission.msPoll = ITL_TIME_POLL;
		sysITLMission.sTimer2 = ITL_TIME_STACK;
		while( sysITLMission.sTimer2 )
		{

		    if( sysITLMission.billSta & ITL_BILL_STACKED )
			{
				sysITLMission.billStaCtr |= ITL_BILL_STACKED;
                sysITLMission.billSta &= ~ITL_BILL_STACKED;
			}

            if( sysITLMission.billSta & ITL_BILL_CREDIT )
			{
				sysITLMission.billStaCtr |= ITL_BILL_CREDIT;
				sysITLMission.billSta &= ~ITL_BILL_CREDIT;
			}

		    //if( ( sysITLMission.billSta & ITL_BILL_STACKED ) )
 		    //if( ( sysITLMission.billSta & ITL_BILL_CREDIT )&&( sysITLMission.billStaCtr&ITL_BILL_READ ) )
			//if( ( sysITLMission.billSta & ITL_BILL_CREDIT )&&( sysITLMission.billSta&ITL_BILL_STACKED )&&( sysITLMission.billStaCtr&ITL_BILL_READ ) )
			if( ( sysITLMission.billStaCtr&ITL_BILL_CREDIT )&&( sysITLMission.billStaCtr&ITL_BILL_STACKED )&&( sysITLMission.billStaCtr&ITL_BILL_READ ) )
         	{    
			    sysITLMission.billStaCtr &= ~ITL_BILL_READ;
			    sysITLMission.billStaCtr &= ~ITL_BILL_CREDIT;
                sysITLMission.billStaCtr &= ~ITL_BILL_STACKED;
				break;
			}
            else if( sysITLMission.msPoll == 0 )
			{
				flag = ITLMission_Poll(); 
        		sysITLMission.msPoll = ITL_TIME_POLL;
			}
		}
        //Stack fail
		if( sysITLMission.sTimer2 == 0 )
		{
			return 1;
		}
	}
	return ITL_ERR_NULL;
}


//暂存的纸币退钞
unsigned char ITLMission_HB_Reject( void )
{
    volatile unsigned char flag = 0;
    if( sysITLMission.billHoldingFlag == 1 )
	{
    	//if( SystemParameter.RefundPermission==1 )
		{
			flag = ITLMission_Reject();
			sysITLMission.msPoll = ITL_TIME_POLL;
			sysITLMission.sTimer2 = ITL_TIME_REJECT;
			while( sysITLMission.sTimer2 )
			{
				if( ( sysITLMission.billSta & ITL_BILL_REJECTED ))
				{
				    sysITLMission.billSta &= ~ITL_BILL_REJECTED;
					//...
					break;
				}
	            else if( sysITLMission.msPoll == 0 )
				{
					flag = ITLMission_Reject(); 
	        		sysITLMission.msPoll = ITL_TIME_POLL;
				}

			}
			//
			if( sysITLMission.sTimer2 == 0 )
			{
				return 1;
			}
		}
	}
	return ITL_ERR_NULL;
}

/*更新钞箱状态*/
void ITL_UpDateStackerStat(void)
{
}


/*****************************************************************************
** Function name:	ITL_ErrJudge	
**
** Descriptions:	判断MDB当前接收数据的状态，有没有错误
**						
**
** parameters:		x:MDB命令返回的状态值
** Returned value:	出错返回TRUE；否则返回FALSE	
** 
*****************************************************************************/
uint8_t ITL_ErrJudge(uint8_t x)
{
  if(sysITLMission.comErrNum > 0) 
  {
     return TRUE;
  }
  return FALSE;
} 




