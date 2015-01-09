/****************************************Copyright (c)****************************************************
**                            POWERORIGIN electronics Co.,LTD.
**
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2011-05-01
** Last Version:        V1.00
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:         
** Created date:       
** Version:           
** Descriptions:       
**
** Rechecked by:
*********************************************************************************************************/
#define	MAINDEF
#include "..\config.h"

#include "..\driver\LPC17xx.h"

#include "..\driver\lpc17xx_nvic.h"
#include "..\driver\lpc17xx_wdt.h"
#include "..\driver\lpc17xx_rtc.h"
#include "..\driver\UartDev.h"
//#include "..\mid\AT45DB.h"
#include "..\driver\AT45DB.h"//120401 by cq change structor
//#include "..\mid\spi.h"
#include "..\driver\spi.h"//120401 by cq change structor
#include "global.h"
#include "function.h"
#include "mdb.h"
#include "MdbNote.h"
#include "lcd.h"
#include "menu.h"
#include "pcf8563t.h"
#include "timer.h"
#include "evb.h"
#include "drv.h"
#include "gprs.h"
#include "DevTaskFun.h"
#include "SaleTaskFun.h"
#include "MdbCoin.h"//120401 by cq change structor
//#include "MdbCoinApp.h"//120401 by cq change structor
#include "Hopper.h"//120401 by cq change structor
//#include "HopperApp.h"//120401 by cq change structor
#include "PulseCoin.h"//120401 by cq change structor
//#include "PulseCoinApp.h"//120401 by cq change structor
//#include "DrvApp.h"//120401 by cq change structor
#include "BillAcceptor.h"
#include "CoinAcceptor.h"
#include "Changer.h"
#include "pcf8563t.h"
#include "RealTime.h"
#include "DebugTrace.h"
#include "ITL.h"
#include "VMC_PC.h"
#include "Acdc.h"


#define  YREGCH				   0	   //RTC寄存器号

//Watchodog time out in 20 seconds
#define WDT_TIMEOUT 	20000000
/*********************************************************************************************************
  有关宏与全局变量的定义
*********************************************************************************************************/
/*********************************************************************************************************
  TASK0 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/
#define TASK0_ID                          6                             /* 任务的ID                     */
#define TASK0_PRIO                        TASK0_ID                      /* 任务优先级                   */
#define TASK0_STACK_SIZE                  256                           /* 定义用户堆栈长度             */
OS_STK  TASK0_STACK[TASK0_STACK_SIZE];                                  /* 定义任务0堆栈                */
extern void    TASK0(void *pdata);                                      /* 声明任务0                    */
/*********************************************************************************************************
  TASKB  任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/
#define TASKB_ID                          7                             /* 任务ID                       */
#define TASKB_PRIO                        TASKB_ID                      /* 任务优先级                   */
#define TASKB_STACK_SIZE                  512                           /* 定义用户堆栈长度             */
OS_STK  TASKB_STACK[TASKB_STACK_SIZE];                                  /* 定义任务B堆栈                */
extern void    TASKB(void *pdata);                                      /* 声明任务B                    */
/*********************************************************************************************************
  TASKC 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/   
#define TASKC_ID                          5                             /* 任务ID                       */
#define TASKC_PRIO                        TASKC_ID                      /* 任务优先级                   */
#define TASKC_STACK_SIZE                  512                           /* 定义用户堆栈长度             */
OS_STK  TASKC_STACK[TASKC_STACK_SIZE];                                  /* 定义任务C 堆栈               */
extern void    TASKC(void *pdata);                                      /* 声明任务C                    */
/*********************************************************************************************************
  Task_Server 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/
#define Task_DEXServer_ID                    8                             /* 任务ID                       */
#define Task_DEXServer_PRIO                  Task_DEXServer_ID                /* 任务优先级                   */
#define Task_DEXServer_STACK_SIZE            512                           /* 定义用户堆栈长度             */
OS_STK  Task_DEXServer_STACK[Task_DEXServer_STACK_SIZE];                      /* 定义任务Task_Server 堆栈     */
extern void    Task_DEXServer(void *pdata);                                /* 声明任务Task_Server          */
/*********************************************************************************************************
  Task_CoinPlus 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/
#define Task_CoinPlus_ID                    9                             /* 任务ID                       */
#define Task_CoinPlus_PRIO                  Task_CoinPlus_ID                /* 任务优先级                   */
#define Task_CoinPlus_STACK_SIZE            512                           /* 定义用户堆栈长度             */
OS_STK  Task_CoinPlus_STACK[Task_CoinPlus_STACK_SIZE];                      /* 定义任务Task_CoinPlus 堆栈     */
extern void    Task_CoinPlus(void *pdata);                                /* 声明任务Task_CoinPlus          */


char bufTx[20];				  //串口发送缓存
char bufRx[20];				  //串口接收缓存

int  retCode;				  //串口返回代码
/*----------------------------------------------------------------------------
  Interface to UartDev
 *----------------------------------------------------------------------------*/
extern UartDev_IOB  uart0;
       UartDev_IOB *pUart;

uint32_t DispMsgFlag = 0,DispMsgFlag1 = 0;
//uint16_t DispModeTimer = VP_DSP_TIME2;



void CreateMbox(void)    
{
	g_InitSem = OSSemCreate(0);
	g_PCSem = OSSemCreate(0);
	g_GPRSMsg = OSMboxCreate(NULL);			// 创建GPRS通讯消息邮箱
	//120621 by cq KeySlowly
	//g_KeyMsg  = OSMboxCreate(NULL);			// 创建按键命令消息邮箱
	g_KeyMsg  = OSQCreate(&KeyMsgArray[0],KEYQUEUE);// 创建按键命令消息消息队列	
	g_MoneyMsg  = OSMboxCreate(NULL);		// 创建按键命令消息邮箱//120621 by cq KeySlowly
	g_CoinPlusMsg  = OSQCreate(&CoinPlusArray[0],COINPLUSQUEUE);// 创建按键命令消息消息队列
	g_PCCMDMsg  = OSMboxCreate(NULL);		// 创建PC命令消息邮箱
	g_CommMsg = OSMboxCreate(NULL);			// 创建设备命令邮箱
	g_CommBackMsg = OSMboxCreate(NULL);		// 创建命令返回邮箱
	g_PCCommMsg = OSMboxCreate(NULL);			// 创建PC设备命令邮箱
	g_PCCommBackMsg = OSMboxCreate(NULL);		// 创建PC命令返回邮箱
	//120914 by cq Cashless
	mbVmcToCashless = OSMboxCreate(NULL);
	mbCashlessToVmc = OSMboxCreate(NULL);
}


//更新系统参数，货道驱动映射关系
void systeminit(void)
{
	 uint16_t	i,j,temp;
	 uint8_t	LayNum =0,ChannelNum =0;

	 XSystemPara(READ);											  //读取系统参数
	 XSystemPara1(READ);
	 //XColSystemPara(READ);//按货道添存货量;by gzz 20121120
	 //XKeySystemVisible(READ);//配置用户菜单可见;by gzz 20121204
	 temp = SOFTVERSION;
	 if((SYSPara.SoftwareVersion & 0xff00) != (temp & 0xff00))	
	 { //系统更新，初始化参数			  				
			RstSystemPara();
				
			XChannels(WRITE);						 //保存系统参数
			XSystemPara(WRITE);
			XSystemPara1(WRITE);
			//XColSystemPara(WRITE);//按货道添存货量;by gzz 20121120
			//XKeySystemVisible(WRITE);//配置用户菜单可见;by gzz 20121204
			XTransaction(WRITE);
			RTC_WriteGPREG(LPC_RTC,YREGCH,0);
	}
	
	XChannels(READ);								  //读取系统参数
	XSystemPara(READ);
	XSystemPara1(READ);
	XSystemPara2(READ);
	XColSystemPara(READ);//按货道添存货量;by gzz 20121120
	XKeySystemVisible(READ);//配置用户菜单可见;by gzz 20121204
	XTransaction(READ);

	for(i = 0;i < MAXLAYER;i++)	
	{					    //计算货道驱动映射关系
		ChnMapPara.LAYTODRV[i] = 0;   //清零
		ChnMapPara.DRVTOLAY[i] = 0;
		if( SYSPara.LayEnable[i] == 0x01 )  
		{
			ChnMapPara.LAYTODRV[LayNum++] = i+1;
			ChnMapPara.DRVTOLAY[i] = LayNum;
		}

		ChannelNum = 0;
		for(j = 0; j < MAXCHANNEL; j++)	
		{
			ChnMapPara.CHTODRV[i][j] = 0;		//数组清零
			ChnMapPara.DRVTOCH[i][j] = 0;
			if( (Channels[i][j].ChannelStat & 0x80) == 0x80 ) 
			{
				
				ChnMapPara.CHTODRV[i][ChannelNum++] = j+1;
				ChnMapPara.DRVTOCH[i][j] = ChannelNum;
			}
		
		}
	}
}

/*----------------------------------------------------------------------------
  SendChar
  Write character to Serial Port.
 *----------------------------------------------------------------------------*/
int SendChar (int ch)  {
  int size = 1;

  //retCode =  pUart->BufTx ((void *)&ch, &size, UartDev_FLAG_BLOCKING);
  return (ch);
}


/*----------------------------------------------------------------------------
  SendString
  Write a const string to Serial Port.
 *----------------------------------------------------------------------------*/
void SendString (const char *str)
{
	while (*str)
	{
		SendChar((int)*str++);
	}
}

