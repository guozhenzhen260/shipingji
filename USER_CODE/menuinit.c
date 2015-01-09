/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Menuinit.c
** Last modified Date:  2012-03-01
** Last Version:         
** Descriptions:        在维护模块中各菜单页面中，用于输入各参数和显示参数值
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2012-03-01
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

//Last Modify Time:03/11/07 01:22
//ReadMe
//屏宽:112
//屏高:64

//#include <intrins.h>
#include <absacc.h>
#include "..\driver\lpc_types.h"
#include "global.h"
#include "function.h"
#include "gbahr1.h"
#include "hopper.h"
#include "ITL.h"
#include "lcd.h"
#include "menu.h"
#include "drv.h"
#include "dispmenu.h"
#include "language.h"
//		   



char *  MenuInit0(void)
{
return NULL;	
}
char *  MenuInitCHIN(void)		   //货道号输入
{
	Cursor_On(LCDUP);
	InputMode = TRUE;
	FDot =  FALSE;
	DOT = FALSE;			//小数禁能	
	PASSWORD = FALSE;
	InputNum = 0;
	offset = 0;
	InputCol = 2;
	InputDone = FALSE ;
	return NULL;	
}
char *  MenuInitLAYIN(void)		   //层道号输入
{
	Cursor_On(LCDUP);
	InputMode = TRUE;
	FDot =  FALSE;
	DOT = FALSE;			//小数禁能	
	PASSWORD = FALSE;
	InputNum = 0;
	offset = 0;
	InputCol = 1;
	InputDone = FALSE ;
	return NULL;	
}


//输入	 密码输入
char *  MenuInitPWIN(void)		   
{
	Cursor_On(LCDUP);
	InputMode = TRUE;
	FDot =  FALSE;
	DOT = FALSE;			//小数禁能
	PASSWORD = TRUE;
	InputNum = 0;
	offset = 0;
	
	return NULL;	
}

char * MenuInitCHNO(void)			//货道号
{

sprintf( StrInit,"%d", 	NowChannel );
//NowCHNO = InputNum;
return  StrInit;	
}

char * MenuInitCHPR(void)		   //货道价格
{
	uint32_t temp = 0;
//	uint8_t chx,chy;

   // if(DRV_ChnChk(NowCHNO,&chx,&chy) == FALSE) {
   //    strcpy( StrInit,"err");
   // } else {   
	   temp = Channels[NowLayerNo][NowCHNO].GoodsPrice;
	//   sprintf( StrInit,"%d.%S", temp/100,DecimalPoint(temp));
	//}
	return DecimalPoint(temp);		
}


char * MenuInitCHST(void)			//货道状态
{
	sprintf( StrInit,"%d", (Channels[NowLayerNo][NowCHNO].ChannelStat & 0x03) );
	return StrInit;		
}
char * MenuInitCHLF(void)			//货道存货
{

	//sprintf( StrInit,"%d", Channels[ChnMapPara.LAYTODRV[NowLayerNo - 1]][ChnMapPara.CHTODRV[ChnMapPara.LAYTODRV[NowLayerNo-1]][NowCHNO-1]].GoodsNum  );

	sprintf( StrInit,"%d", Channels[NowLayerNo][NowCHNO].GoodsNum  );
	return StrInit;		
}

char * MenuInitCHGOODS(void)			//商品编号
{
	sprintf( StrInit,"%d", (Channels[NowLayerNo][NowCHNO].GoodsCode) );
	return StrInit;		
}


char * MenuInitAddGoods(void)			//添货全部货道
{
	ClearScreen();
	if(	InputNum == 9)	
	{
		sprintf( StrInit,Language[ADDFULLLAY][SYSPara.Language] );
	}
	else
	{
		sprintf( StrInit,Language[ADDLAYNO][SYSPara.Language], InputNum );
	}
	return StrInit;		
}

char * MenuInitCHSU(void)		   //  货道成功交易次数
{
	sprintf( StrInit,"%d", Channels[NowLayerNo][NowCHNO].SuccessNum  );
	return StrInit;		
}

char * MenuInitCHMONEY(void)		   //  货道交易金额
{
	return DecimalPoint(Channels[NowLayerNo][NowCHNO].SuccessNum * Channels[NowLayerNo][NowCHNO].GoodsPrice);
}

