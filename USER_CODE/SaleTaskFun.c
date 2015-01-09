/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           SaleTaskFun.c
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        交易任务调用的相关程序,存放跟交易状态，以及显示，比如输入货道页面，或显示投币金额等有关的个页面
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
#include "lcd.h"
#include "context.h"
#include "drv.h"
#include "hopper.h"
#include "SaleTaskFun.h"
#include "VMC_PC.h"
#include "DebugTrace.h"
#include "Function.h"




uint32_t ChgAmount;	  //当前找零值

/*按键处理函数，根据是否有投币，以及根据上个的状态，更新下一个的状态*/
/*参数 state:当前交易状态*/
/*参数 key:按键值*/
/*返回值 交易状态*/
uint8_t KeyProcess(uint8_t state,uint16_t key,uint8_t *ChannelNum)//选货按键;by gzz 20121025
{
	unsigned char x = 0;
	unsigned char y = 0;
	uint8_t   channel,ChannelX,ChannelY;

	//选货按键;by gzz 20121025
	if(SYSPara.KeySel == 1 )
	{
		Trace("\r\n taskb key=%d",key);
		for(x = 0; x<MAXLAYER;x++)	  
		{					
			for(y=0; y<MAXCHANNEL;y++) 
			{  
				channel = x*10+y+11;
				if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
				{
					if( (Channels[ChannelX][ChannelY].GoodsCode == key)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
					{
						*ChannelNum = channel;
						if(g_Amount > 0) 
						{					    
							state = MVC_XUANHUO;
						}
						else
						{
							state = MVC_CHAXUN;
						}
						Trace("\r\n taskb1 channel=%d,state=%d",*ChannelNum,state);
						return state;
					}  
					if(Channels[ChannelX][ChannelY].GoodsCode == key)
					{
						*ChannelNum = channel;
						if(g_Amount > 0) 
						{					    
							state = MVC_XUANHUO;
						}
						else
						{
							state = MVC_CHAXUN;
						}
						Trace("\r\n taskb2 channel=%d,state=%d",*ChannelNum,state);
					}  	
				} 
			}  		
		}	
	}
	else
	{
		switch(key) 
		{			    
			case KEY_0:	
				#if CHNINPUT_MODE == TWO_Bit_MODE	
				//如果允许输入数字，并且还没输入两位数字时，输入数字，并且更新为货道号。将状态改为输入货道状态
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(0);
					g_KeyStat.InputCnt++;
					state = MVC_INPUT;    
				}
				#endif
				break;
		
			case KEY_1:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(1);
					g_KeyStat.InputCnt++;
					state = MVC_INPUT;    
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 1;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0)
					{
					    state = MVC_XUANHUO;
					} 
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_2:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(2);
					g_KeyStat.InputCnt++;  
					state = MVC_INPUT;   
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 2;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					}
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_3:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(3);
					g_KeyStat.InputCnt++;   
					state = MVC_INPUT;  
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 3;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					}
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_4:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(4);
					g_KeyStat.InputCnt++; 
					state = MVC_INPUT;    
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 4;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					} 
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_5:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(5);
					g_KeyStat.InputCnt++;  
					state = MVC_INPUT;   
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 5;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					}
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_6:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(6);
					g_KeyStat.InputCnt++; 
					state = MVC_INPUT;    
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 6;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					}
					else
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_7:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(7);
					g_KeyStat.InputCnt++;  
					state = MVC_INPUT;   
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 7;
				if(g_KeyStat.InputMode > 0) 
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					}
					else
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_8:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(8);
					g_KeyStat.InputCnt++; 
					state = MVC_INPUT;    
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 8;
				if(g_KeyStat.InputMode > 0)
				{
					if(g_Amount > 0)
					{
					    state = MVC_XUANHUO;
					}
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;
		
			case KEY_9:
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if((g_KeyStat.InputMode > 0) && (g_KeyStat.InputCnt < 2)) 
				{
				    g_KeyStat.InputNum = CalcInputNum(9);
					g_KeyStat.InputCnt++; 
					state = MVC_INPUT;    
				}
				#elif CHNINPUT_MODE == ONE_Bit_MODE
				g_KeyStat.InputNum = 9;
				if(g_KeyStat.InputMode > 0)
				{
					if(g_Amount > 0) 
					{
					    state = MVC_XUANHUO;
					} 
					else 
					{
					    state = MVC_CHAXUN;
					}
				}
				#endif
				break;

			case KEY_CLR:
				//按clr键清掉一个货道数字。将状态改为输入货道状态
				#if CHNINPUT_MODE == TWO_Bit_MODE
				if(g_KeyStat.InputCnt > 0) 
				{
				    g_KeyStat.InputNum = g_KeyStat.InputNum/10;
					g_KeyStat.InputCnt--; 
					state = MVC_INPUT;    
				}
				#endif
				break;
		
			case KEY_ENTER:
				//按下enter键后，根据是否有投币，状态设为查询货道状态，或者选货状态，并且根据上个的状态，更新下一个的状态，比如继续交易，或者加钱
				switch(state) 
				{
				    case MVC_INPUT:
						#if CHNINPUT_MODE == TWO_Bit_MODE
						if((g_Amount > 0) && (g_KeyStat.InputCnt >= 2)) 
						{					    
							state = MVC_XUANHUO;
						}
						#endif
						break;
			
					case MVC_RETRANS:
						state = MVC_SALE;
						break;
			
					case MVC_LESSMONEY:
						state = MVC_ADDMONEY;
						break;
			
					default:
						break;
				}
				break;
		
			case KEY_CANCEL:
				switch(state) 
				{
				    //如果当前是在输入数字按键的状态，如果已经有输入一个数字了，执行 RstInputNum();函数直接将这个数字清空
				    //如果没有，则根据是否有投币，状态更新为交易状态，还是初始空闲状态
				    case MVC_INPUT:
						#if CHNINPUT_MODE == TWO_Bit_MODE
						if(g_KeyStat.InputCnt == 0) 
						{
						    if(g_Amount > 0) 
							{					    
							    state = MVC_SALE;
								#if DISP_MODE == LCD_DISPLAY
							    Cursor_Off(LCDUP);
							    #endif
							} 
							else 
							{
							   	RstInputNum();
								sysVPMission.dspTimer2 = 0;//加快显示空闲界面;by gzz 20121106
								state = MVC_BEGIN;
							}
					    } 
						else 
						{
						    RstInputNum();
						}
						#endif
						break;
					//如果当前是交易状态或加钱状态，按下cancel后根据是否交易过一次后，或允许不买货退币，进入找零状态
					case MVC_SALE:
					case MVC_ADDMONEY:
						//如果有交易过，或允许不买货退币，则退币
						if((g_Amount > 0) && (IsHaveSaled() || (SYSPara.TransEscape > 0)) 
							//读卡器没开，或者当前卡里面没钱
							&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0))) 
						{
						    state = MVC_PAYOUT;
					    }
						break;
			        //如果当前是继续交易状态，更新为找零状态
					case MVC_RETRANS:
						state = MVC_PAYOUT;
						break;
			        //如果当前是钱不够状态，更新为交易状态
					case MVC_LESSMONEY:
						state = MVC_SALE;
						break;
			
					default:
						break;
				}				
				break;
		
			default:
				break;
		}
	}
	
	return state;
}

/*操作超时时，根据上个的状态，切换交易状态过程*/
/*参数 state:当前交易状态*/
/*返回值 交易状态*/
uint8_t SaleTimeoutProc(uint8_t state)
{
     switch(state) 
	 {
	     case MVC_INPUT:
			 RstInputNum();
		
			 #if DISP_MODE == LCD_DISPLAY
		     Cursor_Off(LCDUP);
		     #endif
		
			 if(g_Amount > 0) 
			 {			 
				 state = MVC_SALE;
			 } 
			 else 
			 {
			     state = MVC_BEGIN;
			 }
			 break;
			 /*
			 case SALE:
			 case ADDMONEY:
			 TranTotal.Iou = g_Amount;	  	//
			 XSystemPara(WRITE);
			 state = CHAOSHI;
			 break;
			 */
		 case MVC_LESSMONEY:
			 state = MVC_SALE;
			 break;
	
		 case MVC_RETRANS:
			 state = MVC_PAYOUT;
			 break;
	
		 default:
		 	break;
	 }
	 return state;
}

/*依据小数位数，金额的函数*/
/*参数   dispnum:投币金额，以分为单位 dec:小数位*/
/*返回值 显示字符串*/
char strnum[10];
char *SprintDec(uint32_t dispnum, uint8_t dec)
{  
   switch(dec) 
   {
      case 0:
	  sprintf(strnum,"%d",dispnum);
	  break;

	  case 1:
	  sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
	  break;
	  
	  default:
	  sprintf(strnum,"%02d.%02d",dispnum/100,dispnum%100);
	  break;
   }
   return strnum;
}