/*----------------------------------------------------------------------------
  GetKey
  Read character to Serial Port.
 *----------------------------------------------------------------------------*/
int GetKey (void)  {
  int ch;
  int size = 1;


  retCode =  pUart->BufRx ((void *)&ch, &size, UartDev_FLAG_BLOCKING);
  if (size == 0)
    ch = -1;

  return (ch);
}

//return 1需要打开设备，0不需要打开设备
uint8_t IsInTime(uint16_t nowTime,uint16_t beginTime,uint16_t endTime)
{  	
	//Trace("begintime=%d,endtime=%d\n",sysVPMission.saleTime[i],sysVPMission.saleTime[i+1]);
	if( (nowTime>=beginTime)&&(nowTime<=endTime) )			
		return 1;
	else
		return 0;
}

//不用手工补货功能;by gzz 20130122
void AutoAddColumn()
{	
	uint8_t i,j;
	
	for(j = 0; j< MAXLAYER; j++)	
	{
		//Trace("\r\n +");
		for(i = 0; i < MAXCHANNEL;i++)
		{
		    //只有货道状态为1，或3的才可以添货
			//121108 by cq DontReduceNum
			//if(((Channels[j][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER) )
			if((isDontReduceNum())||((Channels[j][i].ChannelStat& 0x03)==CHANNELOK)||((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER))
			{
				//Channels[j][i].GoodsNum = SYSPara.LayMaxGoods[j];
				//Channels[j][i].GoodsNum = colSystemPara.ColMaxGoods[j][i];//按货道添存货量;by gzz 20121120
				Channels[j][i].GoodsNum = 5;
				//Trace("%d, ",colSystemPara.ColMaxGoods[j][i]);
                //更改货道状态
				if((Channels[j][i].ChannelStat& 0x03)==CHANNELOVER)
				{
					Channels[j][i].ChannelStat	&= DRV_CHNSET_BIT;
					Channels[j][i].ChannelStat 	|= CHANNELOK;					
				}	
			}					
		}
	}
}

//重置系统参数，货道对应关系，交易记录
void TurkeySystemSet(void)
{
    uint16_t i,j;
	//uint8_t  *tp;
	//SystemPara tempSysPara;

	/*			
	//1.设业务参数
	SYSPara.MoneyMaxin = 2000;
	SYSPara.MoneyMinLeft = 100;
	SYSPara.ChangeServer = 0;
	SYSPara.Decimal = 2;
	SYSPara.TransMultiMode = 0;
	SYSPara.TransEscape = 0;	
	SYSPara.POSEnable = 0; 
	SYSPara.PCEnable = 0; 
	SYSPara.LCDSel = 2;
	SYSPara.KeySel = 0;//选货按键;by gzz 20121025
	SYSPara.LogoSel = 3;//拼图屏幕样式;by gzz 20121106
	SYSPara.ReaderEnable = 0;//读卡器菜单;by gzz 20121120 
	SYSPara.ReaderCost = 0;

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
	*/
	//SYSPara.GBAEnable = MDB_NOTE_EN;
	//SYSPara.CoinEnable = MDB_COIN_EN;
	//SYSPara.ChangerEnable = MDB_CHANGER_EN;
	//SYSPara.ReaderEnable = 1;//读卡器菜单
	//SYSPara.ReaderCost = 1;  //读卡器扣钱	
	
	SYSPara.ReaderCost = 1;  //读卡器扣钱
	
}


void StartCoinplusTask()
{
	OSTaskCreateExt(Task_CoinPlus,
	                    (void *)0,
	                    &Task_CoinPlus_STACK[Task_CoinPlus_STACK_SIZE-1],
	                    Task_CoinPlus_PRIO,
	                    Task_CoinPlus_ID,
	                    &Task_CoinPlus_STACK[0],
	                    Task_CoinPlus_STACK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  
}


/*********************************************************************************************************
** Function name:       TASK0
** Descriptions:        主任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void TASK0(void *pvData)
{
	uint32_t delay;
	
	pvData = pvData;  
	
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);
	
	// Start watchdog with timeout given
	WDT_Start(WDT_TIMEOUT);			
		   	
	AT45DB_Init();//这个是flash存储器，通过spi总线读写
	
	delay = SystemCoreClock / 10;
	while(--delay);
	
	AT45DB_WaitOK();
	AT45DB_GetFlashPara();
	//AT45DB_Test();		
	
	RTCn_Init();
	
	WDT_Feed(); 
	if(SYSPara1.FlashStat == 0) 
	{
	    systeminit();		   //系统初始化
	}
	WDT_Feed(); 
	CreateMbox() ; 
	while(1) 
	{
		OSTaskCreateExt(TASKB,
	                    (void *)200,
	                    &TASKB_STACK[TASKB_STACK_SIZE-1],
	                    TASKB_PRIO,
	                    TASKB_ID,
	                    &TASKB_STACK[0],
	                    TASKB_STACK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  
	    WDT_Feed(); 
		OSTaskCreateExt(TASKC,
	                    (void *)0,
	                    &TASKC_STACK[TASKC_STACK_SIZE-1],
	                    TASKC_PRIO,
	                    TASKC_ID,
	                    &TASKC_STACK[0],
	                    TASKC_STACK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 
	  
	   // ZlgipInitial(GucMcuIp,GucMcuGateWay,GucMcuMark);
	   WDT_Feed(); 
	   //使用邮箱存mdb硬币;by gzz 20130105
	   OSTaskCreateExt(Task_CoinPlus,
	                    (void *)0,
	                    &Task_CoinPlus_STACK[Task_CoinPlus_STACK_SIZE-1],
	                    Task_CoinPlus_PRIO,
	                    Task_CoinPlus_ID,
	                    &Task_CoinPlus_STACK[0],
	                    Task_CoinPlus_STACK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	   WDT_Feed(); 
	   OSTaskCreateExt(Task_DEXServer,
	                    (void *)0,
	                    &Task_DEXServer_STACK[Task_DEXServer_STACK_SIZE-1],
	                    Task_DEXServer_PRIO,
	                    Task_DEXServer_ID,
	                    &Task_DEXServer_STACK[0],
	                    Task_DEXServer_STACK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  		   	
	
	   	while (1) 
		{
			WDT_Feed(); 
			if(IsReInitStat()) 
			{
			    OSTaskDel(TASKB_PRIO);
				OSTaskDel(TASKC_PRIO);
				OSTaskDel(Task_DEXServer_PRIO);
				OSTaskDel(Task_CoinPlus_PRIO);

				RstDevStatus();
				ReturnSaleStat();							
				break;
			}
			
			OSTimeDly(OS_TICKS_PER_SEC);
	    }
	} 
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        用户程序入口函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main(void)
{	
	targetInit();                                                       /* 初始化目标板，切勿删除       */
    pinInit();                                                          /* 引脚初始化                   */                                                                     
	VCC_POWERON();

	EVB_PWRON();//120426 by cq temperature control

    OSInit();                                                                                                       
  //  OSTaskCreate(TASK0, (void *)0, &stkMainTask[sizeof(stkMainTask) / 4 - 1], TASK0_PRIO);
    OSTaskCreate(TASK0, (void *)0, &TASK0_STACK[TASK0_STACK_SIZE-1], TASK0_PRIO);
    OSStart();
}
 
/*********************************************************************************************************
** Function name:       TASKB
** Descriptions:        维护数据处理，主操作流程在这里
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  TASKB (void *pdata)
{	
	extern uint32_t ChgAmount;
  
    uint8_t 	DispMode,i,j,err;
	/*当前交易状态当确实要开始交易操作时，将临时交易状态的值赋予他； 上次交易状态用来保存最近的一次交易状态，和进行重启恢复使用； 临时交易状态，常用*/
	uint8_t		CurrentStat,LastStat,SaleStat,cSta;
	/*实际物理货道编号 实际货道行号 实际货道列号*/
	uint8_t     ChannelNum,ChannelX,ChannelY;
	/*接收按键及投币消息指针*/
	uint16_t 	*PKeyInput;
	/*接收按键及投币消息*/
	uint16_t	KeyInput;
	/*当前收到的投币值*/														   		
	uint16_t	Money;	
	/*当前疑问金额*/
	uint32_t    DoubtMoney;	
	uint8_t 	InputSys=0;
	uint32_t    DebtMoney=0;	
	uint8_t     state,cMsg,cCashlessKeyInputFlag = 0;

	//120821 by cq ReturnCoinKey
	uint8_t		ReturnCoinKey=0;
	uint32_t	DisintOutput;
	uint16_t	nowTime,beginTime,endTime;
	uint8_t		CompressDev=0,LEDDev=0,DevSet=0;
	
	//RTC_TIME_Type CurrentTime;			

	pdata = pdata;
	//change by liya 20121013
	if(SYSPara.Language == 2)
	{
		LCD_WhiteBlock(0);
		ClearScreen();
	}
	//添加品途屏幕的兼容;by gzz 20121025
	else
	{
		ClearScreen();
	}

    //*********************
    //1、初始化各系统参数值
    //*********************
	ReturnSaleStat();//进入交易状态
	SetInputMode();//允许输入数字按键
	RstInputNum();//重置数字按键
	RstHaveSaleFlag();//复位已进行过交易标志

	LastStat = MVC_VMCRST;//重置最后一次交易状态

//	strcpy(SYSPara.GSMNO,"13706966847");
	DoubtMoney = TranTotal.DoubtMoney;//设疑问金额
	EnableMoneyIn();//使能投币器设置
	
	/*查看和恢复掉电Amount数据，得到掉电前得投币金额，将其保存到交易记录中*/
	g_Amount = RTC_ReadGPREG(LPC_RTC,YREGCH);
	if((g_Amount > 0) && (g_Amount < SYSPara.MoneyMaxin*2)) 
	{
	    TranTotal.Iou = g_Amount;	  	//欠费总额
		XTransaction(WRITE);
		RTC_WriteGPREG(LPC_RTC,YREGCH,0);
	}

	g_Amount = 0;
	SaleStat = MVC_BEGIN;//设交易状态
	LastStat = MVC_BEGIN;//设上次交易状态

	//--------------------------
	//初始化设置
	SYSPara.LCDSel=2;//拼图屏幕样式;by gzz 20121106
	SYSPara2.TenColumn = 1;//十货道
	//专门为土耳其设置的程序
	TurkeySystemSet();
	//==========================

	
	SYSPara2.AutoColumn = 1;//开启自动补货
	
		
	#if DISP_MODE == LCD_DISPLAY 
	LCD_Init();	
	SCLK_BL_SET();//拉高，以避免屏幕闪烁;by gzz 20130121 	
	#endif
	
	init_timer(0, TIME_INTERVAL); 	  //初始化定时器0  Xms
    enable_timer(0);				  //使能定时器0
	InitKey();	                      //键盘初始化

//	Draw_FullBmp(Disp_Bmp);

	//120821 by cq ReturnCoinKey
	ReturnCoinKey_SetInput();


	OSTimeDly(OS_TICKS_PER_SEC/10);	

	//*****************************************************************************
    //2、等待各设备的初始化完成，初始化动作在TASKC中作，完成后，会发送邮箱g_InitSem
    //*****************************************************************************
    DispVersionText();//120704 by cq DispVersion
	DispInitText();	//显示"设备初始化中，请稍候"

	OSSemPend(g_InitSem,OS_TICKS_PER_SEC * 20,&err);	//等待设备初始化完成

	//if(SYSPara.CoinEnable==PULSE_COIN_EN )
	//{
	//	StartCoinplusTask();
	//}
    //选货按键;by gzz 20121025
	if(SYSPara.KeySel == 1 )
	{	
		UpdateSelectionLed();//更新灯的显示
	}

	//不用手工补货功能;by gzz 20130122
	if(SYSPara2.AutoColumn != 0)
	{
		AutoAddColumn();
	}
	
   	CurrentStat = MVC_BEGIN;//设当前交易状态
	TimerDly.CheckDly = SALETIME3;//设一个时间，来定时检测设备状态
	//120621 by cq KeySlowly
	//OSMboxAccept(g_KeyMsg);//检测是否有按键按下
	OSQAccept(g_KeyMsg);//检测是否有按键按下
	DispMode = 1;	
	if(DevSet==0)
	{
		DevSet = 1;
		//if( (SYSPara2.LEDTime.DeviceOn>0)||(SYSPara2.CompressorTime.DeviceOn>0) )
		
		CompressDev = sysVPMission.ACDCCompressorCtr;
		LEDDev = sysVPMission.ACDCLedCtr;
		if(SYSPara2.CompressorTime.DeviceOn>0) 
		{
			CompressDev = 1;
		}
		if(SYSPara2.LEDTime.DeviceOn>0)
		{
			LEDDev = 1;
		}
		
		//acdc设备，最高字节为命令编码，其他为设备控制
		DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)LEDDev<<1)+CompressDev;
		MsgBack.CmdType = DisintOutput;
		OSMboxAccept(g_CommBackMsg);
		OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
		//Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][5],strlen(Menu_ACDC[SYSPara.Language][5]));
		OSTimeDly(OS_TICKS_PER_SEC / 2);
		Trace("\r\n 1.LEDDev=%d,CompressDev=%d",LEDDev,CompressDev);
		
	}
	while(1) 
	{
		while(1) 
		{	
			//Trace("\r\n+++begin sys");
		    //*********************
		    //3、开机进入维护模式
		    //*********************
		    //如果开机时，就检测到拨到维护状态	    
			if(!MAINTEN_SWITCH()) 
			{				   
	           OSTimeDly(2);
			   if(!MAINTEN_SWITCH()) //消抖
			   {
			   	   //Trace("\r\n+++EnterMain");	
				   EnterMaintainStat();//模式设为维护状态
				   			   
			       if(IsMoneyInEN()) //如果投币器状态显示是使能的，予以禁能
				   {
					   SetTouBiStat(CMD_ALL_DIS);  //禁能投币  
					   DisableMoneyIn();//如果投币器状态显示设为禁能
				   }	
                   //Trace("\r\n+++1");	
				   //如果pc有开启，上报pc
				   if( SYSPara.PCEnable == 1 )
				   {
					   OSMboxAccept(g_PCCommMsg);
					   MsgBack.CmdPCCMDMSG = ((uint32_t)VP_ADMIN_ENTER_MENU << 24); 
					   OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送
				   }
				   
				   //Trace("\r\n+++2");	
	
				   #if DISP_MODE == LCD_DISPLAY
				       //Trace("\r\n+++Menu");
					   Cursor_Off(LCDUP);
			           menu();//进入维护菜单
		           #endif
			   }				   
            } 
			else 
			{		
			    //*****************************************************************************
			    //4、从维护菜单退回交易模式后，检测发现他的g_pvmcFlag.Maintain状态还是维护状态
			    //设为0xFF,然后一直等待，等待TASK0任务发现这个状态后，将TASKB,TASKC,TASK_DEXSserver任务
			    //删除后，重新启动
			    //*****************************************************************************
				if(IsMaintainStat()) 
				{
					/*设置ReInitStat后，等待任务重启*/
					SetReInitStat();			    //退出维护状态

					//Trace("\r\n+++QuitMain");

					//如果pc有开启，上报pc
					if( SYSPara.PCEnable == 1 )
					{
					   OSMboxAccept(g_PCCommMsg);
					   MsgBack.CmdPCCMDMSG = ((uint32_t)VP_ADMIN_QUIT_MENU << 24); 
					   OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送
				    }	
					
					
					while(1) 
					{
					    OSTimeDly(OS_TICKS_PER_SEC * 5);
					}
				}

                //*******************************************
				//5、如果是pc通讯，直到通讯成功才进入交易状态
				//*******************************************
				if(( SYSPara.PCEnable == 1 )&&(InputSys == 0))
				{					
					OSSemPend(g_PCSem,0,&err);	//一直等待，直到设备初始化完成
					InputSys = 1;
				}

				//*********************
			    //6、开始交易模式
			    //*********************
				ReturnSaleStat();

				//Trace("\r\nReady opt");
				
				if(IsDeviceOK()) //如果设备是正常状态	
				{	     
				    
					//*********************************
				    //7、在设备正常下，做各个参数的更新
				    //*********************************
				    
				    //在空闲状态下，交替显示现金消费，请插入纸币或硬币
					if(SaleStat == MVC_BEGIN) //如果当前状态是初始空闲状态
					{
					//120621 by cq LCDswitch
						
					/*
						if(DispMode == 0) 
						{
							DispMode = 1;
						}
						else 
						{
							DispMode = 0;
						}
					
						if(DispModeTimer < VP_DSP_TIME1) 
						{
							DispMode = 1;
						}
						else 
						{
							DispMode = 0;
						}
						
						if(DispModeTimer == 0)
							DispModeTimer = VP_DSP_TIME2;
							*/
							
						
						if(DispMode == 1)
							DispMode = 0;
						else
							DispMode = 1;
						/*if(DispMsgFlag>=300)
						{
							DispBeginText(DispMode);
							DispMsgFlag = 0;
						}
						else
							DispMsgFlag++;	*/
						if(sysVPMission.dspTimer2==0)
						{
							sysVPMission.dspTimer2=VP_DSP_TIME4;
							DispBeginText(DispMode);
						}
					}			

					if(SYSPara.ReaderEnable)
					{
						PKeyInput = OSMboxPend(mbCashlessToVmc,1,&err);
						
						if (err == OS_NO_ERR)
						{
							cMsg = (uint8_t)*PKeyInput;
							cSta = Vmc_HandleReaderMsg(cMsg);
							if(cSta != MVC_IDLE)
								SaleStat = cSta;
							cCashlessKeyInputFlag = 1;							
							break;
						}
					}
					//PKeyInput = Vmc_PendFromReader(&SaleStat,SaleStat,1);//120914 by cq Cashless

					//等待按键及投币信号等，如果有则更新按键信息等
					//120621 by cq KeySlowly
					//KeySlowly Begin
					//PKeyInput = OSMboxPend(g_KeyMsg,OS_TICKS_PER_SEC * 2,&err);

					//120821 by cq ReturnCoinKey
					//if(ReturnCoinKey_Get())
					if(ReturnCoinKey_Get()&&(g_Amount > 0)&&(SYSPara.CoinEnable!=MDB_COIN_EN) && ((IsHaveSaled()) || (SYSPara.TransEscape > 0)) ) //如果有交易过，或允许不买货退币，则退币
					{
						//Trace("\r\nReturnCoinKey_Get ((LPC_GPIO0->FIOPIN >> 22) & 0x01) == 0"); 
						Trace("\r\nReturKey=%d,g_Amount=%d,havesale=%d,trans=%d",ReturnCoinKey_Get(),g_Amount,IsHaveSaled(),SYSPara.TransEscape);
						ReturnCoinKey = 1;
						//SaleStat = MVC_PAYOUT;
						break;
					}
					else
					{
						//Trace("\r\nReturnCoinKey_Get == 1");
					}
					


					//等待按键信号					
					PKeyInput = OSQPend(g_KeyMsg,1,&err);
					KeyInput = *PKeyInput;		    
					if(err == OS_NO_ERR) 
					{		
						//Trace("\r\n TASKB,PendKeyMsg=%d,%d",KeyInput);
	#if DISP_MODE == LCD_DISPLAY
					    Cursor_Off(LCDUP);						
						OSTimeDly(OS_TICKS_PER_SEC / 10);
	#endif
						//Trace("\r\n++presskey,pkey=%x,key=%x",*PKeyInput,KeyInput);
						break;
					}	
					//Trace("\r\n +%d,%d",i,sysVPMission.dspTimer1);
					//i++;
					//等待投币信号等，如果有则更新按键信息等
					PKeyInput = OSMboxPend(g_MoneyMsg,1,&err);
					//KeySlowly End
					
					KeyInput = *PKeyInput;		    
					if(err == OS_NO_ERR) 
					{		
						//Trace("\r\n TASKB,PendMoneyMsg=%d",KeyInput);
						#if DISP_MODE == LCD_DISPLAY
					    Cursor_Off(LCDUP);
					    #endif
						break;
					}				

					//操作超时，用来切换新的交易状态 
					if(TimerDly.OperateDly == 0) 
					{			
						SaleStat = SaleTimeoutProc(SaleStat);//操作超时切换交易状态过程

						if(SaleStat != MVC_BEGIN) 
						{
						    KeyInput = KEY_NULL;//清空按键值
							break;
						}
					}

					//定时检测设备状态
					if(TimerDly.CheckDly == 0)
					{			
					    TimerDly.CheckDly = SALETIME3;
					    DoDingshiMsg(); 					    
					}

					//如果没有投币金额，重新使能投币设备
					//121106 by cq BillSingleWork
					/*
					if((g_Amount == 0)
					   && IsMoneyInEN() 
					   && (CashState.ChangerState == CHANGER_OK) 
					   && (CashState.NoteState & NOTE_DEV_DIS)) 
					*/
					if((g_Amount == 0)
					   && IsMoneyInEN() 
					   && ((isBillSingleWork())||(CashState.ChangerState == CHANGER_OK) )
					   && (CashState.NoteState & NOTE_DEV_DIS)) 
					{
					    SetTouBiStat(CMD_NOTE_EN); 	         //使能纸币投币
					}

					
					//处理pc发送的指令	
					//120621 by cq PCcommunication
					/*
					state = VP_CMD_Opt();
					if(state != 0xEE)
					{						
						SaleStat = state;
						//Trace("\r\n two salestat=%d",SaleStat);						
						break;
					}*/

					if( SYSPara.PCEnable == 2 )
					{
						  state = VP_CMD_Opt();						  
						  if(state != 0xEE)
						  {
								SaleStat = state;		
								
								break;
						  }
						  OSTimeDly(OS_TICKS_PER_SEC/10);
					}
					//选货按键;by gzz 20121025
					if(SYSPara.KeySel == 1 )
					{	
						UpdateSelectionLed();//更新灯的显示
					}

					/*
					//添加展示灯控制;by gzz 20121229
					if( (SYSPara2.LEDTime.DeviceOn==1)||(SYSPara2.CompressorTime.DeviceOn==1) )
					{
						Trace("\r\n Begin ledcomp");
						CompressDev = sysVPMission.ACDCCompressorCtr;
						LEDDev = sysVPMission.ACDCLedCtr;
						RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);//获取rtc时间
						OSTimeDly(2);						
						nowTime = (TranData.Time.HOUR * 100) + TranData.Time.MIN;
						if( nowTime <= 2500 )
						{
							if(SYSPara2.LEDTime.DeviceOn==1)
							{
								//交易状态下检测，是否开启展示灯
								beginTime = (SYSPara2.LEDTime.HourOn * 100) + SYSPara2.LEDTime.MinOn;
								endTime = (SYSPara2.LEDTime.HourOff * 100) + SYSPara2.LEDTime.MinOff;
								if((IsInTime(nowTime,beginTime,endTime))&&(sysVPMission.ACDCLedCtr == 0))
								{
									LEDDev = 1;
									DevSet = 1;
								}
								else if((!(IsInTime(nowTime,beginTime,endTime)))&&(sysVPMission.ACDCLedCtr == 1))
								{
									LEDDev = 0;
									DevSet = 1;
								}
								Trace("\r\n now=%d,begin=%d,end=%d,LEDDev=%d,DevSet=%d",nowTime,beginTime,endTime,LEDDev,DevSet);
							}

							if(SYSPara2.CompressorTime.DeviceOn==1)
							{
								//交易状态下检测，是否开启压缩机
								beginTime = (SYSPara2.CompressorTime.HourOn * 100) + SYSPara2.CompressorTime.MinOn;
								endTime = (SYSPara2.CompressorTime.HourOff * 100) + SYSPara2.CompressorTime.MinOff;
								if((IsInTime(nowTime,beginTime,endTime))&&(sysVPMission.ACDCCompressorCtr == 0))
								{
									CompressDev = 1;
									DevSet = 1;
								}
								else if((!(IsInTime(nowTime,beginTime,endTime)))&&(sysVPMission.ACDCCompressorCtr == 1))
								{
									CompressDev = 0;
									DevSet = 1;
								}
								Trace("\r\n now=%d,begin=%d,end=%d,CompressDev=%d,DevSet=%d",nowTime,beginTime,endTime,CompressDev,DevSet);
							}

							if(DevSet == 1)
							{
								DevSet = 0;
								//Trace("\r\n LEDDev=%d,CompressDev=%d",LEDDev,CompressDev);
								//acdc设备，最高字节为命令编码，其他为设备控制
								DisintOutput =	((uint32_t)CMD_ACDC << 24) + ((uint32_t)LEDDev<<1)+CompressDev;
								MsgBack.CmdType = DisintOutput;
								OSMboxAccept(g_CommBackMsg);
								OSMboxPost(g_CommMsg,&MsgBack.CmdType); 
								//Write_LCD(LINE0,Menu_ACDC[SYSPara.Language][5],strlen(Menu_ACDC[SYSPara.Language][5]));
								OSTimeDly(OS_TICKS_PER_SEC / 2);
							}
						}
						
					}*/					
				} 
				//***************************************************************************************
			    //8、在设备故障下，将投币金额找零并进入显示暂停服务，和错误代码中，然后自检，直到故障恢复
			    //***************************************************************************************
				else 
				{			                
				    //如果有投币金额，将它状态改为找零，直接找零
					if(g_Amount > 0) 
					{
					    KeyInput = KEY_NULL;		//
						RstInputMode();
						SaleStat = MVC_PAYOUT;
						break;
					}

	                //如果投币器是使能的，将它禁能
					if(IsMoneyInEN()) 
					{
						DisableMoneyIn();						
						SetTouBiStat(CMD_ALL_DIS);  //禁能投币
					}
	
					/*正常情况下TranTotal.DoubtMoney = DoubtMoney，
					 *当出现纸币器识别到escrowed state，执行stack命令后，却没有出现stacked state时，
					 *TranTotal.DoubtMoney > DoubtMoney，大于的部分即刚才出现在escrowed state的纸币面额
					 */
					 //由于纸币器压钞故障引发的系统故障，将未压成功的金额存入系统疑问金额中
					if(TranTotal.DoubtMoney > DoubtMoney) 
					{
					    TranTotal.DoubtMoney -= DoubtMoney;
					    DoubtMoney = TranTotal.DoubtMoney;
						XTransaction(WRITE);
					}						   	  				   	   		               
					
					#if DISP_MODE == LCD_DISPLAY
				    Cursor_Off(LCDUP);
				    #endif										
					
					//进入显示暂停服务，和错误代码中，然后自检，直到故障恢复
					TimerDly.CheckDly = SALETIME3;
					while(1) 
					{			  
						  DispErrText();//显示暂停服务，和错误代码
						  //如果拨到维护状态	
						  if(!MAINTEN_SWITCH()) 
						  {	    
						      SaleStat = MVC_BEGIN;
							  SetInputMode();//开启数字按键
							  RstInputNum();//初始化按键
						      break;
						  }
						  //设备自检，重新恢复正常后，跳出到正常交易模式
						  else if(IsDeviceOK()) 
						  {
						      SaleStat = MVC_BEGIN;			
							  SetInputMode();
							  RstInputNum();
							  EnableMoneyIn();//使能投币器状态
							  SetTouBiStat(CMD_ALL_EN);//使能投币器 
								//120621 by cq KeySlowly
							  //OSMboxAccept(g_KeyMsg);							  
							  OSQAccept(g_KeyMsg);
							  sysVPMission.dspTimer2 = 0;//加快显示空闲界面;by gzz 20121106	
							  break;					//跳出到正常交易模式
						  }	
						  //还是故障
						  else 
						  {			
						      //时间到后，定时重新检测一遍
						      if(TimerDly.CheckDly == 0) 
							  {		
							      TimerDly.CheckDly = SALETIME3;
								  DoDingshiMsg(); 
							  }
						  }
						  
						  //11、处理pc发送的指令
						  //120621 by cq PCcommunication
						  /*
						  state = VP_CMD_Opt();						  
						  if(state != 0xEE)
						  {
								SaleStat = state;								
						  }
						  OSTimeDly(OS_TICKS_PER_SEC/10);
						  */

						  if( SYSPara.PCEnable == 2 )
						  {
							  state = VP_CMD_Opt();						  
							  if(state != 0xEE)
							  {
									SaleStat = state;								
							  }
							  OSTimeDly(OS_TICKS_PER_SEC/10);
						  }						  
					}
			    }
		    }

			
		}

		
		//*******************************************************************************************************
	    //9、如果有投币，就累加到投币金额中，并判断，如果现在投币金额大于最大交易金额，或剩余零币，则将投币器禁能
	    //*******************************************************************************************************
        //最高位为1,则说明本次为客户投币了

		//120914 by cq Cashless
		/*
		if(cCashlessKeyInputFlag == 1)
		{
			cCashlessKeyInputFlag = 0;
		}
		else 
		*/	
		if (KeyInput & 0x8000) 
		{  					
			RstInputNum();
			Money = KeyInput & 0x7FFF;//得到本次投币金额,包括纸币和硬币
			KeyInput = KEY_NULL;
		//  if((Money % 100) == 0) 		 		//投币面值为100的倍数
		//  { 
				g_Amount += Money;	//累加投币金额								

                //如果是大于最高交易上限，或者大于可找零上限，则予以相应设备禁能
				if(IsMoneyInEN()) 
				{
					if(g_Amount >= SYSPara.MoneyMaxin) 
					{
						DisableMoneyIn();
						SetTouBiStat(CMD_ALL_DIS);      //禁能投币
					} 
					//121106 by cq BillSingleWork
					else if(isBillSingleWork())
					{
						//dosen't dis_note
					}		
					/*
					else if(g_Amount >= g_CoinLeft) 
					{
					    SetTouBiStat(CMD_NOTE_DIS);      //禁能投币
					}
					*/
				}		
				
				if( (SaleStat != MVC_ADDMONEY) && (g_Amount > 0) )
				{	  
				   SaleStat = MVC_SALE;					//进入销售状态 否则是ADDMONEY
				}		 
		//  } 
	
			RTC_WriteGPREG(LPC_RTC,YREGCH,g_Amount);	//暂存当前余额
		}
		else if((KeyInput >> 8) > 0) 
		{			//命令
		    switch(KeyInput >> 8) 
			{
			    case OVER_VALUE_INDEX:
					SaleStat = MVC_OVERVALUE;
					break;

				case RETURN_MONEY_INDEX://退币请求
				    //Trace("\r\n 1.%d,%d,%d,%d,%d",g_Amount,IsHaveSaled(),(SYSPara.TransEscape > 0),(SYSPara.ReaderEnable==0),(Reader_GetFundsMultiplyUnit()==0));
				                          //如果有交易过，或允许不买货退币，则退币
					if((g_Amount > 0) && (IsHaveSaled() || (SYSPara.TransEscape > 0)) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0))) 
					{
						//Trace("\r\n 2.");
					    SaleStat = MVC_PAYOUT;
					}
					break;

				default:
					break;
			}									    
		} 		
		//*********************************************************************
	    //10、按键处理函数，根据是否有投币，以及根据上个的状态，更新下一个的状态
	    //*********************************************************************
		//说明本次为按键了,根据是否有投币，状态设为查询货道状态，或者选货状态
		else 
		{									//按键
		    /*按键处理*/
			SaleStat = KeyProcess(SaleStat,KeyInput,&ChannelNum);//选货按键;by gzz 20121025		
		}

		//120821 by cq ReturnCoinKey
		if(ReturnCoinKey == 1)
		{
			ReturnCoinKey = 0;
			SaleStat = MVC_PAYOUT;
		}



		
		//*******************************************
	    //11、根据当前所处的交易状态，进行相应的操作
	    //*******************************************
		while(1) 
		{
			//Trace("\r\n now salestat=%d",SaleStat);
			CurrentStat = SaleStat;//把真正的交易状态保存到CurrentStat中
			switch(SaleStat) 
			{
				case MVC_CHAXUN:						//查询状态
					RstInputMode();
					//选货按键;by gzz 20121025
					if(SYSPara.KeySel == 0 )
					{
						ChannelNum = g_KeyStat.InputNum;	//得到货道状态				 
					}
					if(DRV_ChnChk(ChannelNum,&ChannelX,&ChannelY) == TRUE) 
					{
					    DispChaxunText(ChannelX,ChannelY,KeyInput);//显示查询结果//选货按键;by gzz 20121025
						
						//OSTimeDly(OS_TICKS_PER_SEC * 2);
						OSTimeDly(OS_TICKS_PER_SEC / 2);
			 
						SaleStat = MVC_BEGIN;//回到初始空闲状态
						OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
						SetInputMode();//允许数字按键
					} 
					else 
					{
					    SaleStat = MVC_INPUTERR;//货道输入错误
					}
					RstInputNum();
					sysVPMission.dspTimer2 = 0;//加快显示空闲界面;by gzz 20121106
					break;
												    //操作超时
				case MVC_CHAOSHI:
					RstInputMode();
					DispChaoshiText();//显示操作超时
					OSTimeDly(OS_TICKS_PER_SEC * 2);
					SaleStat = MVC_SAVEDAT;//需要保存交易记录
					break;

				case MVC_INPUT:						    //输入货道数字状态
					SetInputMode();
					TimerDly.OperateDly = SALETIME2;   
					SaleStat = DispInputText();		  //显示输入货道的页面		
					break;

				case MVC_INPUTERR:					    //货道输入错误
					RstInputMode();
					DispInputErrText();//显示货道输入错误

					OSTimeDly(OS_TICKS_PER_SEC * 2);

					RstInputNum();

					#if CHNINPUT_MODE == TWO_Bit_MODE				
					SaleStat = MVC_INPUT;
					#elif CHNINPUT_MODE == ONE_Bit_MODE
					if(g_Amount > 0) 
					{
					    SaleStat = MVC_SALE;
					} 
					else 
					{
						SaleStat = MVC_BEGIN;
						sysVPMission.dspTimer2 = 0;//加快显示空闲界面;by gzz 20121106
						SetInputMode();
					}
					#endif
					
					OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
					break;
			
				case MVC_SALE:						    //交易状态，等待投币
				    //Trace("\r\n three salestat=%d,g_Amount=%d",SaleStat,g_Amount);
				    //Trace("\r\n input saletime");
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					DispSaleText();    //显示等待投币的页面
					
					if( (!IsMoneyInEN()) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0)))
					{
						if(g_Amount < g_CoinLeft) 
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_ALL_EN);       //使能投币
						}
						else if(g_Amount < SYSPara.MoneyMaxin)
						{
						    EnableMoneyIn();
							SetTouBiStat(CMD_COIN_EN);      //使能投币
						}
					}
					//选货按键;by gzz 20121025
					if(SYSPara.KeySel == 1 )
					{	
						UpdateSelectionLed();//更新灯的显示
					}
					if((SYSPara.ReaderEnable==1)&&(Reader_GetFundsMultiplyUnit()==0)) 
					{
						Vmc_PostToReader(VTR_READER_DISABLE);
					}
					break;

				case MVC_OVERVALUE:					    //投币超限
					RstInputMode();
					DispOverText();//显示纸币超过上限
					
					OSTimeDly(OS_TICKS_PER_SEC * 2);
					//SaleStat = LastStat;
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					if(g_Amount > 0) //找零不足	
					{
						DispSaleText();    //显示等待投币的页面
					}
					
					if( (!IsMoneyInEN()) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0)))
					{
						if(g_Amount < g_CoinLeft) 
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_ALL_EN);		//使能投币
						}
						else if(g_Amount < SYSPara.MoneyMaxin)
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_COIN_EN);		//使能投币
						}
					}
					
					SaleStat = LastStat;
					break;

				case MVC_XUANHUO:						//选货
					RstInputMode();
					//选货按键;by gzz 20121025
					if(SYSPara.KeySel == 0 )
					{
						ChannelNum = g_KeyStat.InputNum;
					}
					RstInputNum();	

					

					//如果pc有开启，上报pc选货
					if( SYSPara.PCEnable == 1 )
					{
					   DRV_ChnChk(ChannelNum,&ChannelX,&ChannelY);						   	
					   OSMboxAccept(g_PCCommMsg);
					   MsgBack.CmdPCCMDMSG = (((uint32_t)VP_BUTTON_RPT << 24) + (VP_BUT_GOODS<<16) + (Channels[ChannelX][ChannelY].GoodsCode)); 
					   OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送
					   //Trace("\r\nbutton=%d,msg=%x",Channels[ChannelX][ChannelY].GoodsCode,MsgBack.CmdPCCMDMSG);
				    }		

					OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
					
					if(DRV_ChnChk(ChannelNum,&ChannelX,&ChannelY) == TRUE)
					{
					    //ChannelNum = GetChannelNo(ChannelX,ChannelY);						
						if((Channels[ChannelX][ChannelY].ChannelStat & DRV_STAT_MASK) != CHANNELOK)//若缺货返回重选	
						{	
							SetHaveSaleFlag();//已进行过交易设置
							//Trace("\r\n goto wuhuo");
						    SaleStat = MVC_WUHUO;		
					    } 
						else if(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) //金额大于单价，则出货	
						{						 		   
							//120914 by cq Cashless
						    //SaleStat = MVC_CHUHUO;
						    SetHaveSaleFlag();//已进行过交易设置

							if( (SYSPara.ReaderEnable)&&(Reader_GetFundsMultiplyUnit()>0) )
							{
								//really set the cost money
								//120926 by cq Cashless
								//Reader_SetItemValue(Channels[ChannelX][ChannelY].GoodsPrice);
								if(SYSPara.ReaderCost)
								{
									Reader_SetItemValue(Channels[ChannelX][ChannelY].GoodsPrice);
								}
								else
								{
									Reader_SetItemValue(0);
								}
								Vmc_PostToReader(VTR_VEND_REQUEST); 

								Vmc_PendFromReader(&SaleStat,MVC_SALE,OS_TICKS_PER_SEC * 10);//120914 by cq Cashless
																
							}
							else
								SaleStat = MVC_CHUHUO;
							
						} 
						else 
						{	
						    SaleStat = MVC_LESSMONEY;	//钱不够			   	
					    }
				    } 
					else 
					{
					     SaleStat = MVC_INPUTERR;//货道输入错误
					}
					break;
			
				case MVC_WUHUO:						    //无货
					//Trace("\r\n input wuhuo");
					RstInputMode();
					DispWuhuoText(ChannelX,ChannelY);//显示无货
					
					OSTimeDly(OS_TICKS_PER_SEC * 2);
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					DispSaleText();    //显示等待投币的页面
					
					if( (!IsMoneyInEN()) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0)))
					{
						if(g_Amount < g_CoinLeft) 
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_ALL_EN);       //使能投币
						}
						else if(g_Amount < SYSPara.MoneyMaxin)
						{
						    EnableMoneyIn();
							SetTouBiStat(CMD_COIN_EN);      //使能投币
						}
					}					

					//Trace("\r\n goto saletime");
					OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
					SaleStat = MVC_SALE;
					break;
												    
				case MVC_CHUHUO:						//出货
					RstInputMode();
					DispChuhuoText();//显示正在出货

				    if(IsMoneyInEN()) //如果投币器是使能的，将它禁能
					{
						DisableMoneyIn();
						SetTouBiStat(CMD_ALL_DIS);      //禁能投币
					}
					
					if(IsEscrowHold()) //判断是否有纸币暂存
					{			   
					    DoNoteStackMsg();//压钞
					}				
					
					SaleStat = DoChuhuoMsg(ChannelNum,ChannelX,ChannelY);//发送出货命令
					RTC_WriteGPREG(LPC_RTC,YREGCH,g_Amount);

					if(!IsDoubtTrans()) //记录疑问交易
					{				
						Channels[ChannelX][ChannelY].SuccessNum++;
						TranTotal.SuccessNum++;

						//用于季度结账用;by gzz 20121009
						Channels[ChannelX][ChannelY].SuccessNumJidu++;
					} 
					else 
					{
						Channels[ChannelX][ChannelY].DoubtNum++;
						TranTotal.DoubtNum++;
						RstDoubtTrans();
					}
					//选货按键;by gzz 20121025
					if(SYSPara.KeySel == 1 )
					{	
						UpdateSelectionLed();//更新灯的显示
					}
					OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
					break;
				
				case MVC_RETRANS:					    //继续购买
					//RstInputMode();
					//TimerDly.OperateDly = SALETIME2;
					//DispRetransText();//显示是否继续购买
					//break;	
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					DispSaleText();    //显示等待投币的页面
					
					if( (!IsMoneyInEN()) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0)))
					{
						if(g_Amount < g_CoinLeft) 
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_ALL_EN);       //使能投币
						}
						else if(g_Amount < SYSPara.MoneyMaxin)
						{
						    EnableMoneyIn();
							SetTouBiStat(CMD_COIN_EN);      //使能投币
						}
					}					

					//Trace("\r\n goto saletime");
					SaleStat = MVC_SALE;
					break;	
				
				case MVC_LESSMONEY:						//金额不足
					RstInputMode();	
					//TimerDly.OperateDly = SALETIME2;   			   					   
					DispLessText(Channels[ChannelX][ChannelY].GoodsPrice);		//显示钱不够			   						   
					/**/
					OSTimeDly(OS_TICKS_PER_SEC * 2);	   
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					DispSaleText();    //显示等待投币的页面
					
					if( (!IsMoneyInEN()) 
						//读卡器没开，或者当前卡里面没钱
						&& ((SYSPara.ReaderEnable==0)||(Reader_GetFundsMultiplyUnit()==0)))
					{
						if(g_Amount < g_CoinLeft) 
						{
							EnableMoneyIn();
							SetTouBiStat(CMD_ALL_EN);       //使能投币
						}
						else if(g_Amount < SYSPara.MoneyMaxin)
						{
						    EnableMoneyIn();
							SetTouBiStat(CMD_COIN_EN);      //使能投币
						}
					}					

					//Trace("\r\n goto saletime");
					OSQFlush(g_KeyMsg);//120621 by cq KeySlowly
					SaleStat = MVC_SALE;
					break;
			
				case MVC_ADDMONEY:						//加钱购买所选商品
					SetInputMode();
					TimerDly.OperateDly = SALETIME;   
					SaleStat = DispAddMoneyText(ChannelX,ChannelY);//显示加钱状态金额
					break;
			
				case MVC_PAYOUT:						//找零
					//硬币器可以不用;by gzz 20130128
					if(SYSPara.CoinEnable > 0) 
					{
						RstInputMode();
						ChgAmount = 0;
					    DispPayoutText();//显示正在找零
						   
					    if(IsEscrowHold()) //纸币有暂存，就退钞
						{
						    DoNoteReturnMsg();
						}
						
						if(IsMoneyInEN()) //投币器有使能，就禁能
						{
							DisableMoneyIn();
							SetTouBiStat(CMD_ALL_DIS);      //禁能投币
						}

						//找零 
						/*										 
						DoCoinOutMsg();
						RTC_WriteGPREG(LPC_RTC,YREGCH,g_Amount);	

					 	HopStat = DoHopperOutMsg();	*/
						DoPayOutMsg();		 //发送找零命令
						RTC_WriteGPREG(LPC_RTC,YREGCH,g_Amount);
					
						if(g_Amount > 0) //找零不足	
						{			  							            
							TranTotal.Iou = g_Amount;	  	    //	
							if( SYSPara.PCEnable == 1 )						
							{
								if(SYSPara.Decimal==1)
								{
								  	DebtMoney = TranTotal.Iou * 10;
								}
								else if(SYSPara.Decimal==2)
								{
								  	DebtMoney = TranTotal.Iou; 
								}	
								
							    OSMboxAccept(g_PCCommMsg);						   
							    MsgBack.CmdPCCMDMSG = ((uint32_t)VP_DEBT_RPT << 24) + DebtMoney;
							    OSMboxPost(g_PCCommMsg,&MsgBack.CmdPCCMDMSG);//通知系统进入维护模式的命令发送		
							    //Trace("\r\n==B debt=%x",MsgBack.CmdPCCMDMSG);
							}
							DispIOUText();	//显示欠费																             										 						        					 							
						} 
						else 
						{
						    if(ChgAmount > 0) //有找零值大于0
							{				
								DispChangeText();//显示取零钱
							}					
						}
						/*
						if(HopStat > 0) {				    //Hopper找零出错
							SetDoubtTrans();
						}
						*/
						if(!IsDoubtTrans()) //判断是否是疑问交易
						{
							//TranTotal.SuccessNum++;
						} 
						else 
						{
							//TranTotal.DoubtNum++;
							RstDoubtTrans();//复位疑问交易设置
						}
					}
					
					RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);//获取rtc时间

					OSTimeDly(OS_TICKS_PER_SEC);

					SaleStat = MVC_SAVEDAT;				
					break;

                //*******************************************
			    //12、交易结束后，进行复位和保存记录的操作
			    //*******************************************
				case MVC_SAVEDAT:					    //保存交易记录
					RstInputMode();
					/*正常情况下TranTotal.DoubtMoney = DoubtMoney，
					 *当出现纸币器识别到escrowed state，执行stack命令后，却没有出现stacked state时，
					 *TranTotal.DoubtMoney > DoubtMoney，大于的部分即刚才出现在escrowed state的纸币面额
					 */
					 //由于纸币器压钞故障引发的系统故障，将未压成功的金额存入系统疑问金额中
					if(TranTotal.DoubtMoney > DoubtMoney) 
					{
						TranTotal.DoubtMoney -= DoubtMoney;
						DoubtMoney = TranTotal.DoubtMoney;
					}
					/*************************************************************/
					//保存交易记录
					XTransaction(WRITE);
					
					/*************************************************************/		 	
					//交易结束，复位交易数据
					g_Amount = 0;
					for(i = 0;i<MAXINCOME; i++) 
					{
						TranData.IncomeCH[i] = 0;
					}
					for(i = 0;i<MAXOUTCOME; i++) 
					{
						TranData.OutcomeCH[i] = 0;
					}
					for(i = 0;i<MAXLAYER; i++)
					{
						for(j = 0;j<MAXCHANNEL; j++)
						{
						    TranData.Goods[i][j] = 0;
						}
					}
					TranData.Money = 0;			
										
					//保存通道记录
					XChannels(WRITE);				
					/*************************************************************/			                 								

					DispEndText();//显示交易结束

					OSTimeDly(OS_TICKS_PER_SEC);
					
					SaleStat = MVC_BEGIN;//返回状态
					sysVPMission.dspTimer2 = 0;//加快显示空闲界面;by gzz 20121106
					SetInputMode();
					RstHaveSaleFlag();//复位已进行过交易标志
				    RTC_WriteGPREG(LPC_RTC,YREGCH,0);

                    /*************************************************************/		
					//交易结束后，重新复位使能，如果设备有问题，就禁能
					if(!IsMoneyInEN()) 
					{
						SetTouBiStat(CMD_ALL_EN);            //使能投币 					
						EnableMoneyIn();
					} 
					//121106 by cq BillSingleWork
					//else if((CashState.ChangerState == CHANGER_OK) 
					          //&& (CashState.NoteState & NOTE_DEV_DIS)) 
					else if(((isBillSingleWork()) || (CashState.ChangerState == CHANGER_OK) )
								  && (CashState.NoteState & NOTE_DEV_DIS)) 
					{
					    SetTouBiStat(CMD_NOTE_EN); 	         //使能纸币投币
					} 
					else 
					{
						if((CashState.ChangerState != CHANGER_OK) 
						   && ((CashState.NoteState & NOTE_DEV_DIS) == 0)) 
						{
						   	SetTouBiStat(CMD_NOTE_DIS); 	 //禁能纸币投币  						
						}
					}				
					if((SYSPara.ReaderEnable==1)&&(Reader_GetFundsMultiplyUnit()==0)) 
					{
						Vmc_PostToReader(VTR_READER_ENABLE);
					}
					break;
			
				default:
					SetInputMode();
					break;
			}

			//****************************
		    //13、交易结束后，保存交易参数
		    //****************************
			LastStat = CurrentStat;//将当前交易状态保存到最后交易状态中
			if(SaleStat == MVC_BEGIN) 
			{
			   LastStat = MVC_BEGIN;
			}
			if(SaleStat == MVC_SALE) 
			{
			   break;
			}
			if((CurrentStat == SaleStat) || (SaleStat == MVC_BEGIN)) 
			{
				//OSMboxAccept(g_KeyMsg);//120621 by cq KeySlowly
				break;
			}
		}							
    }		  	   
}

