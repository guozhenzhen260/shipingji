//Last Modify Time:03/11/07 01:22
//ReadMe
//屏宽:112
//屏高:64

//#include <intrins.h>
#include <absacc.h>
#include "..\driver\lpc_types.h"
#include "global.h"
#include "pcf8563t.h"
#include "RealTime.h"
#include "function.h"
#include "gbahr1.h"
#include "ITL.h"
#include "hopper.h"
#include "lcd.h"
#include "language.h"
#include "menu.h"
#include "drv.h"
#include "dispmenu.h"
#include "DebugTrace.h"
#include "VMC_PC.h"
#include "acdc.h"


//		   


uint8_t		InLayerNO;
uint8_t		InCHNO;
uint8_t		NowChannel;	 	//当前货道行列号
uint8_t		NowLayerNo;	 	//当前层号
uint8_t		NowLayerAll;
uint8_t   	NowCHNO;		//active channel
uint8_t     m_SetArrayNo;   //传给pc机的当前货道ID

uint32_t	TempNewPassword;
uint32_t	TempNewPassword2;
uint8_t		InputCol;
uint8_t		BusinessNo;


uint8_t		PASSWORD;
uint8_t		DOT;		//小数点使能
uint8_t		FDot;		//小数点标志位
uint8_t		FDot1;
uint8_t		FDot2;
uint8_t		FSave;		  

uint8_t    	(*KeyFuncPtr)(void);    		//按键功能指针
char *  	(*KeyInitPtr)(void);    		//按键初始化指针

uint8_t		CursorLine;
unsigned int  	iMenuID   ;
unsigned int   	iMenuNextID ;
unsigned char 	cMenuTemp1;
unsigned char  	cMenuTemp2;
unsigned char  	cMenuTemp3;

char  		StrPoint[6];
char  		StrInit[22]; 
uint8_t  	InputTest;
uint8_t  	InputMode;
uint8_t  	InputSite ;			//数字显示位置
uint8_t  	offset;
uint32_t  	InputNum;		 	//key input num
uint8_t  	InputDone;			//已进入输入状态  20110823
uint8_t     ChnTestFlag = 0;

char  		strkey[16];




const char *     Menu_WrongPassword[4][1]=
{
 	{
 	
	   	"密码错误"
	},
	 {
 		
		"Password Error!"
 	 },
	 //change by liya 2012-10-16
	 {
	 "Неверный пароль"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Yanl\xf1\x31\xf1\x5f \xf1\x5fifre"
	 }
};


const char *     Menu_ChannelUpdate[4][4]=
{
	 {
	 "添货失败",
	 "货道未初始化",	
	 "货道故障",
	 "超过最大存货量"
	 },
	 {
	 "Load failed",
	 "Refill not allowed",
	 "Refill not allowed",
	 "Exceed the limit"
	 },
	 //change by liya 2012-10-16
	 {
	 "Тим товаров не удается",
	 "Грузовая дорога не включена",
	 "Отказ грузового дорожного",
	 "превышает максимальную"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Tim mal ba\xf1\x5far\xf1\x31s\xf1\x31z",
	 "Kargo Karayolu etkin de\xf1\x1fil",
	 "Kargo Karayolu yetmezli\xf1\x1fi",
	 "maksimumu a\xf1\x5f\xf1\x31yor"
	 }
};

//添加压缩机控制;by gzz 20121224
const char *     Menu_ACDC[4][7]=
{
	 {
	 "up打开, down关闭",
	 "展示灯测试",
	 "展示灯已打开",
	 "展示灯已关闭",
	 "压缩机测试",
	 "压缩机已打开",
	 "压缩机已关闭"
	 },
	 {
	 "up-open,down-close",
	 "Light test",	
	 "Light on",
	 "Light off",
	 "Compressor test",
	 "Compressor on",
	 "Compressor off"
	 },
	 {
	 "up, down",
	 "LCD тест",	
	 "LCD открыто",
	 "LCD близко",
	 "компрессор тест",
	 "открыто",
	 "близко"
	 },
	 {
	 "up, down",
	 "LED testi",	
	 "LED a\xf0\xe7\xf1\x31k",
	 "LED kapal\xf1\x31",
	 "kompres\xf0\xF6r Testi",
	 "Kmpr a\xf0\xe7\xf1\x31k",
	 "Kmpr kapal\xf1\x31"
	 }
};



//显示按键键值 
void keydisplay(uint8_t key)		 //used by keytest
{  
   	
	if(key>9) 
	{			  //ascii
		InputNum = 0;
   		//key = 0;
		offset = 0;
		strkey[0]=key;				
		strkey[1] = 0x00;
		//change by liya 2012-10-16
		if(SYSPara.Language==2)
			Write_LCD(LINE1,strkey,strlen(strkey));
		else
			Write_LCD(InputSite,strkey,strlen(strkey));
	}
    else
	{

      sprintf(strkey+offset,"%x",key);
	  //change by liya 2012-10-16
	  if(SYSPara.Language==2)
			Write_LCD(LINE1,strkey,strlen(strkey));
	  else
      		Write_LCD(InputSite,strkey,strlen(strkey));
   	}
}

//在输入参数的值的页面，比如输入金额等，输入值之后，分别进行组合操作
void keyget(uint8_t key)
{  
   	//Trace("\r\nget key=%d",key);
	if(key>11)
	{		//cancel	    
		InputNum = 0;
   		key = 0;
		offset = 0;
		strcpy(strkey,"        ");	//must be modify
		//Trace("\r\nif key>11,key=%d,Inputsite=%x",strkey,InputSite);
		//change by liya 2012-10-16
		if(SYSPara.Language==2)
			Write_LCD(LINE1,strkey,strlen(strkey));
		else
			Write_LCD(InputSite,strkey,strlen(strkey));
		SetCursorPost(InputSite) ;
	}
    else if((key == 11) )
	{		//back
	 	if (offset >0)	
		{
			if(FDot2 == TRUE)
			{
				FDot2 = FALSE;
				InputNum = (InputNum /10) *10;
			}
			else if(FDot1 == TRUE)
			{
				FDot1 = FALSE;
				InputNum = (InputNum /100) *100; 
			}
			else if(FDot == TRUE)
			{
				FDot = FALSE;
				InputNum = InputNum	/100;
			}
			else
			{
				InputNum = InputNum /10;
			}
			
			sprintf(strkey+(offset--)-1," ");
			//Trace("\r\nif key=11,key=%d,Inputsite=%x",strkey,InputSite);
			//change by liya 2012-10-16
			if(SYSPara.Language==2)
				Write_LCD(LINE1,strkey,strlen(strkey));
			else
				Write_LCD(InputSite,strkey,strlen(strkey));
			SetCursorPost(InputSite+offset/2);
		}
		else
		   InputDone = FALSE;
	}
	else if(offset <(InputCol)) 
	{	  
		if((key == 10) && (FDot == FALSE) && (DOT == TRUE))
		{		//小数点 显示
		 	FDot =  TRUE;
			FDot1 = FALSE;
			FDot2 = FALSE;
			sprintf(strkey+offset++,"%s",".");
			//Trace("\r\nif key=10,key=%d,Inputsite=%x",strkey,InputSite);
			//change by liya 2012-10-16
			if(SYSPara.Language==2)
				Write_LCD(LINE1,strkey,strlen(strkey));
			else
				Write_LCD(InputSite,strkey,strlen(strkey));
		}
		else if((key <10))
		{
		
			InputDone = TRUE;
		/*	if(offset > 5) {	  
				    offset = 0;
					InputNum = 0;
					strcpy(strkey,"        ");	//must be modify
					Write_LCD(InputSite,strkey,strlen(strkey));
			}
		  */ 
			if(FDot == TRUE)	
			{
			  	if(FDot1 == TRUE)	
				{
					if(FDot2 == TRUE)	
					{
						return;
					}
					else
					{
						FDot2 = TRUE;			   //小数点后两位
						InputNum += key;
					}
				}
				else
				{
					FDot1 = TRUE;			   //小数点后一位
					InputNum = InputNum *100 + key*10;
				}
			}
			else
			{
				InputNum = InputNum *10 + key;
	
			}
	
	//   if(CurMenuPtr[CurMenuNo].keypro & 0x40)
	   	  
	      	if (PASSWORD)	
	      		sprintf(strkey+offset++,"%s","*");
	      	else
				sprintf(strkey+offset++,"%x",key);
			//Trace("\r\nif key<10,key=%d,Inputsite=%x",strkey,InputSite);
			//change by liya 2012-10-16
			//添加品途屏幕的兼容;by gzz 20121025
			if( (SYSPara.Language==2)||(SYSPara.LCDSel==2) )
				Write_LCD(LINE1,strkey,strlen(strkey));
			else
		      	Write_LCD(InputSite,strkey,strlen(strkey));
	
			
			
		//			if(offset > (InputCol)) {	  
			
		//		     	MsgBack.keyback =  KEY_ENTER;
		//				OSMboxPost(g_KeyMsg,&MsgBack.keyback);
	//					offset = 0;					   //20110824
			//			InputNum = 0;
			//			strcpy(strkey,"        ");	//must be modify
			//			Write_LCD(InputSite,strkey,strlen(strkey));
		}
	
//		OSTimeDly(OS_TICKS_PER_SEC /4);			   //20110824
   	}
}

/*
void key_set(uint8_t key)
{
   if(CurMenuPtr[CurMenuNo].keypro & 0x80)
   {
      CurMenuPtr[0].valueget = key;
	  CurMenuPtr[1].valueget = key;

	  setok = 1;

	  Cursor_Off();
   }
}

  */

uint8_t  AllChannelStat(uint8_t	layno)	 //当前层号的所有货道状态
{
	uint8_t	j,chno,chno1;
	int8_t n,m;
	char	strtemp1[48],strtemp2[48];
	
	
	
	strtemp1[0] = 0x00;
	ClearScreen();
	if(MAXCHANNEL >4)
		chno = 4;
	else
		chno = MAXCHANNEL;
	chno1 = MAXCHANNEL -chno;
	n =  ChnMapPara.LAYTODRV[layno]-1;
	for(j = 0;j< chno;j++)	
	{
		if(ChnMapPara.CHTODRV[n][j] != 0)	
		{
			m =  ChnMapPara.CHTODRV[n][j]-1;
			if((Channels[n][m].ChannelStat  ) == 0x80)	
				sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSDISABLE][SYSPara.Language]);
			//else if((Channels[n][m].ChannelStat & 0x01) == 0x00)	
			else if((Channels[n][m].ChannelStat  ) == 0x82)
				sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSFAULT][SYSPara.Language]);
			else if((Channels[n][m].ChannelStat  ) == 0x83)	
			{
			    sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
			} 
			else 
			{
				if(Channels[n][m].GoodsNum == 0) 
				{
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
				} 
				else 
				{
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNORMAL][SYSPara.Language]);
				}
			}
			strcat(strtemp1,strtemp2);
		}
	}

	Write_LCD(LINE0,strtemp1,strlen(strtemp1));
	strtemp1[0] = 0x00;
	if (chno1 >0)	
	{
		for(j = 4;j< (chno1+4);j++)	
		{
			if(ChnMapPara.CHTODRV[n][j] != 0)	
			{
				m =  ChnMapPara.CHTODRV[n][j]-1;
				if((Channels[n][m].ChannelStat ) == 0x80)	
					sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSDISABLE][SYSPara.Language]);
			//	else if((Channels[n][m].ChannelStat & 0x01) == 0x00)	
				else if((Channels[n][m].ChannelStat  ) == 0x82)
					sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSFAULT][SYSPara.Language]);
				else if((Channels[n][m].ChannelStat  ) == 0x83)	
				{
			        sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
			    }
				else 
				{
					if(Channels[n][m].GoodsNum == 0) 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
					} 
					else 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNORMAL][SYSPara.Language]);
					}
				}
				strcat(strtemp1,strtemp2);
			}
		}
		
		Write_LCD(LINE1,strtemp1,strlen(strtemp1));
	}

	//添加品途屏幕的兼容;by gzz 20121025
	if(IsLargeLCD())
	{
		layno++;
		strtemp1[0] = 0x00;
		if(MAXCHANNEL >4)
			chno = 4;
		else
			chno = MAXCHANNEL;
		chno1 = MAXCHANNEL -chno;
		n =  ChnMapPara.LAYTODRV[layno]-1;
		for(j = 0;j< chno;j++)	
		{
			if(ChnMapPara.CHTODRV[n][j] != 0)	
			{
				m =  ChnMapPara.CHTODRV[n][j]-1;
				if((Channels[n][m].ChannelStat  ) == 0x80)	
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSDISABLE][SYSPara.Language]);
				//else if((Channels[n][m].ChannelStat & 0x01) == 0x00)	
				else if((Channels[n][m].ChannelStat  ) == 0x82)
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSFAULT][SYSPara.Language]);
				else if((Channels[n][m].ChannelStat  ) == 0x83)	
				{
				    sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
				} 
				else 
				{
					if(Channels[n][m].GoodsNum == 0) 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
					} 
					else 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNORMAL][SYSPara.Language]);
					}
				}
				strcat(strtemp1,strtemp2);
			}
		}

		Write_LCD(LINE2,strtemp1,strlen(strtemp1));
		strtemp1[0] = 0x00;
		if (chno1 >0)	
		{
			for(j = 4;j< (chno1+4);j++)	
			{
				if(ChnMapPara.CHTODRV[n][j] != 0)	
				{
					m =  ChnMapPara.CHTODRV[n][j]-1;
					if((Channels[n][m].ChannelStat ) == 0x80)	
						sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSDISABLE][SYSPara.Language]);
				//	else if((Channels[n][m].ChannelStat & 0x01) == 0x00)	
					else if((Channels[n][m].ChannelStat  ) == 0x82)
						sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSFAULT][SYSPara.Language]);
					else if((Channels[n][m].ChannelStat  ) == 0x83)	
					{
				        sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
				    }
					else 
					{
						if(Channels[n][m].GoodsNum == 0) 
						{
							sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
						} 
						else 
						{
							sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNORMAL][SYSPara.Language]);
						}
					}
					strcat(strtemp1,strtemp2);
				}
			}
			
			Write_LCD(LINE3,strtemp1,strlen(strtemp1));
		}
	}


	return TRUE;
}


