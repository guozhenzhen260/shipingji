/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Changer.h
** Last modified Date:  2011-10-26
** Last Version:         
** Descriptions:        找零器器驱动程序相关函数声明
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-10-26
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
#ifndef __CHNGER_H 
#define __CHNGER_H

void Changer_SetDisStat(uint8_t chn);
void Changer_SetCommErr(uint8_t chn);
uint8_t Changer_CommErr(uint8_t chn);
uint8_t Changer_LowStat(void);
void Changer_UpDateStat(uint8_t status, uint8_t chn);
void Changer_UpDatepBuf(uint8_t num, uint8_t chn);
void Changer_Init(void);
void Changer_Payout(uint32_t moneyout);
void Changer_CheckStat(void);
#endif