//用于季度结账用;by gzz 20121009
char * MenuInitCHSUJIDU(void)		   //  货道成功交易次数
{
	sprintf( StrInit,"%d", Channels[NowLayerNo][NowCHNO].SuccessNumJidu  );
	return StrInit;		
}

char * MenuInitCHMONEYJIDU(void)		   //  货道交易金额
{
	return DecimalPoint(Channels[NowLayerNo][NowCHNO].SuccessNumJidu * Channels[NowLayerNo][NowCHNO].GoodsPrice);
}


char * MenuInitCHDO(void)		   //货道疑问交易次数
{
	sprintf( StrInit,"%d", Channels[NowLayerNo][NowCHNO].DoubtNum  );
	return StrInit;		
}

char * MenuInitNOST(void)		  //纸币器状态显示
{
	if(((SYSPara.GBAEnable == MDB_NOTE_EN) && (SYSPara1.NoteStat == MBV_STAT_OK))
	   || ((SYSPara.GBAEnable == HR_NOTE_EN) && (SYSPara1.NoteStat == HR_STAT_OK))
	   || ((SYSPara.GBAEnable == ITL_NOTE_EN) && (SYSPara1.NoteStat == ITL_STAT_OK))) 
	{
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language] );
	}
	else 
	{
	 	sprintf( StrInit,"%s%02x",  Language[SFAULT][SYSPara.Language] ,SYSPara1.NoteStat );
	}
	return StrInit;		
}

char * MenuInitCOST(void)		   //硬币器状态显示
{
	if (SYSPara1.CoinsStat == 0)
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language] );
	else
	 	sprintf( StrInit,"%s%02x",   Language[SFAULT][SYSPara.Language] ,SYSPara1.CoinsStat);
	return StrInit;		
}
char * MenuInitHOST1(void)			//找零器#1状态显示
{	
	//120716 by cq HoppeyUse
	if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
		sprintf( StrInit,"%s",	 Language[SNORMAL][SYSPara.Language]  );//120622 by cq HopperTest
	else if ((SYSPara1.HopperStat[0] & HOPPER_ERR_MASK) == 0)
		sprintf( StrInit,"%s",   Language[SNORMAL][SYSPara.Language]  );
	else
	 	sprintf( StrInit,"%s%02x",  Language[SFAULT][SYSPara.Language] ,SYSPara1.HopperStat[0]);
		
	return StrInit;		
}
char * MenuInitHOST2(void)		   //找零器#2状态显示
{
	//120716 by cq HoppeyUse
	if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
		sprintf( StrInit,"%s",	 Language[SNORMAL][SYSPara.Language]  );//120622 by cq HopperTest
	else if ((SYSPara1.HopperStat[1] & HOPPER_ERR_MASK) == 0)
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language] );
	else
	 	sprintf( StrInit,"%s%02x", Language[SFAULT][SYSPara.Language],SYSPara1.HopperStat[1]);
		
	return StrInit;		
}
char * MenuInitHOST3(void)			//找零器#3状态显示
{
	//120716 by cq HoppeyUse
	if(SYSPara.ChangerEnable == MDB_CHANGER_EN) 
		sprintf( StrInit,"%s",	 Language[SNORMAL][SYSPara.Language]  );//120622 by cq HopperTest
	else if ((SYSPara1.HopperStat[2] & HOPPER_ERR_MASK) == 0)
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language]  );
	else
	 	sprintf( StrInit,"%s%02x", Language[SFAULT][SYSPara.Language] ,SYSPara1.HopperStat[2]);
		
	return StrInit;		
}

char * MenuInitHOST2_ReaderCost(void)		   //找零器#2状态显示
{
	//120926 by cq Cashless
	if(SYSPara.ReaderCost)//开
	{
		if(SYSPara.Language)//英文
		{
			strcpy( StrInit,"ON");
		}
		else
		{
			strcpy( StrInit,"开");
		}
	}
	else
	{
		if(SYSPara.Language)//英文
		{
			strcpy( StrInit,"OFF");
		}
		else
		{
			strcpy( StrInit,"关");
		}
	}
	
	/*
	if ((SYSPara1.HopperStat[1] & HOPPER_ERR_MASK) == 0)
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language] );
	else
	 	sprintf( StrInit,"%s%02x", Language[SFAULT][SYSPara.Language],SYSPara1.HopperStat[1]);
		
	sprintf( StrInit,"%s",	 Language[SNORMAL][SYSPara.Language]  );//120622 by cq HopperTest
	*/

	return StrInit;		
}