/*显示暂停服务，和错误代码*/
void DispErrText(void)
{
	#if DISP_MODE == LCD_DISPLAY
    char    str2[48],strtemp[20];
    #endif	

	uint8_t  i;
	uint16_t status;
	uint8_t len = 0;	
	uint8_t str[48];

	status = GetDevStatus();

	if(status > 0) 
	{
		for(i = 16; i > 0; i--) 
		{
		    if(status & (1 << i)) 
			{
			    break;
			}
		}
	
		switch(1 << i) 
		{
		    case DEV_ERR_BIT_NOTE:
				status = DEV_ERR_NOTE;
				break;
	
			case DEV_ERR_BIT_COIN:
				status = DEV_ERR_COIN;
				break;
	
			case DEV_ERR_BIT_CHANGER:
				status = DEV_ERR_CHANGER;
				break;
	
			case DEV_ERR_BIT_COINLOW:
				status = DEV_ERR_COINLOW;
				break;
	
			case DEV_ERR_BIT_CHNOUT:
				status = DEV_ERR_CHNOUT;
				break;
	
			case DEV_ERR_BIT_CHNALL:
				status = DEV_ERR_CHNALL;
				break;
	
			case DEV_ERR_BIT_DRVCOMM:
				status = DEV_ERR_DRVCOMM;
				break;
	
			case DEV_ERR_BIT_CHNDIST:
				status = DEV_ERR_CHNDIS;
				break;
	
			case DEV_ERR_BIT_PARASET:
				status = DEV_ERR_PARASET;
				break;
	
			case DEV_ERR_BIT_LCDCOMM:
				status = DEV_ERR_LCDCOMM;
				break;
	
			case DEV_ERR_BIT_PRICESET:
				status = DEV_ERR_PRICESET;
				break;
	
			case DEV_ERR_BIT_GOODSCHK:
				status = DEV_ERR_GOODSCHK;
				break;
	
			case DEV_ERR_BIT_FLASH:
				status = DEV_ERR_FLASH;
				break;

			case DEV_ERR_BIT_RTC:
				status = DEV_ERR_RTC;
				break;
	
			default:
				RstDevStatus();
				break;
	
		}
   	
		#if DISP_MODE == LCD_DISPLAY	
		//sprintf(strtemp, "%02x",status);
		//strcpy(str2, Disp_Fault[SYSPara.Language][1]);
		//strcat(str2, strtemp);
				 
		//WriteFull_LCD(LINE0,Disp_Fault[SYSPara.Language][0],strlen(Disp_Fault[SYSPara.Language][0]));
		sprintf(strtemp, "(%02x)",status);
		//strcpy(str2, Disp_Fault[SYSPara.Language][1]);
		//strcat(str2, strtemp);
		strcpy(str2, strtemp);
		switch(status)
		{
			case DEV_ERR_NOTE:
				strcat(str2, Disp_Fault[SYSPara.Language][4]);	
				break;
			case DEV_ERR_COIN:
				strcat(str2, Disp_Fault[SYSPara.Language][5]);	
				break;	
			case DEV_ERR_CHANGER:
				strcat(str2, Disp_Fault[SYSPara.Language][3]);	
				break;	
			case DEV_ERR_COINLOW:
				strcat(str2, Disp_Fault[SYSPara.Language][6]);	
				break;	
			case DEV_ERR_CHNOUT:
				strcat(str2, Disp_Fault[SYSPara.Language][7]);	
				break;	
			case DEV_ERR_CHNALL:
				strcat(str2, Disp_Fault[SYSPara.Language][8]);	
				break;	
			case DEV_ERR_DRVCOMM:
				strcat(str2, Disp_Fault[SYSPara.Language][9]);	
				break;		
			case DEV_ERR_CHNDIS:
				strcat(str2, Disp_Fault[SYSPara.Language][10]);	
				break;	
			case DEV_ERR_PARASET:
				strcat(str2, Disp_Fault[SYSPara.Language][11]);	
				break;	
			case DEV_ERR_LCDCOMM:
				strcat(str2, Disp_Fault[SYSPara.Language][12]);	
				break;	
			case DEV_ERR_PRICESET:
				strcat(str2, Disp_Fault[SYSPara.Language][13]);	
				break;	
			case DEV_ERR_GOODSCHK:
				strcat(str2, Disp_Fault[SYSPara.Language][14]);	
				break;	
			case DEV_ERR_FLASH:
				strcat(str2, Disp_Fault[SYSPara.Language][15]);	
				break;	
			case DEV_ERR_RTC:
				strcat(str2, Disp_Fault[SYSPara.Language][16]);	
				break;	
			//case DEV_ERR_USERDOOR:
			//	strcat(str2, Disp_Fault[SYSPara.Language][17]);	
			//	break;	
			//case DEV_ERR_MAINDOOR:
			//	strcat(str2, Disp_Fault[SYSPara.Language][18]);	
			//	break;		
		}
		if(sysVPMission.dspErrTimer == 0)	
		{
			sysVPMission.dspErrTimer = VP_DSP_TIME4*3;	
			if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			{			
				ClearScreen();
				RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);//获取rtc时间
				OSTimeDly(2);
				//1故障提示
				Pitu_LCDPrintf(LINE01,0,"↑",strlen("↑"),0,3);
				Pitu_LCDPrintf(LINE01,40,Disp_Fault[1][19],strlen(Disp_Fault[1][19]),0,1);
				Pitu_LCDPrintf(LINE03,40,Disp_Fault[1][0],strlen(Disp_Fault[1][0]),0,0);
				strcpy(str2, Disp_Fault[SYSPara.Language][0]);
				strcat(str2, strtemp);
				Pitu_LCDPrintf(LINE11,40,str2,strlen(str2),0,0);
				//Pitu_LCDPrintf(LINE13,40,str2,strlen(str2),0,0);
				//2划线
				Pitu_LCDClearLine(LINE22,1,0);
				//3时间日期
				len = sprintf( str, "%04d", TranData.Time.YEAR);
				Pitu_LCDPrintf(LINE23,40,str,len,0,1);
				len = sprintf( str, "%02d-%02d",TranData.Time.MONTH,TranData.Time.DOM);
				Pitu_LCDPrintf(LINE31,40,str,len,0,1);
				len = sprintf( str, "%02d:%02d", TranData.Time.HOUR, TranData.Time.MIN);
				Pitu_LCDPrintf(LINE23,96,str,len,0,3);	
				OSTimeDly(OS_TICKS_PER_SEC / 10);
			}
			else
			{
				WriteFull_LCD(LINE0,Disp_Fault[SYSPara.Language][0],strlen(Disp_Fault[SYSPara.Language][0]));
				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{			
					WriteFull_LCD(LINE1,"",0);
					WriteFull_LCD(LINE2,str2,strlen(str2));
					WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
				}
				else
				{
					WriteFull_LCD(LINE1,str2,strlen(str2));
				}
			}
		}
		
		
	    #endif
	}
}

/*显示正在初始化*/
void DispInitText(void)
{
#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		Pitu_LCDClearLine(LINE1, 0,0);
		Pitu_LCDPutLOGOBmp(LINE1,20,SYSPara.LogoSel);
		//OSTimeDly(200*3);
		//Pitu_LCDPutLOGOBmp(LINE13,20,2);
		//OSTimeDly(200*3);
		WriteFull_LCD(LINE3,Disp_Init[SYSPara.Language][0],strlen(Disp_Init[SYSPara.Language][0]));
		WriteFull_LCD(LINE32,Disp_Wait[SYSPara.Language][0],strlen(Disp_Wait[SYSPara.Language][0]));
	}
	else
	{
		WriteFull_LCD(LINE0,Disp_Init[SYSPara.Language][0],strlen(Disp_Init[SYSPara.Language][0]));
		WriteFull_LCD(LINE1,Disp_Wait[SYSPara.Language][0],strlen(Disp_Wait[SYSPara.Language][0]));
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())
		{
			WriteFull_LCD(LINE2,"",0);
			WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
		}
	}
#endif
}


//120704 by cq DispVersion
/*显示软件版本号*/
void DispVersionText(void)
{
	uint16_t	tempVersion;
	char  		VerInit[48];
	uint8_t	    Verlen;
	#if DISP_MODE == LCD_DISPLAY
	//120704 by cq DispVersion
	//tempVersion = SOFTVERSION;
	tempVersion = 143;
	tempVersion = (tempVersion & 0x00ff);
	Verlen=sprintf( VerInit,"    Version%d.%02d",tempVersion/100, tempVersion%100);
	//Verlen=sprintf( VerInit,"    7636  V%d.%d",tempVersion/100, tempVersion%100);
	//添加品途屏幕的兼容;by gzz 20121025
	if(IsLargeLCD())
	{
		WriteFull_LCD(LINE1,VerInit,Verlen);
		WriteFull_LCD(LINE2,"",0);
		WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
	}
	else
	{
		WriteFull_LCD(LINE0,VerInit,Verlen);
		WriteFull_LCD(LINE1,"",0);
	}
	OSTimeDly(OS_TICKS_PER_SEC);
	#endif
}


