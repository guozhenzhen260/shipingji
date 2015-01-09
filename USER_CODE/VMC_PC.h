#ifndef __VMC_PC_H__
#define __VMC_PC_H__


#define VP_TIME_OUT        10
#define VP_TIME_POLL       50
#define VP_TIME_OUNT_NUM   10
#define VP_TIME_WAIT_GAME  10
#define VP_COM_RETRY       2  

#define VP_ERR_NULL     0
#define VP_ERR_PAR      1   //mission parameter error
#define VP_ERR_COM      2   //communication error

#define VP_SF             0xE7
#define VP_PROTOCOL_VER   0x01//0x01---0x02版本号设为2;by gzz 20110823
#define VP_PROTOCOL_ACK   0x80
#define VP_PROTOCOL_NAK   0x00

#define VP_REQUEST_G_C    0x01
#define VP_DEV_COIN       0
#define VP_DEV_BILL       1
#define VP_DEV_BUSCARD    2
#define VP_DEV_BANKCARD   3
#define VP_DEV_MOBILE     4
#define VP_DEV_SCHOOL          5
#define VP_DEV_CTR_LED1        5
#define VP_DEV_CTR_COMPRESSOR1 7
#define VP_DEV_GAME_LED        16
#define VP_DEV_RTC             17
#define VP_DEV_ESCROWIN        100
#define VP_DEV_ESCROWOUT       101

#define VP_ADMIN_GOODSADDALL  1
#define VP_ADMIN_GOODSADDCOL  2
#define VP_ADMIN_CHANGEADD    3
#define VP_ADMIN_GOODSCOLUMN  4
#define VP_ADMIN_ENTER_MENU   5
#define VP_ADMIN_QUIT_MENU    6
#define VP_ADMIN_COL_PRICE    7
#define VP_ADMIN_GOODSADDTRAY 8
#define VP_ADMIN_GETBILL        9
#define VP_ADMIN_GOODSADDSLAVE  10

#define VP_BUT_GAME           0
#define VP_BUT_NUMBER         0
#define VP_BUT_GOODS          2
#define VP_STA_CHANGE_ENOUGH  20


//VMC-->PC
#define VP_TEXT_MSG      0
#define VP_ACK_RPT       1
#define VP_NAK_RPT       2
#define VP_POLL          3
#define VP_STARTUP_RPT   4
#define VP_VMC_SETUP     5
#define VP_PAYIN_RPT     6
#define VP_PAYOUT_RPT    7
#define VP_VENDOUT_RPT   8
#define VP_REQUEST       9
#define VP_ADMIN_RPT     10
#define VP_ACTION_RPT    11   //V2
#define VP_BUTTON_RPT    12
#define VP_STATUS_RPT    13
#define VP_COLUMNSTA_RPT 14
#define VP_CARD_RPT      15
#define VP_COST_RPT      16//扣款后，返回扣款执行结果;by gzz 20110823
#define VP_DEBT_RPT      17//返回欠条结果;by gzz 20110825
#define VP_VENDOUT_REQ   18

#define VP_MT_MIN_SEND  VP_TEXT_MSG
#define VP_MT_MAX_SEND  VP_VENDOUT_REQ

//PC-->VMC
#define VP_ACK             128
#define VP_NAK             129
#define VP_INITIAL_OK      130
#define VP_VENDOUT_IND     131
#define VP_RESET_IND       132
#define VP_CONTROL_IND     133
#define VP_GET_STATUS      134
#define VP_HOUDAO_IND      135
#define VP_POSITION_IND    136
#define VP_PAYOUT_IND      137    //V2
#define VP_GET_COLUMNSTA   138
#define VP_COST_IND        139//执行扣款命令;by gzz 20110823
#define VP_MT_MIN_RECEIVE  VP_ACK
#define VP_MT_MAX_RECEIVE  VP_COST_IND


extern void VPSerialInit( void );
extern unsigned char VPBusTxMsg( void );
extern unsigned char VPBusFrameUnPack( void );
extern unsigned char VPMsgPackSend( unsigned char msgType, unsigned char flag, unsigned char snCtr );

extern unsigned char VP_CMD_ColumnPar( void );
extern unsigned char VP_CMD_GoodsPar( void );
extern unsigned char VP_CMD_Init_OK( void );
extern unsigned char VP_CMD_Vendout( void );
extern unsigned char VP_CMD_Reset( void );
extern unsigned char VP_CMD_Control( void );
extern unsigned char VP_CMD_GetStatus( void );
extern unsigned char VP_CMD_Payout( void );
extern unsigned char VP_CMD_GetColumnSta( void );

