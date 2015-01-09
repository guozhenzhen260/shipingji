/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               
** Latest modified Date:    2010-04-23
** Latest Version:          1.00
** Descriptions:            通过flash保存的读取交易记录等参数，用户编写的环境接口函数,必须在特权模式运行
**                          注意:通常是通过页读写的，这里面得页是由芯片自己定义的,通过spi总线读写             
**
**--------------------------------------------------------------------------------------------------------
** Created by:              yqh
** Created date:            
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
 
#include "global.h"
//#include "..\mid\AT45DB.h"
#include "..\driver\AT45DB.h"//120401 by cq change structor
#include "function.h"
#include "Menu.h"
#include "dispmenu.h"

/**************************AT45DB161D芯片介绍及实际应用说明**********************************************/
/*
一，AT45DB161D芯片介绍
	1.1 AT45DB161D为SPI接口的Nor型Flash
	1.2 4096页，每页512/528字节，总容量为2M字节
	1.3 可按扇区、快、页擦除
二，实际应用
	2.1 设备参数  第7页
	2.2 总的交易金额记录	 第8页
	2.3 系统参数  第9页
    2.4 货道参数  10--20页
    2.5 货道添存货量参数 第21页
    2.6 新的系统参数 第70页
    2.7 用户菜单可见 第50页
	2.8 交易记录  从500页-4000	
*/
/**************************************End Of File*******************************************************/

//保存和读取货道参数		10--20页
 void XChannels(uint8_t mode)
{	
//	uint8_t *tp1,*tp2;
    uint8_t *tp2;
	uint16_t	i = 0;
	int16_t	temp,Pages;
	
//	tp1 = sSysData;
	tp2 = (uint8_t *) &Channels[0];
	temp = (sizeof(GoodsChannel))* MAXCHANNEL*MAXLAYER;
	 /*  for( i= 0; i<temp;i++)	{
	   		if (mode == READ)
	   			*tp2 = *tp1;
	   		else
	   			*tp1 = *tp2;
	   		tp1++;
	   		tp2++;
	   	}
	*/
	
	Pages = (temp / PAGESIZE)	+1;

	
	if (mode == WRITE)	
	{
		 for(i = 0;i<Pages ;i++)
		 {
		 	AT45DB_WritePage(10+i, tp2);//每页每页的写数据
		 	tp2+=PAGESIZE;
		 }
	}	    		
	else
	{
		while( temp>0)
		{
			if(temp >  PAGESIZE)
				AT45DB_ReadByte(10+i, 0, PAGESIZE, tp2);//每页每页的读数据
			else
				AT45DB_ReadByte(10+i, 0, temp, tp2);
			
			i++;
			temp -=  PAGESIZE;
			tp2 +=	PAGESIZE;
		}
 	}					  

	 
}

//保存和读取系统参数,这个系统参数在交易过程中不会改变	   第9页
void XSystemPara(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp,Pages;
	
	Pages = Pages;

	tp1 = (uint8_t *) &SYSPara;
	temp = (sizeof(SystemPara));
	
	Pages = (temp / PAGESIZE)	+1;
	
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(9, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(9, 0, temp, tp1);
	}					


}

//保存和读取系统参数,这个系统参数在交易过程中不会改变	   第9页
void XSystemPara2(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp,Pages;
	
	Pages = Pages;

	//第70页//添加新的系统参数;by gzz 20121219
	tp1 = (uint8_t *) &SYSPara2;
	temp = (sizeof(SystemPara2));
	
	Pages = (temp / PAGESIZE)	+1;
	
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(70, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(70, 0, temp, tp1);
	}	

}


//按货道添存货量;by gzz 20121120
//保存和读取货道添存货量参数,这个系统参数在交易过程中不会改变	   第21页
void XColSystemPara(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp,Pages;
	
	Pages = Pages;

	tp1 = (uint8_t *) &colSystemPara;
	temp = (sizeof(ColSystemPara));
	
	Pages = (temp / PAGESIZE)	+1;
	
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(21, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(21, 0, temp, tp1);
	}					  

}


//保存和读取系统参数,这个系统参数在交易过程中会改变	   第7页
void XSystemPara1(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp,Pages;
	
	Pages = Pages;		

	tp1 = (uint8_t *) &SYSPara1;
	temp = (sizeof(SystemPara1));
	
	Pages = (temp / PAGESIZE)	+1;
	
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(7, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(7, 0, temp, tp1);
	}					  

}

