/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           CoinAcceptor.h
** Last modified Date:  2011-10-27
** Last Version:         
** Descriptions:        硬币器驱动程序相关函数声明
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-10-27
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
#ifndef __COINACCEPTOR_H 
#define __COINACCEPTOR_H

#define COINACCEPTOR_STACKED    0x40
#define COINACCEPTOR_RETURN     0x41

typedef struct
{
	uint8_t  Level;			 //硬币器等级
	uint16_t Code;			 //国家代码
	uint8_t  Scale;			 //比例因子
	uint16_t Decimal;		 //10^小数位数
	uint16_t Routing;		 //Bit is set to indicate a coin type can be routed to the tube
} CoinParaStruct;

void CoinAcceptor_Init(void);
void CoinAcceptor_PollProc(void);
void CoinAcceptor_ReadBuf(uint8_t *buf, uint8_t len);
void CoinAcceptor_UpDateDeposit(uint8_t route, uint8_t chn);
void CoinAcceptor_UpDateStat(uint16_t stat);
void CoinAcceptor_SetTubeFull(void);
void CoinAcceptor_SetCommErr(void);
void CoinAcceptor_SetValueErr(void);
void CoinAcceptor_SetCoinPara(CoinParaStruct coinpara);
uint8_t CoinAcceptor_GetCoinLevel(void);
uint8_t CoinAcceptor_GetCoinScale(void);
uint8_t CoinAcceptor_GetCoinDecimal(void);
void CoinAcceptor_SetRstStat(void);
uint8_t CoinAcceptor_CommErr(void);
uint8_t CoinAcceptor_Available(void);
uint8_t CoinAcceptor_Enable(uint16_t cointype);
uint8_t CoinAcceptor_Poll(uint16_t cointype);
uint16_t CoinAcceptor_CheckTubeStat(uint16_t cointype, uint32_t *coinleft);
void CoinAcceptor_CheckStat(void);
#endif
