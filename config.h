/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           config.h
** Last modified Date:  2009-05-15
** Last Version:        V1.01
** Descriptions:        User Configurable File
**
**--------------------------------------------------------------------------------------------------------
** Created By:          LinEnqiang
** Created date:        2009-06-12
** Version:             V1.00
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:       
** Version:             
** Descriptions:       
**
*********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */
  
/*********************************************************************************************************
  固件的特殊代码, 这一段无需改动
*********************************************************************************************************/
#include ".\ZY_CODE\firmware.h"
#include ".\user_code\global.h"
/*********************************************************************************************************
  配置代码定义
*********************************************************************************************************/
#include ".\cfg_file\target_cfg.h"
#include ".\cfg_file\Startup\LPC1700PinCfg.h"
#include ".\driver\lpc17xx_uart.h"
//#include ".\driver\include\lpc_types.h"
//#include    "./USER_CODE/uart1.h"



/*********************************************************************************************************
  在下面用户可以添加自己的配置
*********************************************************************************************************/
#include    <stdio.h>
#include    <string.h>
#include    "./USER_CODE/bsp.h"

#ifdef __cplusplus
	}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
**                                      End Of File
********************************************************************************************************/
