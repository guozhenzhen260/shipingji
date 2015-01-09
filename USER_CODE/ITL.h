#ifndef __ITL_H__
#define __ITL_H__

#define ITL_ERR_NULL     0
#define ITL_ERR_PAR      1   //mission parameter error
#define ITL_ERR_COM      2   //communication error

#define ITL_MSG_STX     0x7F
#define ITL_MSG_SEQ_0   0x00
#define ITL_MSG_SEQ_1   0x80

#define ITL_TIME_POLL     20     //30, 15,  10,  20(2011.4.13), 10, 20
#define ITL_TIME_OUT      100    //50, 100, 150, 1000, 100   
#define ITL_TIME_OUT_NUM  3      //1,3 
#define ITL_TIME_CMD_INTEVEL     100   //800-100
#define ITL_TIME_CMD_INTEVEL_1   20    //200-20
#define ITL_TIME_STACK    15     //3, 8(2011.4.26: wait more time for NV9), 15
#define ITL_TIME_REJECT   3
#define ITL_COM_RETRY     0      //0,1
#define ITL_COM_DELAY     1      //10,1
#define ITL_TIME_RESTART  5    //111227 MDB–≠“ÈITLΩ˚ƒ‹6min to 5s//6 minutes or more time


/*
#define ITL_ACK_OK            0xf0    //+0
#define ITL_ACK_READ          0xef    //+Chn, 
#define ITL_ACK_STACKING      0xcc    //+0
#define ITL_ACK_CREDIT        0xee    //+Chn, valid sign
#define ITL_ACK_STACKED       0xeb    //+0
#define ITL_ACK_REJECTING     0xed    //+0
#define ITL_ACK_REJECTED      0xec    //+0
#define ITL_ACK_SJAM          0xea    //+0
#define ITL_ACK_UJAM          0xe9    //+0
*/

#define ITL_BILL_NULL         0x00
#define ITL_BILL_READ         0x01    //EF+chn(0 or valid chn)
#define ITL_BILL_STACKING     0x02    //CC+NULL
#define ITL_BILL_CREDIT       0x04    //EE+chn...
#define ITL_BILL_STACKED      0x08    //EB+NULL
#define ITL_BILL_REJECTING    0x10    //ED+NULL
#define ITL_BILL_REJECTED     0x20    //EC+NULL
#define ITL_BILL_SJAM         0x40    //
#define ITL_BILL_UJAM         0x80

#define ITL_VALUE_BIT		 (1 << 9) 
#define ITL_COMMERR_BIT		 (1 << 15) //Õ®—∂π ’œ

#define ITL_STAT_OK           0


//sysITLMission.billSta
//ITL_BILL_READ
//ITL_BILL_CREDIT
#define ITL_BILL_VEDNING      0x80

#define ITL_BILL_E_NCFFAR     0x01
#define ITL_BILL_E_NCICBAR    0x02
#define TIL_BILL_E_FATTN      0x04
#define ITL_BILL_E_CBREM      0x08
#define ITL_BILL_E_CBREP      0x10
#define ITL_BILL_E_STAFULL    0x20
#define ITL_BILL_E_DISABLED   0x40
#define ITL_BILL_E_FAIL       0x80

#define ITL_DEV_NULL          0x00
#define ITL_DEV_RESET         0x01
#define ITL_DEV_DISABLED      0x02
//
#define ITL_DEV_NCFFAR        0x04
#define ITL_DEV_NCICBAR       0x08
#define ITL_DEV_CBREM         0x04
#define ITL_DEV_CBREP         0x08
//
#define ITL_DEV_STAFULL       0x10
#define ITL_DEV_FATTN         0x20
#define ITL_DEV_FAIL          0x40


#define ITL_CMD_RESET         0x01
#define ITL_CMD_SETINHIBITS   0x02
#define ITL_CMD_DSPON         0x03
#define ITL_CMD_DSPOFF        0x04
#define ITL_CMD_SETUP         0x05
#define ITL_CMD_HOSTVERSION   0x06
#define ITL_CMD_POLL          0x07
#define ITL_CMD_REJECT        0x08
#define ITL_CMD_DISABLE       0x09
#define ITL_CMD_ENABLE        0x0a
#define ITL_CMD_PROGRAM       0x0b
#define ITL_CMD_GETSN         0x0c
#define ITL_CMD_SLAVEVERSION  0x0d
#define ITL_CMD_CHNVALUE      0x0e
#define ITL_CMD_CHNSEQURITY   0x0f
#define ITL_CMD_CHNRETEACH    0x10
#define ITL_CMD_SYN           0x11
#define ITL_CMD_COINROUTE     0x12
#define ITL_CMD_REJECTCODE    0x17
#define ITL_CMD_HOLD          0x18
#define ITL_CMD_VERSIONEVENT  0x19
//...
#define ITL_CMD_MANUFACTURES  0x30

#define ITL_CMD_MIN  ITL_CMD_RESET
#define ITL_CMD_MAX  ITL_CMD_MANUFACTURES

