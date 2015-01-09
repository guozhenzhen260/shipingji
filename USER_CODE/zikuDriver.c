/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               zikuDriver.c
** Latest modified Date:    2010-07-22
** Latest Version:           
** Descriptions:            GT20L24F6Y字库驱动程序
**
**--------------------------------------------------------------------------------------------------------
** Created by:               
** Created date:            
** Version:                  
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "..\driver\LPC17xx.h"
#include "..\USER_CODE\global.h"
#include "..\driver\spi.h"
#include "zikuDriver.h"

/*---------------------------   GT20L24F6Y_Init --------------------------------------*/
//初始化
void GT20L24F6Y_Init(void) 
{
    /*************************************************************/
	//	spi_init
	/*************************************************************/
	//spi_init();
	//spi_hi_speed(1);
}





 /*---------------------------  GT20L24F6Y_ReadByte   --------------------------------------*/
void GT20L24F6Y_ReadZiku(uint16_t len, uint8_t *pbuf,uint32_t ziAddress) 
{
   uint16_t i;     
  
   spi_sziku(0); 
   
   spi_send(0x03); 

   
   //spi_send(0x00);  
   //spi_send(0x85);
   //spi_send(0xd0);
   spi_send((ziAddress>>16)&0x00ff);  
   spi_send((ziAddress&0x0000ffff)>>8);
   spi_send(ziAddress&0x000000ff);   

   //spi_send(0xFF); 
 
   for(i = 0; i < len; i++) 
   {
   		*(pbuf+i) = spi_send(0xFF);
      //(pbuf[Num]+i) = spi_send(0xFF); 
   }

   spi_sziku(1);
}