/*在空闲状态下，交替显示现金消费，请插入纸币或硬币*/
/*参数 mode:显示方式*/
void DispBeginText(uint8_t mode)
{
	uint8_t pNum,DispLine0,DispLine1;
	uint8_t len = 0;	
	uint8_t str[48];
	static uint8_t timeFLag1 = 0,timeFLag2 = 0;

#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		ClearScreen();//拼图屏幕样式;by gzz 20121106
		RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);//获取rtc时间
		OSTimeDly(2);
		
		Pitu_LCDPutLOGOBmp(LINE0,75,SYSPara.LogoSel);
		len = sprintf( str, "%04d-%02d-%02d",  TranData.Time.YEAR,TranData.Time.MONTH,TranData.Time.DOM);
		//Trace("\r\n %04d-%02d-%02d, %02d:%02d",TranData.Time.YEAR,TranData.Time.MONTH,TranData.Time.DOM,TranData.Time.HOUR, TranData.Time.MIN);
		if( ((TranData.Time.MONTH>=13)||(TranData.Time.DOM>=32))&&(timeFLag1<3) )
		{
			timeFLag1++;
		}
		else
		{
			timeFLag1 = 0;
			Pitu_LCDPrintf(LINE12,66,str,len,0,1);
		}
		
		len = sprintf( str, "%02d:%02d", TranData.Time.HOUR, TranData.Time.MIN);
		if( ((TranData.Time.HOUR>=25)||(TranData.Time.MIN>=62))&&(timeFLag2<3) )
		{
			timeFLag2++;
		}
		else
		{
			timeFLag2 = 0;
			Pitu_LCDPrintf(LINE2,51,str,len,0,2);
		}
	}
	else
	{
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())
		{
			DispLine0 = LINE0;
			DispLine1 = LINE2;
			//WriteFull_LCD(LINE1,"",0);//120621 by cq LCDswitch


			//120621 by cq LCDswitch
			RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);//获取rtc时间
			OSTimeDly(2);
			/*
			if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			{
				ClearScreen();//拼图屏幕样式;by gzz 20121106
				len = sprintf( str, "		  %d-%d-%d", TranData.Time.YEAR,TranData.Time.MONTH,TranData.Time.DOM);
				Pitu_LCDPrintf(LINE03,0,str,len,0,1);
				len = sprintf( str, "  %02d:%02d", TranData.Time.HOUR, TranData.Time.MIN);
				Pitu_LCDPrintf(LINE11,0,str,len,0,2);
				Pitu_LCDClearLine(LINE32,0,0);
			}
			else
			*/
			{
				len = sprintf( str, "  %d/%d/%d %02d:%02d", TranData.Time.YEAR,TranData.Time.MONTH,TranData.Time.DOM,TranData.Time.HOUR, TranData.Time.MIN);
			
				WriteFull_LCD(LINE1,str,len);
				//Trace("%s",str);
				//WriteFull_LCD(LINE3," O(*^_^*)O \\(*^o^*)/",strlen(" O(*^_^*)O \\(*^o^*)/"));
				//WriteFull_LCD(LINE3,Disp_Input[SYSPara.Language][6],strlen(Disp_Input[SYSPara.Language][6]));
				//WriteFull_LCD(LINE3,"						 ",24);//120920 by cq DebugModify
				WriteFull_LCD(LINE3,Disp_Input[SYSPara.Language][6],strlen(Disp_Input[SYSPara.Language][6]));
			}
		}
		else
		{
			DispLine0 = LINE0;
			DispLine1 = LINE1;
		}
		
		if(mode == 0) 
		{
			/*
			if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			{
				Pitu_LCDPrintf(LINE0,0,Disp_Main[SYSPara.Language][0],strlen(Disp_Main[SYSPara.Language][0]),0,0);
				Pitu_LCDPrintf(LINE3,0,Disp_Main[SYSPara.Language][1],strlen(Disp_Main[SYSPara.Language][1]),0,0);
			}
			else
				*/
			{
				WriteFull_LCD(DispLine0,Disp_Main[SYSPara.Language][0],strlen(Disp_Main[SYSPara.Language][0]));
				WriteFull_LCD(DispLine1,Disp_Main[SYSPara.Language][1],strlen(Disp_Main[SYSPara.Language][1]));
			}
		} 
		//120914 by cq Cashless
		else if(SYSPara.ReaderEnable)
		{
			WriteFull_LCD(DispLine0,Disp_Main_2[SYSPara.Language][0],strlen(Disp_Main_2[SYSPara.Language][0]));
			WriteFull_LCD(DispLine1,Disp_Main_2[SYSPara.Language][1],strlen(Disp_Main_2[SYSPara.Language][1]));
		}
		else 
		{
			if(CashState.ChangerState != CHANGER_OK) 
			{
				pNum = 4;
			} 
			else if(CashState.NoteState != NOTE_DEV_OK) 
			{
				pNum = 3;
			} 
			else if(CashState.CoinState != COIN_DEV_OK) 
			{
				pNum = 2;
			} else 
			{
				pNum = 1;
			}

			
			
			if(sysXMTMission.recSVTemp>0)
			{
				if((sysXMTMission.recSVTemp/10)<10)
				{
					len = sprintf( str, "现温%d.%dC  设定%d.%dC", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10,sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
				}
				else
				{
					len = sprintf( str, "现温%d.%dC 设定%d.%dC", sysXMTMission.recPVTemp/10, sysXMTMission.recPVTemp%10,sysXMTMission.recSVTemp/10, sysXMTMission.recSVTemp%10 );
				}
				/*
				if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
				{
					Pitu_LCDPrintf(LINE0,0,str,len,0,0);
				}
				else*/
				{
					WriteFull_LCD(DispLine0,str,len);
				}
			}
			else
			{
				if(pNum == 4) 
				{
					/*
					if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
					{
						Pitu_LCDPrintf(LINE0,0,Disp_Input[SYSPara.Language][5],strlen(Disp_Input[SYSPara.Language][5]),0,0);
					}
					else*/
					{
						WriteFull_LCD(DispLine0,Disp_Input[SYSPara.Language][5],strlen(Disp_Input[SYSPara.Language][5]));
					}
				} 
				else 
				{
					/*
					if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
					{
						Pitu_LCDPrintf(LINE0,0,Disp_Input[SYSPara.Language][0],strlen(Disp_Input[SYSPara.Language][0]),0,0);
					}
					else*/
					{
						WriteFull_LCD(DispLine0,Disp_Input[SYSPara.Language][0],strlen(Disp_Input[SYSPara.Language][0]));
					}
				}
			}	
			/*
			if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			{
				Pitu_LCDPrintf(LINE3,0,Disp_Input[SYSPara.Language][pNum],strlen(Disp_Input[SYSPara.Language][pNum]),0,0);
			}
			else*/
			{
				WriteFull_LCD(DispLine1,Disp_Input[SYSPara.Language][pNum],strlen(Disp_Input[SYSPara.Language][pNum])); 
			}

		}
	}
#endif
}


/*空闲状态下，显示查询结果,是缺货，故障，还是禁能，正常就显示金额*/
/*参数 x:货道层号 y:货道列号*/
void DispChaxunText(uint8_t x,uint8_t y,uint8_t  KeyInput)//选货按键;by gzz 20121025
{			 
	 #if DISP_MODE == LCD_DISPLAY
	 char	str1[48];
  	 char	str2[48];
	 char    strtemp[20];
	 char    *pstr;
	 #endif
	 			    
	 #if DISP_MODE == LCD_DISPLAY	 
	 //选货按键;by gzz 20121025
	 if(SYSPara.KeySel == 1 )
	 {
	 	sprintf(strtemp,"%2d",KeyInput);
	 }
	 else
	 {
	 	sprintf(strtemp,"%2d",g_KeyStat.InputNum);
	 }	 
	 strcpy(str1, Disp_ChInput[SYSPara.Language][0]);
	 strcat(str1, strtemp);
											    
	 strcpy(str2, Disp_ChnPara[SYSPara.Language][0]);
	 strcat(str2, strtemp);
	 if((Channels[x][y].ChannelStat & DRV_STAT_MASK) == CHANNELHARDERR)
	 {
		 strcat(str2, Disp_ChnState[SYSPara.Language][2]);
	 } 
	 else if((Channels[x][y].ChannelStat & DRV_STAT_MASK) == CHANNELDISABLE)
	 {
		 strcat(str2, Disp_ChnState[SYSPara.Language][0]);
	 } 
	 else if((Channels[x][y].ChannelStat & DRV_STAT_MASK) == CHANNELOVER)
	 {
		 strcat(str2, Disp_ChnState[SYSPara.Language][3]);
	 }
	 else 
	 {
		 pstr = SprintDec(Channels[x][y].GoodsPrice,SYSPara.Decimal);
	     strcpy(str2, Disp_ChnPara[SYSPara.Language][1]);	
	     strcat(str2, pstr);	
	 }

	 if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	 {
		//ClearScreen();//拼图屏幕样式;by gzz 20121106
		//划线
		Pitu_LCDClearLine(LINE22,1,0);
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,str1,strlen(str1),0,0);
		Pitu_LCDPrintf(LINE31,40,str2,strlen(str2),0,0);
		OSTimeDly(OS_TICKS_PER_SEC*2);
	 }
	 else
	 {
		 WriteFull_LCD((uint8_t)LINE0,str1,strlen(str1));
		 WriteFull_LCD((uint8_t)LINE1,str2,strlen(str2));
	 }
	 //change by liya 2012-10-16
	 WriteFull_LCD((uint8_t)LINE2,"",0);
	 #endif					 
}

/*显示操作超时*/
void DispChaoshiText(void)
{
    #if DISP_MODE == LCD_DISPLAY
	WriteFull_LCD((uint8_t)LINE0,Disp_Timeout[SYSPara.Language][0],strlen(Disp_Timeout[SYSPara.Language][0]));
	WriteFull_LCD((uint8_t)LINE1,Disp_Blank,strlen(Disp_Blank));
	#endif
}