char * MenuInitHOST3_ReaderEnable(void)			//找零器#3状态显示
{	
	//120921 by cq Cashless
	if(SYSPara.ReaderEnable)//开
	{
		if(SYSPara.Language)//英文
		{
			strcpy( StrInit,"ON");
		}
		else
		{
			strcpy( StrInit,"开");
		}
	}
	else
	{
		if(SYSPara.Language)//英文
		{
			strcpy( StrInit,"OFF");
		}
		else
		{
			strcpy( StrInit,"关");
		}
	}
		
		
	/*
	if ((SYSPara1.HopperStat[2] & HOPPER_ERR_MASK) == 0)
		sprintf( StrInit,"%s",  Language[SNORMAL][SYSPara.Language]  );
	else
	 	sprintf( StrInit,"%s%02x", Language[SFAULT][SYSPara.Language] ,SYSPara1.HopperStat[2]);
		
	sprintf( StrInit,"%s",	 Language[SNORMAL][SYSPara.Language]  );//120622 by cq HopperTest
	*/

	return StrInit;		
}



char * MenuInitIOU(void)			//欠条
{
	//sprintf( StrInit,"%d", SYSPara.Iou  );
	//return StrInit;
	return DecimalPoint(TranTotal.Iou);		
}
char * MenuInitCOCH1(void)			//硬币器通道1币值
{
  
 	return DecimalPoint(SYSPara.CoinsValue[0]);			
}
char * MenuInitCOCH2(void)			//硬币器通道2币值
{
	//sprintf( StrInit,"%d", SYSPara.CoinsValue[1]  );
	return DecimalPoint(SYSPara.CoinsValue[1]);		
}
char * MenuInitCOCH3(void)		  	//硬币器通道3币值
{
	return DecimalPoint(SYSPara.CoinsValue[2]);		
}
char * MenuInitCOCH4(void)		  	//硬币器通道4币值
{
	return DecimalPoint(SYSPara.CoinsValue[3]);		
}
char * MenuInitCOCH5(void)		   	//硬币器通道5币值
{
	return DecimalPoint(SYSPara.CoinsValue[4]);			
}
char * MenuInitCOCH6(void)		   	//硬币器通道6币值
{
	return DecimalPoint(SYSPara.CoinsValue[5]);		
}
char * MenuInitCOCH7(void)				//硬币器通道7币值
{
	return DecimalPoint(SYSPara.CoinsValue[6]);		
}
char * MenuInitCOCH8(void)			  	//硬币器通道8币值
{
	return DecimalPoint(SYSPara.CoinsValue[7]);			
}
char * MenuInitNOCH1(void)			  	//纸币器通道1币值
{
	return DecimalPoint(SYSPara.BillValue[0]);			
}
char * MenuInitNOCH2(void)			   //纸币器通道2币值
{
	return DecimalPoint(SYSPara.BillValue[1]);	
}
char * MenuInitNOCH3(void)			  //纸币器通道3币值
{
	return DecimalPoint(SYSPara.BillValue[2]);		
}
char * MenuInitNOCH4(void)			  //纸币器通道4币值
{
	return DecimalPoint(SYSPara.BillValue[3]);		
}
char * MenuInitNOCH5(void)			  //纸币器通道5币值
{
	return DecimalPoint(SYSPara.BillValue[4]);	
}
char * MenuInitNOCH6(void)			  //纸币器通道6币值
{
	return DecimalPoint(SYSPara.BillValue[5]);	
}
char * MenuInitNOCH7(void)			  //纸币器通道7币值
{
	return DecimalPoint(SYSPara.BillValue[6]);			
}
char * MenuInitNOCH8(void)			  //纸币器通道8币值
{
	return DecimalPoint(SYSPara.BillValue[7]);		
}

