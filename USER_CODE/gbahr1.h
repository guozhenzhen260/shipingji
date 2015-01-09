/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           gbahr.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        GBA纸币器驱动程序相关函数声明
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
#ifndef __GBAHR1_H 
#define __GBAHR1_H

/*状态位*/
#define  HR_ESCROWED_BIT	 (1 << 2)	   //暂存状态
#define  HR_STACKED_BIT		 (1 << 4)	   //压钞状态
#define  HR_RETURNED_BIT	 (1 << 6)	   //退钞状态

#define  HR_CHEATED_BIT      (1 << 0)	   //吞币
#define  HR_BILLREJ_BIT      (1 << 1)	   //拒收
#define  HR_BILLJAM_BIT      (1 << 2)	   //卡钞
#define  HR_BILLFULL_BIT     (1 << 3)	   //钞箱满
#define  HR_LCBPRESENT_BIT   (1 << 4)	   //有钞箱
#define  HR_VALUE_BIT        (1 << 7)	   //通道值对错状态

#define  HR_POWERUP_BIT      (1 << 0)	   //上电状态
#define  HR_CMDERR_BIT       (1 << 1)	   //命令出错
#define  HR_FAILURE_BIT      (1 << 2)	   //故障
#define  HR_COMMERR_BIT      (1 << 15)	   //通讯故障

#define  HR_ERR_MASK1   0x8C			   //第一字节错误状态掩码
#define  HR_ERR_MASK2   0x06			   //第二字节错误状态掩码

#define  HR_STAT_OK		0                  //HR_LCBPRESENT_BIT

#define ACKSwitch()            GbaAck = GbaAck ? 0 : 1

typedef struct               //命令格式
{				 
   uint8_t Stx;  		     //命令头
   uint8_t Lenth;  			 //长度
   uint8_t Ack : 4;			 //ACK值
   uint8_t Msgtype : 3;	 	 //消息类型
   uint8_t Reserve : 1;	     //保留
   uint8_t Data0;	     	 //数据0
   uint8_t Data1;     		 //数据1
   uint8_t Data2;	  		 //数据2
   uint8_t Etx;	  			 //命令尾
   uint8_t chksum;    		 //校验码
} GBACommand;

void Gbahr1_PackAnalysis(uint8_t *buf, uint8_t len);
void Gbahr1_ReadBuf(uint8_t *buf, uint8_t len);
uint8_t Gbahr1_Enable(uint16_t billtype);
uint8_t Gbahr1_Poll(uint16_t billtype);
uint8_t Gbahr1_Stack(uint16_t billtype);
uint8_t Gbahr1_Return(uint16_t billtype);
uint8_t Gbahr1_ErrJudge(uint8_t x);


//120401 by cq change structor
//** Descriptions:        GBA纸币器应用程序相关函数声明
void Gbahr1_InitProc(void);
void Gbahr1_UpDateStackerStat(void);


#endif
