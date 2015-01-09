
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\config.h"
#include "global.h"
#include "evb.h"
#include "uart.h"
#include "hopper.h"
#include "VMC_PC.h"
#include "acdc.h"


#include "Changer.h"//120401 by cq change structor

//#include "uart1.h"//120816 by cq SelectKey
//#include "uart1x.h"//120816 by cq SelectKey

//120816 by cq SelectKey
#define CMDHEAD 		0xED
uint8_t evbAcdcbuf[20];




//120910 by cq ShowLed

#define ID_ACDCMODULE	0x1E
#define ACDC_QUERY		0x51
uint8_t ACDCModuleSN = 0;   //同步序号及编号



//120816 by cq SelectKey
static uint8_t SelectKey_Command_1(EVBCommand SelectKeycom)
{
   uint8_t *pcombuf;
   uint8_t checksum, i, j, k;
   uint8_t  status=0, lenth;

   //Uart1_ClrBuf();
   //Uart1x_ClrBuf();
   
   Uart2_SetEvbMode();
   Uart2_SetParity(PARITY_DIS);
   
   SelectKeycom.Head = CMDHEAD;//0xED;
   SelectKeycom.Lenth = 0x08;//param+2

   pcombuf = (uint8_t *)&SelectKeycom;

   checksum = 0;

   for(i = 0; i < SelectKeycom.Lenth - 1; i++) 
   {
	  checksum ^= pcombuf[i];
   }

   SelectKeycom.chksum = checksum;

   for(i = 0; i < 3; i++) 
   {
   		Trace("\r\nW=%02x",SelectKeycom.Lenth);
      //Uart1_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//Uart1xPutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		Uart2_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);

	  OSTimeDly(OS_TICKS_PER_SEC / 5);

      status = 0;
      checksum = 0;
	 /*
	  if(hoppercom.Com == HOPPER_PAYOUT) 
	  {
	     lenth = Uart2_Read(evbbuf,6);
	  } 
	  else 
	  {
	     lenth = Uart2_Read(evbbuf,8);
	  }
	  */
	 
	 //lenth = Uart1_Read(evbAcdcbuf,8);
	 //lenth = Uart1x_Read(evbAcdcbuf,8);
	 lenth = Uart2_Read(evbAcdcbuf,8);
	 
	 Trace("\r\nL=%02x",lenth);
	 for(k = 0; k < lenth; k++) 	 	
		 Trace(" %02x",evbAcdcbuf[k]);

	  if(lenth > 0) 
	  {		 		 
		 if(evbAcdcbuf[0] != 0xFD) 
		 {
		 	Trace("\r\nERR_STX");
		    status = ERR_STX;
		 } 
		 else 
		 {
			for(j = 0; j < lenth - 1; j++) 
			{
		       checksum ^= evbAcdcbuf[j];
		    }

			if(checksum != evbAcdcbuf[j]) 
			{			
			 	Trace("\r\nERR_CHKSUM");
		       status = ERR_CHKSUM;
			}
			else 
			{
			   break;
		    }
		 }
	  }
	  else 
	  {	  
	  
	  Trace("\r\nERR_TIMEOUT");
         status = ERR_TIMEOUT;
	  }

	  OSTimeDly(OS_TICKS_PER_SEC / 10);
   }

   return status;
}


//1打开，0关闭
uint8_t ACDCModuleQuery(uint8_t ACDCLedCtr, uint8_t ACDCDistCtr)
{
   EVBCommand SelectKeycom;
   uint8_t  status;

   
   //SelectKeycom.Head = CMDHEAD;//0xED;
   //SelectKeycom.Lenth = 0x08;//param+2

   SelectKeycom.SN = ACDCModuleSN;
   SelectKeycom.Com = ACDC_QUERY;//HOPPER_QUERY;
   SelectKeycom.DevNo = ID_ACDCMODULE;

   
   SelectKeycom.rsv1 = (uint8_t)ACDCLedCtr;//sysVPMission.ACDCLedCtr;
   SelectKeycom.rsv2 = (uint8_t)ACDCDistCtr;//sysVPMission.ACDCCompressorCtr;

   status = SelectKey_Command_1(SelectKeycom);

   return status;
}

uint8_t ACDCModuleTest()
{
	while(1)
	{
		Trace("\r\ndo test");
		ACDCModuleQuery(1,1);
		OSTimeDly(OS_TICKS_PER_SEC * 2);
		ACDCModuleQuery(0,0);
		OSTimeDly(OS_TICKS_PER_SEC * 2);
	}

}