char * MenuInitCGVA1(void)			 //找零器#1币值
{
	return DecimalPoint(SYSPara.ChangeValue[0]);		
}
char * MenuInitCGVA2(void)			  //找零器#2币值
{
	return DecimalPoint(SYSPara.ChangeValue[1]);	
}
char * MenuInitCGVA3(void)			  //找零器#3币值
{
	return DecimalPoint(SYSPara.ChangeValue[2]);		
}
char * MenuInitCGSV(void)			  //无零币是否服务
{
	sprintf( StrInit,"%d", SYSPara.ChangeServer  );
	return StrInit;		
}
char * MenuInitPONO(void)			  //小数点位数
{
	sprintf( StrInit,"%d", SYSPara.Decimal  );
	return StrInit;		
}
char * MenuInitSACON(void)			  // 出货确认
{
	//121108 by cq GOCErrorSevice
	//sprintf( StrInit,"%d", SYSPara.GOCCtr  );
	if(isGOCErrorSevice())
		sprintf( StrInit,"%d", SYSPara.GOCErrorSevice  );
	else
		sprintf( StrInit,"%d", SYSPara.GOCCtr  );
	return StrInit;		
}
char * MenuInitPOSEN(void)			  //POS机使能
{
	sprintf( StrInit,"%d", SYSPara.POSEnable  );
	return StrInit;		
}

char * MenuInitPCEN(void)			  //PC端通讯使能
{
	sprintf( StrInit,"%d", SYSPara.PCEnable  );
	return StrInit;		
}

char * MenuInitLCDSEL(void)			  //使用LCD类型
{
	sprintf( StrInit,"%d", SYSPara.LCDSel  );
	return StrInit;		
}

//选货按键;by gzz 20121025
char * MenuInitKeySel(void)			  //是否使用选货按键
{
	sprintf( StrInit,"%d", SYSPara.KeySel  );
	return StrInit;		
}

//拼图屏幕样式;by gzz 20121106
char * MenuInitLogoSel(void)			  //是否使用logo
{
	sprintf( StrInit,"%d", SYSPara.LogoSel  );
	return StrInit;		
}

//不用手工补货功能;by gzz 20130122
char * MenuInitAutoColumn(void)			  
{
	sprintf( StrInit,"%d", SYSPara2.AutoColumn  );
	return StrInit;		
}

//是否使用十货道的机器;by gzz 20130226
char * MenuInitTenColumn(void)			  
{
	sprintf( StrInit,"%d", SYSPara2.TenColumn  );
	return StrInit;		
}


//holdnote 菜单初始化,如果开启暂存，则在系统参数上显示5元暂存，10元暂存选项。否则隐藏
void HoldNoteInit(void)
{

	if(SYSPara.NoteEscrow ==0)
	{
		BusinessNo = BUSINESSNO-2;
	 	KeyTab[MENU_HOLDNOTE].KeyTab_PressDown	  = MENU_COINVALUE;
		KeyTab[MENU_COINVALUE].KeyTab_PressUp	  = MENU_HOLDNOTE;

	}
	else
	{
		BusinessNo = BUSINESSNO;
		KeyTab[MENU_HOLDNOTE].KeyTab_PressDown	  = MENU_HOLDNOTE5;
		KeyTab[MENU_COINVALUE].KeyTab_PressUp	  = MENU_HOLDNOTE10;
	}

}

//配置用户菜单可见;by gzz 20121204
//mode=1初始化可视菜单,2重新保存可视菜单
void KeySystemInit(uint8_t mode)//初始化或重新配置可视菜单
{
	uint8_t i;
	if(mode == 1)
	{
		for(i = 0;i <MAX_KEYTABSTRUCT_NUM; i++)
		{
			KeyTab[i].KeyTab_Visible = keyVisible.visible[i];	
			//Trace("(%d=%d)",i,keyVisible.visible[i]);
		}
	}
	else
	{
		for(i = 0;i <MAX_KEYTABSTRUCT_NUM; i++)
		{
			 keyVisible.visible[i] = KeyTab[i].KeyTab_Visible;		
			 //Trace("[%d=%d]",KeyTab[i].KeyTab_MenuIndex,KeyTab[i].KeyTab_Visible);
		}		
	}

}