//配置用户菜单可见;by gzz 20121204
//配置用户菜单可见,这个系统参数在交易过程中不会改变	   第50页
void XKeySystemVisible(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp,Pages;
	
	Pages = Pages;

	tp1 = (uint8_t *) &keyVisible;
	temp = (sizeof(KeySystemVisible));
	
	Pages = (temp / PAGESIZE)	+1;
	
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(50, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(50, 0, temp, tp1);
	}					  

}

//目前已经使用到第71页了
//保存和读取交易记录  从500页起		 保存和读取总交易金额记录  第8页
void XTransaction(uint8_t mode)
{
 	uint8_t *tp1;
//	uint16_t	i = 0;
	int16_t	temp;
	

	tp1 = (uint8_t *) &TranData;//交易数据记录
	temp = (sizeof(Transaction  ));
	
	
	//交易记录从500开始到4000为止，每交易一次，用去一页保存
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(TranTotal.TranPages, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(TranTotal.TranPages, 0, temp, tp1);
	}					  
	if(TranTotal.TranPages++ > MAXTRANPAGE)
		TranTotal.TranPages = 500;
	
	//保存总的交易金额记录	 第8页
	tp1 = (uint8_t *) &TranTotal;
	temp = (sizeof(TransactionTotal));
	   
	if (mode == WRITE)	
	{
	 	AT45DB_WritePage(8, tp1);
	}	    		
	else
	{
		AT45DB_ReadByte(8, 0, temp, tp1);
	}					  
}


//重置系统参数，货道对应关系，交易记录
void RstSystemPara(void)
{
    uint16_t i,j;
	uint8_t  *tp;
	SystemPara tempSysPara;

	ClearScreen();
	memcpy((uint8_t *)&tempSysPara,(uint8_t *)&SYSPara,sizeof(SYSPara));//系统参数有些无需重新出厂默认;by gzz 20121102
	tp = (uint8_t *)&SYSPara ;
	for(i = 0; i < (sizeof(SystemPara)); i++) 
	{		 		
		*tp++ = 0x00;
	}
	//出厂配置时不予删除;by gzz 20121009
	/*
	tp = (uint8_t *)&TranTotal ;
	for(i = 0; i < (sizeof(TransactionTotal)); i++) 
	{		 				
		*tp++ = 0x00;
	}
	*/
	/*
	tp = (uint8_t *)&TranData ;
	for(i = 0; i< (sizeof(Transaction)); i++) {		 				
		*tp++ = 0x00;
	}
	*/
	tp = (uint8_t *)&Channels[0][0] ;

	for(i = 0; i < ((sizeof(GoodsChannel))*MAXCHANNEL*MAXLAYER); i++) 
	{		 //clear  channels data
		*tp++ = 0x00;
	}

	
	for(i = 0;i < MAXLAYER;i++)	
	{						   //重新计算货道驱动映射关系
		SYSPara.LayEnable[i] = 0x01	;
		SYSPara.LayMaxGoods[i] = MAXGOODS;
		
		ChnMapPara.LAYTODRV[i] = i+1;
		ChnMapPara.DRVTOLAY[i] = i+1;
		
		for(j = 0; j < MAXCHANNEL; j++)	
		{
			//120703 by cq MDBCompatible
			//Channels[i][j].ChannelStat = 0x80;
			//121108 by cq DontReduceNum
			if(!isDontReduceNum())
			{
				Channels[i][j].ChannelStat = 0x83;			 
				Channels[i][j].GoodsNum = 0;
			}
			else
			{
				Channels[i][j].ChannelStat = 0x81; 
				Channels[i][j].GoodsNum = 99;
			}
			
		   Channels[i][j].GoodsPrice = 100;
		   ChnMapPara.CHTODRV[i][j] = j+1;
		   ChnMapPara.DRVTOCH[i][j] = j+1;
		}
	}

	

	SYSPara.SoftwareVersion = SOFTVERSION;
	SYSPara.HardwareVersion	 = HAEDWAREVERDION;

	//121108 by cq RecordPassword
	if(!isRecordPassword())
	{
		SYSPara.Password = 123456;		
		SYSPara.Password2 = 83718557;
		SYSPara.Password3 = 83702153;
	}
	else
	{
		SYSPara.Password = 123456;		
		SYSPara.Password2 = 123456;
		SYSPara.Password3 = 123456;
	}
	
	TranTotal.TranPages = 20;

	//121106 by cq MDBCompatible

	#if PULSEDEVICE
		SYSPara.CoinsValue[3] = 100;
	#endif
	
	#if COINDEVICE
		SYSPara.CoinsValue[0] = 0;//50;																   
		SYSPara.CoinsValue[1] = 0;//100;
		SYSPara.CoinsValue[2] = 0;
		SYSPara.CoinsValue[3] = 0;
	#endif

	#if MBVDEVICE || GBAHRDEVICE
		SYSPara.BillValue[0] = 0;//100;
		SYSPara.BillValue[1] = 0;//500;
		SYSPara.BillValue[2] = 0;//1000;		  
		SYSPara.BillValue[3] = 0;//2000;
	#endif

	//120703 by cq MDBCompatible
	SYSPara.ChangeValue[0] = 0;//50;			
	SYSPara.ChangeValue[1] = 0;//100;			

	SYSPara.MoneyMaxin = 2000;
	SYSPara.MoneyMinLeft = 100;
	SYSPara.ChangeServer = 0;
	SYSPara.Decimal = 2;
	SYSPara.TransMultiMode = 0;
	SYSPara.TransEscape = 0;
	SYSPara.GBAEnable = MDB_NOTE_EN;
	SYSPara.CoinEnable = MDB_COIN_EN;
	SYSPara.ChangerEnable = MDB_CHANGER_EN;
	SYSPara.POSEnable = 0; 
	SYSPara.PCEnable = 0; 
	SYSPara.LCDSel = tempSysPara.LCDSel;
	SYSPara.KeySel = tempSysPara.KeySel;//选货按键;by gzz 20121025
	SYSPara.LogoSel = tempSysPara.LogoSel;//拼图屏幕样式;by gzz 20121106
	SYSPara.ReaderEnable = tempSysPara.ReaderEnable;//读卡器菜单;by gzz 20121120 
	SYSPara.ReaderCost = tempSysPara.ReaderCost;

	//121108 by cq GOCErrorSevice
	SYSPara.GOCCtr = 1;
	SYSPara.GOCErrorSevice = 0; 

	SYSPara2.AutoColumn = 1;
	SYSPara2.TenColumn = 0;
	
}

