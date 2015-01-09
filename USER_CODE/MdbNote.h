/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           gba.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        MDB纸币器驱动程序相关函数声明
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
#ifndef __MDBNOTE_H 
#define __MDBNOTE_H

/*MBV: Mdb Bill Validator*/
#define MBV_RESET        0x30		 //复位命令
#define MBV_SETUP        0x31		 //SETUP 获取配置信息
#define MBV_POLL         0x33		 //轮询
#define MBV_BILLTYPE     0x34		 //使能投币通道命令
#define MBV_ESCROW	     0x35		 //代管状态命令
#define MBV_STACKER      0x36		 //获取存币数
#define MBV_EXPANSION    0x37		 //扩展命令

#define MBV_ID           0x00		 //获取ID

//状态码
#define MBV_DEFMOTOERR         0x01		   //MOTO故障
#define MBV_SENSORERR          0x02		   //传感器故障
#define MBV_ROMCHKERR		   0x04		   //ROM出错
#define MBV_VALIDATORJAM       0x05		   //卡钞
#define MBV_BILLREMOVED        0x07		   //未知方法取钞
#define MBV_CASHBOXERR         0x08		   //钞箱出错
#define MBV_BILLREMOVAL        0x0C		   //试图取钞
/*
#define MBV_DEFMOTOERR_BIT         (1 << 0)
#define MBV_SENSORERR_BIT          (1 << 1)
#define MBV_ROMCHKERR_BIT		   (1 << 2)
#define MBV_VALIDATORJAM_BIT       (1 << 3)
#define MBV_BILLREMOVED_BIT        (1 << 4)
#define MBV_CASHBOXERR_BIT         (1 << 5)
#define MBV_BILLREMOVAL_BIT        (1 << 6)
*/
#define MBV_VALUE_BIT 			   (1 << 8)	 //通道值对错标志
#define MBV_BILLFULL_BIT		   (1 << 9)	 //钞箱满标志
#define MBV_COMMERR_BIT			   (1 << 15) //通讯故障

#define MBV_STAT_OK                0

uint8_t MdbNote_Reset(void);
uint8_t MdbNote_Setup(void);
uint8_t MdbNote_Enable(uint16_t NoteTypeEn);
uint8_t MdbNote_Poll(void);
uint8_t MdbNote_Stack(void);
uint8_t MdbNote_Return(void);
uint8_t MdbNote_GetStackerStat(void);
uint8_t MdbNote_IDRead(void);
void MdbNote_PackAnalysis(uint8_t *buf,uint8_t len);


//120401 by cq change structor
//** Descriptions:        MDB纸币器应用程序相关函数声明
void MdbNote_RstProc(void);
void MdbNote_InitProc(void);
void MdbNote_UpDateStackerStat(void);


#endif /* end __GBA_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