char * MenuInitHOLDNOTE(void)			  //暂存使能
{

	HoldNoteInit();

	sprintf( StrInit,"%d", SYSPara.NoteEscrow  );
	return StrInit;		
}
char * MenuInitHOLDNOTE5(void)			  //暂存使能
{
	sprintf( StrInit,"%d", SYSPara.NoteEscrowType[1]  );
	return StrInit;		
}
char * MenuInitHOLDNOTE10(void)			  //暂存使能
{
	sprintf( StrInit,"%d", SYSPara.NoteEscrowType[2]  );
	return StrInit;		
}
char 	*MenuInitNBUY(void)
{
	sprintf( StrInit,"%d", SYSPara.TransMultiMode  );	  	//允许多次交易
	return StrInit;		
}
char 	*MenuInitRCOIN(void)
{
	sprintf( StrInit,"%d", SYSPara.TransEscape  );	  		//允许不卖货退币
	return StrInit;		
}
char 	*MenuInitOCOIN(void)
{
	sprintf( StrInit,"%d", SYSPara.CoinEnable  );			//硬币器使能
	return StrInit;		
}
char 	*MenuInitONOTE(void)
{
	if(SYSPara.GBAEnable == NOTE_MEI)	
	{
		sprintf( StrInit,"MDB"  );			//纸币器使能
	}
	else if(SYSPara.GBAEnable == NOTE_GBA)	
	{
		sprintf( StrInit,"GBA"  );			//纸币器使能
	}
	else if(SYSPara.GBAEnable == NOTE_ITL)	
	{
		sprintf( StrInit,"SSP"  );			//纸币器使能
	}
	else 
	   sprintf( StrInit,"OFF"  );	
	return StrInit;		
}
//读卡器菜单;by gzz 20121120
char 	*MenuInitONREADER(void)
{
	if(SYSPara.ReaderEnable == READEDR_CLOSE)	
	{
		sprintf( StrInit,"OFF"  );			//读卡器使能
	}
	else if(SYSPara.ReaderEnable == READEDR_MDB)	
	{
		sprintf( StrInit,"MDB"  );			//读卡器使能
	}	
	return StrInit;		
}

//读卡器菜单;by gzz 20121120
char 	*MenuInitOREADERCOST(void)
{
	sprintf( StrInit,"%d", SYSPara.ReaderCost  );			//读卡器扣钱使能
	return StrInit;		
}


char 	*MenuInitOCHANGE(void)
{
	sprintf( StrInit,"%d", SYSPara.ChangerEnable  );			//找零器使能
	return StrInit;		
}
char 	*MenuInitBMIN(void)								//最低余额
{
	return DecimalPoint( SYSPara.MoneyMinLeft);	
}					

char * MenuInitMMAX(void)								//最大收币金额
{
	return DecimalPoint( SYSPara.MoneyMaxin);		
}
char * MenuInitGPRS(void)								//GPRS使能
{
	sprintf( StrInit,"%d", SYSPara.GPRSOnOff  );
	return StrInit;		
}

char * MenuInitSENO(void)							   //服务号码
{
	strcpy( StrInit,SYSPara.SerialNo  );
	//sprintf( StrInit,"%X", SYSPara.SerialNo  );
	return StrInit;		
}

char * MenuInitGSMNO(void)							//GSM号码
{
	strcpy( StrInit,SYSPara.GSMNO  );
	//sprintf( StrInit,"%X", SYSPara.GSMNO  );
	return StrInit;		
}

char * MenuInitTemerature(void)							   //温度
{
	sprintf( StrInit,"%d.%d", sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10);
	return	StrInit;
}



char * MenuInitDBMN(void)								//疑问金额
{
	return DecimalPoint( TranTotal.DoubtMoney);	
}

														//货币总收入
char * MenuInitINCOME(void)
{

	return DecimalPoint( TranTotal.Income);	
	
}
														//纸币总收入
char * MenuInitNOIN(void)
{

	return DecimalPoint( TranTotal.NoteIncome);	
}
														//硬币总收入
char	*MenuInitCOIN(void)
{
	return DecimalPoint( TranTotal.CoinsIncome);		
}

char	*MenuInitTOTR(void)
{														//总交易金额
	
	return DecimalPoint( TranTotal.TotalTrans);			
}

char	*MenuInitTOCH(void)
{
	return DecimalPoint( TranTotal.TotalChange);			//总找零金额
}