//重置可视状态
void RstVisiblePara(void)
{
    uint16_t i;
	
	for(i = 0;i < MAX_KEYTABSTRUCT_NUM;i++)
	{
		switch(KeyTab[i].KeyTab_MenuIndex)
		{
			case MENU_ADDGOODS:
			//设备管理	
			case MENU_HOPPERSTAT3:
			case MENU_GPRSTEST:
			case MENU_SMSTEST:	
			case MENU_ALLCHANNELSTAT:
			case MENU_CHTESTRESULT:	
			case MENU_LCDTEST:
			case MENU_TEMPRATURETEST:
			case MENU_SELECTKEYTEST:
			case MENU_FLASHLEFT:
			//业务参数	
			case MENU_OPENCOIN:
			case MENU_OPENNOTE:
			case MENU_OPENCHANGE:	
			case MENU_OPENREADER:
			case MENU_READERCOST:
			case MENU_CHANGESERVER:	
			case MENU_BALANCEMIN:
			case MENU_DECPOINTNUM:	
			case MENU_SALECONFIRM:
			case MENU_POSENABLE:
			case MENU_VMCPC:
			case MENU_LARGELCD:
			case MENU_SELECTKEY:
			case MENU_LOGOSEL:
			case MENU_ADDCOLUMN:
			case MENU_TENCOLUMN:	
			case MENU_HOLDNOTE:
			case MENU_HOLDNOTE5:
			case MENU_HOLDNOTE10:
			case MENU_HOLDNOTE20:	
			//交易记录	
			case MENU_PAYMENTSJIDU:
			case MENU_GOODSJIDU:
			case MENU_CLEARTRANSJIDU:
			//系统参数
			case MENU_GPRSONOFF:
			case MENU_SERIALNO:
			case MENU_GSMNO:
			case MENU_TEMPERATURE:
			case MENU_SYSLANGUAGE:
			//层架配置	
			case MENU_SETCHANNEL:	
			//出厂默认配置	
			case MENU_DEFAULTSET:	
				KeyTab[i].KeyTab_Visible = MENU_VISIBLE_FAUSE;
				break;
		}
	}	
}