extern unsigned char UpdateSelLed_Trade( void );
extern unsigned char UpdateSelectionLed_GoodsSta( void );
extern unsigned char UpdateGoodsMatrixStatus( unsigned char goodsNum );
extern unsigned char VP_Update_ColumnGoodsPar( void );

extern unsigned char VPMission_Startup_RPT( void );
extern unsigned char VPMission_Setup_RPT( void );
extern unsigned char VPMission_Payin_RPT( unsigned char dev, unsigned int money );
extern unsigned char VPMission_Payout_RPT( unsigned char dev, unsigned int money1, unsigned int money2 );
extern unsigned char VPMission_Cost_RPT( unsigned char dev, unsigned int money );//发送pc扣款后金额;by gzz 20110823
extern unsigned char VPMission_Debt_RPT( unsigned char type, unsigned char dev, unsigned int money );//当上报欠款时，上报pc端;by gzz 20110825
extern unsigned char VPMission_Vendout_RPT( unsigned char status, unsigned char column, unsigned char type, unsigned int cost );
extern unsigned char VPMission_Vendout_REQ( unsigned char column, unsigned int cost );
extern unsigned char VPMission_Request( unsigned char type );
//extern unsigned char VPMission_Admin_RPT( unsigned char type, unsigned char column, unsigned char num );
extern unsigned char VPMission_Admin_RPT( unsigned char type, unsigned char column, unsigned int num );
extern unsigned char VPMission_Action_RPT( unsigned char type, unsigned int value, unsigned char len, unsigned char *dat );
extern unsigned char VPMission_Button_RPT( unsigned char type, unsigned char value );
extern unsigned char VPMission_Card_RPT( void );
extern unsigned char VPMission_Status_RPT( void );
extern unsigned char VPMission_ColumnSta_RPT( void );

extern unsigned char VPMission_Poll( void );
extern unsigned char VPMission_Init( void );

extern unsigned char VPAddSingleColGoods( unsigned char col, unsigned char num, unsigned char oldNum, unsigned char Flag );
extern unsigned char VPChangeColPrice( unsigned char col, unsigned int price, unsigned int oldPrice );
extern unsigned char VPAddAllColGoods( void );
extern unsigned char VPAddLayerColGoods( void );//by gzz 20110429
extern unsigned char VPAddChanges( unsigned char testDev );//by gzz 20110506
extern unsigned char VPSynGoodsCol( void );
extern unsigned char VPCountCoin( unsigned char hopperNum );


//--------------------------------------------------------------
//20_140&40_140&60_140
#define SYS_WAIT_NEXT_PULSE 200   //
#define SYS_WAIT_CARD_IN    1500
#define MACHINE_SET_VMC_PC  
#define VP_PRO_VER          1//1--2版本号设为2;by gzz 20110823
#define VP_MAC_SET_H        1
#define VP_MAC_SET_L        1
#define KEYEXTRAVAL         0x06
#define GOODSTYPEMAX        18
#define EACHGOODSCOLUMNMAX  48
#define COLUMN_NUM_SET      48
#define GOODSMATRIXMODE
#define GOODS_MATRIX_NONE   0xff
#define SELECTION_VAL_MIN   0
#define SELECTION_VAL_MAX   17
#define DSP_WELCOME_UPDATE  8

#define VP_DSP_INTERVERL    250                 //200-250
#define VP_DSP_TIME1        VP_DSP_INTERVERL
#define VP_DSP_TIME2        VP_DSP_INTERVERL*2
#define VP_DSP_TIME3        VP_DSP_INTERVERL*4
#define VP_DSP_TIME4        VP_DSP_INTERVERL*8

//#define SELECTION_NUM     3
/*
#define INPUTGOODSSUM_2     2
#define INPUTGOODSSUM_3     3
#define INPUTGOODSSUM       INPUTGOODSSUM_3
*/
//The system goods matrix_33 bytes
struct GoodsMatrix
{
	uint8_t   GoodsType;
	uint16_t    Price;
	uint8_t   Status;       //00-ok, !=0 error,
	uint8_t   Led;          //
	uint8_t   ColumnNum;
	uint8_t   ColumnMatrix[EACHGOODSCOLUMNMAX];   //Just the GoodsWaySetVal's address.                
	uint8_t   NextColumn;                         //FF is none good column left.
};
extern struct GoodsMatrix sysGoodsMatrix[GOODSTYPEMAX];



#define VPM_LEN_MAX   250
struct VMC_PC_MSG
{
    uint8_t   sf;                //fixed with 0xe7
	uint8_t   len;               //5~250, msg+5
    uint8_t   datLen;            //
	uint8_t   verFlag;           //0x01( none response ); 0x81( need response )
    uint8_t   sn;                //0~255
	uint8_t   msgType;           //
	uint8_t   msg[VPM_LEN_MAX];  //0~250
	uint16_t    chk;
};

