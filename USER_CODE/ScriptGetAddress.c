#include "ScriptGetAddress.h"
#include "uart.h"
#include "global.h"
#include "pcf8563t.h"
#include "SaleTaskFun.h"
#include "UCSGBK.h"
#include "lcd.h"

extern uint32_t DispTimer;
extern uint8_t KeyNumC;

/*****************************************************************************
** Function name:	Draw_RECT	
**
** Descriptions:	图形显示函数 	
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void nDraw_RECT(uint8_t Yaddr,uint8_t Xaddr,const uint8_t * dp)  
{ 
    uint8_t j; 
    uint8_t k=0; 
        
    lcd_send(0x34,LCD_CMD);			 //使用扩展指令集
	for(j = 0; j < 8; j++) { 
        lcd_send(Yaddr++,LCD_CMD);   //Y地址 
        lcd_send(Xaddr,LCD_CMD);     //X地址 
        lcd_send(dp[k++],LCD_DAT);
		lcd_send(dp[k++],LCD_DAT);
    }
	lcd_send(0x36,LCD_CMD);			 //打开绘图显示 
	lcd_send(0x30,LCD_CMD);          //回到基本指令集模式  
} 

/*****************************************************************************
** Function name:	LCD_WhiteBlock	
**
** Descriptions:	LCD整屏显示白块函数 	
**					
**
** parameters:		flag:1全换成白块，0全清掉
** Returned value:	无
** 
*****************************************************************************/
void nLCD_WhiteBlock(uint8_t *nPtr)
{
    uint8_t command_data,i,j,data;

	/*if(flag == 1)
	{
		data = 0xff;
	}
	else
	{
		data = 0x00;
	}*/
	uint8_t Yaddr,Xaddr,k;
	Yaddr = 5;
	Xaddr = 5;	
	k = 0;
   
	command_data=0x36; 
	lcd_send(command_data,LCD_CMD);
	command_data=0x36; 
	lcd_send(command_data,LCD_CMD);
	command_data=0x01; 
	lcd_send(command_data,LCD_CMD);
	//lcd_send(0x80 + i,LCD_CMD);
	lcd_send(0x80,LCD_CMD);
	//for(j = 0;j < 4;j++)
	{
		lcd_send(5,LCD_CMD);   //Y地址 
        lcd_send(5,LCD_CMD);     //X地址 
        lcd_send(nPtr[0],LCD_DAT);
		lcd_send(nPtr[1],LCD_DAT);
		lcd_send(6,LCD_CMD);   //Y地址 
        lcd_send(5,LCD_CMD);     //X地址 
        lcd_send(nPtr[2],LCD_DAT);
		lcd_send(nPtr[3],LCD_DAT);
		lcd_send(7,LCD_CMD);   //Y地址 
        lcd_send(5,LCD_CMD);     //X地址 
        lcd_send(nPtr[4],LCD_DAT);
		lcd_send(nPtr[5],LCD_DAT);
		lcd_send(8,LCD_CMD);   //Y地址 
        lcd_send(5,LCD_CMD);     //X地址 
        lcd_send(nPtr[7],LCD_DAT);
		lcd_send(nPtr[8],LCD_DAT);
		//lcd_send(*nPtr++,LCD_DAT);
	}
	/*for(i = 0;i < 32;i++)
	{
		lcd_send(0x80 + i,LCD_CMD);
		lcd_send(0x80,LCD_CMD);
		for(j = 0;j < 16;j++)
		{
			lcd_send(*nPtr++,LCD_DAT);
		}
	}
	for(i = 0;i < 32;i++)
	{
		lcd_send(0x80 + i,LCD_CMD);
		lcd_send(0x88,LCD_CMD);
		for(j = 0;j < 16;j++)
		{
			lcd_send(*nPtr++,LCD_DAT);
		}
	}
	command_data=0x20; 
  	lcd_send(command_data,LCD_CMD);

	*/	
    /*if(SYSPara.LCDSel==1)
    {
   		//Trace("\r\n start block");
   		command_data=0x36; 
		lcd_send_up(command_data,LCD_CMD);
		command_data=0x36; 
		lcd_send_up(command_data,LCD_CMD);
		command_data=0x01; 
		lcd_send_up(command_data,LCD_CMD);
		for(i = 0;i < 32;i++)
		{
			lcd_send_up(0x80 + i,LCD_CMD);
			lcd_send_up(0x80,LCD_CMD);
			for(j = 0;j < 16;j++)
			{
				lcd_send_up(*nPtr++,LCD_DAT);
			}
		}
		for(i = 0;i < 32;i++)
		{
			lcd_send_up(0x80 + i,LCD_CMD);
			lcd_send_up(0x88,LCD_CMD);
			for(j = 0;j < 16;j++)
			{
				lcd_send_up(*nPtr++,LCD_DAT);
			}
		}
		command_data=0x20; 
	  	lcd_send(command_data,LCD_CMD);
		//Trace("\r\n block over"); 
		
    } 
	*/   
}