/*********************************************************************************************************
** Function name:       TASKC
** Descriptions:        MDB数据处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void  TASKC (void *pdata)
{	
	uint8_t     err;
	uint32_t    *PCmdMsg;
	uint8_t     msg1,msg2,cMsg;
	uint8_t key;
	
	pdata = pdata;
	VCC_POWERON();
	
	Trace("\r\n==1");
												    
	OSTimeDly(OS_TICKS_PER_SEC*3);			 //等待外设初始化


    //****************
    //1、初始化各设备
    //****************
	Uart2_Init();                                //初始化串口
	Uart3_Init();
	Uart0_Init();
	Uart1_Init();//选货按键;by gzz 20121025
	
	Trace("\r\n==2");

	RstDevStatus();			                      //复位设备故障状态
	NoteEscrowClr();							  //清除Escrow状态	
	RstTouBiTest();								  //复位投币测试状态，维护菜单时测试投币使用

	g_CoinLeft = SYSPara.MoneyMaxin;				  //零币值

	InitMoneyDevStat();							  /*初始化非MDB投币找零设备状态*/
	
	CheckPricePara();							  /*检测价格设置是否有0值*/

	Trace("\r\n==3");
	
	EVB_MODE_SET();					 	          //设为EVB模式
	OSTimeDly(2);		
	CheckDriverStat();							  /*检测货道板通讯状态及出货检测模块状态*/	
				
	Trace("\r\n==4");

	GetNoteEnBit();								  /*获取纸币使能通道*/
	GetCoinEnBit();								  /*获取硬币使能通道*/
	GetEscrowMinCh();							  /*获取纸币最低允许暂存通道*/

	Trace("\r\n==5");

	CheckNotePara();							  /*检测是否手动置纸币器异常*/
	CheckCoinPara();							  /*检测是否手动置硬币器异常*/	

	RTCn_Init();                                  /*开启实时时钟*/	

	CheckVMCStat();                               //检测设备状态 

	Trace("\r\n==6");
	
	MDB_MODE_SET();					              //设为MDB模式	
	OSTimeDly(2);	
	Changer_Init();								  //找零器初始化	
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	CoinAcceptor_Init();						  //硬币器初始化

	Trace("\r\n==7");
	
	EnableMoneyType();							 /*确定使能的纸币硬币投币通道*/
	BillAcceptor_Init();						  //纸币器初始化

    OSSemPost(g_InitSem);
	Trace("\r\npc init");	
	//pc机初始化
	if( SYSPara.PCEnable == 1 )
	{
		VPMission_Init();
		OSSemPost(g_PCSem);
	}
	Trace("\r\nInitOK");
	//选货按键;by gzz 20121025
	if(SYSPara.KeySel == 1 )
	{		
		SelectKey_InitProc();//120816 by cq SelectKey
	}

	//120914 by cq Cashless
	Reader_InitPara();

	//121113 by cq Cashless
	//SYSPara.ReaderCost = 0;
	//SYSPara.ReaderEnable = 0;
	
	while(1) 
	{  	
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		//120914 by cq Cashless
		if(SYSPara.ReaderEnable)
		{
			PCmdMsg = OSMboxPend(mbVmcToCashless,OS_TICKS_PER_SEC / 10,&err);
			
			if (err == OS_NO_ERR)  
				cMsg = (uint8_t)*PCmdMsg;
			else
				cMsg = 0;
			
			Reader_MainProc(cMsg);	  
			
			if(Reader_IsComErr())
				SetDevStatus(DEV_ERR_BIT_READER);
			else
				ClrDevStatus(DEV_ERR_BIT_READER);
			  
		}

	
		  //选货按键;by gzz 20121025
		  if(SYSPara.KeySel == 1 )
		  {
		  	  key = GetSelectKey(sysVPMission.selectMode);//选货按键交易时使用跑马灯;by gzz 20121121
			  if(key != KEY_NULL)
			  {				
					MsgBack.keyback = key;
					OSQPost(g_KeyMsg,&MsgBack.keyback);		
					OSTimeDly(OS_TICKS_PER_SEC / 5);
			  }
		  }
		  CheckVMCStat();

		  if((CoinAcceptor_Available() == TRUE) || IsCoinTest())//如果在交易中，硬币器通讯设备是正常的；或者在维护菜单中，正在测试硬币器，就不断予以轮询
		  { 
		      CoinAcceptor_PollProc();				 //硬币器轮询
		  }

		  OSTimeDly(OS_TICKS_PER_SEC / 10);

		  if((BillAcceptor_Available() == TRUE) || IsNoteTest())//如果在交易中，纸币器通讯设备是正常的；或者在维护菜单中，正在测试纸币器，就不断予以轮询
		  { 
		      BillAcceptor_PollProc();				 //纸币器轮询
		  }					  		

		  //120620 by cq PCcommunication
		  if((BeSaleStat())&&( SYSPara.PCEnable == 2 ))  //pc端轮询
		  //if(BeSaleStat())
		  {
		  	  VPMission_Poll();
			  /*
			  if(  sysVPMission.comErrNum >= VP_TIME_OUNT_NUM )
			  {
			  	OSMboxAccept(g_PCCMDMsg);
				MsgBack.CmdPCType = VP_RESET_IND;
				OSMboxPost(g_PCCMDMsg,&MsgBack.CmdPCType);	
			  }
			  */
		  }

		  CheckVMCStat();
		  
		  if(IsEscrowSta())	//如果目前纸币暂存着，等待压钞，就不作任何事
		  {						 
		      OSTimeDly(OS_TICKS_PER_SEC / 10);
			  continue;
		  }


          //*******************************
	      //2、根据接收邮箱信息，作设备操作
	      //*******************************
		  PCmdMsg = OSMboxPend(g_CommMsg,OS_TICKS_PER_SEC / 10,&err);//接收邮箱信息，用来得到本次是要操作什么设备
			  
		  if (err ==OS_NO_ERR)	
		  {
		      //如果是操作EVB设备的命令
			  if ((*PCmdMsg >> 24) >= CMD_TYPE_EVB)	
			  {	                 
				   //EVB出货或指令			   
				   EVB_MODE_SET();	                                 //转到EVB模式
				   OSTimeDly(2);
				   
				   switch(*PCmdMsg >> 24) 
				   {		  
					  case CMD_GOODS_CHECK:		                     //查询货道状态
					      DrvCheckCommand(*PCmdMsg & 0xFF);				 /*货道查询命令*/
						  break;			 
					  
					  case CMD_GOODS_OUT:	                         //出货
							Trace("\r\nCMD_GOODS_OUT");
						  NoteEscrowClr();								 /*到这一步，即使纸币还处在暂存，也清此状态，下同*/
						  DrvOutCommand(*PCmdMsg & 0xFF); 				 /*出货命令*/
						  break;
					  
					  case CMD_HOPPER_CHANGE:	                     //找零
							EVB1_MODE_SET();//120821 by cq HoppeyUse							
							OSTimeDly(2);//120821 by cq HoppeyUse
						  NoteEscrowClr();
						  HopperPayoutCmd(*PCmdMsg & 0xFFFFF,(*PCmdMsg >> 20) & 0x0F);				  				  
						  break;										 /*Hopper找零命令*/
					  //添加压缩机控制;by gzz 20121224	  
					  case CMD_ACDC:			          //操作acdc设备
							Trace("\r\nACDC,%#x,led=%d,dist=%d",*PCmdMsg&0xff,((uint32_t)(*PCmdMsg)>>1) & 0x01,*PCmdMsg & 0x01);
							sysVPMission.ACDCLedCtr = ((uint32_t)(*PCmdMsg)>>1) & 0x01;
							sysVPMission.ACDCCompressorCtr = *PCmdMsg & 0x01;
							ACDCModuleQuery(sysVPMission.ACDCLedCtr,sysVPMission.ACDCCompressorCtr);			   /*ACDC命令*/
							break;		
					  default:
					  	  break;
			       }
				   	  	
				   MDB_MODE_SET();	                                 //转到MDB模式	
				   OSTimeDly(2);		   	
			  }
			  //如果是操作其它非EVB设备的命令，比如MDB，或GBA等 
			  else 
			  {
			     switch(*PCmdMsg >> 24)	
				 {   
			         case CMD_COIN_CHANGE:                           //MDB找零指令
						Trace("\r\nCMD_COIN_CHANGE");
						 NoteEscrowClr();
						 //CoinPayoutCmd(*PCmdMsg);						 /*MDB硬币器找币命令*/
						 Changer_Payout(*PCmdMsg);										 
						 break;

					 case CMD_NOTE_STACK:                            //压钞命令
					     BillAcceptor_Stack(CashTypeEnBit.BillType);;	 /*压钞*/
						 break;

					 case CMD_NOTE_RETURN:                           //退钞命令
					     BillAcceptor_Return(CashTypeEnBit.BillType);	 /*退钞*/
						 break;

					 case CMD_ALL_DIS:		                         //禁能投币设备
					 case CMD_COIN_DIS:                              //禁能硬币器
						 DisCoinDevCmd();								 /*禁能硬币器命令*/

						 if((*PCmdMsg >> 24) == CMD_COIN_DIS) 
						 {
						     MsgBack.pBackMsg = 0;					 
						     OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

						     if(IsMaintainStat())                        //如果是维护状态
							 {
							     RstCoinTest();							 //退出硬币测试
							 }
							 
							 OSTimeDly(OS_TICKS_PER_SEC / 10);

							 break;
						 }

					 case CMD_NOTE_DIS:				 					 
						 DisNoteDevCmd();								 /*禁能纸币器命令*/

						 MsgBack.pBackMsg = 0;					 
						 OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

						 if(IsMaintainStat() && ((*PCmdMsg >> 24) == CMD_NOTE_DIS)) //如果是维护状态
						 {
						     RstNoteTest();
						 }												//退出纸币测试

						 OSTimeDly(OS_TICKS_PER_SEC / 10);
						 break;

					 case CMD_ALL_EN:			                     //使能投币设备				 
					 case CMD_COIN_EN:                               //使能硬币器
				         EnCoinDevCmd();								 /*使能硬币器命令*/

						 if((*PCmdMsg >> 24) == CMD_COIN_EN) 
						 {
						     MsgBack.pBackMsg = 0;					 
						     OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

						     if(IsMaintainStat())                        //如果是维护状态
							 {
							     SetCoinTest();							 //使能硬币测试
							 }
							 
							 OSTimeDly(OS_TICKS_PER_SEC / 10);

							 break;
						 }

					 case CMD_NOTE_EN:				 
						 EnNoteDevCmd();								 /*使能纸币器命令*/

						 MsgBack.pBackMsg = 0;					 
						 OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);

						 if(IsMaintainStat() && ((*PCmdMsg >> 24) == CMD_NOTE_EN)) //如果是维护状态
						 {
						     SetNoteTest();								//使能纸币测试
						 }												

						 OSTimeDly(OS_TICKS_PER_SEC / 10);
						 break;
					 
					 default:
					 	break;		      
			     }			    	  
		      }
	      }	 //if (err ==OS_NO_ERR)	  

		  //*******************************************
	      //3、接收邮箱信息，得到本次是要操作什么PC命令
	      //*******************************************
		  if( SYSPara.PCEnable == 1 )
		  {
			  OSTimeDly(OS_TICKS_PER_SEC / 10);		
			  PCmdMsg = OSMboxPend(g_PCCommMsg,OS_TICKS_PER_SEC / 10,&err);//接收邮箱信息，用来得到本次是要操作什么PC命令
			  if (err ==OS_NO_ERR)	
			  {
			  		//Trace("\r\n==C debt=%x",*PCmdMsg);
			  		switch(*PCmdMsg >> 24)
			  		{
						case VP_ADMIN_ENTER_MENU:
							VPMission_Admin_RPT( VP_ADMIN_ENTER_MENU, 0, 0 );
							break;
						case VP_ADMIN_QUIT_MENU:
					    	VPMission_Admin_RPT( VP_ADMIN_QUIT_MENU, 0, 0 );
					        break;
						case VP_DEBT_RPT:							
							*PCmdMsg &= (~((uint32_t)0xff << 24));
							//Trace("\r\n==C debt,PCmdMsg=%d",*PCmdMsg);
							VPMission_Debt_RPT(0,0,*PCmdMsg);					    	
					        break;	
						case VP_COST_RPT:
							*PCmdMsg &= (~((uint32_t)0xff << 24));
							VPMission_Cost_RPT(0, *PCmdMsg);					    	
					        break;	
						case VP_RESET_IND:
							//pc机初始化
							VPMission_Init();
							OSSemPost(g_PCSem);							
							break;
						case VP_BUTTON_RPT:
							//*PCmdMsg &= (~((uint32_t)0xff << 24));
							msg1 = (*PCmdMsg & ((uint32_t)0xff << 16))>>16;
							msg2 = *PCmdMsg & 0xff;
							//Trace("\r\nbuttonmail=%x,one=%d,two=%d",*PCmdMsg,msg1,msg2);	
							VPMission_Button_RPT( msg1, msg2 );					    	
					        break;		
						default:
							break;
					}		
			  }
		  }
		  
    } 
}