uint8_t  AllChannelResult(uint8_t	layno)	 //当前层号的所有货道测试结果
{
	uint8_t	j,chno,chno1;
	int8_t n,m;
	char	strtemp1[48],strtemp2[48];
	
	
	
	strtemp1[0] = 0x00;
	ClearScreen();
	if(MAXCHANNEL >4)
		chno = 4;
	else
		chno = MAXCHANNEL;
	chno1 = MAXCHANNEL -chno;
	n =  ChnMapPara.LAYTODRV[layno]-1;
	for(j = 0;j< chno;j++)	
	{
		if(ChnMapPara.CHTODRV[n][j] != 0)	
		{
			m =  ChnMapPara.CHTODRV[n][j]-1;
			while(1)
			{
				if((Channels[n][m].TestNum  ) > 0)	
				{	
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSDISABLE][SYSPara.Language]);
					break;
				}
				if((m == MAXCHANNEL) || (ChnMapPara.CHTODRV[n][m++] == 0))
				{
					m = 0;
					n++;
					if(( n == MAXLAYER) )
					{
						break;
					}
				}
			
			}
			while(1)
			{
				if((Channels[n][m].TestNum  ) > 0)	
				{	
					sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSDISABLE][SYSPara.Language]);
					break;
				}
				if((m == MAXCHANNEL) || (ChnMapPara.CHTODRV[n][m++] == 0))
				{
					m = 0;
					n++;
					if(( n == MAXLAYER) )
					{
						break;
					}
				}
			
			}
			strcat(strtemp1,strtemp2);
			Write_LCD(LINE1,strtemp1,strlen(strtemp1));
		}
	}

	Write_LCD(LINE0,strtemp1,strlen(strtemp1));
	strtemp1[0] = 0x00;
	if (chno1 >0)	
	{
		for(j = 4;j< (chno1+4);j++)	
		{
			if(ChnMapPara.CHTODRV[n][j] != 0)	
			{
					m =  ChnMapPara.CHTODRV[n][j]-1;
				if((Channels[n][m].ChannelStat ) == 0x80)	
					sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSDISABLE][SYSPara.Language]);
			//	else if((Channels[n][m].ChannelStat & 0x01) == 0x00)	
				else if((Channels[n][m].ChannelStat  ) == 0x82)
					sprintf( strtemp2,"%d%d%s", layno + 1, j + 1,Language[SSFAULT][SYSPara.Language]);
				else if((Channels[n][m].ChannelStat  ) == 0x83)	
				{
			        sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
			    }
				else 
				{
					if(Channels[n][m].GoodsNum == 0) 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNOGOODS][SYSPara.Language]);
					} 
					else 
					{
						sprintf( strtemp2,"%d%d%s", layno+1, j+1,Language[SSNORMAL][SYSPara.Language]);
					}
				}
				strcat(strtemp1,strtemp2);
			}
		}
		
		Write_LCD(LINE1,strtemp1,strlen(strtemp1));
	}


	return TRUE;
}








void	cleardisplay(void)
{
}


InputDisplay( uint8_t pos)	  //显示数值输入
{
 		InputNum = 0 ;
		cleardisplay();	//clear display
		Line_Reverse(0);
		//InputSite = CursorLine + pos;
		//change by liya 2012-10-16
		if(SYSPara.Language==2)
			Write_LCD(LINE1,"       ",strlen("       "));
		else
	    	Write_LCD(InputSite,"       ",strlen("       "));
		FDot =  FALSE;
		SetCursorPost(InputSite)	;
		Cursor_On(LCDUP);   //光标位置
		InputDone = FALSE; 		 //add
	//	OSMboxAccept(g_KeyMsg);           //在需要的地方加这个，可以清掉多按的按键 20110823
		InputMode = TRUE;
}

//管理员模式密码设置;by gzz 20121213
uint8_t isAdminSet(uint8_t NowKey)
{
	static uint8_t  adminSet=0;
	static uint8_t  turkeySet=0;
	if((NowKey==KEY_8)&&(adminSet==0))
	{
		adminSet++;
		turkeySet++;
	}
	else if((NowKey==KEY_3)&&(adminSet==1))
	{
		adminSet++;
		turkeySet++;
	}
	else if((NowKey==KEY_7)&&(adminSet==2))
	{
		adminSet++;
		turkeySet++;
	}
	else if((NowKey==KEY_1)&&(adminSet==3))
	{
		adminSet++;
		turkeySet++;
	}
	else if((NowKey==KEY_8)&&(adminSet==4))
	{
		adminSet++;
		turkeySet++;
	}
	else if((NowKey==KEY_5)&&(adminSet==5))
	{
		adminSet++;
		turkeySet=0;
	}
	else if((NowKey==KEY_5)&&(adminSet==6))
	{
		adminSet++;
		turkeySet=0;
	}
	else if((NowKey==KEY_7)&&(adminSet==7))
	{
		adminSet=0;
		turkeySet=0;
		if(SYSPara1.AdminStat != 1)
		{
			SYSPara1.AdminStat = 1;
		}
		else
		{
			SYSPara1.AdminStat = 0;
		}
		return 1;
	}	
	else if((NowKey==KEY_8)&&(turkeySet==5))
	{
		adminSet=0;
		turkeySet++;
		//Trace("\r\n turkeySet6");	
	}
	else if((NowKey==KEY_8)&&(turkeySet==6))
	{
		adminSet=0;
		turkeySet++;
		//Trace("\r\n turkeySet7");	
	}
	else if((NowKey==KEY_8)&&(turkeySet==7))
	{
		adminSet=0;
		turkeySet=0;
		//Trace("\r\n we get turkeyset");	
		RstTurkeySystemPara();
		RstTurkeyVisiblePara();
		JudgeHLight(KEY_DOWN);
	}
	else
	{
		adminSet = 0;
		turkeySet=0;
	}
	return 0;
}



void menu(void)				 //维护主菜单
{
	uint8_t err,mode = 0;
	uint8_t *NowKey;
	uint8_t KeyTab_TempVisible;


// BitValue=XBYTE[Addr_Key];
// bOldOnline=(~bOnline);
// b_KeyInt=0;
// b_ReceiveOver=0;
// b_Arrive1S=0;
 //
// DispFaceBmp();
	 //俄罗斯文;by gzz 20121012
	 iMenuNextID=MENU_CHANNEL;
	 iMenuID=MENU_CHANNEL;
	
//	 iMenuNextID=MENU_PASSWORD;
//	 iMenuID=MENU_PASSWORD;
	 KeySystemInit(1);   //用户菜单初始化//配置用户菜单可见;by gzz 20121204
	 RstTurkeyVisibleParaInit();
	 JudgeHLight(KEY_ENTER);
	 InputNum = 0;
	 InputDone = FALSE;	
//	 InputMode = FALSE;
	 offset = 0;
	 InputCol = 8;
	 InputTest = FALSE;
	 DOT = TRUE;
	 PASSWORD = FALSE;
	 NowLayerAll = FALSE;
	 HoldNoteInit();	//holdnote 菜单初始化
	 while(1)
	 {
	// ReadyForKey();
	 //
	// BitValue=XBYTE[Addr_Key];
			//120621 by cq KeySlowly
		  	//NowKey = OSMboxPend(g_KeyMsg,0,&err);		//等待键盘输入
			NowKey = OSQPend(g_KeyMsg,0,&err);		//等待键盘输入

			if (err ==OS_NO_ERR) 
			{   
			    //键盘测试模式，用于测试按键可否使用
			  	if(InputTest == KEYTEST)	
				{	  
					 	switch(*NowKey)
				   		{	
							case KEY_ENTER ://KEY_OK,  KEY_UP,  KEY_ESC,
							      /*
								  KeyFuncPtr=KeyTab[iMenuNextID].CurrentOperate;
							      if( ((*KeyFuncPtr)()) ==  TRUE)	{    //执行当前按键的操作
								 	if(KeyTab[iMenuNextID].KeyTab_PressOk!=999)
								      {
									      iMenuID=iMenuNextID;
									      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressOk;
									      JudgeHLight();
								      }
							      }
							     InputNum = 0;
								 offset = 0;
								 */
								 keydisplay(0x2f);	  //^↑↓√ 	
								 break;
					   		case KEY_UP:
							   	  keydisplay(0x18);	  //^↑↓√ 	
								   break;
							case KEY_CANCEL:
							      if(KeyTab[iMenuNextID].KeyTab_PressEsc!=999)
							      {
								      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressEsc;
								    	JudgeHLight(KEY_CANCEL);
							      }
								 InputTest  = 0x00;
								 break;
					     
						   	case KEY_DOWN:
							     keydisplay(0x19);	  //^↑↓√ 	
								 break;
						   	case KEY_1://KEY_1,  KEY_2,  KEY_3,
							 	 keydisplay(1);
								 break;
						   	case KEY_2:
							 	 keydisplay(2);
								 break;
							case KEY_3:
							  	 keydisplay(3);
								 break;
							case KEY_4://KEY_4,  KEY_5,  KEY_6,
							  	 keydisplay(4);
								 break;
							case KEY_5:
							 	 keydisplay(5);
								 break;
							case KEY_6:
							  	 keydisplay(6);
								 break;
							case KEY_7://KEY_7,  KEY_8,  KEY_9,
							 	 keydisplay(7);
								 break;
							case KEY_8:
								 keydisplay(8);
								 break;
							case KEY_9:
								 keydisplay(9);
								 break;
							case KEY_0:
							 	 keydisplay(0);
								 break;
							case KEY_DOT :
								 keydisplay(0x2e);
								 break;
							case KEY_SET :
								 keydisplay(0x21);
								 break;
							case KEY_NULL:
							  	 break;
							default:
							     break;
					   }
				}
				//货道状态查阅模式
				else if(InputTest == KEYLOOP)	
				{	  
						switch(*NowKey)
					   	{	
							case KEY_ENTER ://KEY_OK,  KEY_UP,  KEY_ESC,
							       break;
					   		case KEY_UP:
							   	   	if (NowLayerNo >0 )
									{
										NowLayerNo --;
										
										AllChannelStat(NowLayerNo);
								   	}
									break;
							case KEY_CANCEL:
							     	InputTest =0x00;
								  	 //
								     if(KeyTab[iMenuNextID].KeyTab_PressEsc!=999)
								     {
									     // iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressEsc;
									    //  iMenuNextID=KeyTab[iMenuID].KeyTab_PressOk;
									      JudgeHLight(KEY_CANCEL);
									  //    KeyFuncPtr=KeyTab[iMenuID].CurrentOperate;
									   //      (*KeyFuncPtr)();    //执行当前按键的操作
								     }
								     
									 break;
									 
						   	case KEY_DOWN:
							     	if (NowLayerNo < (MAXLAYER - 1) )
									{
										if(	ChnMapPara.LAYTODRV[NowLayerNo +1] != 0) 
										{
											NowLayerNo ++;
											AllChannelStat(NowLayerNo);
										}
								   	}
									break;
					     
						   	case KEY_1://KEY_1,  KEY_2,  KEY_3,
							case KEY_2:
							case KEY_3:
							case KEY_4://KEY_4,  KEY_5,  KEY_6,
							case KEY_5:
							case KEY_6:
							case KEY_7://KEY_7,  KEY_8,  KEY_9,
							case KEY_8:
							case KEY_9:
							case KEY_0:
							default:
							     break;
					   }
				}
				else
				{
				    //数字输入状态,这个是用来输入一些数字，比如数量等之后，再更新页面的
					if(InputMode )	
					{	
					  	FSave = TRUE;
						switch(*NowKey)
					   	{
					   		case KEY_ENTER ://KEY_OK,  KEY_UP,  KEY_ESC,
							      KeyFuncPtr=KeyTab[iMenuNextID].CurrentOperate;//先执行具体的操作函数
							      if( ((*KeyFuncPtr)()) ==  TRUE)	
								  {    //执行当前按键的操作
								 		if(KeyTab[iMenuNextID].KeyTab_PressOk!=999)//如果按下Enter后，里面有子菜单，就进入子菜单
									      {
										      iMenuID=iMenuNextID;
										      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressOk;
										     
									      }
								      	JudgeHLight(KEY_ENTER);
										InputNum = 0;
									 	offset = 0;
								  
								  }
							      else
								  {
									 strcpy(strkey,"E   ");	//must be modify
									 //change by liya 2012-10-16
									 if(SYSPara.Language==2)
									 	Write_LCD(2,strkey,strlen(strkey));
									 else
									 	Write_LCD(InputSite,strkey,strlen(strkey));
									 OSTimeDly(OS_TICKS_PER_SEC /2);
									 keyget(12);	 //复位清零
									 offset = 0;
									 //添加品途屏幕的兼容;by gzz 20121025
									if(SYSPara.LCDSel==2) 
										Pitu_LCDClearLine(LINE1,0,0);
								  }
								 break;
					   		case KEY_UP:
							   	   break;
							case KEY_CANCEL:
							     if (InputDone == FALSE )
								 {		//20110823		 
								//	 if(KeyTab[iMenuNextID].KeyTab_PressEsc!=999)		 //20110824
								  //    {
									 if(KeyTab[iMenuNextID].KeyTab_MaxItems == 1)		 //20110824	  单项菜单
									 		 
									// 	if(	iMenuNextID == MENU_INPUTNUM )	
									 		iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressEsc;
									   	 	
											Cursor_Off(LCDUP)	;
										  	InputCol = 8;
											InputMode	 = FALSE;
									 	   	PASSWORD = FALSE;
											JudgeHLight(KEY_CANCEL);
									  	   
								    //  }
							     }
								 else
								 {
									 InputDone = FALSE;		  //20110823
									 InputNum = 0;
									 offset = 0;
									 keyget(12);	 //复位清零
								 }
								 break;
					     	case KEY_CLR :
								   keyget(11);
								   break;
						   	case KEY_DOWN:
							     break;
						   	case KEY_1://KEY_1,  KEY_2,  KEY_3,
							 //    InputNum = 1;
								 keyget(1);
								 break;
						   	case KEY_2:
							  //   InputNum = 2;
								 keyget(2);
								 break;
							case KEY_3:
							  //   InputNum = 3;
								 keyget(3);
								 break;
							case KEY_4://KEY_4,  KEY_5,  KEY_6,
							  //   InputNum = 4;
								 keyget(4);
								 break;
							case KEY_5:
							  //   InputNum = 5;
								 keyget(5);
								 break;
							case KEY_6:
							  //   InputNum = 6;
								 keyget(6);
								 break;
							case KEY_7://KEY_7,  KEY_8,  KEY_9,
							 //    InputNum = 7;
								 keyget(7);
								 break;
							case KEY_8:
							 //    InputNum = 8;
								 keyget(8);
								 break;
							case KEY_9:
							 //    InputNum = 9;
								 keyget(9);
								 break;
							  
							case KEY_0:
							 //    InputNum = 0;
								 keyget(0);
								 break;
							case KEY_DOT :
								 keyget(10);
								 break;
							case KEY_NULL:
							  	 break;
							default:
							     break;
					    }
					}
					//命令状态状态,这个是用来选择菜单的
					else
					{						//
					   switch(*NowKey)
					   {
					   		case KEY_ENTER ://KEY_OK,  KEY_UP,  KEY_ESC,
							      KeyFuncPtr=KeyTab[iMenuNextID].CurrentOperate;
							      if( ((*KeyFuncPtr)()) ==  TRUE)	
								  {    //执行当前按键的操作
								 
									  if(KeyTab[iMenuNextID].KeyTab_PressOk!=999)
								      {
									      iMenuID=iMenuNextID;
									      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressOk;
									      JudgeHLight(KEY_ENTER);
								      }
							      }
							     InputNum = 0;
								 break;
					   		case KEY_UP:
							     if((KeyTab[iMenuNextID].KeyTab_PressUp!=999)&&(iMenuNextID!=999))
							     {
								   //   KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressUp;
								      //用户模式菜单;by gzz 20121130
								      if(SYSPara1.AdminStat != 1)
								      {
									   	  KeyTab_TempVisible = KeyTab[iMenuNextID].KeyTab_PressUp;
									      //Trace("\r\n->%d",KeyTab_TempVisible);
									   	  while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999) )
									   	  {
									   	  	KeyTab_TempVisible = KeyTab[KeyTab_TempVisible].KeyTab_PressUp;
											//Trace("\r\n->%d",KeyTab_TempVisible);
									   	  }	
										  iMenuNextID=KeyTab_TempVisible;
									      //iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressUp;
									      JudgeHLight(KEY_UP);
								      }
									  else
									  {
									   //   KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressUp;
									      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressUp;
									      JudgeHLight(KEY_UP);
								      }
							      }
							     break;
							case KEY_CANCEL:
							     //
							     if(KeyTab[iMenuNextID].KeyTab_PressEsc!=999)
							     {
								      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressEsc;
								    //  iMenuNextID=KeyTab[iMenuID].KeyTab_PressOk;
								      JudgeHLight(KEY_CANCEL);
								  //    KeyFuncPtr=KeyTab[iMenuID].CurrentOperate;
								   //      (*KeyFuncPtr)();    //执行当前按键的操作
							     }
							     else
								 {
						//			  	if(FSave)	{		 //保存标志
											/*
											if(	iMenuNextID ==	 MENU_CHANNEL)	{		 //保存货道参数
												 XChannels(WRITE);
											}
											if(	iMenuNextID ==	 MENU_DRIVER)	{
		
											}
											if(	iMenuNextID ==	 MENU_BUSINESS)	{
												 XSystemPara(WRITE);
											}										
											if(	iMenuNextID ==	 MENU_TRANSACTIONS)	{
												 XTransaction(WRITE);
											}
											if(	iMenuNextID ==	 MENU_SETCHANNEL)	{		 //保存货道参数
												 XChannels(WRITE);
												 XSystemPara(WRITE);
											}						
											*/
											XChannels(WRITE);
											XSystemPara(WRITE);
											XSystemPara2(WRITE);
											XColSystemPara(WRITE);//按货道添存货量;by gzz 20121120
											//配置用户菜单可见;by gzz 20121204
											KeySystemInit(2);   //用户菜单初始化
											XKeySystemVisible(WRITE);
											XTransaction(WRITE);
											SYSPara1.AdminStat = 0;
											
											ClearScreen();
											//添加品途屏幕的兼容;by gzz 20121025
											if(IsLargeLCD())
											{
												Write_LCD((uint8_t)LINE0,"",0);
												Write_LCD((uint8_t)LINE1,"",0);
												Write_LCD((uint8_t)LINE2,"",0);
												Write_LCD((uint8_t)LINE3,(char *)Language[SLSAVEOK][SYSPara.Language],strlen(Language[SLSAVEOK][SYSPara.Language])); 
											}
											else
											{
												Write_LCD((uint8_t)LINE0,(char *)Language[SLSAVEOK][SYSPara.Language],strlen(Language[SLSAVEOK][SYSPara.Language]));
												Write_LCD((uint8_t)LINE1,"",0);
												Write_LCD((uint8_t)LINE2,"",0);
												Write_LCD((uint8_t)LINE3,"",0); 
											}
						    				OSTimeDly(OS_TICKS_PER_SEC );
											//俄罗斯文;by gzz 20121012
											iMenuNextID=MENU_CHANNEL;
											iMenuID=MENU_CHANNEL;
											
										
										//	JudgeHLight();
							//			}
								  		mode = 1;	//exit loop  退出维护菜单
										FSave = FALSE;
								 }
								 break;
					     
						   	case KEY_DOWN:
							     if((KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999))
								 {
								   //   KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressDown;

								   	  //用户模式菜单;by gzz 20121130
								   	  if(SYSPara1.AdminStat != 1)
								      {
									   	  KeyTab_TempVisible = KeyTab[iMenuNextID].KeyTab_PressDown;
									      //Trace("\r\n->%d",KeyTab_TempVisible);
									   	  while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999) )
									   	  {
									   	  	KeyTab_TempVisible = KeyTab[KeyTab_TempVisible].KeyTab_PressDown;
											//Trace("\r\n->%d",KeyTab_TempVisible);
									   	  }							  
										  iMenuNextID=KeyTab_TempVisible;
										  //iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressDown;
									      JudgeHLight(KEY_DOWN);
								   	  }
									  else
									  {
									   //   KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressDown;
									      iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressDown;
									      JudgeHLight(KEY_DOWN);
								      }
							      }
							     break;
							//配置用户菜单可见;by gzz 20121204	 
							case KEY_DOT: 
								 if(SYSPara1.AdminStat == 1)
								 {
								 	if(SYSPara1.VisibleStat == 0)
								 	{
										SYSPara1.VisibleStat = 1;
								 	}
									else
									{
										SYSPara1.VisibleStat = 0;
									}
								 }
								 JudgeHLight(KEY_ENTER);
								 break; 
							//配置用户菜单可见;by gzz 20121204 
							case KEY_SET:
								 if( (SYSPara1.VisibleStat == 1)&&(SYSPara1.AdminStat == 1) )
								 {
									//Trace("\r\n visible= %d",KeyTab[iMenuNextID].KeyTab_Visible);
									if(KeyTab[iMenuNextID].KeyTab_Visible == MENU_VISIBLE_FAUSE)
									{
										KeyTab[iMenuNextID].KeyTab_Visible = MENU_VISIBLE_TRUE;
									}
									else 
									{
										KeyTab[iMenuNextID].KeyTab_Visible = MENU_VISIBLE_FAUSE;
									}
								 }
								 JudgeHLight(KEY_ENTER);
								 break;		  
						   	case KEY_1://KEY_1,  KEY_2,  KEY_3,//管理员模式密码设置;by gzz 20121213
						   		isAdminSet(*NowKey);
								break;
						   	case KEY_2:
							case KEY_3://管理员模式密码设置;by gzz 20121213
								isAdminSet(*NowKey);
								break;
							case KEY_4://KEY_4,  KEY_5,  KEY_6,
							case KEY_5://管理员模式密码设置;by gzz 20121213
								isAdminSet(*NowKey);
								break;
							case KEY_6:
							case KEY_7://KEY_7,  KEY_8,  KEY_9,//管理员模式密码设置;by gzz 20121213
								if(isAdminSet(*NowKey)==1)
								{
									JudgeHLight(KEY_ENTER);
								}
								break;
							case KEY_8://管理员模式密码设置;by gzz 20121213
								isAdminSet(*NowKey);
								break;
							case KEY_9:
							case KEY_0:
							case KEY_NULL:
							     break;
							default:
							     break;
					   }
				    }
				    if (  mode ==  1)  
				    {
						break;				  //exit loop  退出维护菜单
				    }
				}
	  		}

 	}
}


