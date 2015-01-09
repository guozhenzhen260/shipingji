/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           hopper.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        hopperÇý¶¯³ÌÐòÏà¹Øº¯ÊýÉùÃ÷
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
#ifndef __HOPPER_H 
#define __HOPPER_H

#define HOPPERNO    0			//hopper1ºÅ
#define HOPPERNO2   1			//hopper2ºÅ
#define HOPPERNO3   2			//hopper3ºÅ

#define HOPPER_PAYOUT    0x50	//³ö±ÒÃüÁî
#define HOPPER_INQUIRE   0x51	//²éÑ¯ÃüÁî

/*×´Ì¬Î»*/
#define HOPPER_REPEATSN_BIT           (1 << 0)	   //ÖØ¸´·¢ËÍÃüÁî
#define HOPPER_PAYOUTFAIL_BIT         (1 << 0)	   //ÕÒÁãÊ§°Ü
#define HOPPER_BUSY_BIT               (1 << 1)	   //Ã¦
#define HOPPER_DATAERR_BIT            (1 << 2)	   //Êý¾Ý³ö´í
#define HOPPER_COINEMPTY_BIT          (1 << 3)	   //Ó²±ÒÆ÷¿Õ
#define HOPPER_SENSORERR_BIT          (1 << 4)	   //´«¸ÐÆ÷¹ÊÕÏ
#define HOPPER_INSUFFICIENT_BIT       (1 << 5)	   //Ó²±Ò²»×ã
#define HOPPER_MALFUNCTION_BIT        (1 << 6)	   //¹ÊÕÏ
#define HOPPER_CMDERR_BIT             (1 << 7)	   //ÃüÁî³ö´í
/*À©Õ¹×´Ì¬Î»*/
#define HOPPER_DISABLE_BIT			  (1 << 10)	   //ÕÒÁãÆ÷½ûÄÜ
#define HOPPER_COMMERR_BIT			  (1 << 15)	   //Í¨Ñ¶¹ÊÕÏ

#define HOPPER_ERR_MASK               0xFFFC	   //´íÎó×´Ì¬ÑÚÂë
#define HOPPER_MALFUN_MASK            0xFFD4	   //´íÎó×´Ì¬ÑÚÂë

uint8_t Hopper_ErrJudge(uint8_t x);
uint8_t Hopper_GetStatus(void);
uint8_t HopperPayout(uint8_t hopperno, uint16_t amount);
uint8_t HopperInquire(uint8_t hopperno);



//120401 by cq change structor
//** Descriptions:        HopperÓ¦ÓÃ³ÌÐòÏà¹Øº¯ÊýÉùÃ÷
void CheckHopperStat(void);
void Hopper_InitProc(void);
void HopperPayoutCmd(uint32_t MoneyOut, uint8_t hopperch);
void Hopper_PayoutProc(uint32_t moneyout);


#endif