#define ITL_ACK_STACKING      0xcc    //+0
#define ITL_ACK_BCTA          0xd1
#define ITL_ACK_NCFFAR        0xe1
#define ITL_ACK_NCICBAR       0xe2
#define ITL_ACK_CBREM         0xe3
#define ITL_ACK_CBREP         0xe4
#define ITL_ACK_BCTV          0xe5
#define ITL_ACK_FATTN         0xe6
#define ITL_ACK_STAFULL       0xe7    //+0
#define ITL_ACK_DISABLED      0xe8    //+0

#define ITL_ACK_UJAM          0xe9    //+0
#define ITL_ACK_SJAM          0xea    //+0
#define ITL_ACK_STACKED       0xeb    //+0
#define ITL_ACK_REJECTED      0xec    //+0
#define ITL_ACK_REJECTING     0xed    //+0
#define ITL_ACK_CREDIT        0xee    //+Chn, valid sign
#define ITL_ACK_READ          0xef    //+Chn, 
#define ITL_ACK_OK            0xf0    //+0
#define ITL_ACK_RESET         0xf1
#define ITL_ACK_CMDERR        0xf2
#define ITL_ACK_PARERR        0xf3
#define ITL_ACK_CMDAPL        0xf5
#define ITL_ACK_SOFERR        0xf6
#define ITL_ACK_CHKERR        0xf7
#define ITL_ACK_FAIL          0xf8
#define ITL_ACK_KEYERR        0xfa

#define ITL_ACK_MIN   ITL_ACK_STACKING
#define ITL_ACK_MAX   ITL_ACK_KEYERR 


//ÃÌº”ITLœ‡πÿµƒ…˘√˜;by gzz 20110307
//--------------------------------------------------------------
#define ITL_TIME_DISABLED_INIT   5    //111114 by cq π ’œª÷∏¥35--5 
#define ITL_TIME_JAMED_DISABLE   30    //

struct ITL_MSG
{
	uint8_t stx;
	uint8_t seq;
	uint8_t len;
	uint8_t msg[255];
	uint8_t crcL;
	uint8_t crcH;
};

struct ITL_Mission
{
    struct ITL_MSG send;
	struct ITL_MSG receive;
	uint8_t ITLSet;
	uint8_t crcL;
	uint8_t crcH;
    uint16_t  msPoll;
    uint16_t  msTimer2;
	uint16_t  sTimer1;
    uint16_t  sTimer2;     //holding -- credit
	uint8_t comErrNum;
	uint8_t status;
	uint8_t chnCtr1;
	uint8_t chnCtr2;
    uint8_t billSta;
    uint8_t billESta;
    uint8_t billChn;
	uint16_t  billValue;
    uint8_t enableFlag;
    uint8_t billStaCtr;        //Added 2011.4.13 Andy.
	uint16_t  billHoldingValue;  //Added 2011.4.13 Andy.
	uint8_t billHoldingFlag;   //Added 2011.4.13 Andy.
	//
	uint8_t reEnableFlag;
	uint16_t  reEnableTime;
    //    
	uint8_t reEnableFlag1;//111114 by cq π ’œª÷∏¥
	uint16_t  reEnableTime1;//111114 by cq π ’œª÷∏¥
    //    
	uint8_t reEnableFlag2;//111111 by cq ¬ˆ≥Â”≤±“∆˜
	uint16_t  reEnableTime2;//111111 by cq ¬ˆ≥Â”≤±“∆˜
    //
    uint8_t billJamedFlag;
    uint16_t  billJamedTime;
    //
    uint16_t  restartTime;
};
extern struct ITL_Mission sysITLMission;
//======================================================


extern void ITLUpdateCrc( const unsigned char num );
extern void ITLResetCrc( void );
extern void ITLChnCtr(uint16_t billtype);
extern void ITLSerialInit( void );
extern unsigned char ITLBusUnpack( void );
extern unsigned char ITLBusTxMsg( void );
extern unsigned char ITLMsgPackSend( unsigned char msgType, unsigned char flag );


extern unsigned char ITLMission_Syn( void );
extern unsigned char ITLMission_Setup( void );
extern unsigned char ITLMission_SetInhibit(uint16_t billtype);
extern unsigned char ITLMission_Reset( void );
extern unsigned char ITLMission_Dspon( void );
extern unsigned char ITLMission_Dspoff( void );
extern unsigned char ITLMission_Hold( void );
extern unsigned char ITLMission_Enable(uint16_t billtype);
extern unsigned char ITLMission_Disable( void );
extern unsigned char ITLMission_Reject( void );
extern unsigned char ITLMission_Poll( void );
extern unsigned char ITLMission_Init_1(uint16_t billtype);
extern unsigned char ITLMission_Init(uint16_t billtype);
extern unsigned char ITLMission_HB_Stack( void );
extern unsigned char ITLMission_HB_Reject( void );

extern void ITL_UpDateStackerStat(void);
extern void ITLNote_PackAnalysis(uint8_t *buf,uint8_t len);

extern uint8_t ITL_ErrJudge(uint8_t x);//120331 by cq disable warning





#endif   //__ITL_H__