#define VPM_COM_TIMEOUT   1500   //1.5s
#define VPM_STA_PWRON     0x00
#define VPM_STA_FIRSTCOM  0x01
#define VPM_STA_GOODSPAR  0x02
#define VPM_STA_COLUMNPAR 0x04
#define VPM_STA_INITOK    0x08
#define VPM_STA_PCPAROK   0x80

#define VPM_DEVUPDATE_BUSCARD   300
#define VPM_DEVUPDATE_CHANGER1  240  //120-240
#define VPM_DEVUPDATE_CHANGER2  240
#define VPM_DEVUPDATE_CHANGER3  240
#define VPM_DEVUPDATE_GOC       180//20-180   
#define VPM_DEVUPDATE_HOPPEREMP 180
#define VPM_DEVUPDATE_TUBEREMOVE 3//30-3;by gzz 20111017 

struct VP_Mission
{
    struct  VMC_PC_MSG  send;
	struct  VMC_PC_MSG  receive;
    uint8_t  comLock;
	uint16_t   msTimer1;
	uint16_t   msTimer2;
    uint16_t   msTimer3;          //for VP_GameKeyPoll
	uint16_t   msGameTimer1;
    uint16_t   sTimer1;
	uint16_t   sTimer2;
    uint16_t   billSTimer;
	uint16_t   dspTimer2;
    //bit0 - bus card reader
    //bit1 - changer 1
    //bit2 - changer 2
    //bit3 - changer 3
	//bit4 - GOC
	//bit5 -
	//bit6 -
	//bit7 - poll pc first
    uint8_t  VPDevCtr;     //
	uint16_t   sTimerBC;
	uint16_t   sTimerChanger1;
    uint16_t   sTimerChanger2;
    uint16_t   sTimerChanger3;
	uint16_t   sTimerGOC;
	//------------------------------------------------------
	uint16_t   dspTimer1;
	uint8_t  dspUpdateFlag;
	uint16_t   dspErrTimer;
	uint8_t  coinFull;
    uint8_t  changeFlag;
    //bit0 - update balance display, after wrong selection
    //bit1 - update balance display and enable the cash devices
    uint8_t  dspCtr;
	//=======================================================
    uint16_t   systemErr;
	uint8_t  sysStatus;                  //
	uint8_t  PCStatus;
	uint8_t  VPMode;
    uint8_t  VPMode_Key;
    uint16_t   comTimer;
	uint8_t  comErrNum;
    uint8_t  returnKey;                   //Yes or no
	uint8_t  gameKey;                     //Yes or no
	uint8_t  returnKeyLock;               //
	uint8_t  gameKeyLock;                 //
	uint8_t  gameLed;                     //On or off
	uint8_t  key;                         //Selection key
    uint8_t  selItem[GOODSTYPEMAX];       //Selection item
    uint16_t   selPrice[GOODSTYPEMAX];      //Selection price
    uint8_t  columnNum[COLUMN_NUM_SET];   //Column's storage 
	uint8_t  columnItem[COLUMN_NUM_SET];  //Column's item
    uint16_t   readyLed;                    //
	uint16_t   redLed;                      //
	//选货按键;by gzz 20121025
	uint8_t  sel1ReadyLed;               //
	uint8_t  sel1ErrLed;                 //
	uint8_t  sel2ReadyLed;               //
	uint8_t  sel2ErrLed;                 //
	uint8_t  sel3ReadyLed;               //
	uint8_t  sel3ErrLed;                 //
	uint8_t  sel4ReadyLed;               //
	uint8_t  sel4ErrLed;                 //
	uint8_t  sel5ReadyLed;               //
	uint8_t  sel5ErrLed;                 //
	uint8_t  sel6ReadyLed;               //
	uint8_t  sel6ErrLed;                 //
	uint8_t  selectMode;				//选货按键交易时使用跑马灯;by gzz 20121121
    //------------------------------------
    uint8_t  ACDCLedCtr;
    uint8_t  ACDCCompressorCtr;
    //------------------------------------
	uint8_t  proVer;
	uint8_t  columnNumSet;
	uint8_t  selectionNumSet;
	//------------------------------------
	uint8_t  payInDev;
	uint16_t   payInMoney;
	uint8_t  payOutDev;
	uint16_t   payOutMoney1;
	uint16_t   payOutMoney2;
	uint16_t   payOutMoney3;
	uint8_t  payInCoinFlag;
	uint8_t  payInBillFlag;
	uint16_t   payInCoinMoney;
	uint16_t   payInBillMoney;
	uint8_t  escrowInFlag;
	uint8_t  escrowOutFlag;
	uint16_t   escrowMoney;
    //------------------------------------
    uint8_t  changeCmd;
    uint8_t  changeDev;
    uint16_t   changeMoney;
    //------------------------------------
    //接收扣钱命令;by gzz 20110822
    uint8_t  costCmd;
    uint8_t  costDev;
    uint16_t   costMoney;
    //------------------------------------
    //上传欠条数据;by gzz 20110825
    uint8_t  debtType;
    uint8_t  debtDev;
    uint16_t   debtMoney;
	//------------------------------------
	uint8_t  vendGoods;
	uint8_t  vendSta;
	uint8_t  vendColumn;
	uint8_t  vendColumn2;
	uint8_t  vendType;
	uint16_t   vendCost;
    uint8_t  vendCmd;
	uint8_t  goodsType;
	uint8_t  goodsType2;
	//------------------------------------
	uint8_t  vendReqColumn;
	uint16_t vendReqCost;
    //------------------------------------
	uint8_t  request;
	uint8_t  requestFlag;
	//------------------------------------
	uint8_t  ADM_Type;
	uint8_t  ADM_Dat[3];
    //------------------------------------
	uint8_t  ACT_Type;
	uint16_t   ACT_Money;
	uint16_t   ACT_DatLen;
	uint8_t  ACT_Dat[25];
	//-------------------------------------
	uint8_t  BUT_Type;
	uint8_t  BUT_Value;
	//-------------------------------------
	uint8_t  STA_CoinA;
	uint16_t   STA_ChangeNum1;
    uint16_t   STA_ChangeNum2;
	uint8_t  STA_BillA;
    uint8_t  STA_Changer1;
	uint8_t  STA_Changer2;
	uint8_t  STA_VMC;
	uint8_t  STA_Tep1;
	uint8_t  STA_Tep2;
	uint8_t  STA_Bus;
	uint8_t  STA_Bank;
	uint8_t  STA_MobilePOS;
	uint8_t  STA_ICCard;
    uint8_t  E_Tem1;      //For ACDC module 0xfd无意义,0xff故障,0xfe不存在
    uint8_t  E_Tem2;      //For c
    uint16_t  STA_BillCount;//只有连续故障次数大于5次时，才上报;by gzz 20110905
    uint16_t  STA_ICCardCount;//只有连续故障次数大于5次时，才上报;by gzz 20111012
    uint8_t  STA_Ht;
	uint8_t  STA_Oz;
	uint8_t  STA_Rrsw;
	uint8_t  STA_Outdev;
    //-------------------------------------
    uint8_t  resetDev;
    uint8_t  resetCmd;
	//-------------------------------------
	uint8_t  ctrType;
	uint8_t  ctrValue;
    //uint8_t  sTimerHopperEmp;//by gzz 20110502
    //-------------------------------------
	uint8_t  menuTimer;
	uint8_t  menuFlag;
	uint8_t  menuKeyNum;
    uint8_t  menuKey[5];
    //-------------------------------------
    uint8_t  hopperComErr[3];
	uint16_t   hopperTimer;
    uint16_t   hopperCoinNum;
	uint16_t   hopperOutTimer;
    //-------------------------------------
    uint16_t   watchDogTimer;
	uint8_t  watchDogCtr;
	uint8_t  watchDogIng;
    //-------------------------------------
    uint8_t  boughtFlag; 
    //-------------------------------------
    uint8_t  SystemState;//1是非现金交易状态
    uint8_t  hardWareErrShow;//保存硬件故障值，用于显示
    uint8_t GOCTestFlag;//出货检测标记
	uint16_t   sTimertubeRemoved;//检测币管是否锁紧;by gzz 20110827
	//-----------------------------
	
	uint8_t g_Goods5jFlag[COLUMN_NUM_SET];//记录有5角的商品号，如1.5元，2.5元等 111109 by cq 五角货不卖
	uint8_t g_Goods5jRenew;//是否有更新过有5角的商品号
	uint8_t g_Goods5jLeft;//是否只有5角的商品了111109 by cq 五角货不卖
	uint8_t g_Coin5jLittle;//是否现在管子的5角币不足 111109 by cq 五角货不卖

	//--------------------------------
	uint16_t billHoldingValue;//暂存金额
	uint8_t billHoldingFlag;//是否暂存的标志 
	//uint8_t drvOutTimer;//出货超时时间
};
extern struct VP_Mission sysVPMission;
//==============================================================

#endif  //__VMC_PC_H__