void ChangeCharBit(uint8_t *ptr,uint8_t *qtr)
{
	char i,j,m,n,k;
/*
#ifdef DIANZHENG12X12
	m = 0;
	n = 0;
	k = 0;
	for(j=0;j<8;j++)
	{
		k = 0;
		while(k<2)
		{
			for(i=0;i<8;i++)
			{
				if((i+n+2)<14)
					qtr[m] |= ((ptr[i+n+2]&(0x01<<j))>>j)<<(7-i);
				else
					qtr[m] |= ((0x00&(0x01<<j))>>j)<<(7-i);
			}
			if(n==8)
				n = 0;
			else
				n = 8;
			m++;
			k++;
		}
	}
	//m = 0;
	n = 0;
	//k = 0;
	for(j=0;j<8;j++)
	{
		k = 0;
		while(k<2)
		{
			for(i=0;i<8;i++)
			{
				if((i+n+14)<26)
					qtr[m] |= ((ptr[i+14+n]&(0x01<<j))>>j)<<(7-i);
				else
					qtr[m] |= ((0x00&(0x01<<j))>>j)<<(7-i);
			}
			if(n==8)
				n = 0;
			else
				n = 8;
			k++;
			m++;
		}
	}
	return;
#endif
*/
#ifdef	ZILEIXING_8X16 
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
		{	
			*(qtr+j) |= (((*(ptr+i))&(0x01<<j))>>j)<<(7-i);
		}
	}
	m=0;
	for(j=8;j<16;j++)
	{
		for(i=0;i<8;i++)
		{
			*(qtr+j) |= (((*(ptr+i+8))&(0x01<<m))>>m)<<(7-i);
		}
		m++;
	}
	return;
#else
	m = 0;
	n = 0;
	k = 0;
	for(j=0;j<8;j++)
	{
		k = 0;
		while(k<2)
		{
			for(i=0;i<8;i++)
			{
				qtr[m] |= ((ptr[i+n+2]&(0x01<<j))>>j)<<(7-i);
			}
			if(n==8)
				n = 0;
			else
				n = 8;
			m++;
			k++;
		}
	}
	//m = 0;
	n = 0;
	//k = 0;
	for(j=0;j<8;j++)
	{
		k = 0;
		while(k<2)
		{
			for(i=0;i<8;i++)
			{
				qtr[m] |= ((ptr[i+18+n]&(0x01<<j))>>j)<<(7-i);
			}
			if(n==8)
				n = 0;
			else
				n = 8;
			k++;
			m++;
		}
	}
	return;
#endif

}

