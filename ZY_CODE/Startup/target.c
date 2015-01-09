/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info--------------------------------------------------------------------------------
** File name:           target.c
** Last modified Date:  2009-07-22
** Last Version:        V1.00
** Descriptions:        the specific codes for LPC1700 target boards
**                      Every project should include a copy of this file, user may modify it as needed
**------------------------------------------------------------------------------------------------------
** Created by:          chenmingji
** Created date:        2009-07-22
** Version:             V1.00
** Descriptions:        The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
*********************************************************************************************************/
#include  "..\..\config.h"

/*********************************************************************************************************
** Function name:           defaultVectorHandle
** Descriptions:            默认异常处理程序
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void defaultVectorHandle (void)
{
#ifdef DEBUG 
    while (1);
#else
    zyReset(ZY_HARD_RESET);
#endif                                                                  /*  DEBUG                       */
}

/*********************************************************************************************************
** Function name:           targetInit
** Descriptions:            Initialize the target
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void targetInit (void)
{
    extern void *__GpvVectorTable[];
#if VECTOR_TABLE_IN_FLASH == 0
    extern void *__GpvVectorRom[];
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

#if VECTOR_TABLE_IN_FLASH == 0
    zVTOR    = (unsigned long)__GpvVectorRom;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */
    zVTOR    = (unsigned long)__GpvVectorTable;


    zyIrqDisable();

    /*
     *  让FLASH在安全区域运行，避免调试错误
     */  
    zFLASHCFG = ((0x05ul << 12) & (~(0x003f))) | 0x003a;

	/*  
     *  Set system timers for each component  
     */
#if (FPCLK / (FCCLK / 4)) == 1
    zPCLKSEL0 = 0x00000000;                                              /*  PCLK is 1/4 CCLK            */
    zPCLKSEL1 = 0x00000000;
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */

#if (FPCLK / (FCCLK / 4)) == 2
    zPCLKSEL0 = 0xAAAAAAAA;                                              /*  PCLK is 1/2 CCLK            */
    zPCLKSEL1 = 0xAAAAAAAA;   
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */

#if (FPCLK / (FCCLK / 4)) == 4
    zPCLKSEL0 = 0x55555555;                                              /*  PCLK is the same as CCLK    */
    zPCLKSEL1 = 0x55555555;      
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */
    zCLKOUTCFG = 0;  

    /*
     *  关闭PLL，避免调速错误
     */
    if ((zPLL0STAT >> 24) == 1) {
        zPLL0CON = 1;                                                    /*  Enable PLL, disconnected    */
        zPLL0FEED = 0xAA;
        zPLL0FEED = 0x55;
    }
    zPLL0CON = 0;                                                        /*  Disable PLL, disconnected   */
    zPLL0FEED = 0xAA;
    zPLL0FEED = 0x55;
    while(zPLL0STAT & (3 << 24)) {
    }   
    
    /*
     *  设置主要时钟
     */
    zSCS = (zSCS & 0x04) | 0x20;                                          /*  Enable mainOSC,1MHz~20MHz   */
    
    while ((zSCS & (1ul << 6)) == 0) {
    }
    
    zCLKSRCSEL = 0x01;                                                   /*  select main OSC as the PLL  */
                                                                        /*  clock source                */
    zPLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));    
    zPLL0FEED  = 0xAA;                                                   /*  Enable but disconnect PLL   */
    zPLL0FEED  = 0x55;    
    
    zPLL0CON   = 1;
    zPLL0FEED  = 0xAA;                                                   /*  Enable but disconnect PLL   */
    zPLL0FEED  = 0x55;    
    
    while ((zPLL0STAT & (1ul << 24)) == 0) {
    }
    
    zCCLKCFG = (FCCO / FCCLK) - 1;   
    
    while (((zPLL0STAT & (1ul << 26)) == 0)) {                           /*  Check lock bit status       */
    }
    while (((zPLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1)) && 
           (((zPLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1))) {
    }
    
    zPLL0CON  = 3;                                                       /*  connect the PLL             */
    zPLL0FEED = 0xAA;
    zPLL0FEED = 0x55;                                
    while ((zPLL0STAT & (1ul << 25)) == 0) {                             /*  Wait until the PLL is       */ 
    }
                                                                        /*  connected and locked        */
																		  
    /*
     *  设置USB时钟
     */
#if FUSBCLK_EN == 1
#if (FCCO % (FUSBCLK * 2)) != 0
    zPLL1CON = 1;
    zPLL1CFG = ((FUSBCLK / FOSC) - 1) | (1 << 5);

    zPLL1FEED = 0xaa;
    zPLL1FEED = 0x55;
    while ((zPLL1STAT & (1 << 10)) == 0) {
    }
    zPLL1CON  = 3;
    zPLL1FEED = 0xaa;
    zPLL1FEED = 0x55;
#else
    zUSBCLKCFG = (FCCO / (FUSBCLK * 2)) - 1;
    zPLL1CON   = 0;
    zPLL1FEED  = 0xaa;
    zPLL1FEED  = 0x55;    
#endif                                                                  /*  (FCCO % (FUSBCLK * 2))      */
#endif                                                                  /*  FUSBCLK_EN                  */

    /*
     *  设置存储器加速模块
     */
#if FCCLK <= 20000000
    zFLASHCFG = ((0x01ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash访问使用1个CPU时钟     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 20000000 && FCCLK <= 40000000
    zFLASHCFG = ((0x02ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash访问使用2个CPU时钟     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 40000000 && FCCLK <= 60000000
    zFLASHCFG = ((0x03ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash访问使用3个CPU时钟     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 60000000 && FCCLK <= 80000000
    zFLASHCFG = ((0x04ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash访问使用4个CPU时钟     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 80000000 && FCCLK <= 100000000
    zFLASHCFG = ((0x05ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash访问使用5个CPU时钟     */
#endif                                                                  /*  FCCLK                       */

    STCALIB = FCCLK / 100;

    zyIsrSet(SVCI, (unsigned long)svcallHandle, 0x00);

	zyIrqEnable();
    zyIfInit();
    return;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
