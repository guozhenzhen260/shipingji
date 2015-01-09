/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           drv.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        货道驱动驱动程序相关函数声明
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
#ifndef __DRV_H 
#define __DRV_H

#define DRV_DISPENSE        0x70		//出货命令
#define DRV_INQUIRE         0x71		//查询
#define DRV_GOODSCHK        0x80		//货到检测
#define DRV_STATUSCHK       0x81		//查询货道有无货
#define DRV_OPENSLUICE      0x70		//打开阀门
#define DRV_CLOSESLUICE     0x70		//关闭阀门
#define DRV64_CLEARSN       0x72		//清SN号
#define DRV64_TEST          0x73		//测试
#define DRV_CLEARSN         0xFF		//清SN号

#define GOODSCHK_OPEN       1			//打开有无货检测
#define GOODSCHK_CLOSE      0			//关闭有无货检测

#define DRV_ALLCHNDIS_BIT      (1 << 0)	//所有货到不可用
#define DRV_NOGOODS_BIT        (1 << 1)	//无货
#define DRV_HARDERR_BIT        (1 << 2)	//硬件出错
#define DRV_COMMERR_BIT        (1 << 3)	//通讯出错
#define DRV_CHNSET_BIT         (1 << 7)	//货道状态 机械结构通道使能
#define DRV_STAT_MASK		   0x7F		//其他为软件设置状态

/*货道状态位*/
#define DRV_MISSIONFAIL_BIT     (1 << 0)   //命令失败

#if DRV_TYPE == DRV_12
#define DRV_BUSY_BIT            (1 << 1)   //忙
#define GOODS_SOLDOUT_BIT       (1 << 3)   //商品已售完
#define MOTO_NOTRETURN_BIT      (1 << 4)   //电机没回到正确位置
#define MOTO_MISPLACE_BIT       (1 << 5)   //电机不在正确的位置上
#define DRV_ERR_MASK            0x30	   //错误状态掩码
#elif DRV_TYPE == DRV_64
#define GOODS_SOLD_ERR          (1 << 0)   //bit0总故障位
#define GOODS_SOLDOUT_BIT       (1 << 1)   //bit1电机故障
#define MOTO_MISPLACE_BIT       (1 << 2)   //bit2电机在转之前就不在正确的位置上(也算电机故障)
#define MOTO_NOTMOVE_BIT        (1 << 3)   //bit3电机不能转(也算电机故障)
#define MOTO_NOTRETURN_BIT      (1 << 4)   //bit4电机没转到正确位置(也算电机故障)
#define GOODS_NOTPASS_BIT       (1 << 5)   //bit5商品没出(出货确认没检测到)
#define DRV_CMDERR_BIT          (1 << 6)   //bit6命令错误(只有发送命令和查询命令着两个命令，如果发了其他的命令就报错)
#define DRV_HOLESTA_BIT         (1 << 7)   //货道状态
#define DRV_GOCERR_BIT          (1 << 7)   //bit7出货检测模块状态(GOC故障)
#define DRV_ERR_MASK            0x5C	   //错误状态掩码
#endif

#define DRV_NOGOOGS   0x01				   //无货

uint8_t GetDrvStatus(void);
uint8_t Drv_ErrJudge(uint8_t x);
uint8_t DRV_ChnChk(uint8_t channelno,uint8_t *channelx, uint8_t *channely);
uint8_t DRV_Dispense(uint8_t DRVNo, uint8_t chksign);
uint8_t DRV_Inquire(uint8_t DRVNo, uint8_t chksign);
uint8_t DRV_GetStatus(uint8_t DRVNo, uint8_t chksign);
uint8_t DRV_ClearSN(uint8_t DRVNo, uint8_t chksign);
uint8_t DRV_Test(uint8_t DRVNo, uint8_t chksign);


//120401 by cq change structor
//** Descriptions:        货道驱动应用程序相关函数声明

uint8_t ChannelCheck(void);
void CheckDriverStat(void);
void DrvCheckCommand(uint8_t channel);
void DrvOutCommand(uint8_t channel);


#endif /* end __DRV_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
