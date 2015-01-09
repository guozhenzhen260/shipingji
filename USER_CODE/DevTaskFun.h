/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           DevTaskFun.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        设备任务相关函数声明
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
#ifndef __DEVTASKFUN_H
#define __DEVTASKFUN_H

void InitMoneyDevStat(void);
void CheckPricePara(void);
void GetEscrowMinCh(void);
void GetNoteEnBit(void);
void GetCoinEnBit(void);
void CheckNotePara(void);
void CheckCoinPara(void);
void EnableMoneyType(void);
void DisCoinDevCmd(void);
void DisNoteDevCmd(void);
void EnCoinDevCmd(void);
void EnNoteDevCmd(void);
void CheckVMCStat(void);
#endif