/*交易状态下和空闲状态下，显示输入货道的页面*/
/*返回值 交易状态*/
uint8_t DispInputText(void)
{
#if DISP_MODE == LCD_DISPLAY
	char	str1[48];
	char	strtemp[20];
#endif
	
	uint8_t state = MVC_INPUT;

	
	if(g_KeyStat.InputCnt > 0) //已经输入一位按键
	{
	#if DISP_MODE == LCD_DISPLAY	   		 		  
		strcpy(str1,Disp_ChInput[SYSPara.Language][0]);
		if(g_KeyStat.InputCnt >= 2) 
		{
			sprintf(strtemp,"%02d",g_KeyStat.InputNum);
		}
		else 
		{
			sprintf(strtemp,"%d",g_KeyStat.InputNum);
		}
		strcat(str1,strtemp);	  
		if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		{
			//ClearScreen();//拼图屏幕样式;by gzz 20121106
			//划线
			Pitu_LCDClearLine(LINE22,1,0);
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,str1,strlen(str1),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_ChInput[1][0],strlen(Disp_ChInput[1][0]),0,0);
			//OSTimeDly(OS_TICKS_PER_SEC);
		}
		else
		{
			WriteFull_LCD(LINE0,str1,strlen(str1)); 
			if(g_Amount == 0) 
			{
				WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));
				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					WriteFull_LCD(LINE2,Disp_Blank,strlen(Disp_Blank));
					WriteFull_LCD(LINE3,Disp_Blank,strlen(Disp_Blank));
				}
			}
			else 
			{	   
				/*
				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
				{
					WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][2],strlen(Disp_Confirm[SYSPara.Language][2]));
					WriteFull_LCD(LINE2,Disp_Confirm[SYSPara.Language][3],strlen(Disp_Confirm[SYSPara.Language][3]));
				}
				else
				{
					WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][0],strlen(Disp_Confirm[SYSPara.Language][0]));
				}	
				*/
				WriteFull_LCD(LINE1,"",0);
				WriteFull_LCD(LINE2,"",0);
				WriteFull_LCD(LINE3,"",0);
			}	   
			SetCursorPost(LINE0+(strlen(Disp_ChInput[SYSPara.Language][0])+1)/2);					
			if(g_KeyStat.InputCnt < 2)
			{
				Cursor_On(LCDUP);
			}
			else
			{
				Cursor_Off(LCDUP);
			}	   
		}
    #endif
	
		if((g_Amount == 0) && (g_KeyStat.InputCnt >= 2)) 
		{
			state = MVC_CHAXUN; 		
		}
		//不用按enter直接出货;by gzz 20121114
		else if((g_Amount > 0) && (g_KeyStat.InputCnt >= 2)) 
		{		
			//Trace("\r\n try to xuanhuo,readksale=%d",sysVPMission.readySale);
			OSTimeDly(OS_TICKS_PER_SEC / 5);
			state = MVC_XUANHUO;			
		}
	}
	else //还未输入按键
	{
	#if DISP_MODE == LCD_DISPLAY
		if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		{
			//ClearScreen();
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_ChInput[SYSPara.Language][0],strlen(Disp_ChInput[SYSPara.Language][0]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_ChInput[1][0],strlen(Disp_ChInput[1][0]),0,0);
		}
		else
		{
			WriteFull_LCD(LINE0,Disp_ChInput[SYSPara.Language][0],strlen(Disp_ChInput[SYSPara.Language][0]));
			if(g_Amount == 0)
			{
				WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));
			} 
			
			else 
			{
				//
				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
				{
					//WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][2],strlen(Disp_Confirm[SYSPara.Language][2]));
					//WriteFull_LCD(LINE2,Disp_Confirm[SYSPara.Language][3],strlen(Disp_Confirm[SYSPara.Language][3]));
					WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));
					WriteFull_LCD(LINE2,Disp_Blank,strlen(Disp_Blank));
				}
				else
				{
					//WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][0],strlen(Disp_Confirm[SYSPara.Language][0]));
					WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));
				}	 
			}
			SetCursorPost(LINE0+(strlen(Disp_ChInput[SYSPara.Language][0])+1)/2);
			Cursor_On(LCDUP);
		}
	#endif
	}
	
	return state;
}


/*显示货道输入错误*/
void DispInputErrText(void)
{
	#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();//拼图屏幕样式;by gzz 20121106
		//划线
		Pitu_LCDClearLine(LINE22,1,0);
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,Disp_ChInput[SYSPara.Language][1],strlen(Disp_ChInput[SYSPara.Language][1]),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_ChInput[1][1],strlen(Disp_ChInput[1][1]),0,0);
		//OSTimeDly(OS_TICKS_PER_SEC);
	}
	else
	{
		WriteFull_LCD(LINE0,Disp_ChInput[SYSPara.Language][1],strlen(Disp_ChInput[SYSPara.Language][1]));
	    WriteFull_LCD(LINE1,Disp_ChInput[SYSPara.Language][2],strlen(Disp_ChInput[SYSPara.Language][2]));
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
		{
			WriteFull_LCD(LINE2,"",0);//120920 by cq DebugModify
			WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
		}
	}
	#endif
}

/*显示等待投币的页面*/
void DispSaleText(void)
{
#if DISP_MODE == LCD_DISPLAY
	char	str1[48];
	char	*pstr;
#endif
	
#if DISP_MODE == LCD_DISPLAY
	pstr = SprintDec(g_Amount,SYSPara.Decimal);
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		ClearScreen();
		//首行提示
		if(SYSPara.Language != 1)
		{
			if(IsHaveSaled())
			{
				sprintf(str1,"      %s/%s",Disp_Deal[SYSPara.Language][0],Disp_Deal[1][0]);
			}
			else
			{
				sprintf(str1,"         %s/%s",Disp_Deal[SYSPara.Language][9],Disp_Deal[1][9]);
			}
		}
		else
		{
			if(IsHaveSaled())
			{
				sprintf(str1,"      %s",Disp_Deal[1][0]);
			}
			else
			{
				sprintf(str1,"         %s",Disp_Deal[1][9]);
			}
		}
		Pitu_LCDPrintf(LINE02,0,str1,strlen(str1),0,0);
		//金额
		strcpy(str1, Disp_Deal[SYSPara.Language][8]);
		Pitu_LCDPrintf(LINE11,20,str1,strlen(str1),0,0);
		strcpy(str1, pstr);
		Pitu_LCDPrintf(LINE1,51,str1,strlen(str1),0,2);
		//划线
		Pitu_LCDClearLine(LINE22,1,0);
		//交易提示
		if(IsHaveSaled())
		{
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_Deal[SYSPara.Language][11],strlen(Disp_Deal[SYSPara.Language][11]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_Deal[1][11],strlen(Disp_Deal[1][11]),0,0);

			Pitu_LCDPrintf(LINE23,112,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,152,Disp_Deal[SYSPara.Language][12],strlen(Disp_Deal[SYSPara.Language][12]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,152,Disp_Deal[1][12],strlen(Disp_Deal[1][12]),0,0);
		}
		else
		{
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_Deal[SYSPara.Language][10],strlen(Disp_Deal[SYSPara.Language][10]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_Deal[1][10],strlen(Disp_Deal[1][10]),0,0);
		}
	}
	else
	{
		if(IsHaveSaled())
		{
			strcpy(str1, Disp_Deal[SYSPara.Language][0]);
		}
		else
		{
			strcpy(str1, Disp_Deal[SYSPara.Language][9]);
		}		
		strcat(str1, pstr);

		WriteFull_LCD(LINE0,str1,strlen(str1));
		WriteFull_LCD(LINE1,Disp_ChInput[SYSPara.Language][0],strlen(Disp_ChInput[SYSPara.Language][0]));
		
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())//修改交易提示;by gzz 20121009
		{	
			/*
			WriteFull_LCD(LINE2,Disp_Deal[SYSPara.Language][6],strlen(Disp_Deal[SYSPara.Language][6])); 
			if(IsHaveSaled())
			{
				WriteFull_LCD(LINE3,Disp_Deal[SYSPara.Language][7],strlen(Disp_Deal[SYSPara.Language][7])); 
			}
			else
			{
				WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
			}	
			*/
			WriteFull_LCD(LINE2,"",0);
			WriteFull_LCD(LINE3,"",0);
		}
		
	}
#endif
}



/*显示纸币超过上限*/
void DispOverText(void)
{
#if DISP_MODE == LCD_DISPLAY	
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,Disp_Deal[SYSPara.Language][4],strlen(Disp_Deal[SYSPara.Language][4]),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_Deal[1][4],strlen(Disp_Deal[1][4]),0,0);
	}
	else
	{
	
		WriteFull_LCD((uint8_t)LINE0,Disp_Deal[SYSPara.Language][4],strlen(Disp_Deal[SYSPara.Language][4]));
		WriteFull_LCD((uint8_t)LINE1,Disp_Blank,strlen(Disp_Blank));
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
		{
			WriteFull_LCD(LINE2,"",0);//120920 by cq DebugModify
			WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
		}
	}
#endif
}


/*显示无货*/
void DispWuhuoText(uint8_t x,uint8_t y)
{	
	#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,Disp_Deal[SYSPara.Language][3],strlen(Disp_Deal[SYSPara.Language][3]),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_Deal[1][3],strlen(Disp_Deal[1][3]),0,0);
	}
	else
	{
		//   ClearScreen();			 
		WriteFull_LCD(LINE0,Disp_Deal[SYSPara.Language][3],strlen(Disp_Deal[SYSPara.Language][3]));
		WriteFull_LCD(LINE1,Disp_ChInput[SYSPara.Language][2],strlen(Disp_ChInput[SYSPara.Language][2]));
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
		{
			WriteFull_LCD(LINE2,"",0);//120920 by cq DebugModify
			WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
		}
	}
	#endif
}

/*显示正在出货*/
void DispChuhuoText(void)
{
	#if DISP_MODE == LCD_DISPLAY	
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();//拼图屏幕样式;by gzz 20121106
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][0],strlen(Disp_GoodsOut[SYSPara.Language][0]),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][0],strlen(Disp_GoodsOut[1][0]),0,0);
	}
	else
	{	    
	    WriteFull_LCD(LINE0,Disp_Wait[SYSPara.Language][0],strlen(Disp_Wait[SYSPara.Language][0]));
		WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][0],strlen(Disp_GoodsOut[SYSPara.Language][0]));
		//添加品途屏幕的兼容;by gzz 20121025
		if(IsLargeLCD())//修改输入货道出货的信息;by gzz 20121016
		{
			WriteFull_LCD(LINE2,"",0);//120920 by cq DebugModify
			WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
		}
	}
    #endif 
}

