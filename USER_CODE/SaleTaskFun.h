/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           SaleTaskFun.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        交易任务相关函数声明
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
#ifndef __SALETASKFUN_H
#define __SALETASKFUN_H

uint8_t KeyProcess(uint8_t state,uint16_t key,uint8_t *ChannelNum);
uint8_t SaleTimeoutProc(uint8_t state);
void DispInitText(void);
void DispBeginText(uint8_t mode);
void DispChaxunText(uint8_t x,uint8_t y,uint8_t  KeyInput);//选货按键;by gzz 20121025
void DispChaoshiText(void);
uint8_t DispInputText(void);
void DispInputErrText(void);
void DispSaleText(void);
void DispOverText(void);
void DispWuhuoText(uint8_t x,uint8_t y);
void DispChuhuoText(void);
uint8_t DoChuhuoMsg(uint8_t ch,uint8_t x,uint8_t y);
void DispRetransText(void);
void DispLessText(uint32_t	GoodsPrice);
uint8_t DispAddMoneyText(uint8_t x,uint8_t y);
void DispPayoutText(void);
void DoCoinOutMsg(void);
uint8_t DoHopperOutMsg(void);
void DispIOUText(void);
void DispChangeText(void);
void DispEndText(void);
void DispErrText(void);
void DoDingshiMsg(void);
void DoNoteStackMsg(void);
void DoNoteReturnMsg(void);
void DoPayOutMsg(void);
uint8_t VP_CMD_Opt(void);

#endif