char  *DecimalPoint( uint32_t money)		  //价格小数点显示
{
	uint8_t i;

	i = money %100 ;
	if(	SYSPara.Decimal == 0)	 
	{
		sprintf( StrInit,"%d", money) ;
	}	
	if(	SYSPara.Decimal == 1)  
	{
		sprintf( StrInit,"%d.%d", money/10,money%10);
	}
	if(	SYSPara.Decimal == 2) 
	{
		sprintf( StrInit,"%d.%d%d",money/100, i/10,i%10);
	}
	return	StrInit;
}


/*****************************************************************************
** Function name:	MenuFunctionPW	
**
** Descriptions:	对密码，进行判断是否正确 	
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
//120729 by cq RecordPassword /**/
uint8_t  MenuFunctionPW2(void)					//密码
{
//char strtemp[16];
	if (InputNum == 0)
		return FALSE;
	else 
	{
		//Trace("\r\n pwd=%d",SYSPara.Password);
		if(InputNum == SYSPara.Password2)
		{
			InputMode = FALSE;
			PASSWORD = FALSE;
			Cursor_Off(LCDUP);
			return TRUE;
		}
		else 
		{
			//Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[Language][0][0],strlen(&Menu_WrongPassword[Language][0][0])); 
			Cursor_Off(LCDUP);
			Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[SYSPara.Language][0][0],strlen(Menu_WrongPassword[SYSPara.Language][0]));
			
			OSTimeDly(OS_TICKS_PER_SEC );
			JudgeHLight(KEY_ENTER);
		//								strcpy(strtemp,MenuNULL);
		//	DispTextLine(2,strtemp,0);
			return FALSE;
		}
	}
}



