
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "..\config.h"
#include "global.h"
#include "evb.h"
#include "uart.h"
#include "hopper.h"
#include "VMC_PC.h"
#include "Uart1.h"


#include "Changer.h"//120401 by cq change structor

//#include "uart1.h"//120816 by cq SelectKey
//#include "Uart1x.h"//120816 by cq SelectKey
//#include "Uart3x.h"


//120816 by cq SelectKey
#define CMDHEAD 		0xED
#define SELECTKEYNUM			6	 
uint8_t SelectKeySN[SELECTKEYNUM],SelectKeyNo[SELECTKEYNUM];   //同步序号及编号

#define SELECTKEY_QUERY		0x51
#define SELECTKEY_QUERYRUN	0x52//选货按键交易时使用跑马灯;by gzz 20121121


#define ID_SELECTION1	0x16//0xA1
#define ID_SELECTION2	0x17//0xA2
#define ID_SELECTION3	0x18//0xA3
#define ID_SELECTION4	0x19
#define ID_SELECTION5	0x1A
#define ID_SELECTION6	0x1B


uint8_t evbbuf2[20];

#define KEY1_OK 1 << 0
#define KEY2_OK 1 << 1
#define KEY3_OK 1 << 2
#define KEY4_OK 1 << 3
#define KEY5_OK 1 << 4
#define KEY6_OK 1 << 5

#define KEY1_ER 1 << 8
#define KEY2_ER 1 << 9
#define KEY3_ER 1 << 10
#define KEY4_ER 1 << 11
#define KEY5_ER 1 << 12
#define KEY6_ER 1 << 13