/*发送接收出货消息，作出货操作*/
/*参数 ch:客户输入的货道编号*/
/*参数 x:货道层号 y:货道列号*/
/*返回值 交易状态*/
uint8_t DoChuhuoMsg(uint8_t ch,uint8_t x,uint8_t y)
{
	uint8_t state,err,statusDisp=0;
	uint32_t  *pComStatus;

	//121019 by cq Cashless
	uint8_t cVendSuccessFlag = 0;
	
#if DRVDEVICE
	//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
	OSMboxAccept(g_CommBackMsg);
	MsgBack.CmdType = ch + ((uint32_t)CMD_GOODS_OUT << 24); 
	OSMboxPost(g_CommMsg,&MsgBack.CmdType);//通知系统有出货命令发送

	pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 20,&err);//通知系统出货完成后，它的返回命令(返回所有的出货结果，不一定是成功)
	//等待出货结束
	if(err == OS_NO_ERR) 
	{				//出货情况	

	    /*********GOC打开的情况下********************/
		if(SYSPara.GOCCtr > 0)
		{
			 //1.判断GOC是否故障bit7:
			 if( BeSaleStat() &&(*pComStatus & DRV_GOCERR_BIT) )
			 {
			 	//GOC故障->扣钱，然后goc报故障
				statusDisp=1;
			 } 
			 else 
			 {
			 	//GOC置为正常
				
				//2.先判断GOC是否检测到bit5，而且在交易状态:					
                if(BeSaleStat() && ((*pComStatus & GOODS_NOTPASS_BIT) == 0)) 
			   	{
					statusDisp = 1;
						
				}		            
	            else
	            { 
	                   statusDisp = 0;
	            }
				
			 }
		}
		/*********GOC关闭的情况下********************/
		else
		{
			//1总接收状态为0，而且在交易状态 
			if( BeSaleStat() && (*pComStatus == 0) )
	        {
				//扣钱  
				statusDisp = 1;
					
			}	
	        else
	        {
	           	   statusDisp = 0;
	          
	        } 
		}
		if(statusDisp == 0)
		{
			 //显示货道故障
	       #if DISP_MODE == LCD_DISPLAY
		   	   if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			   {
					//ClearScreen();//拼图屏幕样式;by gzz 20121106
					Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
					Pitu_LCDPrintf(LINE23,40,Disp_Fault[SYSPara.Language][2],strlen(Disp_Fault[SYSPara.Language][2]),0,0);
					if(SYSPara.Language != 1)
						Pitu_LCDPrintf(LINE31,40,Disp_Fault[1][2],strlen(Disp_Fault[1][2]),0,0);
					OSTimeDly(OS_TICKS_PER_SEC * 2);
			   }
			   else
			   {	
			   		WriteFull_LCD(LINE0,Disp_Fault[SYSPara.Language][2],strlen(Disp_Fault[SYSPara.Language][2]));
			   		WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));	
			   }
		   #endif
		   //设置本次交易为疑问交易
		   if(*pComStatus & DRV_ERR_MASK) 
		   {
			   SetDoubtTrans();
		   }
		}
		else
		{   

		   Trace("\r\n SYSPara.GOCCtr = %x, *pComStatus & MOTO_NOTRETURN_BIT = %x",SYSPara.GOCCtr,(*pComStatus & MOTO_NOTRETURN_BIT));
		   Trace("\r\n (*pComStatus & GOODS_NOTPASS_BIT) == 0 = %x",((*pComStatus & GOODS_NOTPASS_BIT) == 0));
	   
		   g_Amount -= Channels[x][y].GoodsPrice;					  
	  #if DISP_MODE == LCD_DISPLAY		
	  	  if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		  {
			//ClearScreen();//拼图屏幕样式;by gzz 20121106
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][1],strlen(Disp_GoodsOut[1][1]),0,0);
		  }
		  else
		  {
		 	 WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]));
		  }
      #endif 	  

		  //120914 by cq Cashless
		  if( (SYSPara.ReaderEnable)&&(Reader_GetFundsMultiplyUnit()>0) )
		  {
			   cVendSuccessFlag = 1;//121019 by cq Cashless
			  Vmc_PostToReader(VTR_VEND_SUCCESS); 
			  //121013 by cq Cashless
			  state = MVC_RETRANS;//121019 by cq Cashless
			  return state;
		  }  
	    }	
	   /*	
	   //货道为空
	   if(*pComStatus & GOODS_SOLDOUT_BIT) 
	   {
		   state = MVC_WUHUO;
	   } 
	   //商品货道故障
	   else if(*pComStatus > 0) 
	   {
		   //显示货道故障
       #if DISP_MODE == LCD_DISPLAY
	   	   if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		   {
				//ClearScreen();//拼图屏幕样式;by gzz 20121106
				Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
				Pitu_LCDPrintf(LINE23,40,Disp_Fault[SYSPara.Language][2],strlen(Disp_Fault[SYSPara.Language][2]),0,0);
				if(SYSPara.Language != 1)
					Pitu_LCDPrintf(LINE31,40,Disp_Fault[1][2],strlen(Disp_Fault[1][2]),0,0);
				OSTimeDly(OS_TICKS_PER_SEC * 2);
		   }
		   else
		   {	
		   		WriteFull_LCD(LINE0,Disp_Fault[SYSPara.Language][2],strlen(Disp_Fault[SYSPara.Language][2]));
		   		WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));	
		   }
	   #endif


		   //根据故障的发生情况，有些故障还是会扣钱的
	   #if DRV_TYPE == DRV_64
		   //120920 by cq DebugModify
		   
		   if(((SYSPara.GOCCtr == 0) && ((*pComStatus & MOTO_NOTRETURN_BIT) == 0)) 
			  || ((SYSPara.GOCCtr > 0) && ((*pComStatus & GOODS_NOTPASS_BIT) == 0)&& ((*pComStatus & MOTO_NOTRETURN_BIT) == 0)))
		   {
	   #elif DRV_TYPE == DRV_12
		   if(*pComStatus & MOTO_NOTRETURN_BIT) 
		   {
	   #endif

			   Trace("\r\n SYSPara.GOCCtr = %x, *pComStatus & MOTO_NOTRETURN_BIT = %x",SYSPara.GOCCtr,(*pComStatus & MOTO_NOTRETURN_BIT));
			   Trace("\r\n (*pComStatus & GOODS_NOTPASS_BIT) == 0 = %x",((*pComStatus & GOODS_NOTPASS_BIT) == 0));
		   
			   g_Amount -= Channels[x][y].GoodsPrice;
		   #if DISP_MODE == LCD_DISPLAY		
		   		if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
				{
					//ClearScreen();//拼图屏幕样式;by gzz 20121106
					Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
					Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]),0,0);
					if(SYSPara.Language != 1)
						Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][1],strlen(Disp_GoodsOut[1][1]),0,0);
				}
				else
				{
			   		WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]));		
				}
           #endif 	

			   //120914 by cq Cashless
			   if( (SYSPara.ReaderEnable)&&(Reader_GetFundsMultiplyUnit()>0) )
			   {
					cVendSuccessFlag = 1;//121019 by cq Cashless
				   Vmc_PostToReader(VTR_VEND_SUCCESS); 
				   //121013 by cq Cashless
				   state = MVC_RETRANS;//121019 by cq Cashless
				   return state;
			   }
		   }

		   //设置本次交易为疑问交易
		   if(*pComStatus & DRV_ERR_MASK) 
		   {
			   SetDoubtTrans();
		   }
	   } 
	   //出货成功	
	   else 
	   {														
		  //		
		  g_Amount -= Channels[x][y].GoodsPrice;					  
	  #if DISP_MODE == LCD_DISPLAY		
	  	  if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		  {
			//ClearScreen();//拼图屏幕样式;by gzz 20121106
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][1],strlen(Disp_GoodsOut[1][1]),0,0);
		  }
		  else
		  {
		 	 WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]));
		  }
      #endif 	  

		  //120914 by cq Cashless
		  if( (SYSPara.ReaderEnable)&&(Reader_GetFundsMultiplyUnit()>0) )
		  {
			   cVendSuccessFlag = 1;//121019 by cq Cashless
			  Vmc_PostToReader(VTR_VEND_SUCCESS); 
			  //121013 by cq Cashless
			  state = MVC_RETRANS;//121019 by cq Cashless
			  return state;
		  }
	   }		  
	   */	
	   //120622 by cq WaitOutGoods
	   //OSTimeDly(OS_TICKS_PER_SEC * 2);				   
	   OSTimeDly(OS_TICKS_PER_SEC);


	   //出货结束后，根据本次的状态，以及其它一些条件，切换到下一个状态
	   if(*pComStatus & GOODS_SOLDOUT_BIT) 
	   {
		   state = MVC_WUHUO;
	   } 
	   else if((g_Amount == 0) || (g_Amount < SYSPara.MoneyMinLeft) || (SYSPara.TransMultiMode == 0)) 
	   {
		   state = MVC_PAYOUT;
	   }
	   else 
	   {
		   state = MVC_RETRANS;
	   }
	}
	//超时，说明没有出货成功   
	else 
	{
	   SetDoubtTrans(); //超时，说明没有出货成功，设置成疑问交易  
	   state = MVC_PAYOUT;
	}  //出货情况
	

	//如果出货确认板有故障，进行的操作
	if((GetDevStatus() & DEV_ERR_BIT_DRVCOMM) == 0) 
	{	
/**************************************************************/																								
	#if DRV_TYPE == DRV_12
		//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ch + ((uint32_t)CMD_GOODS_CHECK << 24);			
		OSMboxPost(g_CommMsg,&MsgBack.CmdType); 			//查询货道状态
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 2,&err);
	#endif
/*************************************************************/
	}
#endif

	//120914 by cq Cashless
	if( (SYSPara.ReaderEnable)&&(Reader_GetFundsMultiplyUnit()>0) )
	{		
		//121019 by cq Cashless
		//if(state != MVC_SALE)
		if(cVendSuccessFlag == 0)
		{
			Vmc_PostToReader(VTR_VEND_FAILURE);
			//121019 by cq Cashless
			state = MVC_RETRANS;
			return state;
		}
	}

	return state;
}