/*****************************************************************************
** Function name:	MenuFunctionPW	
**
** Descriptions:	对季度密码，进行判断是否正确 	
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
//用于季度结账用;by gzz 20121009
uint8_t  MenuFunctionPW3(void)					//密码
{
//char strtemp[16];
	if (InputNum == 0)
		return FALSE;
	else 
	{
		//Trace("\r\n pwd=%d",SYSPara.Password);
		if(InputNum == SYSPara.Password3)
		{
			InputMode = FALSE;
			PASSWORD = FALSE;
			Cursor_Off(LCDUP);
			return TRUE;
		}
		else 
		{
			//Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[Language][0][0],strlen(&Menu_WrongPassword[Language][0][0])); 
			Cursor_Off(LCDUP);
			Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[SYSPara.Language][0][0],strlen(Menu_WrongPassword[SYSPara.Language][0]));
			
			OSTimeDly(OS_TICKS_PER_SEC );
			JudgeHLight(KEY_ENTER);
		//								strcpy(strtemp,MenuNULL);
		//	DispTextLine(2,strtemp,0);
			return FALSE;
		}
	}
}



//用户模式菜单;by gzz 20121130
uint8_t  MenuFunctionPWAdmin(void)					//密码
{
//char strtemp[16];
	if (InputNum == 0)
		return FALSE;
	else 
	{
		//Trace("\r\n pwd=%d",SYSPara.Password);
		if(InputNum == SYSPara.Password2)
		{
			InputMode = FALSE;
			PASSWORD = FALSE;
			Cursor_Off(LCDUP);
			return TRUE;
		}
		else 
		{
			//Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[Language][0][0],strlen(&Menu_WrongPassword[Language][0][0])); 
			Cursor_Off(LCDUP);
			Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[SYSPara.Language][0][0],strlen(Menu_WrongPassword[SYSPara.Language][0]));
			
			OSTimeDly(OS_TICKS_PER_SEC );
			JudgeHLight(KEY_ENTER);
		//								strcpy(strtemp,MenuNULL);
		//	DispTextLine(2,strtemp,0);
			return FALSE;
		}
	}
}



uint8_t  MenuFunctionPW(void)					//密码
{
//char strtemp[16];
	if (InputNum == 0)
		return FALSE;
	else 
	{
		//Trace("\r\n pwd=%d",SYSPara.Password);
		if(InputNum == SYSPara.Password)
		{
			InputMode = FALSE;
			PASSWORD = FALSE;
			Cursor_Off(LCDUP);
			return TRUE;
		}
		else 
		{
			//Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[Language][0][0],strlen(&Menu_WrongPassword[Language][0][0])); 
		 	Cursor_Off(LCDUP);
			Write_LCD((uint8_t)LINE1,&Menu_WrongPassword[SYSPara.Language][0][0],strlen(Menu_WrongPassword[SYSPara.Language][0]));
			
			OSTimeDly(OS_TICKS_PER_SEC );
			JudgeHLight(KEY_ENTER);
		//								strcpy(strtemp,MenuNULL);
		//	DispTextLine(2,strtemp,0);
			return FALSE;
		}
	}
}

//使用英文界面
uint8_t  MenuFunctionENG(void)			
{
	SYSPara.Language = 1;
	return TRUE;
}
//
//
//使用中文界面
uint8_t  MenuFunctionCHN(void)			
{
	SYSPara.Language = 0;
	return TRUE;
}
//
//使用俄罗斯文界面//俄罗斯文;by gzz 20121012
uint8_t  MenuFunctionRussian(void)			
{
	SYSPara.Language = 2;
	//LCD_GraphCleanScreen();		
	ClearScreen();
	return TRUE;
}

//土耳其语;by gzz 20121115
uint8_t  MenuFunctionTurkey(void)			
{
	SYSPara.Language = 3;
	//LCD_GraphCleanScreen();		
	ClearScreen();
	return TRUE;
}


uint8_t  MenuFunctionDemo1(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionCHIN(void)			//对输入的货道号转换成物理货道号
{
	uint8_t   channel,ChannelX,ChannelY,x,y;
	uint8_t   i=0;
	
	if (InputNum == 0)
		return FALSE;
	else 
	{
	//	if(InputNum  > MAXCHANNEL) 
	//		InputNum = 1;
		NowChannel = InputNum  ;	    
	//	NowLayerNo = (uint8_t)(InputNum /10);
  		if(DRV_ChnChk(InputNum,&NowLayerNo,&NowCHNO) == FALSE) 
		{
       		
			InputDone = FALSE;		  //20110823
			return FALSE;
		}

		//得到向pc上报的货道编号
		for(x = 0; x<MAXLAYER;x++)	  
		{			
			for(y=0; y<MAXCHANNEL;y++) 
			{
				channel = x*10+y+11;
				if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
				{
					if(channel != NowChannel)
					{
						i++;
					}
					else
					{
						m_SetArrayNo = i;
					}
				}			    
			}
		}
		
		Cursor_Off(LCDUP);
		InputMode = FALSE;
		DOT = TRUE;
		InputCol = 8;

		return TRUE;
	}
}
//
void ChannelTest(uint8_t channel)		   //货道测试
{
	uint32_t  ChannelOutput,*pComStatus;
	uint8_t   err,status;
	

	if(DRV_ChnChk(channel,&NowLayerNo,&NowCHNO) == TRUE) 
	{
		Cursor_Off(LCDUP);
		ChannelOutput = channel + ((uint32_t)CMD_GOODS_OUT << 24);
		MsgBack.CmdType = ChannelOutput;
		OSMboxAccept(g_CommBackMsg);
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);				//出货
		sprintf( StrInit,Language[SLTESTING][SYSPara.Language], channel);
		Write_LCD(LINE0,StrInit , strlen(StrInit));
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 20,&err);
		
		status = *pComStatus;

		#if DRV_TYPE == DRV_64
		status &= ~DRV_GOCERR_BIT;
		#endif

		ClearScreen();
		if(err == OS_NO_ERR) 
		{   
		   if(status & DRV_ERR_MASK) 
		   {
		      sprintf( StrInit,Language[CHBAD][SYSPara.Language], channel);//sprintf( StrInit,"货道:%d故障", channel);
			  Write_LCD(LINE0,StrInit , strlen(StrInit));

			  if(ChnTestFlag == 1) 
			  {
			      Channels[NowLayerNo][NowCHNO].TestNum++;
			  }

	   #if DRV_TYPE == DRV_64
		   }
		   else if(status & (GOODS_SOLDOUT_BIT|GOODS_NOTPASS_BIT)) 
		   {
	   #elif DRV_TYPE == DRV_12
		   } 
		   else if(status & GOODS_SOLDOUT_BIT) 
		   {
	   #endif
		      sprintf( StrInit,Language[CHEMPTY][SYSPara.Language], channel);//sprintf( StrInit,"货道:%d无货", channel);
			  Write_LCD(LINE0,StrInit , strlen(StrInit));
			  
		   } 
		   else 
		   {
		       sprintf( StrInit,Language[CHOK][SYSPara.Language], channel);
			 // sprintf( StrInit,"货道:%d正常", channel);
			  Write_LCD(LINE0,StrInit , strlen(StrInit));
		   }
		 //  Write_LCD(LINE0,Language[SLCHANNELOK][SYSPara.Language],strlen(Language[SLCHANNELOK][SYSPara.Language]));
		} 
		else 
		{
		   Write_LCD(LINE0,Language[SLCHANNELBAD][SYSPara.Language],strlen(Language[SLCHANNELBAD][SYSPara.Language]));
		   if(ChnTestFlag == 1)
		   {
		       Channels[NowLayerNo][NowCHNO].TestNum++;
		   }
		}
		Cursor_Off(LCDUP);
		
	}
	Write_LCD(LINE1,"                    ",strlen("                    "));
//	OSTimeDly(OS_TICKS_PER_SEC *2);	
	

}

uint8_t  MenuFunctionTESTCHIN(void)			 //货道测试输入
{
	uint8_t i,j;
	uint16_t 	*PKeyInput;
	uint16_t	KeyInput;	
	uint8_t   err;
	uint16_t   timeout;
	uint8_t   channel;

	if (InputNum == 99)	
	{
		SYSPara1.CHTestNO = 0;
		for(i = 0; i<MAXLAYER;i++)	  
		{
			for(j=0; j<MAXCHANNEL;j++) 
			{
			    Channels[i][j].TestNum = 0;
			}
		}
		ChnTestFlag = 1;
		while(1)
		{
			SYSPara1.CHTestNO++;			
			for(i = 0; i<MAXLAYER;i++)	  
			{
				for(j=0; j<MAXCHANNEL;j++) 
				{
					channel = i*10+j+11;

					//121105 by cq TenChannels
					if((channel >= 20) && (channel % 10 == 0))
						channel -= 10;
					
					ChannelTest(channel);
			
					if(DRV_ChnChk(channel,&NowLayerNo,&NowCHNO) == TRUE) 
					{
					    timeout = OS_TICKS_PER_SEC * 2;
					} 
					else 
					{
					    timeout = 1;
					}

					//120621 by cq KeySlowly
					//PKeyInput = OSMboxPend(g_KeyMsg,timeout,&err);					
				  	PKeyInput = OSQPend(g_KeyMsg,timeout,&err);

					KeyInput = *PKeyInput;
					if(err == OS_NO_ERR) 
					{   
						if((KeyInput == KEY_ENTER)||(KeyInput == KEY_CANCEL)) 
						{
					    	Cursor_Off(LCDUP);
							InputMode = FALSE;
							ChnTestFlag = 0;
							if( SYSPara.PCEnable == 1 )
							{
								VPMission_ColumnSta_RPT();
							}							
							return TRUE;
						}
					}		
				}
			}		
		}
//		ChnTestFlag = 0;
	
	}
	else 
	{
	//	if(InputNum  > MAXCHANNEL) 
	//		InputNum = 1;
		NowChannel = InputNum  ;	    
	//	NowLayerNo = (uint8_t)(InputNum /10);
  		if(DRV_ChnChk(NowChannel,&NowLayerNo,&NowCHNO) == FALSE) 
       	{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
		
		ChannelTest(NowChannel);
		OSTimeDly(OS_TICKS_PER_SEC *2);	
	}
	Cursor_Off(LCDUP);
	InputMode = FALSE;

	//120621 by cq KeySlowly
	//OSMboxAccept(g_KeyMsg);           //在需要的地方加这个，可以清掉多按的按键 20110823
	OSQAccept(g_KeyMsg);           //在需要的地方加这个，可以清掉多按的按键 20110823

	if( SYSPara.PCEnable == 1 )
	{
		VPMission_ColumnSta_RPT();
	}
	return TRUE;

}
//
uint8_t MenuFunctionCHTestResult(void)
{
 	//所有货道测试结果

	InputTest   = KEYLOOP;
	NowLayerNo = 0;
	AllChannelResult(NowLayerNo);
	
	return TRUE;


}


uint32_t  DecimalPointInput( uint32_t money)		   //小数点输出
{
	uint32_t i;


	if(	SYSPara.Decimal == 0)	 
	{
			i = money /100 ;
	}	
	if(	SYSPara.Decimal == 1)  
	{
		i = money /10 ;
	}
	if(	SYSPara.Decimal == 2) 
	{
		i = money  ;
	}
	return	i;
}

//选货按键;by gzz 20121025
//当修改商品单价时选1，当修改商品编号时选2
uint8_t  UpdateChannelPrice(uint8_t pricemode)
{
	unsigned char x = 0;
	unsigned char y = 0;
	uint8_t   channel,ChannelX,ChannelY;
	uint32_t  price;

	if(pricemode==1)
	{
		price = Channels[NowLayerNo][NowCHNO].GoodsPrice;
	}
	else if(pricemode==2)
	{
		for(x = 0; x<MAXLAYER;x++)	  
		{					
			for(y=0; y<MAXCHANNEL;y++) 
			{  
				channel = x*10+y+11;
				if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
				{
					if( (Channels[ChannelX][ChannelY].GoodsCode  ==	Channels[NowLayerNo][NowCHNO].GoodsCode ) && (!((ChannelX == NowLayerNo)&&(ChannelY == NowCHNO))) )
					{
						Trace("\r\n channel=%d",channel);
						price = Channels[ChannelX][ChannelY].GoodsPrice;
					}	
				} 
			}		
		}	
	}
		
	//Trace("\r\n taskb key=%d",key);
	for(x = 0; x<MAXLAYER;x++)	  
	{					
		for(y=0; y<MAXCHANNEL;y++) 
		{  
			channel = x*10+y+11;
			if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
			{
				if( Channels[ChannelX][ChannelY].GoodsCode  ==	Channels[NowLayerNo][NowCHNO].GoodsCode )
				{
					Channels[ChannelX][ChannelY].GoodsPrice  =	price;
				}	
			} 
		}		
	}	

}


//
uint8_t  MenuFunctionCHPR(void)							//channel price
{
	uint32_t  oldPrice = 0;
	if (InputMode)
	{
		Cursor_Off(LCDUP)	;
		InputCol = 8;
		InputMode = FALSE;

	    oldPrice = Channels[NowLayerNo][NowCHNO].GoodsPrice;
		 

		if( SYSPara.PCEnable == 1 )
		{
		    VPChangeColPrice( m_SetArrayNo, (InputNum), oldPrice );	
			//if(VPChangeColPrice( m_SetArrayNo, (InputNum), oldPrice ))
			//{
				/*****************************************/
				//无小数点直接输入方式
				/******************************************/
				 //Channels[NowLayerNo][NowCHNO].GoodsPrice	= (InputNum);
				
				/******************************************/
				//小数点输入方式
				/******************************************/
				/*
					if(FDot == TRUE)
						Channels[NowLayerNo][NowCHNO].GoodsPrice	= DecimalPointInput(InputNum);
					else
						Channels[NowLayerNo][NowCHNO].GoodsPrice	= DecimalPointInput(InputNum*100);
				*/
				//VPMission_ColumnSta_RPT();
			//}	
		}
		else
		{
			/*****************************************/
			//无小数点直接输入方式
			/******************************************/
			 Channels[NowLayerNo][NowCHNO].GoodsPrice	= (InputNum);

			//选货按键;by gzz 20121025
			if(SYSPara.KeySel == 1 )
			{
				UpdateChannelPrice(1);
			}
			
			/******************************************/
			//小数点输入方式
			/******************************************/
			/*
				if(FDot == TRUE)
					Channels[NowLayerNo][NowCHNO].GoodsPrice	= DecimalPointInput(InputNum);
				else
					Channels[NowLayerNo][NowCHNO].GoodsPrice	= DecimalPointInput(InputNum*100);
			*/
		}
		
		//	XChannels(WRITE);
	}
	else
	{
				InputCol = 5;
			InputDisplay( 0x07)	 ;
	}

	return TRUE;
}
//
//
uint8_t  MenuFunctionCHST(void)						//	货道状态
{
//	uint8_t ChannelState[5] = {1,0,2,2,4};
    uint8_t GoodsNumChange=0;
	uint8_t  oldNum = 0;

	
	if (InputMode)
	{
		
		//if(InputNum <=4) {
		if((InputNum <=3))  			
		{			 //货道状态0-4						
			Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
			Channels[NowLayerNo][NowCHNO].ChannelStat 	|= InputNum & 0x03;
			//Trace("\r\n change state,%d,%d,%d",NowLayerNo,NowCHNO,m_SetArrayNo);
			if(((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELDISABLE)||((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELHARDERR)||((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOVER))
			{
				//Trace("\r\n change num");
				GoodsNumChange=1;
				oldNum=Channels[NowLayerNo][NowCHNO].GoodsNum;
				
				//121108 by cq DontReduceNum
				if(!isDontReduceNum())
				{
					Channels[NowLayerNo][NowCHNO].GoodsNum=0;
				}
			}
			if(((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOK)&&(Channels[NowLayerNo][NowCHNO].GoodsNum==0))
			{
				Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
				Channels[NowLayerNo][NowCHNO].ChannelStat 	|= CHANNELOVER;
			}
			if( SYSPara.PCEnable == 1 )
			{
				//修改存货数量
				if(GoodsNumChange==1)
				{
					VPAddSingleColGoods( m_SetArrayNo, Channels[NowLayerNo][NowCHNO].GoodsNum, oldNum, 1 );
				}
				//上传货道状态
				VPMission_ColumnSta_RPT();
			}			
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
		InputCol = 1;
		InputDisplay( 0x07)	 ;
	}

	return TRUE;
}
uint8_t  MenuFunctionCHLF(void)						//	货道存货
{
	uint32_t  oldNum = 0;
	if (InputMode)
	{
		
		if((InputNum <=MAXGOODS))  
		{				 //货道存货0-16
			//Channels[NowLayerNo][NowCHNO].GoodsNum	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;

			oldNum = Channels[NowLayerNo][NowCHNO].GoodsNum;			

			if( SYSPara.PCEnable == 1 )
			{
				//Trace("\r\n NowLayerNo=%d,NowCHNO=%d,m_SetArrayNo=%d",NowLayerNo,NowCHNO,m_SetArrayNo);
				//更改存货数量
				if(VPAddSingleColGoods( m_SetArrayNo, (InputNum), oldNum, 1 ))
				{
					//Channels[NowLayerNo][NowCHNO].GoodsNum	= InputNum;
					//更改货道状态
					if((Channels[NowLayerNo][NowCHNO].GoodsNum	== 0 )&&((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOK))
					{
						Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
						Channels[NowLayerNo][NowCHNO].ChannelStat 	|= CHANNELOVER;					
					}
					else if((Channels[NowLayerNo][NowCHNO].GoodsNum	!= 0 )&&((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOVER))
					{
						Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
						Channels[NowLayerNo][NowCHNO].ChannelStat 	|= CHANNELOK;					
					}	
					
					//上传货道状态
					VPMission_ColumnSta_RPT();
				}	
			}
			else
			{
				//原先货道状态0,添货，不能添货，并予以提示
                if((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELDISABLE)
                {					
					WriteFull_LCD(LINE0,Menu_ChannelUpdate[SYSPara.Language][0],strlen(Menu_ChannelUpdate[SYSPara.Language][0]));
					WriteFull_LCD(LINE1,Menu_ChannelUpdate[SYSPara.Language][1],strlen(Menu_ChannelUpdate[SYSPara.Language][1]));
					OSTimeDly(OS_TICKS_PER_SEC);
                } 
				//2.原先货道状态2添货，不能添货，并予以提示
				else if((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELHARDERR) 
                {					
					WriteFull_LCD(LINE0,Menu_ChannelUpdate[SYSPara.Language][0],strlen(Menu_ChannelUpdate[SYSPara.Language][0]));
					WriteFull_LCD(LINE1,Menu_ChannelUpdate[SYSPara.Language][2],strlen(Menu_ChannelUpdate[SYSPara.Language][2]));
					OSTimeDly(OS_TICKS_PER_SEC);
                } 
				//2.超过最大值，不能添货，并予以提示
				else if(InputNum>colSystemPara.ColMaxGoods[NowLayerNo][NowCHNO])//按货道添存货量;by gzz 20121120
				{					
					WriteFull_LCD(LINE0,Menu_ChannelUpdate[SYSPara.Language][0],strlen(Menu_ChannelUpdate[SYSPara.Language][0]));
					WriteFull_LCD(LINE1,Menu_ChannelUpdate[SYSPara.Language][3],strlen(Menu_ChannelUpdate[SYSPara.Language][3]));
					OSTimeDly(OS_TICKS_PER_SEC);
                } 
				else
				{
					Channels[NowLayerNo][NowCHNO].GoodsNum	= InputNum;	
				
					//更改货道状态
					if((Channels[NowLayerNo][NowCHNO].GoodsNum	== 0 )&&((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOK))
					{
						Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
						Channels[NowLayerNo][NowCHNO].ChannelStat 	|= CHANNELOVER;					
					}
					else if((Channels[NowLayerNo][NowCHNO].GoodsNum	!= 0 )&&((Channels[NowLayerNo][NowCHNO].ChannelStat& 0x03)==CHANNELOVER))
					{
						Channels[NowLayerNo][NowCHNO].ChannelStat	&= DRV_CHNSET_BIT;
						Channels[NowLayerNo][NowCHNO].ChannelStat 	|= CHANNELOK;					
					}	
				}								
							
			}
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
		InputCol = 2;
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

//选货按键;by gzz 20121025
uint8_t  MenuFunctionCHGOODS(void)						//	商品编号
{
	if (InputMode)
	{
		//Channels[NowLayerNo][NowCHNO].GoodsNum	= InputNum;
		Cursor_Off(LCDUP)	;
		InputCol = 8;
		InputMode = FALSE;
		
		Channels[NowLayerNo][NowCHNO].GoodsCode = InputNum;
		//选货按键;by gzz 20121025
		if(SYSPara.KeySel == 1 )
		{
			UpdateChannelPrice(2);
		}
	}
	else
	{
		InputCol = 2;
		InputDisplay( 0x07)  ;
	}
	return TRUE;
}



uint8_t  MenuFunctionADDLAYIN(void)				//输入层号
{
	if((InputNum == 0)||(InputNum  > (MAXLAYER +1)) )  
	{
		InputDone = FALSE;		  //20110823
		return FALSE;
	}
	if(InputNum == 9)
		NowLayerAll = TRUE;
	else 
	{
		NowLayerAll = FALSE;
		NowLayerNo = ChnMapPara.LAYTODRV[InputNum -1] -1;
	}
	Cursor_Off(LCDUP);
	InputCol = 8;
	InputMode = FALSE;
	return TRUE;
	
}

//添货全部货道
uint8_t	MenuFunctionAddGoods(void)		   
{
	uint8_t i,j;

	ClearScreen();
	if (NowLayerAll == TRUE)	
	{
		if( SYSPara.PCEnable == 1 )
		{
			VPAddAllColGoods();
			VPMission_ColumnSta_RPT();
		}
		else
		{
			for(j = 0; j< MAXLAYER; j++)	
			{
				for(i = 0; i < MAXCHANNEL;i++)
				{
				    //只有货道状态为1，或3的才可以添货
					//121108 by cq DontReduceNum
					//if(((Channels[j][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER) )
					if((isDontReduceNum())||((Channels[j][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER))
					{
						//Channels[j][i].GoodsNum = SYSPara.LayMaxGoods[j];
						Channels[j][i].GoodsNum = colSystemPara.ColMaxGoods[j][i];//按货道添存货量;by gzz 20121120

                        //更改货道状态
						if((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER)
						{
							Channels[j][i].ChannelStat	&= DRV_CHNSET_BIT;
							Channels[j][i].ChannelStat 	|= CHANNELOK;					
						}	
					}					
				}
			}
			//change by liya 2012-10-16
			if(SYSPara.Language==3)//土耳其语;by gzz 20121115
				DispTextLine(1,"Kargo ba\xf1\x5far\xf1\x31l\xf1\x31 doldurun",0);
			else if(SYSPara.Language==2)
				DispTextLine(1,"Тим грузов успешным",0);
			else
				DispTextLine(1,"填货成功",0);
		}
	}
	else
	{
	    if( SYSPara.PCEnable == 1 )
		{
			VPAddLayerColGoods();
			VPMission_ColumnSta_RPT();
		}
		else 
		{
			if(	SYSPara.LayEnable[NowLayerNo] == 0x01)
			{		   //层开启
				for(i = 0; i < MAXCHANNEL;i++)
				{
					//只有货道状态为1，或3的才可以添货
					//121108 by cq DontReduceNum
					//if(((Channels[NowLayerNo][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[NowLayerNo][i].ChannelStat& 0x03)==CHANNELOVER) )
					if((isDontReduceNum())||((Channels[NowLayerNo][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[NowLayerNo][i].ChannelStat& 0x03)==CHANNELOVER))
					{
						//Channels[NowLayerNo][i].GoodsNum = SYSPara.LayMaxGoods[NowLayerNo];
						Channels[NowLayerNo][i].GoodsNum = colSystemPara.ColMaxGoods[NowLayerNo][i];//按货道添存货量;by gzz 20121120

						//更改货道状态
						if((Channels[NowLayerNo][i].ChannelStat& 0x03)==CHANNELOVER)
						{
							Channels[NowLayerNo][i].ChannelStat	&= DRV_CHNSET_BIT;
							Channels[NowLayerNo][i].ChannelStat 	|= CHANNELOK;					
						}	
					}	
				}
				//change by liya 2012-10-16
				if(SYSPara.Language==3)//土耳其语;by gzz 20121115
					DispTextLine(1,"Kargo ba\xf1\x5far\xf1\x31l\xf1\x31 doldurun",0);
				else if(SYSPara.Language==2)
					DispTextLine(1,"Тим грузов успешным",0);
				else
					DispTextLine(1,"填货成功",0);
			}
			else
			{
				//change by liya 2012-10-16
				if(SYSPara.Language==3)//土耳其语;by gzz 20121115
					DispTextLine(1,"A\xf0\xe7\xf1\x31lmam\xf1\x31\xf1\x5f Raflar",0);
				else if(SYSPara.Language==2)
					DispTextLine(1,"слой Закрытые",0);
				else
					DispTextLine(1,"层未开启",0);
			}
		}
	
	
	}
	NowLayerAll = FALSE;

	OSTimeDly(OS_TICKS_PER_SEC);	
	return TRUE;
}

uint8_t  MenuFunctionSynGoodsCol(void)				//	全系统同步
{
	VPSynGoodsCol();
}

uint8_t  MenuFunctionAddCoinChanges(void)	//	补硬币
{
	VPAddChanges(1);
}

uint8_t  MenuFunctionAddBillChanges(void)	//	取纸币
{
	VPAddChanges(2);
}


//
uint8_t  MenuFunctionDemo6(void)
{
	return TRUE;
}

//纸币器状态测试
uint8_t  MenuFunctionDemo7(void)		   
{
	uint8_t err;
	uint16_t 	*PKeyInput;
	uint16_t	KeyInput;
	uint32_t    Money;
	char    str[24];
	//unsigned char result = 0;
	
	/*
	//1.startup
	do
	{
    	result = VPMission_Startup_RPT();
	}while( result != VP_ERR_NULL );
	Trace("***over startup");
    
	//2.setup
	result = VPMission_Setup_RPT();
	Trace("***over setup");
	
    
    while( 1 )
    {
	    
    	result = VPMission_Poll();
		Trace("***over Poll");
        
        if( (sysVPMission.sysStatus & VPM_STA_INITOK)&&(sysVPMission.sysStatus & VPM_STA_GOODSPAR)&&(sysVPMission.sysStatus & VPM_STA_COLUMNPAR) )
        {
        	VP_Update_ColumnGoodsPar();
        	sysVPMission.sysStatus &= ~VPM_STA_COLUMNPAR;
        	sysVPMission.sysStatus &= ~VPM_STA_GOODSPAR;
            sysVPMission.sysStatus &= ~VPM_STA_INITOK;
        	sysVPMission.VPMode = 1;
            break;
        }

    }
    sysVPMission.resetCmd = 0;
	*/
	
    //Trace(" Helloworld1");
	SetTouBiStat(CMD_NOTE_EN); 	 			//使能纸币投币  	
	//Trace(" Helloworld--two");
	
	Money = 0;
	ClearScreen();	
	//Trace(" Helloworld = %d",3);
	Write_LCD(LINE0,Language[INSERTBILL][SYSPara.Language],strlen(Language[INSERTBILL][SYSPara.Language]));
	Write_LCD(LINE1,Language[BALANCE][SYSPara.Language],strlen(Language[BALANCE][SYSPara.Language]));
	Cursor_On(LCDUP);	
	//Trace(" Helloworld = %d",4);
		
	if(SYSPara.GBAEnable == MDB_NOTE_EN) 
	{
        SYSPara1.NoteStat |= MBV_VALUE_BIT;	 ////
    }
	else 	if(SYSPara.GBAEnable == HR_NOTE_EN) 
	{
        SYSPara1.NoteStat |= HR_VALUE_BIT;	 ////
    }
	else 	if(SYSPara.GBAEnable == ITL_NOTE_EN) 
	{
        SYSPara1.NoteStat |= ITL_VALUE_BIT;	 ////
    }
	while(1) 
	{
		//120621 by cq KeySlowly
		//PKeyInput = OSMboxPend(g_KeyMsg,OS_TICKS_PER_SEC * 2,&err);		
		PKeyInput = OSQPend(g_KeyMsg,OS_TICKS_PER_SEC ,&err);
		KeyInput = *PKeyInput;

		//120621 by cq KeySlowly
		//KeySlowly Begin
		if(err == OS_NO_ERR) 
		{
			/*
		    if(KeyInput & 0x8000) 
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				Money += KeyInput&0x7FFF;			
				strcpy(str,Language[BALANCE][SYSPara.Language]);
				strcat(str,DecimalPoint(Money));
				Write_LCD(LINE1,str,strlen(str));	        
		    }
			*/
			if((KeyInput == KEY_ENTER)||(KeyInput == KEY_CANCEL)) 
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				SetTouBiStat(CMD_NOTE_DIS); 	 //禁能纸币投币  
				OSMboxAccept(g_MoneyMsg);//120621 by cq KeySlowly
				return TRUE;
			}
		}


		
		//等待投币信号等，如果有则更新按键信息等
		PKeyInput = OSMboxPend(g_MoneyMsg,1,&err);
		KeyInput = *PKeyInput;			
		if(err == OS_NO_ERR) 
		{
		    if(KeyInput & 0x8000) 
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				Money += KeyInput&0x7FFF;			
				strcpy(str,Language[BALANCE][SYSPara.Language]);
				strcat(str,DecimalPoint(Money));
				Write_LCD(LINE1,str,strlen(str));	        
		    }
			
		}
		//KeySlowly End
		
	}
//	SetTouBiStat(CMD_NOTE_DIS); 	 //禁能纸币投币  
//	return TRUE;
}
//
uint8_t  MenuFunctionDemo8(void)		   //硬币器状态测试
{
	uint8_t err;
	uint16_t 	*PKeyInput;
	uint16_t	KeyInput;
	uint32_t    Money;
	char    str[24];

	SetTouBiStat(CMD_COIN_EN); 	 //使能硬币投币  
			 
	Money = 0;
	ClearScreen();
	if(SYSPara.ChangerEnable == MDB_CHANGER_EN) //显示币斗剩余币值;by gzz 20130105
	{
		strcpy(str,Language[INSERTCOIN][SYSPara.Language]);
		strcat(str,DecimalPoint(g_CoinRealLeft));
		Write_LCD(LINE0,str,strlen(str));
	}
	else
	{
		Write_LCD(LINE0,Language[INSERTCOIN][SYSPara.Language],strlen(Language[INSERTCOIN][SYSPara.Language]));
	}
	Write_LCD(LINE1,Language[BALANCE][SYSPara.Language],strlen(Language[BALANCE][SYSPara.Language]));
	Cursor_On(LCDUP);
	SYSPara1.CoinsStat |= COIN_VALUE_BIT;	 /////
	while(1) 
	{
		//120621 by cq KeySlowly
		//KeySlowly Begin
		//PKeyInput = OSMboxPend(g_KeyMsg,OS_TICKS_PER_SEC * 2,&err);
		//PKeyInput = OSQPend(g_KeyMsg,OS_TICKS_PER_SEC ,&err);
		PKeyInput = OSQPend(g_KeyMsg,1 ,&err);
		KeyInput = *PKeyInput;
			
		if(err == OS_NO_ERR) 
		{
			/*
		    if(KeyInput & 0x8000)
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				Money += KeyInput&0x7FFF;
				strcpy(str,Language[BALANCE][SYSPara.Language]);
				strcat(str,DecimalPoint(Money));
				Write_LCD(LINE1,str,strlen(str));	        
		    }
			*/
			if((KeyInput == KEY_ENTER)||(KeyInput == KEY_CANCEL)) 
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				SetTouBiStat(CMD_COIN_DIS); 	 //禁能硬币投币 
				OSMboxAccept(g_MoneyMsg);//120621 by cq KeySlowly
				return TRUE;
			}
		}

		
		//等待投币信号等，如果有则更新投币信息等
		//PKeyInput = OSMboxPend(g_MoneyMsg,OS_TICKS_PER_SEC,&err);
		PKeyInput = OSMboxPend(g_MoneyMsg,1,&err);
		KeyInput = *PKeyInput;		    
		if(err == OS_NO_ERR) 
		{	
			//Trace("\r\n TASKB,PendMoneyMsg=%d",KeyInput);
			if(KeyInput & 0x8000)
			{
			    if(Money == 0) 
				{
				   Cursor_Off(LCDUP);
				}
				Money += KeyInput&0x7FFF;
				strcpy(str,Language[BALANCE][SYSPara.Language]);
				strcat(str,DecimalPoint(Money));
				Write_LCD(LINE1,str,strlen(str));	        
		    }
		}
		//KeySlowly End
	}

//SetTouBiStat(CMD_COIN_DIS); 	 //禁能硬币投币 
//return TRUE;
}
uint8_t MenuFunctionNOTETEST(void)
{
	if(SYSPara.GBAEnable == MDB_NOTE_EN) 
	{
	    SYSPara1.NoteStat &= ~MBV_VALUE_BIT;
	}
	else if(SYSPara.GBAEnable == HR_NOTE_EN) 
	{
	    SYSPara1.NoteStat &= ~HR_VALUE_BIT;
	}
	return TRUE;
}

uint8_t MenuFunctionCOINSTEST(void)
{
	SYSPara1.CoinsStat &= ~COIN_VALUE_BIT;
	return TRUE;
}

//120622 by cq HopperTest

uint8_t  HopperTest_Old(uint8_t n)				//找零器状态测试
{
	uint32_t  ChangeOutput,*pComStatus;
	uint8_t err;
/*
	if((SYSPara.ChangerEnable != HOPPER_CHANGER_EN) || (SYSPara.ChangeValue[n-1] == 0)) 
	{
	   SYSPara1.HopperStat[n-1] = HOPPER_DISABLE_BIT;
	   return TRUE;
	} 
	else 
	{



*/
		//SYSPara1.HopperStat[n-1] &= ~HOPPER_DISABLE_BIT;

		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24) + SYSPara.ChangeValue[n-1];
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);    //找零指令						
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*5,&err);


	//}
	
	return TRUE;
}

//120622 by cq HopperTest
uint8_t  HopperTest(uint8_t n)				//找零器状态测试
{
   uint32_t  ChangeOutput,*pComStatus;
   uint8_t err;

   //120716 by cq HoppeyUse
   //if((SYSPara.ChangerEnable != HOPPER_CHANGER_EN) || (SYSPara.ChangeValue[n-1] == 0)) 
   if((SYSPara.ChangerEnable == 0) || (SYSPara.ChangeValue[n-1] == 0)) 
   {
      SYSPara1.HopperStat[n-1] = HOPPER_DISABLE_BIT;
	  return TRUE;
   } 
   else if(SYSPara.ChangerEnable == MDB_CHANGER_EN)//120716 by cq HoppeyUse
   {
	   OSMboxAccept(g_CommBackMsg);
	   MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24) + SYSPara.ChangeValue[n-1];
	   OSMboxPost(g_CommMsg,&MsgBack.CmdType);	  //找零指令					   
	   
	   pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*5,&err);
   }
   else //HOPPER_CHANGER_EN
   {
     SYSPara1.HopperStat[n-1] &= ~HOPPER_DISABLE_BIT;

	 #if HOPPERDEVICE					   				            
     OSMboxAccept(g_CommBackMsg);
	 //货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
	 ChangeOutput =  5*SYSPara.ChangeValue[n-1] + ((uint32_t)CMD_HOPPER_CHANGE << 24) + ((uint32_t)(n-1) << 20);
     OSMboxPost(g_CommMsg,&ChangeOutput);	   //找零指令				

     pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*35,&err);

 	 if(err == OS_NO_ERR) 
	 {
	    if(SYSPara1.HopperStat[n-1] & HOPPER_INSUFFICIENT_BIT) 
		{
		   Write_LCD(LINE0,Language[SLNOENOUGH][SYSPara.Language],strlen(Language[SLNOENOUGH][SYSPara.Language]));
		} 
		else if(SYSPara1.HopperStat[n-1] > 0) 
		{
	       Write_LCD(LINE0,Language[SLFAULT][SYSPara.Language],strlen(Language[SLFAULT][SYSPara.Language]));	
		} 
		else 
		{
		   if(*pComStatus == 5*SYSPara.ChangeValue[n-1]) 
		   {
		      Write_LCD(LINE0,Language[SLNORMAL][SYSPara.Language],strlen(Language[SLNORMAL][SYSPara.Language]));
		   } 
		   else 
		   {
		      Write_LCD(LINE0,Language[SLFAULT][SYSPara.Language],strlen(Language[SLFAULT][SYSPara.Language]));	
		   }
		}  
     } 
	 else
	 {	    
     
		Write_LCD(LINE0,Language[SLFAULT][SYSPara.Language],strlen(Language[SLFAULT][SYSPara.Language]));	
	 } 
	 OSTimeDly(OS_TICKS_PER_SEC * 2);	
	 #endif
   }
   return TRUE;
}//

uint8_t  Hopper1Test(void)//
{
  HopperTest(1);	//
  return TRUE;
} 
uint8_t  Hopper2Test(void)//
{
  HopperTest(2);	//
  return TRUE;
} 
uint8_t  Hopper3Test(void)//
{
  HopperTest(3);	//
  return TRUE;
} 

//121113 by cq Cashless
uint8_t  Hopper2Test_ReaderCost(void)//
{
	//120926 by cq Cashless
  //HopperTest(2);	//
  
	if(SYSPara.ReaderCost)
	{
		SYSPara.ReaderCost = 0;
	}
	else
	{
		SYSPara.ReaderCost = 1;
	}
	
  return TRUE;
} 

//121113 by cq Cashless
uint8_t  Hopper3Test_ReaderEnable(void)//
{
	//120921 by cq Cashless
  //HopperTest(3);	//
  
  if(SYSPara.ReaderEnable)
  {
	  SYSPara.ReaderEnable = 0;
  }
  else
  {
	  SYSPara.ReaderEnable = 1;
  }

  
  return TRUE;
} 


uint8_t  MenuFunctionDemo9(void)	//hopper 测试，异常返回 false
{
	return TRUE;
}
//
uint8_t  MenuFunctionKeyTest(void)	 //键盘测试
{
	InputTest   = KEYTEST;
	return TRUE;
}
//
uint8_t  MenuFunctionAllChannelStat(void)	//所有货道状态
{
	//InputTest   = KEYLOOP;
	InLayerNO = 0;
	//AllChannelStat(NowLayerNo);

	return TRUE;
}
uint8_t  MenuFunctionAllCHResult(void)	//所有货道状态
{
	char strtemp1[48];
	
	//InputTest   = KEYLOOP;
	InCHNO = 0;
	InLayerNO  = 0;
	ClearScreen();
	//change by liya 2012-10-16
	if(SYSPara.Language==3)//土耳其语;by gzz 20121115
		sprintf( strtemp1,"Kargo Karayolu testi kez%d", SYSPara1.CHTestNO);
	else if(SYSPara.Language==2)
		sprintf( strtemp1,"Количество тестов%d", SYSPara1.CHTestNO);
	else
		sprintf( strtemp1,"货道测试次数%d", SYSPara1.CHTestNO);
	Write_LCD(LINE0,strtemp1,strlen(strtemp1));
		OSTimeDly(OS_TICKS_PER_SEC );	
	return TRUE;
}
//

uint8_t  MenuFunctionChannelTest(void)
{

	return TRUE;
}

uint8_t  MenuFunctionIRTest(void)
{
    uint32_t  *pComStatus;
	uint8_t err;
	//出货确认模块的测试
    #if DRVDEVICE							  
	if(SYSPara.GOCCtr > 0) 
	{
		//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
		MsgBack.CmdType = 11 + ((uint32_t)CMD_GOODS_CHECK << 24); 
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 5,&err); 
	
		if(err == OS_NO_ERR) 
		{
			if (*pComStatus & DRV_GOCERR_BIT) 
			{
			    Write_LCD(LINE0,Language[SCHKERR][SYSPara.Language],strlen(Language[SCHKERR][SYSPara.Language]));	
			} 
			else 
			{
			    Write_LCD(LINE0,Language[SCHKOK][SYSPara.Language],strlen(Language[SCHKOK][SYSPara.Language]));
			}
		}
	} 
	else 
	{
	    Write_LCD(LINE0,Language[SCHKOFF][SYSPara.Language],strlen(Language[SCHKOFF][SYSPara.Language]));
	}
	
	OSTimeDly(OS_TICKS_PER_SEC );
	#endif 
    
	return TRUE;
}

uint8_t  MenuFunctionLCDTest(void)	//显示屏测试
{
	char strtemp1[48];
	
	//InputTest   = KEYLOOP;
	InCHNO = 0;
	InLayerNO  = 0;
	ClearScreen();	
	//change by liya 2012-10-16
	if(SYSPara.Language==3)//土耳其语;by gzz 20121115
		sprintf( strtemp1,"UP-vurgulay\xf1\x31n,DOWN-OK");
	else if(SYSPara.Language==2)
		sprintf( strtemp1,"UP-Выделите,DOWN-OK");
	else
		sprintf( strtemp1,"UP键反白，DOWN键正常显示");
	Write_LCD(LINE0,strtemp1,strlen(strtemp1));
		OSTimeDly(OS_TICKS_PER_SEC );
	//背光暗亮一下	
	SCLK_BL_CLR();
	OSTimeDly(OS_TICKS_PER_SEC * 2);
	SCLK_BL_SET();
	return TRUE;
}

uint8_t  MenuFunctionDOLCDTest(void)	//真正做显示屏测试
{				
	ClearScreen();	
	LCD_WhiteBlock(1);
	OSTimeDly(OS_TICKS_PER_SEC * 2 );	
	LCD_WhiteBlock(0);
	return TRUE;
}

uint8_t  MenuFunctionTemperatureTest(void)	//温控器测试
{	
	ClearScreen();
	if(SYSPara.Language==0)
	{
		Write_LCD(LINE0,"温控器测试",10);
	}
	else
	if(SYSPara.Language==1)
	{
		Write_LCD(LINE0,"Temperature",11);
	}
	else
	{
		Write_LCD(LINE0,"Проверка термостата",strlen("Проверка термостата"));
	}
	return TRUE;
}




uint8_t  MenuFunctionDoTempeTest(void)	//真正做温控器测试
{	
	uint8_t len = 0;
	uint8_t key = 0;
	uint8_t str[20];
	
	//while( 1 )
	//{		
		//XMTMission_GetTemperature();
		
		//WaitForWork( 500, NULL );
		/*
		len = sprintf( str, "%02bx,%02bx,%02bx,%02bx,%02bx", sysXMTMission.recBuf[0], sysXMTMission.recBuf[1],sysXMTMission.recBuf[2],sysXMTMission.recBuf[3],sysXMTMission.recBuf[4] );
		DisplayStr( 0, 0, 1, str, len ); 
		len = sprintf( str, "%02bx,%02bx,%02bx,%02bx,%02bx", sysXMTMission.recBuf[5], sysXMTMission.recBuf[6],sysXMTMission.recBuf[7],sysXMTMission.recBuf[8],sysXMTMission.recBuf[9]);
		DisplayStr( 0, 1, 1, str, len );
		WaitForWork( 5000, NULL );
		
		
		len = sprintf( str, "rec=%02bx,%02bx,%02bx,%02bx", sysXMTMission.recPV[0], sysXMTMission.recPV[1],sysXMTMission.recSV[0],sysXMTMission.recSV[1] );
		DisplayStr( 0, 0, 1, str, len );
		len = sprintf( str, "PV=%u.%u,SV=%u.%u", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10, sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
		DisplayStr( 0, 1, 1, str, len ); 
		WaitForWork( 2000, NULL );
		*/

		ClearScreen();
		if(SYSPara.Language==0)
		{
			len = sprintf( str, "机内温度:%u.%u C", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10 );
			Write_LCD(LINE0,str,len);
			len = sprintf( str, "设定温度:%u.%u C", sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
			Write_LCD(LINE1,str,len);
		}
		else
		if(SYSPara.Language==1)
		{
			len = sprintf( str, "Machine Tempe:%u.%u C", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10 );	
			Write_LCD(LINE0,str,len);
			len = sprintf( str, "Set Tempe:%u.%u C", sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
			Write_LCD(LINE1,str,len);
		}
		else
		{
			len = sprintf( str, "машина температура:%u.%u C", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10 );	
			Write_LCD(LINE0,str,len);
			len = sprintf( str, "установка температура:%u.%u C", sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
			Write_LCD(LINE1,str,len);	
		}
		OSTimeDly(OS_TICKS_PER_SEC * 5);
		return TRUE;
		
		/*
		key = GetKey();
		if((key==KEY_CANCEL)||(key==KEY_SUBMIT))
		{
			break;
		}
		WaitForWork( 100, NULL );	
		*/
	//}		
}


uint8_t  MenuFunctionXuanhuoTest(void)	//选货按键测试
{	
	ClearScreen();
	if(SYSPara.Language==0)
	{
		Write_LCD(LINE0,"选货按键测试",12);
	}
	else
	{
		Write_LCD(LINE0,"choose",11);
	}
	return TRUE;
}

uint8_t  MenuFunctionDoXuanhuoTest(void)	//真正做选货按键测试
{	
	uint8_t *NowKey;
	uint8_t err=0;
	uint8_t len = 0;
	uint8_t str[20];
	uint8_t out[4];

	ClearScreen();
	if(SYSPara.Language==0)
	{
		WriteFull_LCD(LINE0,"请按选货按键",12);
		WriteFull_LCD(LINE1,"U,D,C,E退出",10);
	}
	else
	{
		WriteFull_LCD(LINE0,"Press choose",12);		
	}
	UpdateSelectionLedTest(1);
	//OSTimeDly(OS_TICKS_PER_SEC * 5);
	
	while(1)
	{	
		//Trace("\r\n input gametest");
		OSTimeDly(OS_TICKS_PER_SEC / 10);				
		NowKey = OSQPend(g_KeyMsg,1,&err);
		if(err == OS_NO_ERR) 
		{		
			//Trace("\r\n TASKB,PendKeyMsg=%d,%d",KeyInput);
			//if(*NowKey == KEY_CANCEL) 
			//{
				//Trace("\r\n cancel");
				//break;
			//}
			ClearScreen();
						
			if(SYSPara.Language==0)
			{		
				len = sprintf( str, "按键:%d", *NowKey );
			}
			else
			{
				len = sprintf( str, "choose:%d", *NowKey );	
			}
			Write_LCD(LINE0,str,len);

			//退出
			if(*NowKey == KEY_UP) 
			{
				out[0] = KEY_UP;
			}
			else if( (*NowKey == KEY_DOWN)&&(out[0] == KEY_UP) ) 
			{
				out[1] = KEY_DOWN;
			}
			else if( (*NowKey == KEY_CANCEL)&&(out[1] == KEY_DOWN) ) 
			{
				out[2] = KEY_CANCEL;
			}
			else if( (*NowKey == KEY_ENTER)&&(out[2] == KEY_CANCEL) ) 
			{
				out[3] = KEY_CANCEL;
				break;
			}
			else
			{
				memset(out,0,sizeof(out));
			}
		}	
	}	
	//Trace("\r\n output gametest");
	UpdateSelectionLedTest(0);	
}


//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionTempTest(void)	//压缩机测试
{	
	ClearScreen();
	Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][4],strlen(Menu_ACDC[SYSPara.Language][4]));
	Write_LCD(LINE1,Menu_ACDC[SYSPara.Language][0],strlen(Menu_ACDC[SYSPara.Language][0]));
	return TRUE;
}


//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionDoTempsTest(void)	//真正做压缩机测试
{	
	uint8_t *NowKey;
	uint8_t err=0;
	uint32_t  DisintOutput;
	
	ClearScreen();	
	//ACDCModuleTest();
	Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][4],strlen(Menu_ACDC[SYSPara.Language][4]));
	
	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		NowKey = OSQPend(g_KeyMsg,1,&err);//等待键盘输入
		if (err ==OS_NO_ERR) 
		{
			if(*NowKey == KEY_UP) 
			{
				//acdc设备，最高字节为命令编码，其他为设备控制
				DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)sysVPMission.ACDCLedCtr<<1)+ACDCCOMPER_UP;
				MsgBack.CmdType = DisintOutput;
				OSMboxAccept(g_CommBackMsg);
				OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
				Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][5],strlen(Menu_ACDC[SYSPara.Language][5]));
			}
			else if(*NowKey == KEY_DOWN) 
			{
				//acdc设备，最高字节为命令编码，其他为设备控制
				DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)sysVPMission.ACDCLedCtr<<1)+ACDCCOMPER_DOWN;
				MsgBack.CmdType = DisintOutput;
				OSMboxAccept(g_CommBackMsg);
				OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
				Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][6],strlen(Menu_ACDC[SYSPara.Language][6]));
			}
			else if(*NowKey == KEY_CANCEL) 
			{
				break;
			}	
		}
		
	}	
	return TRUE;	
}


