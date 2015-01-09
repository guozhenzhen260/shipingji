/*******************************************************************


 本文主要定义一些全局变量 

 采用预定义方式,避免其它文件包含时重复定义而出错 

********************************************************************/
#ifndef GLOBAL
#define GLOBAL

#ifdef __cplusplus
    extern "C" {
#endif
#include "..\config.h"
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc17xx_gpio.h"
#include "..\driver\lpc17xx_rtc.h"
#include "..\driver\lpc17xx_pinsel.h"
#include "keypad.h"
#include "timer.h"
#include "MdbNote.h"
#include "Mdbcoin.h"
#include "gbahr1.h"
#include "uart.h"
#include "uart1.h"
#include "i2c.h"

//120914 by cq Cashless
#include "reader.h"
#include "task.h"

//#define MDBDEBUG        1		     	//MDB调试
#define EVBDEBUG        0			 	//EVB调试
#define GBADEBUG        0			 	//232纸币器调试
#define BEEPDEBUG       0			 	//投币时蜂鸣器响

#define SOFTVERSION	    0x1300			//软件版本xx.yy  xx会更新缺省参数
#define HAEDWAREVERDION	0x1700			//硬件版本

/*键盘配置*/
#define  KEY_4X6   0			 		//4*6键盘
#define  KEY_2X8   1			 		//2*8键盘
#define  KEY_MODE    KEY_2X8

/*显示方式配置*/
#define LCD_DISPLAY  0	                //LCD显示模式
//#define LED_DISPLAY  1	                //LED显示模式
#define DISP_MODE	 LCD_DISPLAY

/*货道号输入方式配置*/
#define ONE_Bit_MODE	 0	            //1位货道编号输入方式
#define TWO_Bit_MODE	 1	            //2位货道编号输入方式
#define CHNINPUT_MODE	 TWO_Bit_MODE

/*货道驱动板配置*/
#define DRV_12 	  0						//12货道板
#define DRV_64 	  1						//64货道板
#define DRV_TYPE 	  DRV_64

/*FLASH配置*/
#define AT45DB081D   0					//AT45DB081D
#define AT45DB161D   1  				//AT45DB161D
#define FLASH_TYPE   AT45DB161D

/*时钟配置*/
#define PCF8563T_CFG 0					//PCF8563T
#define LPC_RTC_CFG  1  				//LPC_RTC
#define RTC_TYPE     PCF8563T_CFG

#define SALETIME    30000				//投币超时时间(SALETIME/100)s
#define SALETIME2   500				    //3000-500按键操作时间(SALETIME2/100)s
#define SALETIME3   3000				//(SALETIME3/100)s

/*以下为1配置相应设备，为0则无相应设备*/
#define GBAHRDEVICE    1				//GBA HR1
#define MBVDEVICE      1				//MDB纸币器
#define COINDEVICE     1				//MDB硬币器
#define PULSEDEVICE    1				//并行脉冲设备
#define HOPPERDEVICE   1				//Hopper
#define DRVDEVICE      1				//货道驱动板

/*纸币器类型定义*/
#define	NOTE_CLOSE	   	0
#define	NOTE_MEI		1
#define	NOTE_GBA		2
#define	NOTE_ITL		3

//读卡器菜单;by gzz 20121120
//读卡器定义
#define	READEDR_CLOSE	   	0
#define	READEDR_MDB			1


#if  FLASH_TYPE == AT45DB081D
#define PAGESIZE 264		//08
#elif  FLASH_TYPE == AT45DB161D
#define PAGESIZE 528		//16
#endif	

#if KEY_MODE == KEY_4X6
#elif KEY_MODE == KEY_2X8
#define KEY_1      0x0C			 //按键 1
#define KEY_2      0x0D
#define KEY_3      0x0E
#define KEY_4      0x09
#define KEY_5      0x08
#define KEY_6      0x0F
#define KEY_7      0x0B
#define KEY_8      0x0A
#define KEY_9      0x04
#define KEY_0      0x06
#define KEY_DOT    0x05			//按键 .
#define KEY_SET    0x03
#define KEY_CLR    0x03
#define KEY_UP     0x07
#define KEY_DOWN   0x00
#define KEY_CANCEL 0x01
#define KEY_ENTER  0x02			//按键 ENTER
#endif

/****************************************************************************************/
#define MVC_BEGIN    	  0							//初始状态
#define MVC_SALE 	      1							//交易状态
#define MVC_XUANHUO    	  2							//选货
#define MVC_WUHUO    	  3							//无货
#define MVC_CHUHUO  	  4							//出货
#define MVC_LESSMONEY 	  5							//钱不够
#define MVC_ADDMONEY 	  6							//加钱
#define MVC_RETRANS  	  7							//继续交易
#define MVC_PAYOUT 	      8							//找零
#define MVC_OVERVALUE	  9							//面值太大
#define MVC_INPUT	      10							//输入货道数字状态
#define MVC_INPUTERR	  11						//货道输入错误
#define MVC_CHAXUN	      12							//货道查询状态
#define MVC_SAVEDAT		  13                         //需要保存交易记录
#define MVC_CHAOSHI		  14                        //操作超时状态
#define MVC_VMCRST		0xF0
#define MVC_IDLE        0xFF
/*******************************************************************************************/

#define MAXLAYER 		8	 					//最大层道数 8
#define MAXLAYER_6 		6	 					//最大层道数 8


//121105 by cq TenChannels
#if 0//换成10个货道的;by gzz 20121226
#define MAXCHANNEL 		8	 					//最大货道数 8
#else
#define MAXCHANNEL 		10	 					//最大货道数 8
#endif

#define HOPPERNUM 		3	 					//hopper数

#define	MAXGOODS		26						//货道最大填货数
#define	MAXINCOME		32						//最大收币通道数
#define	MAXOUTCOME		32						//最大找币通道数
#define	MAXTRANPAGE		4000					//最大存储交易页面数

#define WRITE			1
#define READ			0

//设备状态
#define DEVICE_OK   		   0x00		  		//设备正常
#define DEV_ERR_NOTE           0xF1		  		//纸币器故障
#define DEV_ERR_COIN           0xF2		  		//硬币器故障
#define DEV_ERR_CHANGER        0xF3		  		//找零器器故障
#define DEV_ERR_COINLOW        0xF4		  		//找零器硬币不足
#define DEV_ERR_CHNOUT         0xF5		  		//所有通道无货
#define DEV_ERR_CHNALL         0xF6		  		//所有通道故障
#define DEV_ERR_DRVCOMM        0xF7		  		//货道驱动板故障
#define DEV_ERR_CHNDIS         0xF8		  		//所有货道不可用
#define DEV_ERR_PARASET        0xF9		  		//参数设置出错
#define DEV_ERR_LCDCOMM        0xFA		  		//LCD故障
#define DEV_ERR_PRICESET       0xFB		  		//价格设置出错，设置成0了
#define DEV_ERR_GOODSCHK       0xFC		  	//出货确认模块出错
#define DEV_ERR_FLASH          0xFD		  		//Flash出错
#define DEV_ERR_RTC            0xFE		  		//实时时钟出错
#define DEV_ERR_READER         0xC4       //120914 by cq Cashless

#define DEV_ERR_BIT_NOTE       (1 << 0)		  		//纸币器故障
#define DEV_ERR_BIT_COIN       (1 << 1)		  		//硬币器故障
#define DEV_ERR_BIT_CHANGER    (1 << 2)		  		//找零器器故障
#define DEV_ERR_BIT_COINLOW    (1 << 3)		  		//找零器硬币不足
#define DEV_ERR_BIT_CHNOUT     (1 << 4)		  		//所有通道无货
#define DEV_ERR_BIT_CHNALL     (1 << 5)		  		//所有通道故障
#define DEV_ERR_BIT_DRVCOMM    (1 << 6)		  		//货道驱动板故障
#define DEV_ERR_BIT_CHNDIST    (1 << 7)		  		//所有货道不可用
#define DEV_ERR_BIT_PARASET    (1 << 8)		  		//参数设置出错
#define DEV_ERR_BIT_LCDCOMM    (1 << 9)		  		//LCD故障
#define DEV_ERR_BIT_PRICESET   (1 << 10)		  		//价格设置出错，设置成0了
#define DEV_ERR_BIT_GOODSCHK   (1 << 11)		  		//出货确认模块出错
#define DEV_ERR_BIT_FLASH      (1 << 12)		  		//Flash出错
#define DEV_ERR_BIT_RTC        (1 << 13)		  		//实时时钟出错
#define DEV_ERR_BIT_READER	   (1 << 14)	//120914 by cq Cashless

//货币通道
#define BILLCHMAX			8			   		//纸币器最大通道数
#define COINCHMAX			8			   		//硬币器最大通道数
#define HOPPERCHMAX			HOPPERNUM	   		//找零器最大通道数 = 3

#define DEV_IN1_BASE       	0	 			 	//投币设备1在IncomeValue[MAXINCOME]中的起始位置
#define DEV_IN1_NUM			8	  				//投币设备1通道数 <= 8
#define DEV_IN2_BASE        16
#define DEV_IN2_NUM			8		
#define DEV_OUT1_BASE       0	 				//找零设备1在OutcomeValue[MAXINCOME]中的起始位置
#define DEV_OUT1_NUM		8	 				//找零设备1通道数 <= 8
#define DEV_OUT2_BASE       16
#define DEV_OUT2_NUM		8
/*注：不要重叠*/

#define CHANGE_HOPPER_BASE		DEV_OUT2_BASE	      //Hopper找零通道起始值   
#define CHANGE_HOPPER_NUM       HOPPERNUM             //Hopper找零通道数
#define COIN_PULSE_BASE			DEV_IN1_BASE		  //PULSE投币通道起始值
#define COIN_PULSE_NUM  		DEV_IN1_NUM			  //PULSE投币通道数
#define COIN_MDB_BASE		    DEV_IN1_BASE		  //MDB硬币器投币通道起始值
#define COIN_MDB_NUM  		    DEV_IN1_NUM			  //MDB硬币器投币通道数
#define CHANGE_COIN_BASE		DEV_OUT1_BASE	      //MDB硬币器找零通道起始值
#define CHANGE_COIN_NUM  		DEV_OUT1_NUM		  //MDB硬币器找零通道数
#define NOTE_MDB_BASE			DEV_IN2_BASE		  //MDB纸币器投币通道起始值
#define NOTE_MDB_NUM  		    DEV_IN2_NUM			  //MDB纸币器投币通道数
#define NOTE_GBA_BASE			DEV_IN2_BASE		  //GBA纸币器投币通道起始值
#define NOTE_GBA_NUM  		    DEV_IN2_NUM			  //GBA纸币器投币通道数
#define NOTE_ITL_BASE			DEV_IN2_BASE		  //GBA纸币器投币通道起始值
#define NOTE_ITL_NUM  		    DEV_IN2_NUM			  //GBA纸币器投币通道数

/*注：COIN_MDB_BASE和CHANGE_COIN_BASE为同一设备，必须设一样的值 */

#define CMD_TYPE_EVB			0x80				 //EVB命令
#define CMD_TYPE_MDB			0x00				 //MDB命令

/*EVB命令 >= 0x80*/
#define CMD_GOODS_CHECK      0x80				//检测货道有无货命令
#define CMD_GOODS_OUT        0x81				//出货命令
#define CMD_HOPPER_CHANGE    0x82				//HOPPER找零命令
#define CMD_ACDC        	 0x85				//ACDC设备//添加压缩机控制;by gzz 20121224


/*MDB及其他命令 < 0x80*/
#define CMD_COIN_CHANGE      0x00				//MDB硬币器找零命令
#define CMD_ALL_DIS          0x01				//所有投币设备禁能命令
#define CMD_ALL_EN           0x02				//所有投币设备使能命令
#define CMD_NOTE_DIS         0x03				//纸币器禁能命令
#define CMD_NOTE_EN          0x04				//纸币器使能命令
#define CMD_COIN_DIS         0x05				//硬币器禁能命令
#define CMD_COIN_EN          0x06   			//硬币器使能命令
#define CMD_NOTE_STACK       0x07				//压钞命令
#define CMD_NOTE_RETURN      0x08				//退钞命令

#define COIN_DEV_OK           0					//硬币器状态 正常
#define COIN_DEV_ER           (1 << 0)			//硬币器状态 故障
#define COIN_DEV_DIS          (1 << 1)			//硬币器状态 禁能
#define COIN_DEV_PERR         (1 << 2)			//硬币器状态 参数错
#define COIN_DEV_NONE         (1 << 3)			//硬币器状态 无

#define NOTE_DEV_OK           0					//纸币器状态 正常
#define NOTE_DEV_ER           (1 << 0)			//纸币器状态 故障
#define NOTE_DEV_DIS          (1 << 1)			//纸币器状态 禁能
#define NOTE_DEV_PERR         (1 << 2)			//纸币器状态 参数错
#define NOTE_DEV_NONE         (1 << 3)			//纸币器状态 无

#define CHANGER_OK            0					//找零器状态 正常
#define CHANGER_ER            (1 << 0)			//找零器状态 故障
//#define CHANGER_DIS           (1 << 1)				//找零器状态 禁能
#define CHANGER_LOW           (1 << 2)			//找零器状态 缺币
#define CHANGER_NONE          (1 << 3)			//找零器状态 无

#define HR_NOTE_EN	          NOTE_GBA			//使能GBA纸币器
#define MDB_NOTE_EN	          NOTE_MEI			//使能MDB纸币器
#define ITL_NOTE_EN	          NOTE_ITL			//使能ITL纸币器


#define PULSE_COIN_EN 	      1					//使能脉冲硬币器
#define MDB_COIN_EN 	      2					//使能MDB硬币器
#define HOPPER_CHANGER_EN 	  1					//使能HOPPER找零器
#define MDB_CHANGER_EN 	      2					//使能MDB找零器

#define MDB_COINNUMMIN        10			    //MDB找零器	最少硬币数设置

//货道状态
#define CHANNELDISABLE		  0	                //货道禁能
#define CHANNELOK			  1	                //货道正常
#define CHANNELHARDERR		  2	                //货道出错
#define CHANNELOVER			  3					//货道缺货
//#define CHANNELCOMMERR		4

#define  OVER_VALUE_INDEX	   0x01	            //面值太大
#define  RETURN_MONEY_INDEX	   0x02	            //退币

#define  COIN_TEST_BIT         0x01	            //维护状态下，硬币投币测试使能位
#define  NOTE_TEST_BIT         0x02	            //维护状态下，纸币投币测试使能位
					 			
//#define GetChannelNo(x,y)    (x) * MAXCHANNEL + (y) + 1;	
//#define GetChannelNo(x,y)    (MAXLAYER - (x) - 1) * MAXCHANNEL + (MAXCHANNEL - (y) - 1) + 1;
//#define GetChannelNo(x,y)      (MAXLAYER - (x) - 1) * MAXCHANNEL + (y) + 1;	 //获取实际货道号
#define GetChannelNo8(x,y)      (MAXLAYER - (x) - 1) * 8 + (y) + 1;	 //获取8货道实际货道号
#define GetChannelNo10(x,y)      (MAXLAYER - (x) - 1) * 10 + (y) + 1;	 //获取10货道实际货道号


#define SetHaveSaleFlag()  	   g_pvmcFlag.HaveSale = 1			//已进行过交易设置
#define RstHaveSaleFlag()  	   g_pvmcFlag.HaveSale = 0			//复位已进行过交易标志
#define IsHaveSaled()      	   (g_pvmcFlag.HaveSale == 1)			//判断是否已进行过交易

#define SetDoubtTrans()    	   g_pvmcFlag.DoubtTrans = 1			//疑问交易设置
#define RstDoubtTrans()        g_pvmcFlag.DoubtTrans = 0			//复位疑问交易设置
#define IsDoubtTrans()         (g_pvmcFlag.DoubtTrans == 1)		//判断是否是疑问交易

#define EnableMoneyIn()        g_pvmcFlag.MoneyInEN = 1			//使能投币器设置
#define DisableMoneyIn()       g_pvmcFlag.MoneyInEN = 0			//禁能投币器设置
#define IsMoneyInEN()          (g_pvmcFlag.MoneyInEN == 1)		//判断是否是使能投币器状态

#define EnterMaintainStat()    g_pvmcFlag.Maintain = 1			//维护状态
#define ReturnSaleStat()   	   g_pvmcFlag.Maintain = 0			//交易状态
#define SetReInitStat()        g_pvmcFlag.Maintain = 0xFF			//从维护状态退回交易状态
#define BeSaleStat()          (g_pvmcFlag.Maintain == 0)			//判断是否是交易状态
#define IsMaintainStat()      (g_pvmcFlag.Maintain == 1)			//判断是否是维护状态
#define IsReInitStat()        (g_pvmcFlag.Maintain == 0xFF)		//判断是否是从维护状态退回交易状态

#define SetDevStatus(bit)      g_pvmcFlag.DevStatus |= (bit)		//设置设备故障为
#define ClrDevStatus(bit)      g_pvmcFlag.DevStatus &= ~(bit)		//清楚设备故障位
#define GetDevStatus()         g_pvmcFlag.DevStatus				//获取设备故障状态
#define RstDevStatus()         g_pvmcFlag.DevStatus = DEVICE_OK	//复位设备故障状态
#define IsDeviceOK()           (g_pvmcFlag.DevStatus == DEVICE_OK)//判断设备故障状态

#define NoteEscrowSet()        g_pvmcFlag.BillEscrowed = 1	    //有纸币待压钞
#define NoteEscrowHold()       g_pvmcFlag.BillEscrowed = 2	    //纸币暂存状态
#define NoteEscrowClr()        g_pvmcFlag.BillEscrowed = 0		//清除纸币暂存状态
#define IsEscrowSta()          (g_pvmcFlag.BillEscrowed == 1)		//判断是否有待压钞的纸币
#define IsEscrowHold()         (g_pvmcFlag.BillEscrowed == 2)		//判断是否处于暂存状态

#define SetCoinTest()          g_pvmcFlag.TouBiTest |= COIN_TEST_BIT	  //开始测试硬币器
#define SetNoteTest()          g_pvmcFlag.TouBiTest |= NOTE_TEST_BIT	  //开始测试纸币器
#define RstTouBiTest()         g_pvmcFlag.TouBiTest = 0				  //复位投币测试标志
#define RstCoinTest()          g_pvmcFlag.TouBiTest &= ~COIN_TEST_BIT	  //结束测试硬币器
#define RstNoteTest()          g_pvmcFlag.TouBiTest &= ~NOTE_TEST_BIT	  //结束测试纸币器
#define IsCoinTest()           (g_pvmcFlag.TouBiTest & COIN_TEST_BIT)	  //正在测试硬币器
#define IsNoteTest()           (g_pvmcFlag.TouBiTest & NOTE_TEST_BIT)	  //正在测试纸币器

/*用于交易选货*/
#define RstInputNum()          g_KeyStat.InputNum = 0; g_KeyStat.InputCnt = 0  //InputNum是指按键输入的数字值，重置复位数字按键
#define CalcInputNum(key)      g_KeyStat.InputNum * 10 + key		           //计算输入值
#define RstInputMode()	       g_KeyStat.InputMode = 0			           //禁止输入数字按键
#define SetInputMode()	       g_KeyStat.InputMode = 1			           //允许输入数字按键

//添加品途屏幕的兼容;by gzz 20121025
#define IsLargeLCD() 		 ( (SYSPara.LCDSel==1)||(SYSPara.LCDSel==2) )

/*------------------------- GLOBAL CONSTANTS ------------------------ */
//	#define TRUE            1       /* boolean TRUE value */
//	#define FALSE           0       /* boolean FALSE value */
//	#define BUFFERSIZE      240      /* size of character input buffer */
//	#define FIXED           0       /* fixed tune */
//	#define boolean         int     /* make a boolean data type */
//	#define ESC             27      /* ESCAPE character */
//	#define OK              1       /* successful function execution */
//	#define ERROR           0       /* unsuccessful function execution */
//	#define SPACE           0x20    /* hex representation for a space */
//	#define LF              0x0A    /* Line Feed */
//	#define CR              0x0D    /* carriage return*/
//	#define BS              0x08    // back space
//	#define EOB             0x07    // end of block (bell)
																									   
typedef struct 
{
//uint8_t		ChannelCode;			   //货道编码
uint16_t	GoodsCode;	   					//商品编码
uint32_t	GoodsPrice;	   					//商品价格
uint8_t		ChannelStat;	 				//货道状态;与货道设置一样，0未测试，1正常，2故障，3缺货	
/*
//货道状态
bit7用作货道配置开关 置位为开 清零为关
#define CHANNELDISABLE		  0	                //货道禁能
#define CHANNELOK			  1	                //货道正常
#define CHANNELHARDERR		  2	                //货道出错
#define CHANNELOVER			  3					//货道缺货
//#define CHANNELCOMMERR		4
*/     
uint8_t		GoodsNum;		 				//商品数量
uint32_t	SuccessNum;					 	//单货道成功交易次数
uint32_t	DoubtNum;					 	//单货道疑问交易次数
uint8_t		TestNum;						//测试故障次数

//用于季度结账用;by gzz 20121009
uint32_t	SuccessNumJidu;					 	//单货道成功交易次数


}GoodsChannel ;		 					//货道参数	total 16 word

typedef struct	
{
	uint8_t		ColMaxGoods[MAXLAYER][MAXCHANNEL];	//每个货道最大填货数
}ColSystemPara;//按货道添存货量;by gzz 20121120

typedef struct	
{
	uint8_t		visible[245];	//每个菜单可见
}KeySystemVisible;//配置用户菜单可见;by gzz 20121204

typedef struct	
{
uint16_t	SoftwareVersion;				//软件版本
uint16_t	HardwareVersion;				//硬件版本	
uint32_t	Password;

uint8_t     Language;						//语言选择

uint32_t	ChangeValue[HOPPERCHMAX];	//找零器面值
uint32_t	MoneyMaxin;		   			//最大投币值
uint32_t	MoneyMinLeft;	   			//最低余额
char		SerialNo[10];
char		GSMNO[16];
uint32_t    CoinsValue[COINCHMAX];	  	//硬币面值
uint32_t    BillValue[BILLCHMAX];	 	//纸币面值


uint8_t		LayEnable[8];				//层道启用开关
uint8_t		LayMaxGoods[8];				//每层最大填货数





uint8_t		ChangeServer;		   		//无零币是否服务
uint8_t		GPRSOnOff;
uint8_t     TransMultiMode;				//允许多次交易
uint8_t     TransEscape;				//允许不卖货退币

uint8_t		POSEnable;					//POS机开启
uint8_t     PCEnable;		   			//使能PC机
uint8_t     LCDSel;                     //使用大屏
uint8_t     KeySel;                     //使用选货按键//选货按键;by gzz 20121025
uint8_t     LogoSel;                    //拼图屏幕样式;by gzz 20121106
uint8_t     GBAEnable;		   			//使能纸币器
uint8_t     CoinEnable;		   			//使能硬币器

uint8_t     ChangerEnable;		   		//使能找零器
uint8_t		Decimal;		   			//小数位数
uint8_t     GOCCtr;			   			//出货确认

uint8_t     GOCErrorSevice;			   	//121108 by cq GOCErrorSevice

uint8_t     NoteEscrow;					//暂存状态开关
uint8_t     NoteEscrowType[BILLCHMAX];	//纸币暂存状态

uint32_t	Password2;//120729 by cq RecordPassword /**/
uint32_t	Password3;//用于季度结账用;by gzz 20121009

uint8_t     ReaderEnable;				//120914 by cq Cashless
uint8_t     ReaderCost;					//120926 by cq Cashless

}SystemPara;							//系统参数

//添加压缩机控制;by gzz 20121224
typedef struct	
{
uint8_t DeviceOn;
uint8_t HourOn;
uint8_t	MinOn;
uint8_t HourOff;
uint8_t	MinOff;
}ACDC_TIME;

typedef struct	
{
uint8_t 	BillEnable; 				//纸币使能位//可以设置不收纸币;by gzz 20121219
ACDC_TIME	CompressorTime;		  		//压缩机开关时间
ACDC_TIME	LEDTime;		  			//展示灯开关时间
uint8_t     AutoColumn;					//不用手工补货功能;by gzz 20130122
uint8_t     TenColumn;					//是否使用十货道的机器;by gzz 20130226
}SystemPara2;							//系统参数//添加新的系统参数;by gzz 20121219


/*状态值为相应设备的状态值，详见各设备手册*/
typedef struct	
{
uint16_t    NoteStat;			   			//纸币器状态
uint16_t	CoinsStat;		   				//硬币器状态
uint16_t    HopperStat[HOPPERCHMAX]; 		//找零器状态
uint8_t	    CHTestNO;						//货道测试次数
uint8_t     GOCStat;			   			//出货确认模块状态
uint8_t     FlashStat;					    //Flash状态
uint8_t     RTCStat;					    //RTC状态
uint8_t     AdminStat;					    //管理员模式状态//用户模式菜单;by gzz 20121130
uint8_t     VisibleStat;				    //菜单可视状态//配置用户菜单可见;by gzz 20121204
}SystemPara1;							    //系统参数1	

typedef struct	
{
	 uint8_t	IncomeCH[MAXINCOME];		  	//收币通道记录
	 uint8_t	Goods[MAXLAYER][MAXCHANNEL]; 	//出货数量
	 uint32_t	Money;							//交易金额
	 uint8_t	OutcomeCH[MAXOUTCOME];		  	//找零记录
	 RTC_TIME_Type	Time;		  				//交易时间
	 uint16_t	Check;							//
} Transaction;

typedef struct	
{
	uint16_t	IncomeCH[MAXINCOME];		  	//收币通道记录//最大收币通道数32
	uint16_t	Goods[MAXLAYER][MAXCHANNEL]; 	//出货数量	   64
	uint32_t	Money;							//总金额
	uint16_t	OutcomeCH[MAXOUTCOME];		  	//找零记录 32
	RTC_TIME_Type	Time;		  				//交易时间	  
	 
	uint16_t	TranPages;	  					//交易记录保存页面号	
	uint32_t	Income;		  					//总收币值
	uint32_t	NoteIncome;	  					//纸币总收币值
	uint32_t	CoinsIncome;  					//硬币总收币值
	uint32_t	TotalTrans;	  					//交易总金额
	uint32_t	TotalChange;  					//找零总金额
	uint32_t	CoinsChange[HOPPERCHMAX];	 	//找零器出币数
	uint32_t	SuccessNum;	  					//成功交易总次数
	uint32_t	DoubtNum;	  					//疑问交易总次数
	uint32_t		Iou;	  					//欠费总额
	uint32_t	DoubtMoney;	  					//疑问金额
	//用于季度结账用;by gzz 20121009
	uint32_t	IncomeJidu;		  					//总收币值
	uint32_t	NoteIncomeJidu;	  					//纸币总收币值
	uint32_t	CoinsIncomeJidu;  					//硬币总收币值
	uint32_t	TotalTransJidu;	  					//交易总金额
	uint32_t	TotalChangeJidu;  					//找零总金额
	uint32_t	CoinsChangeJidu[HOPPERCHMAX];	 	//找零器出币数

	uint16_t	Check;
}TransactionTotal;	   						//存储必须小于256长度

typedef struct
{
	/*Bit is set to indicate a coin type is enable or full*/
	uint16_t CoinTypeEN,BillTypeEN;		    //CoinTypeEN,BillTypeEN：设置的使能位
	uint16_t BillType,CoinType;				//BillType,CoinType：当前的使能位
	uint16_t CoinFullBit;					//CoinFullBit：MDB硬币器各通道满标志位
} CashTypeEnBitStruct;						//现金设备通道使能位

typedef struct
{
	uint8_t  CHTODRV[MAXLAYER][MAXCHANNEL];	//货道驱动位映射参数
    uint8_t	 DRVTOCH[MAXLAYER][MAXCHANNEL];	//驱动货道位映射参数
    uint8_t	 LAYTODRV[MAXLAYER];			//层驱动位映射
    uint8_t	 DRVTOLAY[MAXLAYER];			//驱动层位映射
} ChannelMapPara;							/*货道实际编号与输入编号的映射关系参数*/

typedef struct
{
	/*硬币器纸币器找零器正常或故障状态*/		
    uint8_t   CoinState;
	uint8_t   NoteState;
	uint8_t   ChangerState;	 //
} CashStatStruct;							//设备状态

typedef struct
{
    uint32_t  CmdType,pBackMsg;				//命令类型，返回值
	uint32_t   keyback;						//按键消息返回值//120622 by cq NewLayer
    uint32_t  moneyback;					//投币消息返回值
    uint32_t  coinPlusback;					//投币消息返回值
    uint32_t  CmdPCType;       				//PC命令类型，TASKC发命令给TASKB
    uint32_t  CmdPCCMDMSG;       			//PC命令类型，TASKB发命令给TASKC
    uint32_t  CmdPCCMDMSGBack;    			//PC命令类型，TASKC返回值TASKB    

	//120914 by cq Cashless
	uint32_t msgRTV;
	uint32_t msgVTR;
} MessageBack;								//命令返回值

typedef struct
{
	/*定时TimeDly*10ms*/
	uint16_t  TimeDly;					   //等待出货/出币结束
	uint16_t  OperateDly;				   //等待客户操作
	uint16_t  CheckDly;  	     		   //定时检测设备状态
} TimerDelay;							   //定时

typedef struct  
{
    uint8_t   Channel;						//找零器通道
	uint32_t  Value;						//找零器通道对应面值
}  ChangerPara;								//用于hopper按面值大小排序

/*定义交易选货时按键的状态，用于程序中判断交易状态跳转*/
typedef struct  
{
    uint8_t   InputMode:4;					//按键输入模式，允许输入数字（InputMode = 1）和禁止输入数字模式
	uint8_t   InputCnt:4;					//按键输入位数，两位或一位
	uint8_t   InputNum;						//按键输入值 
}  KEYSTAT;									//用于选货时按键输入

/*定义程序中用到的各种标志，一同放到该结构体中*/
typedef struct  
{   
	uint8_t HaveSale:1;						//是否已进行过交易标志
	uint8_t DoubtTrans:1;					//疑问交易标志
	uint8_t MoneyInEN:1;					//投币器状态标志
	uint8_t TouBiTest:3;					//投币测试标志
	uint8_t BillEscrowed:2;					//纸币代管状态标志
	uint8_t Maintain;						//维护状态标志
	uint16_t DevStatus;						//设备故障状态
}  PVMCFLAG;

//---------------------------------------------
//XMT温控;by gzz 20110105
#define XMT_CTR_DEVADDR     0x01//设备地址
#define XMT_CTR_ADDR        (0x80+XMT_CTR_DEVADDR)//设备头
#define XMT_CTR_READTEMP    0x52//读操作标记
#define XMT_CTR_SETTEMP     0x43//写操作标记
#define XMT_CTR_CMDSETTMP   0x00//操作参数代号:读写温度
#define XMT_CTR_CMDSETHY    0x05//操作参数代号:读写温度控制偏移精度
#define XMT_CTR_GETTEMPOPT  1//读温度
#define XMT_CTR_SETTEMPOPT  0//设定温度
#define XMT_GET_TEMPTIME    1000
#define XMT_TIME_GETTEMPERATURE 10//检测温度





typedef struct 
{ 
    uint8_t Addr;//设备地址
    uint8_t cmd;//读操作标记:52H，写操作:43H
    uint8_t cmdSet;//操作参数代号
    uint8_t cmdPar[2];//设定的值
    uint8_t crcEnd;//crc校验值
    uint8_t recPV[2];
    uint8_t recSV[2];
	uint8_t recHY[2];
    uint16_t  recPVTemp;//返回现在室温
	uint16_t  recSVTemp;//返回设定的温度
	uint16_t  recHYTemp;//返回设定的温度控制偏移精度
	uint16_t  temperaTimer;//返回设定的温度
	uint8_t recBuf[30];	
	uint8_t rectemp;
	uint8_t GetTempeTime;//检测温度
}	XMT_Mission;
//=============================================

 

#ifdef	MAINDEF
#define MAIN_EXTERN 
#else
#define MAIN_EXTERN    extern
#endif

MAIN_EXTERN		GoodsChannel		Channels[MAXLAYER][MAXCHANNEL];	//货道参数组
MAIN_EXTERN		uint16_t			IncomeValue[MAXINCOME];			//投币通道对应币值
MAIN_EXTERN		uint16_t			OutcomeValue[MAXOUTCOME];		//出币通道对应币值
MAIN_EXTERN   	SystemPara			SYSPara;						//系统参数组
MAIN_EXTERN   	SystemPara1			SYSPara1;						//系统参数组1
MAIN_EXTERN   	SystemPara2			SYSPara2;						//系统参数组2//添加新的系统参数;by gzz 20121219
MAIN_EXTERN  	Transaction			TranData;						//交易数据记录
MAIN_EXTERN  	TransactionTotal	TranTotal;						//总交易记录
MAIN_EXTERN  	XMT_Mission         sysXMTMission;                  //温度控制
MAIN_EXTERN  	ColSystemPara		colSystemPara;					//按货道添存货量;by gzz 20121120
MAIN_EXTERN  	KeySystemVisible	keyVisible;					    //配置用户菜单可见;by gzz 20121204




/*最小面值的找零通道*/
MAIN_EXTERN  uint8_t g_ChangeMinCh,g_EscrowMinCh;
/*当前纸币数*/				     								
MAIN_EXTERN  uint16_t g_BillNum;				        
/*当前各通道储币数 最小硬币面值*/
MAIN_EXTERN  uint16_t g_CoinTypeNum[COINCHMAX],g_CoinMinValue;		 
/*投币设备使能位结构体定义*/
MAIN_EXTERN  CashTypeEnBitStruct CashTypeEnBit;
/*用于几个hopper面值大小排序*/
MAIN_EXTERN  ChangerPara HopperPara[HOPPERCHMAX];
/*选货时按键状态*/
MAIN_EXTERN  KEYSTAT	    g_KeyStat;
/*程序中用到的各种标志*/
MAIN_EXTERN  PVMCFLAG		g_pvmcFlag;
/*货道实际编号与输入编号的映射关系参数*/
MAIN_EXTERN  ChannelMapPara		ChnMapPara;
/*硬币器纸币器找零器正常或故障状态*/		
MAIN_EXTERN  CashStatStruct CashState;	  //
/*消息命令返回值*/
MAIN_EXTERN  MessageBack  MsgBack;
/*当前投币总额 当前剩余零币量,当前剩余5角(即#1通道)的硬币值,,当前剩余1元(即#2通道)的硬币数量,设备管理中显示当前剩余真实零币金额,*/
MAIN_EXTERN  uint32_t     g_Amount,g_CoinLeft,g_coin1Num,g_coin2Num,g_CoinRealLeft;
/*定时变量结构体*/
MAIN_EXTERN  TimerDelay   TimerDly;


//120621 by cq KeySlowly
/*按键队列大小*/
#define 	    KEYQUEUE       2 
#define 	    COINPLUSQUEUE       20 
MAIN_EXTERN  	void        *KeyMsgArray[KEYQUEUE]; // 按键队列数组
MAIN_EXTERN  	void        *CoinPlusArray[COINPLUSQUEUE]; // 脉冲硬币队列数组


MAIN_EXTERN 	OS_EVENT	*g_InitSem;		// 信号量定义,用以通知系统初始化完成
MAIN_EXTERN 	OS_EVENT	*g_PCSem;		// 信号量定义,用以通知系统PC端初始化完成
MAIN_EXTERN  	OS_EVENT	*g_CommBackMsg;	// 邮箱信号量定义,用以通知系统出货完成后，它的返回命令(返回所有的出货结果，不一定是成功)。找零也一样,以及其它的命令返回都可以
MAIN_EXTERN 	OS_EVENT	*g_GPRSMsg;		// 信号量定义,用以通知GPRS有命令到达
MAIN_EXTERN    	OS_EVENT 	*g_CommMsg;		// 邮箱信号量定义,用以通知系统有出货命令发送,或找零命令,以及其它的命令发送都可以
MAIN_EXTERN 	OS_EVENT 	*g_KeyMsg;		// 队列信号量定义,用以通知系统有按键命令
MAIN_EXTERN 	OS_EVENT 	*g_MoneyMsg;	// 邮箱信号量定义,用以通知系统有投币命令发送//120621 by cq KeySlowly
MAIN_EXTERN 	OS_EVENT 	*g_CoinPlusMsg;	// 队列信号量定义,用以通知系统有脉冲硬币投币命令发送
MAIN_EXTERN 	OS_EVENT 	*g_PCCMDMsg;	// 邮箱信号量定义,TASKC在轮询时，发现有PC命令发送，就发送给TASKB处理
MAIN_EXTERN    	OS_EVENT 	*g_PCCommMsg;	// 邮箱信号量定义,TASKB处理pc任务时，发送给TASKC要求处理
MAIN_EXTERN  	OS_EVENT	*g_PCCommBackMsg;// 邮箱信号量定义,TASKC处理完毕之后，返回TASKB
//120914 by cq Cashless
MAIN_EXTERN		OS_EVENT	*mbVmcToCashless;// 邮箱信号量定义,vmc给读卡器下发的指令
MAIN_EXTERN		OS_EVENT	*mbCashlessToVmc;// 邮箱信号量定义,读卡器给vmc返回的状态

/*
例如:出货邮箱流程如下
TASKB中
DoChuhuoMsg()：
//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
MsgBack.CmdType = ch + ((uint32_t)CMD_GOODS_OUT << 24); 
OSMboxPost(g_CommMsg,&MsgBack.CmdType);//通知系统有出货命令发送

TASKC中：
PCmdMsg = OSMboxPend(g_CommMsg,OS_TICKS_PER_SEC / 10,&err);//接收邮箱信息，用来得到本次是要操作什么设备

TASKC中
DrvOutCommand()：
MsgBack.pBackMsg = pStatus;					 
//OSMboxPost(g_CommBackMsg,(uint32_t *)&pStatus);
OSMboxPost(g_CommBackMsg,&MsgBack.pBackMsg);


TASKB中
DoChuhuoMsg()：
//货道编码（32位）：最高位置1代表EVB命令，最高字节为命令编码，其他为通道号
OSMboxAccept(g_CommBackMsg);
pComStatus = OSMboxPend(g_CommBackMsg,OS_TICKS_PER_SEC * 20,&err);//通知系统出货完成后，它的返回命令(返回所有的出货结果，不一定是成功)

*/




#ifdef __cplusplus
    }
#endif



#endif