//获得字符的Unicode码
//***************************************************************************************
//函 数 名：GetScriptUnicode
//函数功能：将字符串通过绘图的方式将其显示在LCD屏上
//参    数：DispRow:1：第一行；2：第二行
//			DispLCDFlag:1：上半屏，2：下半屏
//			str:需要显示字符串
//			strlen：需要显示字符串的长度
//			ChangeWhite:1:反白，0：不反白
//返 回 值：0
//***************************************************************************************
uint8_t GetScriptUnicode(uint8_t DispRow,uint8_t DispLCDFlag,const char *str,uint8_t strLen,uint8_t ChangeWhite)
{
	uint8_t nUniCode[48]={0},i,j;
	uint8_t mUniCode[24]={0};
	uint32_t UnicodeLen = 0,UnicodeLen1=0;
	uint32_t ScriptAdd[48]={0};
	uint32_t ScriptAdd1[10]={0};
	uint8_t OneScriptUnicd[20]={0};
	uint8_t nStr[24]={0};

#ifdef ZILEIXING_8X16
	uint8_t ptr[16]={0},qtr[16]={0};
#else
	uint8_t ptr[34]={0},qtr[34]={0};
#endif
	uint8_t keyStr[4]={0};
	//求得unicode码
	UnicodeLen = gsmEncodeUcs2(str,nUniCode,strlen(str));
	OS_ENTER_CRITICAL(); 	 //   ucosii add//change by gzz 20121013
	//通过unicode码。根据公式求出字的点阵地址
	Script_GetAddress(nUniCode,ScriptAdd,UnicodeLen/2);
	OS_EXIT_CRITICAL();	//   ucosii add
	//将点阵地址传给芯片，求出点阵排列，并绘字
	DispStrZiFu(DispRow,DispLCDFlag,ScriptAdd,UnicodeLen/2,ChangeWhite);
	return 0;
}

void DispStrZiFu(uint8_t DispRow,uint8_t DispLCDFlag,uint32_t *ZiAddress,uint8_t DispCount,uint8_t ChangeWhite)
{
	uint8_t i,j,h;
#ifdef ZILEIXING_8X16
	uint8_t ptr[16]={0},qtr[16]={0};
	uint8_t ptr1[16]={0},qtr1[16]={0};
#else
	uint8_t ptr[34]={0},qtr[34]={0};
#endif
	uint8_t str[24][17]={0};
	uint8_t str1[24][17]={0}; 

	//求出所有字的点阵排列，并保存到二维数组中
	for(i=0;i<DispCount;i++)
	{
	 	//传入地址求得点阵排列
#ifdef ZILEIXING_8X16
		GT20L24F6Y_ReadZiku(16,str[i],ZiAddress[i]);
#else
		GT20L24F6Y_ReadZiku(34,ptr,ZiAddress[i]);
#endif
		//矩阵倒转
		ChangeCharBit(str[i],str1[i]);
	}
	if(DispLCDFlag == 2)
	{
		//显示操作
		lcd_send(0x34,LCD_CMD);			 			//使用扩展指令集
		lcd_send(0x08,LCD_CMD);
	}
	else
	if(DispLCDFlag == 1)
	{
		//显示操作
		lcd_send_up(0x34,LCD_CMD);			 			//使用扩展指令集
		lcd_send_up(0x08,LCD_CMD);	
	}
	//	
	for(j=0;j<17;j++)
	{
		if(DispRow == 1)
		{
			if(DispLCDFlag == 2)
			{
				lcd_send(0x80+j,LCD_CMD);
				lcd_send(0x80,LCD_CMD);   	//X地址 
			}
			else
			if(DispLCDFlag == 1)
			{
				lcd_send_up(0x80+j,LCD_CMD);
				lcd_send_up(0x80,LCD_CMD);   	//X地址 
			}
			 
		}  			//Y地址 
		else
		if(DispRow == 2)
		{
			//Y地址
			if(DispLCDFlag == 2)
			{
				lcd_send(0x80+j+15,LCD_CMD);
				lcd_send(0x80,LCD_CMD);   	//X地址 
			}
			else
			if(DispLCDFlag == 1)
			{
				lcd_send_up(0x80+j+15,LCD_CMD);
				lcd_send_up(0x80,LCD_CMD);   	//X地址 
			}
		}
		/*if(ChangeWhite==1)
		{
			if(strlen(str1)<24)
			{
				for(sl=0;sl<(24-strlen(str1));sl++)
				{
					str1[strlen(str1)] = 0xff;
				}
			}
			//DispCount = strlen(str1);
		} */
		//lcd_send(0x80,LCD_CMD);   	//X地址 
		for(i=0;i<24;i++)
		{
			//将同一行显示的数据的首位同事发送出去
			//lcd_send(str1[i][j],LCD_DAT);
			//Y地址
			if(DispLCDFlag == 2)
			{
				if(ChangeWhite==1)
					lcd_send(0xff&(~str1[i][j]),LCD_DAT);
				else
					lcd_send(str1[i][j],LCD_DAT);
			}
			else
			if(DispLCDFlag == 1)
			{
				if(ChangeWhite==1)
				{
					lcd_send_up(0xff&(~str1[i][j]),LCD_DAT);
				}
				else
					lcd_send_up(str1[i][j],LCD_DAT);
			}
		} 
	}
	if(DispLCDFlag == 2)
	{
		lcd_send(0x0c,LCD_CMD);
		lcd_send(0x36,LCD_CMD);			 //使用扩展指令集
		lcd_send(0x30,LCD_CMD);          //回到基本指令集模式  	 
	}
	else
	if(DispLCDFlag == 1)
	{
		lcd_send_up(0x0c,LCD_CMD);
		lcd_send_up(0x36,LCD_CMD);			 //使用扩展指令集
		lcd_send_up(0x30,LCD_CMD);          //回到基本指令集模式  		
	}
}