/*********************************************************************************************************
** Function name:       Task_CoinPlus
** Descriptions:        
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  Task_CoinPlus (void *pdata)
{
	/*接收按键及投币消息指针*/
	uint16_t 	*PKeyInput;
	/*接收按键及投币消息*/
	uint32_t	KeyInput;
	uint8_t 	err;
	
	//if(SYSPara.CoinEnable==PULSE_COIN_EN )//使用邮箱存mdb硬币;by gzz 20130105
	{
		//Trace("\r\n enterTASKCoin");
		while(1)
		{
			//等待脉冲投币信号					
			PKeyInput = OSQPend(g_CoinPlusMsg,1,&err);
			//KeyInput = *PKeyInput;		    
			if(err == OS_NO_ERR) 
			{						
				MsgBack.moneyback = *PKeyInput;   //money编码（16位）：最高位置1，其他为面值	
				//Trace("\r\n TASKCoin,money=%#x,moneyback=%#x",*PKeyInput,MsgBack.moneyback);
				while( OSMboxPost(g_MoneyMsg,&MsgBack.moneyback) != OS_NO_ERR)
			    {
			    	//Trace("\r\n PostErr");
			   		OSTimeDly(OS_TICKS_PER_SEC / 100);
			    }
				//Trace("\r\n PostOK");		
				//Trace("\r\n .TASKC,money=%d",MsgBack.moneyback & 0x7FFF);
			}
			OSTimeDly(OS_TICKS_PER_SEC / 100);
		}
	}
}