//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDTest(void)	//展示灯测试
{	
	ClearScreen();
	Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][1],strlen(Menu_ACDC[SYSPara.Language][1]));
	Write_LCD(LINE1,Menu_ACDC[SYSPara.Language][0],strlen(Menu_ACDC[SYSPara.Language][0]));
	return TRUE;
}


//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionDoLEDTest(void)	//真正做展示灯测试
{	
	uint8_t *NowKey;
	uint8_t err=0;
	uint32_t  DisintOutput;
	
	ClearScreen();	
	//ACDCModuleTest();
	Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][1],strlen(Menu_ACDC[SYSPara.Language][1]));
	
	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		NowKey = OSQPend(g_KeyMsg,1,&err);//等待键盘输入
		if (err ==OS_NO_ERR) 
		{
			if(*NowKey == KEY_UP) 
			{
				//acdc设备，最高字节为命令编码，其他为设备控制
				DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)ACDCLEDCTR_UP<<1)+sysVPMission.ACDCCompressorCtr;
				MsgBack.CmdType = DisintOutput;
				OSMboxAccept(g_CommBackMsg);
				OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
				Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][2],strlen(Menu_ACDC[SYSPara.Language][2]));
			}
			else if(*NowKey == KEY_DOWN) 
			{
				//acdc设备，最高字节为命令编码，其他为设备控制
				DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)ACDCLEDCTR_DOWN<<1)+sysVPMission.ACDCCompressorCtr;
				MsgBack.CmdType = DisintOutput;
				OSMboxAccept(g_CommBackMsg);
				OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
				Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][3],strlen(Menu_ACDC[SYSPara.Language][3]));
			}
			else if(*NowKey == KEY_CANCEL) 
			{
				break;
			}	
		}
		
	}	
	return TRUE;	
}


