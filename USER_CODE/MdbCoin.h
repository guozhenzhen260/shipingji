/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Mdbcoin.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        MDB硬币器驱动程序相关函数声明
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
#ifndef __MDBCOIN_H 
#define __MDBCOIN_H

/*状态码*/
#define COIN_NOCREDIT          0x03	   //不能进入对应通道
#define COIN_SENSORERR         0x04	   //传感器故障
#define COIN_UNPLUGGED         0x06	   //unplugged
#define COIN_TUBEJAM           0x07	   //出币卡币
#define COIN_ROMCHKERR         0x08	   //ROM出错
#define COIN_ROUTINGERR        0x09	   //进币通道出错
#define COIN_JAM               0x0C	   //投币卡币
#define COIN_REMOVAL           0x0D	   //试图移除硬币
/*
#define COIN_NOCREDIT_BIT        (1 << 0)
#define COIN_SENSORERR_BIT       (1 << 1)
#define COIN_UNPLUGGED_BIT       (1 << 2)
#define COIN_TUBEJAM_BIT         (1 << 3)
#define COIN_ROMCHKERR_BIT       (1 << 4)
#define COIN_ROUTINGERR_BIT      (1 << 5)
#define COIN_JAM_BIT             (1 << 6)
*/
#define COIN_VALUE_BIT          0//(1 << 13)	 //通道值对错标志
#define COIN_FULL_BIT			0//(1 << 14)	 //硬币满标志
#define COIN_COMMERR_BIT        (1 << 15)	 //通讯故障

#define COIN_STAT_OK			0

#define COIN_RESET        0x08		 //复位命令
#define COIN_SETUP        0x09		 //获取配置信息
#define TUBE_STATUS       0x0A		 //获取通道硬币数
#define COIN_POLL         0x0B		 //轮询
#define COIN_TYPE         0x0C		 //使能投币通道
#define COIN_DISPENSE     0x0D		 //出币
#define COIN_EXPANSION    0x0F		 //扩展命令
#define COIN_ID           0x00		 //获取ID
#define COIN_FEATURE_EN   0x01		 //使能扩展命令
#define COIN_PAYOUT       0x02		 //出币
#define COIN_PAYOUT_ST    0x03		 //出币状态
#define COIN_PAYOUT_POLL  0x04		 //出币轮询
#define COIN_DIAGNOSTIC   0x05		 //状态诊断

uint8_t MdbCoin_Reset(void);
uint8_t MdbCoin_Setup(void);
uint8_t MdbCoin_GetTubeStatus(void);
uint8_t MdbCoin_Enable(uint16_t CoinsTypeEn);
uint8_t MdbCoin_Disable(void);
uint8_t MdbCoin_Dispense(uint8_t amount, uint8_t type);
uint8_t MdbCoin_Poll(void);
uint8_t MdbCoin_GetID(void);
uint8_t MdbCoin_FeatureEN(void);
uint8_t MdbCoin_PayOut(uint8_t amount);
uint8_t MdbCoin_GetPaySt(void);
uint8_t MdbCoin_PayoutPoll(void);
uint8_t MdbCoin_Diagnostic(void);
void MdbCoin_PackAnalysis(uint8_t *buf, uint8_t len);



//120401 by cq change structor
//** Descriptions:        MDB硬币器应用程序相关函数声明
void MdbCoin_InitProc(void);
void CoinPayoutCmd(uint32_t MoneyPay);
uint16_t MdbCoin_CheckTubeStat(uint32_t *coinleft);
void MdbCoin_PayoutProc(uint32_t MoneyPay);


#endif /* end __COIN_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