char	*MenuInitCOVA1(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChange[0]);		//#1找零金额
	return StrInit;		
} 
char	*MenuInitCOVA2(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChange[1]);		//#2找零金额
	return StrInit;		
} 
char	*MenuInitCOVA3(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChange[2]);		//#3找零金额
	return StrInit;		
}  

//用于季度结账用;by gzz 20121009
														//货币总收入
char * MenuInitINCOMEJIDU(void)
{

	return DecimalPoint( TranTotal.IncomeJidu);	
	
}
														//纸币总收入
char * MenuInitNOINJIDU(void)
{

	return DecimalPoint( TranTotal.NoteIncomeJidu);	
}
														//硬币总收入
char	*MenuInitCOINJIDU(void)
{
	return DecimalPoint( TranTotal.CoinsIncomeJidu);		
}

char	*MenuInitTOTRJIDU(void)
{														//总交易金额
	
	return DecimalPoint( TranTotal.TotalTransJidu);			
}

char	*MenuInitTOCHJIDU(void)
{
	return DecimalPoint( TranTotal.TotalChangeJidu);			//总找零金额
}

char	*MenuInitCOVA1JIDU(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChangeJidu[0]);		//#1找零金额
	return StrInit;		
} 
char	*MenuInitCOVA2JIDU(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChangeJidu[1]);		//#2找零金额
	return StrInit;		
} 
char	*MenuInitCOVA3JIDU(void)
{
	sprintf( StrInit,"%d", TranTotal.CoinsChangeJidu[2]);		//#3找零金额
	return StrInit;		
}  



char	*MenuInitSUNUM(void)							   //成功交易次数
{
	sprintf( StrInit,"%d", TranTotal.SuccessNum );
	return StrInit;		
} 

char	*MenuInitDBNUM(void)						   //疑问交易次数
{
	sprintf( StrInit,"%d", TranTotal.DoubtNum  );
	return StrInit;		
} 
char	*MenuInitFLASH(void)
{

	sprintf( StrInit,"%d", (MAXTRANPAGE - TranTotal.TranPages)  );
	return StrInit;	
}

char * MenuInitYear(void)				   //年
{
	sprintf( StrInit,"%d", TranData.Time.YEAR  );
	return StrInit;		
}
char * MenuInitMonth(void)				  //月
{
	sprintf( StrInit,"%d", TranData.Time.MONTH  );
	return StrInit;		
}
char * MenuInitDate(void)				  //日
{
	sprintf( StrInit,"%d", TranData.Time.DOM  );
	return StrInit;		
}
char * MenuInitHour(void)				  //时
{
	sprintf( StrInit,"%d", TranData.Time.HOUR  );
	return StrInit;		
}
char * MenuInitMinute(void)				  //分
{
	sprintf( StrInit,"%d", TranData.Time.MIN  );
	return StrInit;		
}

//添加压缩机控制;by gzz 20121224
char * MenuInitCompressOn(void)				  //压缩机时间控制
{
	sprintf( StrInit,"%d", SYSPara2.CompressorTime.DeviceOn  );
	return StrInit;		
}

char * MenuInitCompressHourOn(void)				  //压缩机打开时
{
	sprintf( StrInit,"%d", SYSPara2.CompressorTime.HourOn  );
	return StrInit;		
}
char * MenuInitCompressMinuteOn(void)				  //压缩机打开分
{
	sprintf( StrInit,"%d", SYSPara2.CompressorTime.MinOn  );
	return StrInit;		
}
char * MenuInitCompressHourOff(void)				  //压缩机关闭时
{
	sprintf( StrInit,"%d", SYSPara2.CompressorTime.HourOff  );
	return StrInit;		
}
char * MenuInitCompressMinuteOff(void)				  //压缩机关闭分
{
	sprintf( StrInit,"%d", SYSPara2.CompressorTime.MinOff  );
	return StrInit;		
}

//添加展示灯控制;by gzz 20121229
char * MenuInitLEDOn(void)				  //展示灯时间控制
{
	sprintf( StrInit,"%d", SYSPara2.LEDTime.DeviceOn  );
	return StrInit;		
}

