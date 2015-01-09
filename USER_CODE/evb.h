/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           evb.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        evb协议驱动程序相关函数声明
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
#ifndef __EVB_H 
#define __EVB_H

#define ChangeSN(sn)           ++sn? sn : sn++
           		 				 
typedef struct          //定义命令格式 
{				 
   uint8_t Head;  		//命令头
   uint8_t Lenth;  		//长度
   uint8_t SN;	      	//同步序号
   uint8_t Com;	     	//命令码
   uint8_t DevNo;     	//设备号
   uint8_t rsv1;	  	//保留
   uint8_t rsv2;	  	//出货检测（1:开 0:关）
   uint8_t chksum;    	//校验码
} EVBCommand;

#ifdef EVBDEF
uint8_t evbbuf[8];
#else
extern uint8_t evbbuf[8];	   
#endif

#endif /* end __EVB_H */

/*****************************************************************************
**                            End Of File
******************************************************************************/