/*显示是否继续购买*/
void DispRetransText(void)
{ 
	#if DISP_MODE == LCD_DISPLAY
	WriteFull_LCD(LINE0,Disp_Deal[SYSPara.Language][2],strlen(Disp_Deal[SYSPara.Language][2]));
	WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][0],strlen(Disp_Confirm[SYSPara.Language][0]));
	#endif
}

/*显示钱不够*/
void DispLessText(uint32_t	GoodsPrice)
{	   
	char    *pstr;
	char	str2[48];
	
	#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		pstr = SprintDec(GoodsPrice,SYSPara.Decimal);	    	
	    strcpy(str2, Disp_Deal[SYSPara.Language][1]);
		strcat(str2, Disp_ChnPara[SYSPara.Language][1]);
		strcat(str2, pstr);
		//ClearScreen();//拼图屏幕样式;by gzz 20121106
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,str2,strlen(str2),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_Deal[1][1],strlen(Disp_Deal[1][1]),0,0);
		OSTimeDly(OS_TICKS_PER_SEC*2);
	}
	else
	{
		WriteFull_LCD(LINE0,Disp_Deal[SYSPara.Language][1],strlen(Disp_Deal[SYSPara.Language][1]));
		//WriteFull_LCD(LINE1,Disp_Confirm[SYSPara.Language][0],strlen(Disp_Confirm[SYSPara.Language][0]));
	}
	#endif	
}

/*在交易状态下，显示加钱状态的页面*/
/*参数 x:货道层号 y:货道列号*/
/*返回值 交易状态*/
uint8_t DispAddMoneyText(uint8_t x,uint8_t y)
{
    #if DISP_MODE == LCD_DISPLAY
	char	str1[48];
	char    *pstr;
	#endif

	uint8_t state = MVC_ADDMONEY;;

	#if DISP_MODE == LCD_DISPLAY
	pstr = SprintDec(g_Amount,SYSPara.Decimal);
	strcpy(str1, Disp_Deal[SYSPara.Language][0]);
	strcat(str1, pstr);

	WriteFull_LCD(LINE0,str1,strlen(str1));
	WriteFull_LCD(LINE1,Disp_ChInput[SYSPara.Language][0],strlen(Disp_ChInput[SYSPara.Language][0]));
	#endif
	
	if(g_Amount >= Channels[x][y].GoodsPrice) 
	{		
	    //state = CHUHUO;	
		state = MVC_INPUT;									    	
	}
	return state;
}

/*显示正在找零*/
void DispPayoutText(void)
{
#if DISP_MODE == LCD_DISPLAY
	char	str2[48];
	char	*pstr;
#endif
	
	if(g_Amount > 0) 
	{					
    #if DISP_MODE == LCD_DISPLAY
		pstr = SprintDec(g_Amount,SYSPara.Decimal);
		strcpy(str2, Disp_Change[SYSPara.Language][1]);
		strcat(str2, pstr);

		if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		{
			Pitu_LCDClearLine(LINE1,0,2);
			Pitu_LCDPrintf(LINE11,20,Disp_Deal[SYSPara.Language][8],strlen(Disp_Deal[SYSPara.Language][8]),0,0);
			pstr = SprintDec(0,SYSPara.Decimal);
			Pitu_LCDPrintf(LINE1,51,pstr,strlen(pstr),0,2);
			//ClearScreen();
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_Change[SYSPara.Language][1],strlen(Disp_Change[SYSPara.Language][1]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_Change[1][1],strlen(Disp_Change[1][1]),0,0);					
		}
		else
		{
			WriteFull_LCD(LINE0,Disp_Blank,strlen(Disp_Blank));
			WriteFull_LCD(LINE1,str2,strlen(str2)); 
			//添加品途屏幕的兼容;by gzz 20121025
			if(IsLargeLCD())
			{			
				WriteFull_LCD(LINE2,"",0);
				WriteFull_LCD(LINE3,"",0);//120920 by cq DebugModify
			}
		}
		#endif
	}	
}


/*发送接收Coin找零消息，作MDB找零操作*/
void DoCoinOutMsg(void)
{
    uint8_t err;
	uint32_t  *pComStatus;
	
	#if COINDEVICE	  
	if((SYSPara.CoinEnable == MDB_COIN_EN) && (g_Amount > 0)
	   && ((SYSPara1.CoinsStat & (~COIN_FULL_BIT)) == COIN_STAT_OK))
	{
		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24) + g_Amount;
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令						
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*(g_Amount/g_CoinMinValue/2+35),&err);//返回找零结果
		//等待找零结束//MDB硬币器找零
		if(err == OS_NO_ERR) 
		{	 								     		 						    	 							
		
		   if(SYSPara1.CoinsStat != COIN_STAT_OK) //硬币器故障，设为疑问交易
		   {
			  SetDoubtTrans();
		   }
		   if(g_Amount >= *pComStatus) 
		   {
		      g_Amount -= *pComStatus;						  	
		   } 
		   else if(*pComStatus > g_Amount)
		   {
		      g_Amount = 0;
		   }
		   ChgAmount += *pComStatus;		   				       						
		} 
		//超时，说明没有出币成功   
		else 
		{
		   SetDoubtTrans();//超时，说明没有出币成功，设为疑问交易  
		}									//MDB硬币器找零						
	}
	#endif 
}

/*发送压钞命令消息*/
void DoNoteStackMsg(void)
{
    uint8_t err;
	
	OSMboxAccept(g_CommBackMsg);
	MsgBack.CmdType =  ((uint32_t)CMD_NOTE_STACK << 24);
	OSMboxPost(g_CommMsg,&MsgBack.CmdType);//发送压钞命令
	OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*15,&err);//等待压钞结果
}

/*发送暂存纸币退钞命令消息*/
void DoNoteReturnMsg(void)  
{
    uint8_t err,moneych;
	uint32_t  *pComStatus;
	
	OSMboxAccept(g_CommBackMsg);
	MsgBack.CmdType =  ((uint32_t)CMD_NOTE_RETURN << 24);//发送退钞命令
	OSMboxPost(g_CommMsg,&MsgBack.CmdType);
	pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*15,&err);//等待退币结果
	if(err == OS_NO_ERR) 
	{
	    moneych = *pComStatus;	    
		if(g_Amount >= SYSPara.BillValue[moneych])	
		{
		    ChgAmount += SYSPara.BillValue[moneych];
			g_Amount -= SYSPara.BillValue[moneych];
		} 
		else 
		{
		    ChgAmount += g_Amount;
			g_Amount = 0;
		}
	}
}

/*发送找零命令消息*/
void DoPayOutMsg(void)  
{
    uint8_t err;
	uint32_t  *pComStatus;
	
	if(g_Amount > 0) 
	{
		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24) + g_Amount;
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令						
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*(g_Amount/g_CoinMinValue/2+60),&err);
		//等待找零结束
		if(err == OS_NO_ERR) 
		{	 			//MDB硬币器找零					     		 						    	 							
		   if(g_Amount >= *pComStatus) 
		   {
		      g_Amount -= *pComStatus;						  	
		   } 
		   else if(*pComStatus > g_Amount) 
		   {
		      g_Amount = 0;
		   }
		   ChgAmount += *pComStatus;		   				       						
		}
	}
}

/*发送接收Hopper找零命令消息,作hopper找零操作*/
/*返回值 交易状态*/
uint8_t DoHopperOutMsg(void)
{
    uint8_t   state,i,j,pNum,err;
	uint32_t  ChangeBase;
	uint32_t  *pComStatus;
	
	#if HOPPERDEVICE  //找零
	state = 0;						   				
	if(SYSPara.ChangerEnable == HOPPER_CHANGER_EN) 
	{
		for(i = HOPPERCHMAX; (g_Amount>0)&&(i>0); i--) 
		{           
			//ChangeBase = SYSPara.ChangeValue[i-1];
			//j = i-1;
			ChangeBase = HopperPara[i-1].Value;
			j = HopperPara[i-1].Channel;
			
			if((ChangeBase > 0) && ((SYSPara1.HopperStat[j] & HOPPER_ERR_MASK) == 0)) 
			{
				pNum = g_Amount / ChangeBase;
				if(pNum > 0) 
				{
					state |= 1 << j;
					//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，接下来4位为hopper号，其他20位为出币值
					OSMboxAccept(g_CommBackMsg);
					MsgBack.CmdType =  (pNum * ChangeBase) + ((uint32_t)CMD_HOPPER_CHANGE << 24) + ((uint32_t)j << 20);
			        OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令				
			
			        pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*(pNum/4+35),&err);
			
					if(err == OS_NO_ERR) 
					{	   			//Hopper找零
					   //Hopper通讯故障
					   if(SYSPara1.HopperStat[j] & HOPPER_COMMERR_BIT) 
					   {
				          //设备故障
						  SetDoubtTrans();
						 //  break;
			           }
					   if(g_Amount >= *pComStatus) 
					   {
				          g_Amount -= *pComStatus;
						  if((SYSPara1.HopperStat[j] & (~HOPPER_INSUFFICIENT_BIT)) == 0) 
						  {
						      state &= ~(1 << j);
						  }
					   } 
					   else if(*pComStatus > g_Amount)
					   {
					      g_Amount = 0;
					   }
					   ChgAmount += *pComStatus;
				    }
					//超时，找零失败，需要设置成疑问交易
					else 
				    {
					   SetDoubtTrans();
					  // break;
					} //Hopper找零
			    }		  
			}
		}
	}				 
	#endif
	return state;
}