char * MenuInitLEDHourOn(void)				  //展示灯打开时
{
	sprintf( StrInit,"%d", SYSPara2.LEDTime.HourOn  );
	return StrInit;		
}
char * MenuInitLEDMinuteOn(void)				  //展示灯打开分
{
	sprintf( StrInit,"%d", SYSPara2.LEDTime.MinOn  );
	return StrInit;		
}
char * MenuInitLEDHourOff(void)				  //展示灯关闭时
{
	sprintf( StrInit,"%d", SYSPara2.LEDTime.HourOff  );
	return StrInit;		
}
char * MenuInitLEDMinuteOff(void)				  //展示灯关闭分
{
	sprintf( StrInit,"%d", SYSPara2.LEDTime.MinOff  );
	return StrInit;		
}


char * MenuInitLAYNO(void)				  //层号
{

	sprintf( StrInit,"%d", NowLayerNo +1 );
	//NowLayerNo = InputNum;
	return  StrInit;	
}
char * MenuInitLAYENABLE(void)			  //层开关
{
	sprintf( StrInit,"%d", SYSPara.LayEnable[NowLayerNo]  );
	return StrInit;		
}

char * MenuInitMAXGOODS(void)			  //最大填货数
{
	sprintf( StrInit,"%d", SYSPara.LayMaxGoods[NowLayerNo] );
	return StrInit;		
}

char * MenuInitCH1(void)				  //货道开关
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][0].ChannelStat &0x80)>>7) );
	return StrInit;		
}

char * MenuInitCH2(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][1].ChannelStat &0x80)>>7)  );
	return StrInit;		
}

char * MenuInitCH3(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][2].ChannelStat &0x80)>>7)  );
	return StrInit;		
}

char * MenuInitCH4(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][3].ChannelStat &0x80)>>7)  );
	return StrInit;		
}

char * MenuInitCH5(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][4].ChannelStat &0x80)>>7) );
	return StrInit;		
}

char * MenuInitCH6(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][5].ChannelStat &0x80)>>7)  );
	return StrInit;		
}

char * MenuInitCH7(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][6].ChannelStat &0x80)>>7)  );
	return StrInit;		
}

char * MenuInitCH8(void)
{
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][7].ChannelStat &0x80)>>7)  );
	return StrInit;		
}


//121105 by cq TenChannels
char * MenuInitCH9(void)
{
#if (MAXCHANNEL >= 9)
	sprintf( StrInit,"%d", ((Channels[NowLayerNo][8].ChannelStat &0x80)>>7)  );
#else
	sprintf( StrInit,"%d", 0  );
#endif
	return StrInit;		
}

//121105 by cq TenChannels
char * MenuInitCH0(void)
{
#if (MAXCHANNEL >= 10)
		sprintf( StrInit,"%d", ((Channels[NowLayerNo][9].ChannelStat &0x80)>>7)  );
#else
		sprintf( StrInit,"%d", 0  );
#endif
	return StrInit;		
}


//按货道添存货量;by gzz 20121120
char * MenuInitCHNum(uint8_t ID)
{
	sprintf( StrInit,"%d", colSystemPara.ColMaxGoods[NowLayerNo][ID]);
	return StrInit;		
}

char * MenuInitCH1Num(void)				  //货道开关
{	
	 MenuInitCHNum(0);		
}

char * MenuInitCH2Num(void)
{	
	 MenuInitCHNum(1);		
}


char * MenuInitCH3Num(void)
{	
	 MenuInitCHNum(2);		
}


char * MenuInitCH4Num(void)
{	
	 MenuInitCHNum(3);		
}


char * MenuInitCH5Num(void)
{	
	 MenuInitCHNum(4);		
}


char * MenuInitCH6Num(void)
{	
	 MenuInitCHNum(5);		
}


char * MenuInitCH7Num(void)
{	
	 MenuInitCHNum(6);		
}


char * MenuInitCH8Num(void)
{	
	 MenuInitCHNum(7);		
}



//121105 by cq TenChannels
char * MenuInitCH9Num(void)
{
#if (MAXCHANNEL >= 9)
	MenuInitCHNum(8);
#endif
}


//121105 by cq TenChannels
char * MenuInitCH0Num(void)
{
#if (MAXCHANNEL >= 10)
	MenuInitCHNum(9);
#endif
}