//
//
uint8_t  MenuFunctionDemo12(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo13(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo14(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo15(void)
{
	
	return TRUE;
}
//
uint8_t  FunctionCGVA(uint8_t id)		 //找零器金额
{
	if (InputMode)
	{
		
		
	//	if(FDot == TRUE)
		//	SYSPara.ChangeValue[id-1]	= DecimalPointInput(InputNum);
	//	else
	//		SYSPara.ChangeValue[id-1]	= DecimalPointInput(InputNum*100);
			
		 	SYSPara.ChangeValue[id-1]	= (InputNum);
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
	//	}
	}
	else
	{
			InputCol = 5;
			InputDisplay( 0x07)	 ;
	}	

	return TRUE;
}
//

uint8_t  MenuFunctionCGVA1(void)
{
	return FunctionCGVA(1);
}
//
uint8_t  MenuFunctionCGVA2(void)
{
	return FunctionCGVA(2);
}
//
uint8_t  MenuFunctionCGVA3(void)
{
	return FunctionCGVA(3);
}
	
	

uint8_t  FunctionCOCH(uint8_t id)		   //硬币器通道
{
	if (InputMode)
	{
		   /*
			if(FDot == TRUE)
			SYSPara.CoinsValue[id-1]	= DecimalPointInput(InputNum);
		else
			SYSPara.CoinsValue[id-1]	= DecimalPointInput(InputNum*100);
			 */
			SYSPara.CoinsValue[id-1]	= (InputNum);
			if(InputNum > 0) 
			{
			    CashTypeEnBit.CoinTypeEN |= 1 << (id-1);
			} 
			else 
			{
			    CashTypeEnBit.CoinTypeEN &= ~(1 << (id-1));
			}
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
	//	}
	}
	else
	{
			InputCol = 5;
			InputDisplay( 0x07)	 ;
	}	

	return TRUE;
}
	
//
uint8_t  MenuFunctionCOCH1(void)
{
	return FunctionCOCH(1);
}
uint8_t  MenuFunctionCOCH2(void)
{
	return FunctionCOCH(2);
}
	
uint8_t  MenuFunctionCOCH3(void)
{
	return FunctionCOCH(3);
}
	
uint8_t  MenuFunctionCOCH4(void)
{
	return FunctionCOCH(4);
}
	
uint8_t  MenuFunctionCOCH5(void)
{
	return FunctionCOCH(5);
}
	
uint8_t  MenuFunctionCOCH6(void)
{
	return FunctionCOCH(6);
}
	
uint8_t  MenuFunctionCOCH7(void)
{
	return FunctionCOCH(7);
}
	
uint8_t  MenuFunctionCOCH8(void)
{
	return FunctionCOCH(8);
}
	
	
	

uint8_t  FunctionNOCH(uint8_t id)			//纸币器通道金额
{
	if (InputMode)
	{
		/*
		if(FDot == TRUE)
			SYSPara.GBAValue[id-1]	= DecimalPointInput(InputNum);
		else
			SYSPara.GBAValue[id-1]	= DecimalPointInput(InputNum*100);
		*/
			SYSPara.BillValue[id-1]	= (InputNum);
			if(InputNum > 0) 
			{
			    CashTypeEnBit.BillTypeEN |= 1 << (id-1);
				SYSPara2.BillEnable |= 1 << (id-1); //可以设置不收纸币;by gzz 20121219
			} 
			else 
			{
			    CashTypeEnBit.BillTypeEN &= ~(1 << (id-1));
				SYSPara2.BillEnable &= ~(1 << (id-1)); //可以设置不收纸币;by gzz 20121219
			}
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
	//	}
	}
	else
	{
			InputCol = 5;
			InputDisplay( 0x07)	 ;
	}	

	return TRUE;
}
//

//
uint8_t  MenuFunctionNOCH1(void)
{
	return FunctionNOCH(1);
}
uint8_t  MenuFunctionNOCH2(void)
{
	return FunctionNOCH(2);
}
uint8_t  MenuFunctionNOCH3(void)
{
	return FunctionNOCH(3);
}
uint8_t  MenuFunctionNOCH4(void)
{
	return FunctionNOCH(4);
}
uint8_t  MenuFunctionNOCH5(void)
{
	return FunctionNOCH(5);
}
uint8_t  MenuFunctionNOCH6(void)
{
	return FunctionNOCH(6);
}
uint8_t  MenuFunctionNOCH7(void)
{
	return FunctionNOCH(7);
}
uint8_t  MenuFunctionNOCH8(void)
{
	return FunctionNOCH(8);
}


uint8_t  MenuFunctionCGSV(void)
{
	if (InputMode)
	{
		
		if(InputNum <=1) 
		{
		//if((InputNum >=0) && (InputNum <=1))  {			 //是否服务状态0-1
			SYSPara.ChangeServer	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x08)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionPONO(void)
{
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=2))  
		{				 //小数点位数
			SYSPara.Decimal	= InputNum;
			InputCol = 8;
			Cursor_Off(LCDUP)	;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x08)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionSACON(void)
{
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=1))  
		{				 //出货确认
			//121108 by cq GOCErrorSevice
			//SYSPara.GOCCtr	= InputNum;
			if(isGOCErrorSevice())
				SYSPara.GOCErrorSevice	= InputNum;
			else
			{
				//if((SYSPara2.AutoColumn	!= 0)&&(InputNum == 0) )
				//	return FALSE;
				//else
					SYSPara.GOCCtr	= InputNum;
			}
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
		InputCol = 1;
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionPOSEN(void)
{
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=1))  
		{				 //POS使能
			SYSPara.POSEnable	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

uint8_t  MenuFunctionPCEN(void)
{
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=10))  
		{				 //PC使能
			SYSPara.PCEnable	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}


uint8_t  MenuFunctionLCDSEL(void)
{
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		//添加品途屏幕的兼容;by gzz 20121025
		if((InputNum <=10))  
		{				 //选择LCD
			SYSPara.LCDSel	= InputNum;
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

//选货按键;by gzz 20121025
uint8_t  MenuFunctionKeySel(void)
{
	if (InputMode)
	{	
	
		if(InputNum <=1)  
		{				 //选择LCD
			SYSPara.KeySel	= InputNum;
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

//拼图屏幕样式;by gzz 20121106
uint8_t  MenuFunctionLogoSel(void)
{
	if (InputMode)
	{	
	
		if(InputNum <=10)  
		{				 //选择Logo
			SYSPara.LogoSel	= InputNum;
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

//不用手工补货功能;by gzz 20130122
uint8_t  MenuFunctionAutoColumn(void)
{
	if (InputMode)
	{	
	
		if(InputNum <=1)  
		{				 //选择Logo
			SYSPara2.AutoColumn	= InputNum;
			if(InputNum == 1)
			{
				SYSPara.GOCCtr = 1;
				AutoAddColumn();//不用手工补货功能;by gzz 20130122
			}
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

//是否使用十货道的机器;by gzz 20130226
uint8_t  MenuFunctionTenColumn(void)
{
	if (InputMode)
	{	
	
		if(InputNum <=1)  
		{				 //选择Logo
			SYSPara2.TenColumn	= InputNum;			
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}


uint8_t  MenuFunctionHOLDNOTE(void)
{

	
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=1))  
		{				 //POS使能
			SYSPara.NoteEscrow	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionHOLDNOTE5(void)
{

	
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=1))  
		{				 //POS使能
			SYSPara.NoteEscrowType[1] = InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionHOLDNOTE10(void)
{

	
	if (InputMode)
	{
		
	//	if(InputNum <=1)  {
		if((InputNum <=1))  
		{				 //POS使能
			SYSPara.NoteEscrowType[2]	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}

uint8_t  MenuFunctionNBUY(void)		   				//允许多次交易
{
 	if (InputMode)
	{
		
		if((InputNum <=1))  
		{				 //
			SYSPara.TransMultiMode	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
//


uint8_t  MenuFunctionRCOIN(void)
{
 	if (InputMode)
	{
		
		if((InputNum <=1))  
		{			//允许不卖货退币
			SYSPara.TransEscape	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		} 	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
		InputCol = 1;
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
//


uint8_t  MenuFunctionOCOIN(void)			   //开启硬币器
{
 	if (InputMode)
	{
		
		if((InputNum <=2))  
		{			
			SYSPara.CoinEnable	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}

	return TRUE;
}
//
uint8_t  MenuFunctionONOTE(void)			   //开启纸币器
{
 /*	if (InputMode){
		
		if((InputNum >=0) && (InputNum <=2))  {			
			SYSPara.GBAEnable	= InputNum;
			Cursor_Off()	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else {
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
*/
	return TRUE;
}

//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionONREADER(void)			   //开启读卡器
{
 
	return TRUE;
}

//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionREADERSetOFF(void)
{
	SYSPara.ReaderEnable = READEDR_CLOSE;
	return TRUE;
}

//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionREADERSetMDB(void)
{
	SYSPara.ReaderEnable = READEDR_MDB;
	return TRUE;
}




uint8_t  MenuFunctionONREADERCOST(void)//读卡器菜单;by gzz 20121120
{
 	if (InputMode)
	{
		
		if((InputNum <=1))  
		{			//允许不卖货退币
			SYSPara.ReaderCost	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		} 	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
		InputCol = 1;
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
//


uint8_t  MenuFunctionSetMEI(void)
{
	SYSPara.GBAEnable = NOTE_MEI;
	return TRUE;
}
uint8_t  MenuFunctionSetGBA(void)
{
	SYSPara.GBAEnable = NOTE_GBA;
	return TRUE;
}
uint8_t  MenuFunctionSetITL(void)
{
	SYSPara.GBAEnable = NOTE_ITL;
	return TRUE;
}

uint8_t  MenuFunctionSetClose(void)
{
	SYSPara.GBAEnable = NOTE_CLOSE;
	return TRUE;
}

//
uint8_t  MenuFunctionOCHANGE(void)			   //开启找零器
{
 	if (InputMode)
	{
		
		if((InputNum <=2))  
		{			
			SYSPara.ChangerEnable	= InputNum;
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
//

					
uint8_t  MenuFunctionBMIN(void)						   //最小找零金额
{
 	if (InputMode){
		
	//	if(FDot == TRUE)
	//		SYSPara.MoneyMinLeft	= DecimalPointInput(InputNum);
	//	else
	//		SYSPara.MoneyMinLeft	= DecimalPointInput(InputNum*100);
				SYSPara.MoneyMinLeft	= (InputNum);
		
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
	//	}
	}
	else
	{
			InputCol = 5;
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}


uint8_t  MenuFunctionMMAX(void)						   //最大收币金额
{
 	if (InputMode)
	{
		
	//	if(FDot == TRUE)
	//		SYSPara.MoneyMaxin	= DecimalPointInput(InputNum);
	//	else
	//		SYSPara.MoneyMaxin	= DecimalPointInput(InputNum*100);
		 		SYSPara.MoneyMaxin	= (InputNum);
				InputCol = 8;
			Cursor_Off(LCDUP)	;
			InputMode = FALSE;
	//	}
	}
	else
	{
				InputCol = 5;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
//
//
uint8_t  MenuFunctionGPRS(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionSENO(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionGSMNO(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionTemprature(void)
{
	if (InputMode)
	{		   
			if(InputNum > 0) 
			{
				sysXMTMission.recSVTemp	= (InputNum);
				sysXMTMission.recPVTemp	= (InputNum);
			}
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
	//	}
	}
	else
	{
			InputCol = 5;
			InputDisplay( 0x07)  ;
	}	

	return TRUE;
}


//
uint8_t  MenuFunctionDBMN(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo23(void)
{
	return TRUE;
}
//
uint8_t MenuFunctionDemo24(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo25(void)
{
	return TRUE;
}
//
//
uint8_t  MenuFunctionDemo26(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo27(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo28(void)
{
	return TRUE;
}

//
uint8_t  MenuFunctionDemo29(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo30(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo31(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo32(void)
{
	return TRUE;
}
//
//
uint8_t  MenuFunctionDemo33(void)
{
	return TRUE;
}
//
uint8_t  MenuFunctionDemo34(void)
{
	return TRUE;
}

uint8_t  MenuFunctionDemo35(void)
{
	return TRUE;
}
uint8_t  MenuFunctionDemo36(void)
{
	return TRUE;
}

uint8_t  MenuFunctionDemo37(void)
{
	return TRUE;
}
uint8_t  MenuFunctionDemo38(void)
{
	return TRUE;
}


uint8_t  MenuFunctionCONF(void)//	 清除交易记录
{
	uint8_t	*tp;
	uint16_t 	i,j;

	//保存季度交易数据
	uint32_t	IncomeJidu;		  					//总收币值
	uint32_t	NoteIncomeJidu;	  					//纸币总收币值
	uint32_t	CoinsIncomeJidu;  					//硬币总收币值
	uint32_t	TotalTransJidu;	  					//交易总金额
	uint32_t	TotalChangeJidu;  					//找零总金额
	uint32_t	CoinsChangeJidu[HOPPERCHMAX];	 	//找零器出币数

	for(i = 0;i< MAXLAYER;i++)	
	{
		for(j = 0;j<	 MAXCHANNEL	;j++) 
		{
			Channels[i][j].DoubtNum = 0;
			Channels[i][j].SuccessNum = 0;
		}
	}
	//用于季度结账用;by gzz 20121009
	IncomeJidu = TranTotal.IncomeJidu;
	NoteIncomeJidu = TranTotal.NoteIncomeJidu;
	CoinsIncomeJidu = TranTotal.CoinsIncomeJidu;
	TotalTransJidu = TranTotal.TotalTransJidu;
	TotalChangeJidu = TranTotal.TotalChangeJidu;
	
	for(i = 0; i < HOPPERCHMAX; i++)
	{
		CoinsChangeJidu[i] = TranTotal.CoinsChangeJidu[i];
	}
	
	tp = (uint8_t *)&TranTotal ;
	for(i = 0; i < (sizeof(TransactionTotal)); i++)
	{		 				
		*tp++ = 0x00;
	}
	TranTotal.TranPages = 20;
	//用于季度结账用;by gzz 20121009
	TranTotal.IncomeJidu  = IncomeJidu;
	TranTotal.NoteIncomeJidu  = NoteIncomeJidu;
	TranTotal.CoinsIncomeJidu  = CoinsIncomeJidu;
	TranTotal.TotalTransJidu  = TotalTransJidu;
	TranTotal.TotalChangeJidu  = TotalChangeJidu;
	for(i = 0; i < HOPPERCHMAX; i++)
	{
		TranTotal.CoinsChangeJidu[i] = CoinsChangeJidu[i];
	}
	return TRUE;
}

//用于季度结账用;by gzz 20121009
uint8_t  MenuFunctionCONFJIDU(void)//	 清除交易记录
{
	uint8_t	*tp;
	uint16_t 	i,j;

	
	for(i = 0;i< MAXLAYER;i++)	
	{
		for(j = 0;j<	 MAXCHANNEL	;j++) 
		{			
			Channels[i][j].SuccessNumJidu = 0;
		}
	}
	
	TranTotal.IncomeJidu  = 0;
	TranTotal.NoteIncomeJidu  = 0;
	TranTotal.CoinsIncomeJidu  = 0;
	TranTotal.TotalTransJidu  = 0;
	TranTotal.TotalChangeJidu  = 0;
	for(i = 0; i < HOPPERCHMAX; i++)
	{
		TranTotal.CoinsChangeJidu[i] = 0;
	}
	return TRUE;
}


uint8_t  MenuFunctionYear(void)			   //设置年
{
	if (InputMode)
	{
		if( (InputNum >2000 ) && (InputNum < 2099 ))
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			TranData.Time.YEAR	= InputNum;
			RTCn_SetTime (RTCN_TYPE_YEAR, (uint32_t)(TranData.Time.YEAR));
			return TRUE;
		}
		else
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
			InputCol = 4;
	InputDisplay( 0x07)	 ;

	return TRUE;
}
uint8_t  MenuFunctionMonth(void)		   //设置月
{
	if (InputMode)
	{
		if( (InputNum >0 ) && (InputNum < 13 ))
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			TranData.Time.MONTH	= InputNum;
			RTCn_SetTime (RTCN_TYPE_MONTH,(uint32_t)(TranData.Time.MONTH));
	   		return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
		InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionDate(void)				//设置日
{
	if (InputMode)
	{
		if( (InputNum >0 ) && (InputNum < 32 ))
		{
		
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			TranData.Time.DOM	= InputNum;
			RTCn_SetTime (RTCN_TYPE_DATE,(uint32_t)(TranData.Time.DOM));
			
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionHour(void)				   //设置时
{
	if (InputMode)
	{
		if(InputNum <= 24 )
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			TranData.Time.HOUR	= InputNum;
			RTCn_SetTime (RTCN_TYPE_HOUR, (uint32_t)(TranData.Time.HOUR));
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionMinute(void)				 //设置分
{
	if (InputMode)
	{
		if(InputNum < 60 )
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			TranData.Time.MIN	= InputNum;
			RTCn_SetTime (RTCN_TYPE_MIN, (uint32_t)(TranData.Time.MIN));
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	return TRUE;
}


//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionCompressOn(void)//压缩机时间控制
{
	if (InputMode)
	{	
	
		if(InputNum <=2)  
		{				 //选择LCD
			SYSPara2.CompressorTime.DeviceOn	= InputNum;
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionCompressHourOn(void)				   //压缩机打开时
{
	if (InputMode)
	{
		if(InputNum <= 24 )
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			SYSPara2.CompressorTime.HourOn	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionCompressMinuteOn(void)				 //压缩机打开分
{
	if (InputMode)
	{
		if(InputNum < 60 )
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			SYSPara2.CompressorTime.MinOn	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	return TRUE;
}
uint8_t  MenuFunctionCompressHourOff(void)				   //压缩机关闭时
{
	if (InputMode)
	{
		if( (InputNum >= SYSPara2.CompressorTime.HourOn)&&(InputNum <= 24 ) )
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			SYSPara2.CompressorTime.HourOff	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionCompressMinuteOff(void)				 //压缩机关闭分
{
	if (InputMode)
	{
		if( ((SYSPara2.CompressorTime.HourOff+InputNum) > (SYSPara2.CompressorTime.HourOn+SYSPara2.CompressorTime.MinOn))&&(InputNum < 60 ) )
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			SYSPara2.CompressorTime.MinOff	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	return TRUE;
}


//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDOn(void)//展示灯时间控制
{
	if (InputMode)
	{	
	
		if(InputNum <=2)  
		{				 //选择LCD
			SYSPara2.LEDTime.DeviceOn	= InputNum;
			Cursor_Off(LCDUP)	;
			Cursor_Off(LCDDOWN)	;
			InputCol = 8;
			InputMode = FALSE;
		}	
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	else
	{
			InputCol = 1;
			InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionLEDHourOn(void)				   //展示灯打开时
{
	if (InputMode)
	{
		if(InputNum <= 24 )
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			SYSPara2.LEDTime.HourOn	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionLEDMinuteOn(void)				 //展示灯打开分
{
	if (InputMode)
	{
		if(InputNum < 60 )
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			SYSPara2.LEDTime.MinOn	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	return TRUE;
}
uint8_t  MenuFunctionLEDHourOff(void)				   //展示灯关闭时
{
	if (InputMode)
	{
		if( (InputNum >= SYSPara2.LEDTime.HourOn)&&(InputNum <= 24 ) )
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			SYSPara2.LEDTime.HourOff	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}
uint8_t  MenuFunctionLEDMinuteOff(void)				 //展示灯关闭分
{
	if (InputMode)
	{
		if( ((SYSPara2.LEDTime.HourOff+InputNum) > (SYSPara2.LEDTime.HourOn+SYSPara2.LEDTime.MinOn))&&(InputNum < 60 ) )
		{
			Cursor_Off(LCDUP)	;
			InputCol = 8;
			InputMode = FALSE;
			SYSPara2.LEDTime.MinOff	= InputNum;
			
		  		
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	return TRUE;
}


uint8_t  MenuFunctionNEWPW(void)//new password
{
	if (InputMode)
	{
		
		
			TempNewPassword	= InputNum;
			Cursor_Off(LCDUP)	;
			InputMode = FALSE;
		
	}
	else
	{
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionNEWPW2(void)//new password
{
	if (InputMode)
	{
		
		
			TempNewPassword2	= InputNum;
			Cursor_Off(LCDUP)	;
			InputMode = FALSE;
		
	}
	else
	{
		InputDisplay( 0x07)	 ;
	}
	return TRUE;
}
uint8_t  MenuFunctionLAYIN(void)				//输入层号
{
  


	if((InputNum == 0)||(InputNum  > (MAXLAYER )) )  
	{
		InputDone = FALSE;		  //20110823
		return FALSE;
	}
	NowLayerNo = InputNum -1;
	Cursor_Off(LCDUP);
	InputCol = 8;
	InputMode = FALSE;
	return TRUE;
	
}
//

//

uint8_t  MenuFunctionLAYENABLE(void)			//开启层号
{
	uint8_t i,LayNum =0 ;

	if (InputMode)
	{
		if( (InputNum < 2 ))
		{
			Cursor_Off(LCDUP);
			InputCol = 8;
			InputMode = FALSE;
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

uint8_t  MenuFunctionMAXGOODS(void)					//最大填货数
{
//uint8_t i,ChannelNum =0 ;
	uint8_t i = 0;

	if (InputMode)
	{
		if((InputNum <= MAXGOODS ))
		{
			
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			SYSPara.LayMaxGoods[NowLayerNo] = InputNum;
			for(i = 0; i < MAXCHANNEL;i++)
			{
				colSystemPara.ColMaxGoods[NowLayerNo][i] = InputNum;
			}
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)	 ;
	
	return TRUE;
}

uint8_t  MenuFunctionCH(uint8_t ID)				  //开启货道
{
	uint8_t i,ChannelNum =0 ;

	if (InputMode)
	{
		if((InputNum < 2 ))
		{
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
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
uint8_t  MenuFunctionCH1(void)
{
	return  MenuFunctionCH(0);
}
uint8_t  MenuFunctionCH2(void)
{
	return  MenuFunctionCH(1);
}
uint8_t  MenuFunctionCH3(void)
{
	return  MenuFunctionCH(2);
}
uint8_t  MenuFunctionCH4(void)
{
	return  MenuFunctionCH(3);
}
uint8_t  MenuFunctionCH5(void)
{
	return  MenuFunctionCH(4);
}
uint8_t  MenuFunctionCH6(void)
{
	return  MenuFunctionCH(5);
}
uint8_t  MenuFunctionCH7(void)
{
	return  MenuFunctionCH(6);
}
uint8_t  MenuFunctionCH8(void)
{
	return  MenuFunctionCH(7);
}


//121105 by cq TenChannels
uint8_t  MenuFunctionCH9(void)
{
#if (MAXCHANNEL >= 9)
		return	MenuFunctionCH(8);
#else
		return TRUE;
#endif
}

uint8_t  MenuFunctionCH0(void)
{
#if (MAXCHANNEL >= 10)
		return	MenuFunctionCH(9);
#else
		return TRUE;
#endif
}

//按货道添存货量;by gzz 20121120
uint8_t  MenuFunctionCHNum(uint8_t ID)				  //货道存货数量配置
{
//uint8_t i,ChannelNum =0 ;
	uint8_t i = 0;

	if (InputMode)
	{
		if((InputNum <= MAXGOODS ))
		{
			
			Cursor_Off(LCDUP)	;
				InputCol = 8;
			InputMode = FALSE;
			colSystemPara.ColMaxGoods[NowLayerNo][ID] = InputNum;
			return TRUE;
		}
		else 
		{
			InputDone = FALSE;		  //20110823
			return FALSE;
		}
	}
	InputCol = 2;
	InputDisplay( 0x07)  ;
	
	return TRUE;
}


uint8_t  MenuFunctionCH1Num(void)
{
	return  MenuFunctionCHNum(0);
}
uint8_t  MenuFunctionCH2Num(void)
{
	return  MenuFunctionCHNum(1);
}
uint8_t  MenuFunctionCH3Num(void)
{
	return  MenuFunctionCHNum(2);
}
uint8_t  MenuFunctionCH4Num(void)
{
	return  MenuFunctionCHNum(3);
}
uint8_t  MenuFunctionCH5Num(void)
{
	return  MenuFunctionCHNum(4);
}
uint8_t  MenuFunctionCH6Num(void)
{
	return  MenuFunctionCHNum(5);
}
uint8_t  MenuFunctionCH7Num(void)
{
	return  MenuFunctionCHNum(6);
}
uint8_t  MenuFunctionCH8Num(void)
{
	return  MenuFunctionCHNum(7);
}


//121105 by cq TenChannels
uint8_t  MenuFunctionCH9Num(void)
{
#if (MAXCHANNEL >= 9)
		return	MenuFunctionCHNum(8);
#else
		return TRUE;
#endif
}

uint8_t  MenuFunctionCH0Num(void)
{
#if (MAXCHANNEL >= 10)
		return	MenuFunctionCHNum(9);
#else
		return TRUE;
#endif
}


uint8_t  MenuFunctionCONFSET(void)//	 默认设置
{
	uint8_t	*tp;
	uint16_t 	i,j;

	for(i = 0;i< MAXLAYER;i++)	
	{
		for(j = 0;j<	 MAXCHANNEL	;j++) 
		{
			Channels[i][j].DoubtNum = 0;
			Channels[i][j].SuccessNum = 0;

			//用于季度结账用;by gzz 20121009
			Channels[i][j].SuccessNumJidu = 0;
		}
	}
	//出厂配置时不予删除;by gzz 20121009
	/*
	tp = (uint8_t *)&TranTotal ;
	for(i = 0; i < (sizeof(TransactionTotal)); i++)
	{		 				
		*tp++ = 0x00;
	}
	TranTotal.TranPages = 20;
	*/
	RstSystemPara();//配置系统参数和货道参数
	RstVisiblePara();//配置可视参数
	return TRUE;
}

//用户模式菜单;by gzz 20121130
uint8_t  MenuFunctionADMINSET(void)//	管理员模式
{
	SYSPara1.AdminStat = 1;
	return  TRUE;
}