/*显示欠费，或找零失败*/
void DispIOUText(void)
{
    #if DISP_MODE == LCD_DISPLAY
  	char	str2[48];
	char    *pstr;
	#endif
	
	#if DISP_MODE == LCD_DISPLAY
	pstr = SprintDec(g_Amount,SYSPara.Decimal);						
	strcpy(str2, Disp_Change[SYSPara.Language][3]);
	strcat(str2, pstr);

	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		if(CashState.ChangerState & CHANGER_LOW) 
		{		    
			Pitu_LCDPrintf(LINE23,40,Disp_Change[SYSPara.Language][2],strlen(Disp_Change[SYSPara.Language][2]),0,0);
		}
		else
		{		
			strcat(str2, Disp_Change[SYSPara.Language][5]);
			Pitu_LCDPrintf(LINE23,40,str2,strlen(str2),0,0);
		}			

		if(SYSPara.Language != 1)		
			Pitu_LCDPrintf(LINE31,40,Disp_Change[1][5],strlen(Disp_Change[1][5]),0,0);	
		OSTimeDly(OS_TICKS_PER_SEC);
	}
	else
	{						
		if(CashState.ChangerState & CHANGER_LOW) 
		{
		    WriteFull_LCD(LINE0,Disp_Change[SYSPara.Language][2],strlen(Disp_Change[SYSPara.Language][2]));
		}
		else
		{
		    WriteFull_LCD(LINE0,Disp_Change[SYSPara.Language][5],strlen(Disp_Change[SYSPara.Language][5]));
		}
		WriteFull_LCD(LINE1,str2,strlen(str2));
	}
	#endif
}

/*显示请取零钱*/
void DispChangeText(void)
{
	#if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		//ClearScreen();
		Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
		Pitu_LCDPrintf(LINE23,40,Disp_Change[SYSPara.Language][0],strlen(Disp_Change[SYSPara.Language][0]),0,0);
		if(SYSPara.Language != 1)
			Pitu_LCDPrintf(LINE31,40,Disp_Change[1][0],strlen(Disp_Change[1][0]),0,0);					
	}
	else
	{
		WriteFull_LCD(LINE0,Disp_Change[SYSPara.Language][0],strlen(Disp_Change[SYSPara.Language][0]));
		OSTimeDly(OS_TICKS_PER_SEC);
	}
	#endif
}

/*显示交易结束*/
void DispEndText(void)
{
    #if DISP_MODE == LCD_DISPLAY
	if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
	{
		ClearScreen();	
		/*
		//Trace("\r\n now end=%d",SYSPara.Language);
		if(SYSPara.Language==0)
		{
			//Trace("\r\n1");
			Pitu_LCDPutPicBmp(1);
		}
		else
		*/
		{
			//Trace("\r\2");
			Pitu_LCDPutLOGOBmp(LINE0,75,SYSPara.LogoSel);
			Pitu_LCDPrintf(LINE11,32,Disp_End[1][0],strlen(Disp_End[1][0]),0,3);
			//Pitu_LCDPrintf(LINE21,16,Disp_End[SYSPara.Language][1],strlen(Disp_End[SYSPara.Language][1]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE23,32,Disp_End[SYSPara.Language][2],strlen(Disp_End[SYSPara.Language][2]),0,0);			
		}
		OSTimeDly(OS_TICKS_PER_SEC * 2);
		
	}
	else
	{
		WriteFull_LCD(LINE0,Disp_End[SYSPara.Language][0],strlen(Disp_End[SYSPara.Language][0]));
		WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));
	}
	#endif
}

/*定时发送命令消息，来轮询检测设备*/
void DoDingshiMsg(void)
{
    uint8_t err;

	//当硬币器设为mdb时，只有缺币才自检;by gzz 20121031
	//121106 by cq BillSingleWork
	if(isBillSingleWork())
	{
		//do nothing
	}
	else if(SYSPara.ChangerEnable==MDB_CHANGER_EN)
	{
	    /*零币不足时，定时检查找零器*/
		if(CashState.ChangerState & CHANGER_LOW) 
		{			
			OSMboxAccept(g_CommBackMsg);
			MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24);
			OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令						
			
			OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*10,&err);
		} 
	}
	else if(SYSPara.ChangerEnable==HOPPER_CHANGER_EN)
	{
	    //需要一直自检1元的币斗有没空，没空时就用1元的找
		//if(CashState.ChangerState & CHANGER_LOW) 
		if(g_Amount == 0)//只有在空闲状态才自检
		{			
			OSMboxAccept(g_CommBackMsg);
			MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24);
			OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令						
			
			OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*10,&err);
		} 
	}

	/*货道板故障时，定时检查货道板*/
	#if DRV_TYPE == DRV_64
	if(GetDevStatus() & (DEV_ERR_BIT_GOODSCHK|DEV_ERR_BIT_DRVCOMM)) 
	{
	#elif DRV_TYPE == DRV_12
	if(GetDevStatus() & DEV_ERR_BIT_DRVCOMM) 
	{
	#endif
	
	#if DRVDEVICE							  
	//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
	OSMboxAccept(g_CommBackMsg);
	MsgBack.CmdType = 11 + ((uint32_t)CMD_GOODS_CHECK << 24); 
	OSMboxPost(g_CommMsg,&MsgBack.CmdType);
	
	OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 5,&err); 
	#endif   
	}
}



/*显示按PC要求找零*/
void Disp_VP_CMD_PayoutText(uint32_t CurrentChg)  
{
    #if DISP_MODE == LCD_DISPLAY
  	char	str2[48];
	char    *pstr;
	#endif
	   
	if(CurrentChg > 0) 
	{					
	    #if DISP_MODE == LCD_DISPLAY
		pstr = SprintDec(CurrentChg,SYSPara.Decimal);
		strcpy(str2, Disp_Change[SYSPara.Language][1]);
	    strcat(str2, pstr);
	
	    WriteFull_LCD(LINE0,Disp_Blank,strlen(Disp_Blank));
		WriteFull_LCD(LINE1,str2,strlen(str2));					    
		#endif
    }	
}	



/*按PC要求发送找零命令消息*/
void Do_VP_CMD_PayoutMsg(uint32_t CurrentChg)  
{
    uint8_t err;
	uint32_t  *pComStatus;
	
	if(CurrentChg > 0) 
	{		
		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ((uint32_t)CMD_COIN_CHANGE << 24) + CurrentChg;
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);	   //找零指令						
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC*(CurrentChg/g_CoinMinValue/2+60),&err);
		//等待找零结束
		if(err == OS_NO_ERR) 
		{	 			//MDB硬币器找零					     		 						    	 							
		   if(CurrentChg >= *pComStatus) 
		   {
		      CurrentChg -= *pComStatus;						  	
		   } 
		   else if(*pComStatus > sysVPMission.changeMoney) 
		   {
		      CurrentChg = 0;
		   }
		   //sysVPMission.changeMoney += *pComStatus;		   				       						
		}
		if(SYSPara.Decimal==1)
	    {			
			sysVPMission.changeMoney = CurrentChg * 10;
	    }
	    else if(SYSPara.Decimal==2)
	    {			
			sysVPMission.changeMoney = CurrentChg;
	    }	
	}	
}



/*显示按PC要求找零的欠费，或找零失败*/
void Disp_VP_CMD_Payout_IOUText(void)
{
    #if DISP_MODE == LCD_DISPLAY
  	char	str2[48];
	char    *pstr;
	#endif
	
	#if DISP_MODE == LCD_DISPLAY
	pstr = SprintDec(sysVPMission.changeMoney,SYSPara.Decimal);						
	strcpy(str2, Disp_Change[SYSPara.Language][3]);
	strcat(str2, pstr);
	   						
	if(CashState.ChangerState & CHANGER_LOW) 
	{
	    WriteFull_LCD(LINE0,Disp_Change[SYSPara.Language][2],strlen(Disp_Change[SYSPara.Language][2]));
	}
	else
	{
	    WriteFull_LCD(LINE0,Disp_Change[SYSPara.Language][5],strlen(Disp_Change[SYSPara.Language][5]));
	}
	WriteFull_LCD(LINE1,str2,strlen(str2));
	#endif
}