/*------------------------------------------------------
函数名称：
			Script_GetAddress
函数功能：
			获取字符的点阵查询地址
函数参数：
			ScriptType——字符类型:SCRIPT_ACSII——ACSII、SCRIPT_OTHER——非ACSII
			ScriptStr——unicode码值
函数返回值：
			返回该字符在GT20L上的存储地址
--------------------------------------------------------*/
uint32_t Script_GetAddress(uint8_t *ScriptStr,uint32_t *GetAddress,uint8_t ziCount)
{
	uint32_t ScriptAddress=0;
	uint32_t MyUnicode1[20] = {0};
	uint32_t MyUnicode2[20] = {0};
	uint8_t ScriptType = 0,i,j;

	j = 0;
	for(i=0;i<ziCount*2;)
	{
		//ASCII码地址	
		if(ScriptStr[i]==0x00)
		{
			MyUnicode2[j] = (uint32_t)ScriptStr[i+1];
			ScriptType = LADING8X16;
		}
		//其他国家字体unicode地址
		else
		{
			MyUnicode2[j] = (uint32_t)(ScriptStr[i]<<8)+(uint32_t)ScriptStr[i+1];
#ifdef	ZILEIXING_8X16
			ScriptType = LADING8X16;
#else
			ScriptType = DIANZHENBUDENGKUANG16;
#endif	  
			
		}
		i+=2;
		j++;
	}
	//找到相应的点阵排列规则，并求得地址
	for(i=0;i<ziCount;i++)
	{
		switch(ScriptType)
		{
			//5X7点ASCII字符
			case ASCII5X7:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)(MyUnicode1[i]*8);
				break;
			//7X8点ASCII字符
			case ASCII7X8:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)(MyUnicode1[i]*8+768);
				break;
			//6X12点ASCII字符
			case ASCII6X12:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*12+1536);
				break;
			//8X16点ASCII字符
			case ASCII8X16:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*16+3072);
				break;
			//8X16点粗体ASCII字符
			case ASCII8X16CU:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*16+5120);
				break;
			//12点阵不等宽ASCII方头(Arial)字符
			case ASCII12_FANGTOU:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*26+6656);
				break;
			//12点阵不等宽ASCII白正(Times)字符
			case ASCII12_BAIZHENG:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*26+9152);
				break;
			//16点阵不等宽ASCII方头(Arial)字符
			case ASCII16_FANGTOU:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*34+11648);
				break;
			//16点阵不等宽ASCII白正(Times)字符
			case ASCII16_BAIZHENG:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*34+14912);
				break;
			//24点阵不等宽ASCII方头(Arial)字符
			case ASCII24_FANGTOU:
				if(MyUnicode1[i]>=0x00&&MyUnicode1[i]<=0xff)
					ScriptAddress = (uint32_t)((MyUnicode1[i]-0x20)*74+18176);
				break;
			//8x16点拉丁文系字符
			case LADING8X16:
				if(MyUnicode2[i]>=0x0020&&MyUnicode2[i]<=0x007F)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0020)*16+25280);
				else
				if(MyUnicode2[i]>=0x00A0&&MyUnicode2[i]<=0x017F)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0040)*16+25280);
				else
				if(MyUnicode2[i]>=0x01A0&&MyUnicode2[i]<=0x01CF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x01A0+320)*16+25280);
				else
				if(MyUnicode2[i]>=0x01F0&&MyUnicode2[i]<=0x01FF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x01F0+368)*16+25280);
				else
				if(MyUnicode2[i]>=0x0210&&MyUnicode2[i]<=0x021F)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0210+384)*16+25280);
				else
				if(MyUnicode2[i]>=0x1EA0&&MyUnicode2[i]<=0x1EFF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x1EA0+400)*16+25280);	
			//8X16点希腊文系字符
				if(MyUnicode2[i]>=0x0370&&MyUnicode2[i]<=0x03CF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0370)*16+33216);
			//8X16点基里尔文系字符
				if(MyUnicode2[i]>=0x0400&&MyUnicode2[i]<=0x045F)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0400)*16+34752);
				else
				if(MyUnicode2[i]>=0x0490&&MyUnicode2[i]<=0x04FF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0490+96)*16+34752);
			//8x16希伯来文系字符
				if(MyUnicode2[i]>=0x0590&&MyUnicode2[i]<=0x05FF)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0590)*16+38080);
			//8x16泰文字符
				if(MyUnicode2[i]>=0x0E00&&MyUnicode2[i]<=0x0E5F)
					ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0E00)*16+39872);
				break;
			case DIANZHENBUDENGKUANG16:
				//不等宽阿拉伯文
				if( MyUnicode2[i] >= 0x0600 && MyUnicode2[i] <= 0x06FF )//alb 1 
				    ScriptAddress = (uint32_t)(34*(MyUnicode2[i]-0x0600)+ 80320); 
				else 
				if( MyUnicode2[i] >= 0xfb50 && MyUnicode2[i] <= 0xfbff )//alb 2 
				    ScriptAddress = (uint32_t)(34*(16*16+MyUnicode2[i]-0xfb50)+ 80320); 
				else 
				if( MyUnicode2[i] >= 0xfe70 && MyUnicode2[i] <= 0xfeff )//alb 3 
				    ScriptAddress = (uint32_t)(34*(16*11+16*16+MyUnicode2[i]-0xfe70)+ 80320);
				//不等宽拉丁文
				else
				if(MyUnicode2[i] >= 0x0020 && MyUnicode2[i]<=0x007f)
					ScriptAddress = (uint32_t)((MyUnicode2[i] - 0x0020)*34+99904);
				else 
				if (MyUnicode2[i]>=0x00A0 && MyUnicode2[i]<=0x017F) 
	        		ScriptAddress = (uint32_t)((MyUnicode2[i]-0x00A0+96) * 34 +99904); 
				else 
				if (MyUnicode2[i]>=0x01A0 && MyUnicode2[i]<=0x01CF) 
				    ScriptAddress = (uint32_t)((MyUnicode2[i]-0x01A0+320) * 34 +99904); 
				else 
				if (MyUnicode2[i]>=0x01F0 && MyUnicode2[i]<=0x01FF) 
				    ScriptAddress = (uint32_t)((MyUnicode2[i]-0x01F0+368) * 34 +99904); 
				else 
				if (MyUnicode2[i]>=0x0210 && MyUnicode2[i]<=0x021F) 
				    ScriptAddress = (uint32_t)((MyUnicode2[i]-0x0210+384) * 34 +99904); 
				else 
				if (MyUnicode2[i]>=0x1EA0 && MyUnicode2[i]<=0x1EFF) 
				    ScriptAddress = ((uint32_t)((MyUnicode2[i]-0x1EA0+400) * 34 +99904));
				//不等宽希腊文
				if (MyUnicode2[i]>=0x0370 && MyUnicode2[i]<=0x03CF) 
	    			ScriptAddress=(uint32_t)((MyUnicode2[i]-0x0370) * 34 +116768);
				//不等宽基里尔文
				if (MyUnicode2[i]>=0x0400 && MyUnicode2[i]<=0x045F) 
				    ScriptAddress=(uint32_t)((MyUnicode2[i]-0x0400) * 34 +120032); 
				else 
				if (MyUnicode2[i]>=0x0490 && MyUnicode2[i]<=0x04FF) 
				    ScriptAddress=(uint32_t)((MyUnicode2[i]-0x0490+96) * 34 +120032);
				break;
		}
		//保存地址
		*(GetAddress++) = ScriptAddress;
	}
	
	return ScriptAddress;
}