/*********************************************************************************************************
** Function name:       Task_Server
** Descriptions:        
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  Task_DEXServer (void *pdata)
{
	uint8_t i;
  	int sizeRx, sizeTx;
  //  unsigned char c;

    pdata = pdata;	

/* Expansion statement ----------------------------------------------------- */
	// DeInit NVIC and SCBNVIC
//	NVIC_DeInit();
//	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
//	NVIC_SetPriorityGrouping(0x05);
 /*
	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif
*/
	/* End of Expansion statement ------------------------------------------------ */

    
		//UART0_Init();
		pUart = &uart0;          /* Select 'uart0' as active communication interface. */
		pUart->Init();
		/*
		while(1) {
			sizeRx = UART0_Read(PCdata,50);
			if(sizeRx) {
			    UART0_Write(PCdata,sizeRx);	
				PCdata[sizeRx] = '\0';			    
				if(!strncmp(PCdata,"GPRS Test",9)) {				    
					break;
				}				
			}
			OSTimeDly(OS_TICKS_PER_SEC);
		}
			
		UART_FullModemForcePinState(LPC_UART1,UART1_MODEM_PIN_DTR,ACTIVE);
		UART_FullModemConfigMode(LPC_UART1,UART1_MODEM_MODE_AUTO_RTS,ENABLE);
		//SendString ("CMSIS Serial Device Example\r\n\r\n");
			
		GPRS_Test();
		*/	

		 while (1) 
		 {
		      //for (i = 0; i < 10000; i++);
		      sizeRx = 20;
		      retCode =  pUart->BufRx ((void *)&bufRx[0], &sizeRx, UartDev_FLAG_NONBLOCKING);
		      for (i = 0; i < sizeRx; i++) 
			  {
			  		bufTx[i] = bufRx[i];
			  }
		      sizeTx = sizeRx;
		      //retCode =  pUart->BufTx((void *)&bufTx[0], &sizeTx, UartDev_FLAG_NONBLOCKING);
		      if (sizeRx > 0) 
			  {
		      //  SendString("\r\n");
		      }
		     
			  OSTimeDly(OS_TICKS_PER_SEC / 4);
		}
		  
			
//
//		if(TRUE == UART0_ReceiveNByte(Buf_Global, 512))     // 如果能够成功在接收缓冲池获取512字节数据
//        {
//            UART0_SendNByte(Buf_Global, 512);               // 那么将此512字节数据压入发送缓冲池
//        }
//        OSTimeDly(OS_TICKS_PER_SEC/30);                     
					
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/