//120816 by cq SelectKey
static uint8_t SelectKey_Command(EVBCommand SelectKeycom)
{
   uint8_t *pcombuf;
   uint8_t checksum, i, j, k;
   uint8_t  status=0, lenth;

   //Uart1_ClrBuf();
   //Uart3x_ClrBuf();
   
   //Uart2_SetEvbMode();
   //Uart2_SetParity(PARITY_DIS);
   
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
   		//Trace("\r\nW=%02x",SelectKeycom.Lenth);
      //Uart1_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
      //选货按键;by gzz 20121025
	  //Uart3xPutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
	  //Trace("\r\n begin send");
	  /*
	  Uart1_SendByte(SelectKeycom.Head);
	  Uart1_SendByte(SelectKeycom.Lenth);
	  Uart1_SendByte(SelectKeycom.SN);
	  Uart1_SendByte(SelectKeycom.Com);
	  Uart1_SendByte(SelectKeycom.DevNo);
	  Uart1_SendByte(SelectKeycom.rsv1);
	  Uart1_SendByte(SelectKeycom.rsv2);
	  Uart1_SendByte(SelectKeycom.chksum);
	  */
	  //Uart3_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
	  Uart1_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);

	  //OSTimeDly(OS_TICKS_PER_SEC / 40);
	  OSTimeDly(OS_TICKS_PER_SEC / 100);

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
	 //Trace("\r\n begin read");
	 //lenth = Uart1_Read(evbbuf2,8);
	 //选货按键;by gzz 20121025
	 //lenth = Uart3_Read(evbbuf2,8);
	 lenth = Uart1_Read(evbbuf2,8);
	 //Trace("\r\n end send");
	 
	 //Trace("\r\nL=%02x",lenth);
	 //for(k = 0; k < lenth; k++) 	 	
	//	 Trace(" %02x",evbbuf2[k]);

	  if(lenth > 0) 
	  {		 		 
		 if(evbbuf2[0] != 0xFD) 
		 {
		 	Trace("\r\nERR_STX");
		    status = ERR_STX;
		 } 
		 else 
		 {
			for(j = 0; j < lenth - 1; j++) 
			{
		       checksum ^= evbbuf2[j];
		    }

			if(checksum != evbbuf2[j]) 
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



//120816 by cq SelectKey
//选货按键交易时使用跑马灯;by gzz 20121121
//ledMode:0正常状态;1交易状态
uint8_t SelectKeySet(uint8_t selectkeych, uint8_t ReadyLed, uint8_t ErrLed,uint8_t ledMode)
{
   EVBCommand SelectKeycom;
   uint8_t  status;

   
   //SelectKeycom.Head = CMDHEAD;//0xED;
   //SelectKeycom.Lenth = 0x08;//param+2

   SelectKeycom.SN = SelectKeySN[selectkeych];
   //选货按键交易时使用跑马灯;by gzz 20121121
   if(ledMode==0)
   		SelectKeycom.Com = SELECTKEY_QUERY;//HOPPER_PAYOUT;
   else	if(ledMode==1)
   		SelectKeycom.Com = SELECTKEY_QUERYRUN;//HOPPER_PAYOUT;
   		
   SelectKeycom.DevNo = SelectKeyNo[selectkeych];

   
   SelectKeycom.rsv1 = ReadyLed;//0;//ok//
   SelectKeycom.rsv2 = ErrLed;//0xff;//err//

   status = SelectKey_Command(SelectKeycom);

   return status;
}




/*
uint8_t SelectKeySetled(uint8_t selectkeych, uint16_t amount)
{
   EVBCommand SelectKeycom;
   uint8_t  status;

   
   //SelectKeycom.Head = CMDHEAD;//0xED;
   //SelectKeycom.Lenth = 0x08;//param+2

   SelectKeycom.SN = SelectKeySN[selectkeych];
   SelectKeycom.Com = SELECTKEY_QUERY;//HOPPER_PAYOUT;
   SelectKeycom.DevNo = SelectKeyNo[selectkeych];

   
   SelectKeycom.rsv1 = (uint8_t)amount;//0;//ok//
   SelectKeycom.rsv2 = (uint8_t)(amount >> 8);//0xff;//err//

   status = SelectKey_Command(SelectKeycom);

   return status;
}
*/




//120816 by cq SelectKey
uint8_t GetSelectKey(uint8_t ledMode)
{
    uint8_t      pStatus,i,j;
	uint8_t key=KEY_NULL,loopOnce=0;
	uint16_t      a,b;

	uint16_t led;

	//while(1)
	{
		for(j = 0; j < 6; j++)
		{		
			/*
			//led = 0x3f3f;
			//led = (sysVPMission.sel2ErrLed<<8)|(sysVPMission.sel2ReadyLed);
			//led = sysVPMission.sel2ErrLed;
			//led = (led<<8)|sysVPMission.sel2ReadyLed;
			led = 0x3f3f;
			pStatus = SelectKeySetled(j, led);
			*/
			
			switch(j)
			{
				case 0://选货按键交易时使用跑马灯;by gzz 20121121
					pStatus = SelectKeySet(j, sysVPMission.sel1ReadyLed,sysVPMission.sel1ErrLed,ledMode); 
					break;
				case 1:
					pStatus = SelectKeySet(j, sysVPMission.sel2ReadyLed,sysVPMission.sel2ErrLed,ledMode);
					break;
				case 2:
					pStatus = SelectKeySet(j, sysVPMission.sel3ReadyLed,sysVPMission.sel3ErrLed,ledMode);
					break;
				case 3:
					pStatus = SelectKeySet(j, sysVPMission.sel4ReadyLed,sysVPMission.sel4ErrLed,ledMode);
					break;
				case 4:
					pStatus = SelectKeySet(j, sysVPMission.sel5ReadyLed,sysVPMission.sel5ErrLed,ledMode);
					break;
				case 5:
					pStatus = SelectKeySet(j, sysVPMission.sel6ReadyLed,sysVPMission.sel6ErrLed,ledMode);
					break;	
			}
			
			
			
			
			//无按键时返回0xfd 0x8 00 00 0x16 0xff 00 0x1c,其中0x16代表选货板的设备号,下一个字节代表按键号,如果没有按下，返回0xff,有按下，返回相应哪个按钮按下的
			if(pStatus == 0)
			{
				key = evbbuf2[5];
				
				if( key != 0xff )
				{
					key = key+6*j+1;
				
					Trace("    key = %u",key);

					
					for (b = 100; b > 0; b--) 
					{
	    				for (a = 0; a < 9000; a++);
							{BEEPON();}
						for (a = 0; a < 9000; a++);
							{BEEPOFF();}
					}	//蜂鸣器叫 chen
					
					
					return key;
					
					//Beep();
				}
				
			}
		}

		return key;
	}

}

unsigned char UpdateSelectionLed( void )
{
	uint8_t i = 0;
	unsigned char x = 0;
	unsigned char y = 0;
	uint8_t   channel,ChannelX,ChannelY;

    

	if(g_Amount > 0)
	{
		//sysVPMission.sel1ErrLed = 0;
	    sysVPMission.sel1ReadyLed = 0;
		//sysVPMission.sel2ErrLed = 0;
	    sysVPMission.sel2ReadyLed = 0;
		//sysVPMission.sel3ErrLed = 0;
	    sysVPMission.sel3ReadyLed = 0;
		//sysVPMission.sel4ErrLed = 0;
	    sysVPMission.sel4ReadyLed = 0;
		//sysVPMission.sel5ErrLed = 0;
	    sysVPMission.sel5ReadyLed = 0;
		//sysVPMission.sel6ErrLed = 0;
	    sysVPMission.sel6ReadyLed = 0;
		for( i=1; i<=KEYEXTRAVAL; i++ )
	    {
	    
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{	if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel1ReadyLed |= 1<<(i-1);							
						}  
					} 
				}  		
			}
			//sysVPMission.sel1ReadyLed |= 1<<i;
	    }
	    for( i=KEYEXTRAVAL+1; i<=KEYEXTRAVAL*2; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL-1);						
						}  
					} 
				}  		
			}		    	
	        //sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	    }
		for( i=KEYEXTRAVAL*2+1; i<=KEYEXTRAVAL*3; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*3+1; i<=KEYEXTRAVAL*4; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel4ReadyLed |= 1<<(i-KEYEXTRAVAL*3-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*4+1; i<=KEYEXTRAVAL*5; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel5ReadyLed |= 1<<(i-KEYEXTRAVAL*4-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*5+1; i<=KEYEXTRAVAL*6; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel6ReadyLed |= 1<<(i-KEYEXTRAVAL*5-1);						
						} 
					} 
				}  		
			}				
	    }
		
		sysVPMission.selectMode = 1;//选货按键交易时使用跑马灯;by gzz 20121121
	}	
	else
	{		
		sysVPMission.sel1ErrLed = 0xff;
	    sysVPMission.sel1ReadyLed = 0;
		sysVPMission.sel2ErrLed = 0xff;
	    sysVPMission.sel2ReadyLed = 0;
		sysVPMission.sel3ErrLed = 0xff;
	    sysVPMission.sel3ReadyLed = 0;
		sysVPMission.sel4ErrLed = 0xff;
	    sysVPMission.sel4ReadyLed = 0;
		sysVPMission.sel5ErrLed = 0xff;
	    sysVPMission.sel5ReadyLed = 0;
		sysVPMission.sel6ErrLed = 0xff;
	    sysVPMission.sel6ReadyLed = 0;
	    
		
		for( i=1; i<=KEYEXTRAVAL; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel1ReadyLed |= 1<<(i-1);
							sysVPMission.sel1ErrLed &= ~(1<<(i-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel1ErrLed |= 1<<i;
	    }
	    for( i=KEYEXTRAVAL+1; i<=KEYEXTRAVAL*2; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL-1);
							sysVPMission.sel2ErrLed &= ~(1<<(i-KEYEXTRAVAL-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	    }
		for( i=KEYEXTRAVAL*2+1; i<=KEYEXTRAVAL*3; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2-1);
							sysVPMission.sel3ErrLed &= ~(1<<(i-KEYEXTRAVAL*2-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*3+1; i<=KEYEXTRAVAL*4; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel4ReadyLed |= 1<<(i-KEYEXTRAVAL*3-1);
							sysVPMission.sel4ErrLed &= ~(1<<(i-KEYEXTRAVAL*3-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*4+1; i<=KEYEXTRAVAL*5; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel5ReadyLed |= 1<<(i-KEYEXTRAVAL*4-1);
							sysVPMission.sel5ErrLed &= ~(1<<(i-KEYEXTRAVAL*4-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*5+1; i<=KEYEXTRAVAL*6; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel6ReadyLed |= 1<<(i-KEYEXTRAVAL*5-1);
							sysVPMission.sel6ErrLed &= ~(1<<(i-KEYEXTRAVAL*5-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
	    
		sysVPMission.selectMode = 0;//选货按键交易时使用跑马灯;by gzz 20121121
	}	

    return VP_ERR_NULL;
}


//mode=1,打开所有灯,mode=0,关闭所有灯,
unsigned char UpdateSelectionLedTest( uint8_t mode )
{
	if(mode == 1)
	{
		sysVPMission.sel1ErrLed = 0xff;
	    sysVPMission.sel1ReadyLed = 0xff;
		sysVPMission.sel2ErrLed = 0xff;
	    sysVPMission.sel2ReadyLed = 0xff;
		sysVPMission.sel3ErrLed = 0xff;
	    sysVPMission.sel3ReadyLed = 0xff;
		sysVPMission.sel4ErrLed = 0xff;
	    sysVPMission.sel4ReadyLed = 0xff;
		sysVPMission.sel5ErrLed = 0xff;
	    sysVPMission.sel5ReadyLed = 0xff;
		sysVPMission.sel6ErrLed = 0xff;
	    sysVPMission.sel6ReadyLed = 0xff;
	}	
	else if(mode == 0)
	{
		sysVPMission.sel1ErrLed = 0;
	    sysVPMission.sel1ReadyLed = 0;
		sysVPMission.sel2ErrLed = 0;
	    sysVPMission.sel2ReadyLed = 0;
		sysVPMission.sel3ErrLed = 0;
	    sysVPMission.sel3ReadyLed = 0;
		sysVPMission.sel4ErrLed = 0;
	    sysVPMission.sel4ReadyLed = 0;
		sysVPMission.sel5ErrLed = 0;
	    sysVPMission.sel5ReadyLed = 0;
		sysVPMission.sel6ErrLed = 0;
	    sysVPMission.sel6ReadyLed = 0;
	}
}		




void SelectKey_InitProc(void)
{
    //uint8_t      pStatus,i,j;
	//uint8_t key=KEY_NULL,loopOnce=0;

	//uint16_t led;
	
	SelectKeyNo[0] = ID_SELECTION1;
	SelectKeyNo[1] = ID_SELECTION2;
	SelectKeyNo[2] = ID_SELECTION3;
	SelectKeyNo[3] = ID_SELECTION4;
	SelectKeyNo[4] = ID_SELECTION5;
	SelectKeyNo[5] = ID_SELECTION6;

	//while(1)
		//GetSelectKey();

}