uint8_t  ColumnFunction(uint8_t ID,uint8_t InputNum,uint8_t NowLayerNo)				  //开启货道
{
	uint8_t i,ChannelNum =0 ;
	//Trace("\r\n input=%d,lay=%d,id=%d",InputNum,NowLayerNo,ID);
	if (1)
	{
		if((InputNum < 2 ))
		{
			InputCol = 8;
			//InputMode = FALSE;
			if (InputNum == 0)
			{
				Channels[NowLayerNo][ID].ChannelStat &= ~(1 << 7);
				colSystemPara.ColMaxGoods[NowLayerNo][ID] = 0;//按货道添存货量;by gzz 20121120
			}
			if (InputNum == 1)
				Channels[NowLayerNo][ID].ChannelStat |= (1 << 7);
			for ( i = 0; i <MAXCHANNEL; i++)
			{
				
				ChnMapPara.CHTODRV[NowLayerNo][i] = 0;		//数组清零
				ChnMapPara.DRVTOCH[NowLayerNo][i] = 0;
				if( (Channels[NowLayerNo][i].ChannelStat & 0x80) == 0x80 )		   
				{
					
					ChnMapPara.CHTODRV[NowLayerNo][ChannelNum++] = i+1;
					ChnMapPara.DRVTOCH[NowLayerNo][i] = ChannelNum;
				}
			}
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 1;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}



uint8_t  LayerFunction(uint8_t InputNum,uint8_t NowLayerNo)			//开启层号
{
	uint8_t i,LayNum =0 ;
	//Trace("\r\n input=%d,lay=%d",InputNum,NowLayerNo);
	if (1)
	{
		if( (InputNum < 2 ))
		{
			InputCol = 8;
			//InputMode = FALSE;
			SYSPara.LayEnable[NowLayerNo] = InputNum;
			for ( i = 0; i <MAXLAYER; i++)
			{
				
				ChnMapPara.LAYTODRV[i] = 0;   //清零
				ChnMapPara.DRVTOLAY[i] = 0;
				if( SYSPara.LayEnable[i] == 0x01 )		   
				{
					
					ChnMapPara.LAYTODRV[LayNum++] = i+1;
					ChnMapPara.DRVTOLAY[i] = LayNum;
				}
			}
			
			return TRUE;
		}
		else
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 1;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}


//重置系统参数，货道对应关系，交易记录
void RstTurkeySystemPara(void)
{
    uint16_t i,j;
	//uint8_t  *tp;
	//SystemPara tempSysPara;

	ClearScreen();
	/*
	memcpy((uint8_t *)&tempSysPara,(uint8_t *)&SYSPara,sizeof(SYSPara));//系统参数有些无需重新出厂默认;by gzz 20121102
	tp = (uint8_t *)&SYSPara ;
	for(i = 0; i < (sizeof(SystemPara)); i++) 
	{		 		
		*tp++ = 0x00;
	}
	

	//121108 by cq RecordPassword
	if(!isRecordPassword())
	{
		SYSPara.Password = 123456;		
		SYSPara.Password2 = 83718557;
		SYSPara.Password3 = 83702153;
	}
	else
	{
		SYSPara.Password = 123456;		
		SYSPara.Password2 = 123456;
		SYSPara.Password3 = 123456;
	}
	
	TranTotal.TranPages = 20;
	*/
				
	//1.设业务参数
	SYSPara.MoneyMaxin = 2000;
	SYSPara.MoneyMinLeft = 100;
	SYSPara.ChangeServer = 0;
	SYSPara.Decimal = 2;
	SYSPara.TransMultiMode = 0;
	SYSPara.TransEscape = 0;
	SYSPara.GBAEnable = MDB_NOTE_EN;
	SYSPara.CoinEnable = MDB_COIN_EN;
	SYSPara.ChangerEnable = MDB_CHANGER_EN;
	SYSPara.POSEnable = 0; 
	SYSPara.PCEnable = 0; 
	SYSPara.LCDSel = 2;
	SYSPara.KeySel = 0;//选货按键;by gzz 20121025
	SYSPara.LogoSel = 3;//拼图屏幕样式;by gzz 20121106
	SYSPara.ReaderEnable = 0;//读卡器菜单;by gzz 20121120 
	SYSPara.ReaderCost = 1;

	//121108 by cq GOCErrorSevice
	SYSPara.GOCCtr = 1;
	SYSPara.GOCErrorSevice = 0; 

	
	SYSPara2.AutoColumn = 1;
	SYSPara2.TenColumn = 1;
	SYSPara.NoteEscrow = 0;
	SYSPara.NoteEscrowType[1]=0;
	SYSPara.NoteEscrowType[2]=0;

	//2.设货道参数
	ColumnFunction(0,0,0);
	ColumnFunction(2,0,0);
	ColumnFunction(4,0,0);
	LayerFunction(0,6);
	LayerFunction(0,7);

	//3.设系统压缩机,展示灯参数
	SYSPara2.CompressorTime.DeviceOn = 2;
	SYSPara2.CompressorTime.HourOn   = 0;
	SYSPara2.CompressorTime.MinOn    = 0;
	SYSPara2.CompressorTime.HourOff  = 23;
	SYSPara2.CompressorTime.MinOff   = 59;
	SYSPara2.LEDTime.DeviceOn = 2;
	SYSPara2.LEDTime.HourOn   = 0;
	SYSPara2.LEDTime.MinOn    = 0;
	SYSPara2.LEDTime.HourOff  = 23;
	SYSPara2.LEDTime.MinOff   = 59;
	
}

//重置可视状态
void RstTurkeyVisiblePara(void)
{
    uint16_t i;
	
	for(i = 0;i < MAX_KEYTABSTRUCT_NUM;i++)
	{
		switch(KeyTab[i].KeyTab_MenuIndex)
		{
			case MENU_ADDGOODS:
			//设备管理	
			case MENU_HOPPERSTAT1:
			case MENU_HOPPERSTAT2:	
			case MENU_HOPPERSTAT3:
			case MENU_GPRSTEST:
			case MENU_SMSTEST:	
			case MENU_ALLCHANNELSTAT:
			case MENU_CHTESTRESULT:	
			case MENU_LCDTEST:
			case MENU_TEMPRATURETEST:
			case MENU_SELECTKEYTEST:
			case MENU_FLASHLEFT:
			//业务参数	
			case MENU_CHANGEVALUE:				
			//case MENU_OPENREADER:
			//case MENU_READERCOST:
			case MENU_CHANGESERVER:	
			case MENU_RETURNCOINS:	
			case MENU_BALANCEMIN:
			case MENU_DECPOINTNUM:	
			case MENU_SALECONFIRM:
			case MENU_POSENABLE:
			case MENU_VMCPC:
			case MENU_LARGELCD:
			case MENU_SELECTKEY:
			case MENU_LOGOSEL:
			case MENU_ADDCOLUMN:
			case MENU_TENCOLUMN:	
			case MENU_HOLDNOTE:
			case MENU_HOLDNOTE5:
			case MENU_HOLDNOTE10:
			case MENU_HOLDNOTE20:	
			//交易记录	
			case MENU_PAYMENTSJIDU:
			case MENU_GOODSJIDU:
			case MENU_CLEARTRANSJIDU:
			//系统参数
			case MENU_GPRSONOFF:
			case MENU_SERIALNO:
			case MENU_GSMNO:
			case MENU_TEMPERATURE:
			case MENU_TEMPSET:
			case MENU_LEDSET:
			//层架配置	
			case MENU_SETCHANNEL:	
			//出厂默认配置	
			case MENU_DEFAULTSET:	
				KeyTab[i].KeyTab_Visible = MENU_VISIBLE_FAUSE;
				break;
		}
	}	
	for(i = 0;i < MAX_KEYTABSTRUCT_NUM;i++)
	{
		switch(KeyTab[i].KeyTab_MenuIndex)
		{			
			//业务参数	
			case MENU_OPENCOIN:				
			case MENU_OPENNOTE:
			case MENU_OPENCHANGE:
			case MENU_OPENREADER:
			case MENU_SYSLANGUAGE:
				KeyTab[i].KeyTab_Visible = MENU_VISIBLE_TRUE;
				break;
		}
	}	
}

//重置可视状态
void RstTurkeyVisibleParaInit(void)
{
    uint16_t i;
	
	for(i = 0;i < MAX_KEYTABSTRUCT_NUM;i++)
	{
		switch(KeyTab[i].KeyTab_MenuIndex)
		{	
			//货道参数
			case MENU_CHANNELNO:
			case MENU_PRICE:
			case MENU_CHANNELLEFT:
			case MENU_CHANNELSTAT:
			case MENU_CHANNELGOODS:	
			//业务参数	
			case MENU_OPENCOIN:				
			case MENU_OPENNOTE:
			case MENU_OPENCHANGE:
			case MENU_OPENREADER:
			case MENU_SYSLANGUAGE:	
				KeyTab[i].KeyTab_Visible = MENU_VISIBLE_TRUE;
				break;
		}
	}	
}



//设置投币器状态 禁能 使能
void SetTouBiStat(uint8_t CommandType)
{
    uint8_t err;

	
	Trace("\r\nSetTouBiStat = %u",CommandType);
	
	OSMboxAccept(g_CommBackMsg);
	MsgBack.CmdType = (uint32_t)CommandType << 24;		  
	OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   						
	OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*8,&err);
}