/*发送接收出货消息，作出货操作*/
/*参数 ch:客户输入的货道编号*/
/*参数 x:货道层号 y:货道列号*/
/*返回值 交易状态*/
uint8_t Do_VP_CMD_ChuhuoMsg(uint8_t ch,uint8_t x,uint8_t y)
{
    uint8_t state,err;
	uint32_t  *pComStatus;

	
	Trace(" \r\nDo_VP_CMD_ChuhuoMsg");
	
	#if DRVDEVICE
	sysVPMission.vendCmd = 1;
	//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
	OSMboxAccept(g_CommBackMsg);

	Trace("\r\nch= %u",ch);
	
	MsgBack.CmdType = ch + ((uint32_t)CMD_GOODS_OUT << 24); 
    OSMboxPost(g_CommMsg,&MsgBack.CmdType);//通知系统有出货命令发送

    pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 20,&err);//通知系统出货完成后，它的返回命令(返回所有的出货结果，不一定是成功)
    sysVPMission.vendCmd = 0;
    //等待出货结束
    if(err == OS_NO_ERR) 
	{	 			//出货情况	
	   //货道为空
	   if(*pComStatus & GOODS_SOLDOUT_BIT) 
	   {
	       state = MVC_WUHUO;
	   } 
	   //商品货道故障
	   else if(*pComStatus > 0) 
	   {
	       //显示货道故障
	       #if DISP_MODE == LCD_DISPLAY
		   WriteFull_LCD(LINE0,Disp_Fault[SYSPara.Language][2],strlen(Disp_Fault[SYSPara.Language][2]));
		   WriteFull_LCD(LINE1,Disp_Blank,strlen(Disp_Blank));					   
		   #endif

           //根据故障的发生情况，有些故障还是会扣钱的
		   #if DRV_TYPE == DRV_64
		   if(((SYSPara.GOCCtr == 0) && (*pComStatus & MOTO_NOTRETURN_BIT)) 
		      || ((SYSPara.GOCCtr > 0) && ((*pComStatus & GOODS_NOTPASS_BIT) == 0)))
		   {
		   #elif DRV_TYPE == DRV_12
		   if(*pComStatus & MOTO_NOTRETURN_BIT) 
		   {
		   #endif
			   if(SYSPara.Decimal==1)
			   {
					g_Amount -= sysVPMission.vendCost/10;
			   }
			   else if(SYSPara.Decimal==2)
			   {
					g_Amount -= sysVPMission.vendCost;
			   }			   	   	
		       #if DISP_MODE == LCD_DISPLAY	
			   if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
			  {
				//ClearScreen();//拼图屏幕样式;by gzz 20121106
				Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
				Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]),0,0);
				if(SYSPara.Language != 1)
					Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][1],strlen(Disp_GoodsOut[1][1]),0,0);
			  }
			  else
			  {
               	WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]));	
			  }
               #endif 	  
		   }
		   VPMsgPackSend( VP_NAK_RPT, 0, 0  );
	   } 
	   //出货成功   
	   else 
	   { 			          			 					 	
 		  //	
 		  if(SYSPara.Decimal==1)
		  {
				g_Amount -= sysVPMission.vendCost/10;
		  }
		  else if(SYSPara.Decimal==2)
		  {
				g_Amount -= sysVPMission.vendCost;
		  }
          #if DISP_MODE == LCD_DISPLAY		
		  if(SYSPara.LCDSel==2)//拼图屏幕样式;by gzz 20121106
		  {
			//ClearScreen();//拼图屏幕样式;by gzz 20121106
			Pitu_LCDPrintf(LINE23,0,"→",strlen("→"),0,3);
			Pitu_LCDPrintf(LINE23,40,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]),0,0);
			if(SYSPara.Language != 1)
				Pitu_LCDPrintf(LINE31,40,Disp_GoodsOut[1][1],strlen(Disp_GoodsOut[1][1]),0,0);
		  }
		  else
		  {
	      	WriteFull_LCD(LINE1,Disp_GoodsOut[SYSPara.Language][1],strlen(Disp_GoodsOut[SYSPara.Language][1]));
		  }
	      #endif 	
		  VPMsgPackSend( VP_ACK_RPT, 0, 0  );
	   }	       

	   OSTimeDly(OS_TICKS_PER_SEC * 2);
	   //出货结束后，根据本次的状态，以及其它一些条件，切换到下一个状态
	   if(*pComStatus & GOODS_SOLDOUT_BIT) 
	   {
	       state = MVC_WUHUO;
	   } 
	   else if((g_Amount == 0) || (g_Amount < SYSPara.MoneyMinLeft) || (SYSPara.TransMultiMode == 0)) 
	   {
	       state = MVC_PAYOUT;
	   }
	   else 
	   {
	       state = MVC_RETRANS;
	   }

    }
	//超时，说明没有出货成功   
	else 
    {
    	state = MVC_PAYOUT;
		VPMsgPackSend( VP_NAK_RPT, 0, 0  );
	}  //出货情况
	

    //如果出货确认板有故障，进行的操作
	if((GetDevStatus() & DEV_ERR_BIT_DRVCOMM) == 0) 
	{	
/**************************************************************/														 							 			
		#if DRV_TYPE == DRV_12
		//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
		OSMboxAccept(g_CommBackMsg);
		MsgBack.CmdType = ch + ((uint32_t)CMD_GOODS_CHECK << 24);			
		OSMboxPost(g_CommMsg,&MsgBack.CmdType);				//查询货道状态
		
		pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 2,&err);
		#endif
/*************************************************************/
    }
	#endif

	return state;
}
	

/*处理按PC要求发送的指令*/
uint8_t VP_CMD_Opt(void)
{
	uint8_t 	err = 0;
	uint8_t 	*PCCmd;
	uint32_t 	CurrentChg;	  //当前找零值
	uint32_t 	CurrentCost;	  //当前扣币值
	uint8_t  	ChannelX,ChannelY;
	uint8_t     state;

	if( SYSPara.PCEnable == 2 )//120620 by cq PCcommunication
	{
		state = 0xEE;
		PCCmd = OSMboxPend(g_PCCMDMsg,10,&err);		
		if (err ==OS_NO_ERR)
		{
			//Trace("\r\nReady PCCmd=%d",*PCCmd);
			switch(*PCCmd)
			{
				  //复位指令	
				  case VP_RESET_IND:
				  {
				  		//Trace("\r\nReady VP_RESET_IND");
						//sysVPMission.resetCmd = 0;
						DispInitText();	//显示"设备初始化中，请稍候"	
						if(IsMoneyInEN()) //投币器有使能，就禁能
						{
							DisableMoneyIn();
							SetTouBiStat(CMD_ALL_DIS);		//禁能投币
						}
						
					    OSMboxAccept(g_PCCommMsg);
					    MsgBack.CmdPCCMDMSG = ((uint32_t)VP_RESET_IND << 24);
					    OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送						   
							
						OSSemPend(g_PCSem,0,&err);	//一直等待，直到设备初始化完成

						EnableMoneyIn(); 
						SetTouBiStat(CMD_ALL_EN);		//使能投币
						
						break;
				  }
				  //找零指令
				  case VP_PAYOUT_IND:
				  {
				  		Trace("\r\nVP_CMD_Opt VP_PAYOUT_IND");
				  	
						//sysVPMission.changeCmd = 0;
						//找零 		
						if(SYSPara.Decimal==1)
					    {
							CurrentChg = sysVPMission.changeMoney/10;
					    }
					    else if(SYSPara.Decimal==2)
					    {
							CurrentChg = sysVPMission.changeMoney;
					    }	

						RstInputMode();
						Disp_VP_CMD_PayoutText(CurrentChg);//显示正在找零
						   
						if(IsMoneyInEN()) //投币器有使能，就禁能
						{
							DisableMoneyIn();
							SetTouBiStat(CMD_ALL_DIS);		//禁能投币
						}

						//120827 by cq PCcommunication
						//Do_VP_CMD_PayoutMsg(CurrentChg);		 //发送找零命令
						g_Amount = CurrentChg;
						DoPayOutMsg();
						sysVPMission.changeMoney = g_Amount;
						 
						//RTC_WriteGPREG(LPC_RTC,YREGCH,g_Amount);
					
						if(sysVPMission.changeMoney > 0) //找零不足 
						{													
							if(SYSPara.Decimal==1)
							{
							  	TranTotal.Iou = sysVPMission.changeMoney / 10;
							}
							else if(SYSPara.Decimal==2)
							{
							  	TranTotal.Iou = sysVPMission.changeMoney; 
							}	
						    OSMboxAccept(g_PCCommMsg);
						    MsgBack.CmdPCCMDMSG = ((uint32_t)VP_DEBT_RPT << 24) + sysVPMission.changeMoney;
						    OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送							   
															
							Disp_VP_CMD_Payout_IOUText();	//显示欠费																																																	
						} 
						else 
						{
							if(CurrentChg > 0) //有找零值大于0
							{				
								DispChangeText();//显示取零钱
							}					
						}
						/*
						if(HopStat > 0) {					//Hopper找零出错
							SetDoubtTrans();
						}
						*/						

						if(g_Amount == 0)  
					    {
					       state = MVC_PAYOUT;
					    }
					    else 
					    {
					       state = MVC_SALE;
					    }
						OSTimeDly(OS_TICKS_PER_SEC);

						break;
									
				  }
				  //扣款指令
				  case VP_COST_IND:
				  {
				  		if(IsMoneyInEN()) //如果投币器是使能的，将它禁能
						{
							DisableMoneyIn();
							SetTouBiStat(CMD_ALL_DIS);      //禁能投币
						}
						
						if(IsEscrowHold()) //判断是否有纸币暂存
						{			   
						    DoNoteStackMsg();//压钞
						}

						if(SYSPara.Decimal==1)
						{
						  	CurrentCost = sysVPMission.costMoney / 10;
						}
						else if(SYSPara.Decimal==2)
						{
						  	CurrentCost = sysVPMission.costMoney; 
						}	

						g_Amount -= CurrentCost;	
						
					    OSMboxAccept(g_PCCommMsg);
					    MsgBack.CmdPCCMDMSG = ((uint32_t)VP_COST_RPT << 24) + sysVPMission.costMoney;
					    OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送						   
							
						
						sysVPMission.costMoney = 0;
						//Trace("\r\n COST_IND g_Amount=%d",g_Amount);
						//扣款结束后，根据本次的状态，以及其它一些条件，切换到下一个状态
					    if((g_Amount == 0) || (g_Amount < SYSPara.MoneyMinLeft) || (SYSPara.TransMultiMode == 0)) 
					    {
					       //Trace("\r\n +++costind=payout");	
					       state = MVC_PAYOUT;
					    }
					    else 
					    {
					       state = MVC_SALE;
					    }
												
				  		break;
				  }	
				  //出货指令
				  case VP_VENDOUT_IND:
				  {

						Trace("\r\n2.VP_CMD_Opt VP_VENDOUT_IND,%d",sysVPMission.vendColumn2);
						//Trace("\r\n 6t=%ld",sysVPMission.pcTeTimer);
				  		RstInputMode();
						DispChuhuoText();//显示正在出货

						//Trace(" VP_VENDOUT_IND 3,%d",sysVPMission.vendColumn);
						/*
					    if(IsMoneyInEN()) //如果投币器是使能的，将它禁能
						{
							DisableMoneyIn();
							SetTouBiStat(CMD_ALL_DIS);      //禁能投币
						}

						Trace(" VP_VENDOUT_IND 4");

						if(IsEscrowHold()) //判断是否有纸币暂存
						{			   
						    DoNoteStackMsg();//压钞
						}				
						*/
						state = Do_VP_CMD_ChuhuoMsg(sysVPMission.vendColumn2 ,ChannelX,ChannelY);//发送出货命令
												
				  		break;
				  }
				  default:
				  		break;
			}			  
		 } 
	}
	return state;

}



