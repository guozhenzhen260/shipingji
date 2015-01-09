/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           BillAcceptor.h
** Last modified Date:  2011-10-26
** Last Version:         
** Descriptions:        纸币器驱动程序相关函数声明
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
#ifndef __BILLACCEPTOR_H 
#define __BILLACCEPTOR_H

#define BILLACCEPTOR_STACKED    0x80
#define BILLACCEPTOR_ESCROWED   0x81
#define BILLACCEPTOR_RETURNED   0x82

typedef struct
{
	uint8_t  Level;			  //纸币器等级
	uint16_t Code;			  //国家代码
	uint16_t Scale;			  //比例因子
	uint16_t Decimal;		  //10^小数位数
	uint16_t StkCapacity;	  //储币容量
	uint16_t Security;		  //安全等级
	uint8_t  EscrowFun;		  //暂存功能
}  BillParaStruct;

uint8_t isBillSingleWork(void);//121106 by cq BillSingleWork

//BillAcceptor.c//纸币器驱动程序
void BillAcceptor_Init(void);//纸币器初始化
uint8_t BillAcceptor_Enable(uint16_t billtype);//纸币器使能
//parameters:		billtype:纸币器使能通道
//Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
uint8_t BillAcceptor_Poll(uint16_t billtype);//纸币器轮询
//Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
void BillAcceptor_PollProc(void);//轮询纸币器，获取状态，并作出相应处理
uint8_t BillAcceptor_Return(uint16_t billtype);//退币
//Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
uint8_t BillAcceptor_Stack(uint16_t billtype);//压钞
//Returned value:	若大于0x80为通讯出错状态，否则为接受到的数据长度	
void BillAcceptor_UpDateStat(uint16_t status);//更新当前纸币器状态
void BillAcceptor_UpdateAcpt(uint8_t route,uint8_t chn);//收到纸币，更新纸币状态信息
//parameters:		rount:BILLACCEPTOR_STACKED,BILLACCEPTOR_ESCROWED,BILLACCEPTOR_RETURNED等，表示当前纸币所处的位置
//					chn:  纸币通道值
void BillAcceptor_ReturnProc(uint8_t moneych);//有纸币压钞时，处理函数
//parameters:		moneych:收币通道
void BillAcceptor_StackProc(uint8_t moneych);//有纸币压钞时，更新交易数据
void BillAcceptor_EscrowProc(uint8_t moneych);//纸币器处于代管状态时，判断纸币面值，并作出相应处理
void BillAcceptor_SetBillPara(BillParaStruct billpara);//设置纸币参数
//parameters:		纸币参数结构体
uint8_t BillAcceptor_ErrJudge(uint8_t len);//判断纸币器通讯状态
// parameters:		len:根据相关协议，正常时表示数据长度，异常时表示异常状态
// Returned value:	TRUE:正常 FALSE:出错	
uint8_t BillAcceptor_CommErr(void);//用于判断是否有通讯故障
// Returned value:	TRUE:故障 FALSE:正常	
uint8_t BillAcceptor_ValueSetErr(void);//用于判断纸币各通道面值设置是否正确
// Returned value:	TRUE:故障 FALSE:正常	
uint8_t BillAcceptor_StackerFull(void);//用于判断钞箱是否满了
// Returned value:	TRUE:故障 FALSE:正常	
uint8_t BillAcceptor_Available(void);//判断纸币器是否可用
//					通讯是否正常，面值设置是否正确等	
// Returned value:	TRUE:故障 FALSE:正常	
void BillAcceptor_SetCommErr(void);//设置纸币器通讯故障状态
void BillAcceptor_SetValueErr(void);//设置纸币器面值设置错误状态
void BillAcceptor_SetStackerFull(void);//设置钞箱满状态
void BillAcceptor_SetRstStat(void);//设置纸币器复位状态，表示纸币器刚刚复位了
void BillAcceptor_SetInValid(void);//设置纸币不可识别状态
void BillAcceptor_ClrInValid(void);//清除纸币不可识别状态
void BillAcceptor_CheckStat(void);//更新NoteStat(NoteStat纸币器状态没有具体的状态码，只是用来判断纸币器是否正常等)
#endif
