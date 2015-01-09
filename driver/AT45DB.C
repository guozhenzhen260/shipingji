/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               AT45DB.c
** Latest modified Date:    2010-07-22
** Latest Version:           
** Descriptions:            AT45DB驱动程序
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
#include "spi.h"
#include "AT45DB.h"

/*---------------------------   AT45DB_Init --------------------------------------*/
//初始化
void AT45DB_Init(void) 
{
    /*************************************************************/
	//	spi_init
	/*************************************************************/
	spi_init();
	spi_hi_speed(1);
}

/*---------------------------   AT45DB_ReadStatus --------------------------------------*/
uint8_t AT45DB_ReadStatus(void) 
{
  uint8_t status;
  
  spi_ss(0); 
  spi_send(0xD7);
  status = spi_send(0xFF);
  spi_ss(1);
  return status;
}

/*---------------------------   AT45DB_GetFlashPara --------------------------------------*/
void AT45DB_GetFlashPara(void) 
{
    uint8_t   status;
  
    status = AT45DB_ReadStatus(); 
	status = (status >> 2) & 0x0F;	
	if(status == 0x09) 
	{
	    //FlashType = AT45DB081D;
		//PageSize = 264;
	} 
	else if(status == 0x0B) 
	{
	    //FlashType = AT45DB161D;
		//PageSize = 528;
	} 
	else 
	{
	    SYSPara1.FlashStat = 1;
	}
}

/*---------------------------    AT45DB_WaitOK   --------------------------------------*/
uint8_t AT45DB_WaitOK(void)
{
   uint8_t   status;
   
   while(1) 
   {
      status = AT45DB_ReadStatus();
	  if(status & 0x80) 
	  {
	     if(status == 0xFF) 
		 {
			 SYSPara1.FlashStat = 1;
		     return 1;
		 } 
		 SYSPara1.FlashStat = 0;
	     return 0;
	  }
   }
}

 /*---------------------------  AT45DB_ReadByte   --------------------------------------*/
void AT45DB_ReadByte(uint16_t PageAddr, uint16_t start, uint16_t len, uint8_t *pbuf) 
{
   uint16_t i;     

   AT45DB_WaitOK();

   spi_ss(0);  
   
   spi_send(0x0B); 

   #if  FLASH_TYPE == AT45DB081D  
   spi_send((uint8_t)(PageAddr >> 7));   
   spi_send((uint8_t)((PageAddr << 1)|(start >> 8))); 
   spi_send((uint8_t)start);
   #elif FLASH_TYPE == AT45DB161D  
   spi_send((uint8_t)(PageAddr >> 6));   
   spi_send((uint8_t)((PageAddr << 2)|(start >> 8))); 
   spi_send((uint8_t)start);
   #endif

   spi_send(0xFF); 

   for(i = 0; i < len; i++) 
   {
      pbuf[i] = spi_send(0xFF); 
   }

   spi_ss(1);
}


 /*---------------------------  AT45DB_WriteBuffer   --------------------------------------*/
void AT45DB_WriteBuffer(uint8_t nbuf, uint16_t start, uint16_t len, uint8_t *pbuf) 
{
   uint16_t i;      

   AT45DB_WaitOK();

   spi_ss(0);  
   
   switch(nbuf) 
   {
      case 1:
	  spi_send(0x84);
	  break;

	  case 2:
	  spi_send(0x87);
	  break;

	  default:
	  break;
   }

   spi_send(0xFF);
   spi_send((uint8_t)(start >> 8));
   spi_send((uint8_t)start);

   for(i = 0; i < len; i++) 
   {
      spi_send(pbuf[i]); 
   }

   spi_ss(1);
}

 /*---------------------------  AT45DB_WriteByte   --------------------------------------*/
void AT45DB_WriteByte(uint8_t nbuf, uint16_t PageAddr, uint16_t start, uint16_t len, uint8_t *pbuf) 
{
//   uint16_t i;     

   AT45DB_WriteBuffer(nbuf, start, len, pbuf);
   
   AT45DB_WaitOK();

   spi_ss(0);
   
   switch(nbuf) 
   {
      case 1:
	  spi_send(0x83);
	  break;

	  case 2:
	  spi_send(0x86);
	  break;

	  default:
	  break;
   } 

   #if  FLASH_TYPE == AT45DB081D    
   spi_send((uint8_t)(PageAddr >> 7));   
   spi_send((uint8_t)(PageAddr << 1));
   #elif FLASH_TYPE == AT45DB161D
   spi_send((uint8_t)(PageAddr >> 6));   
   spi_send((uint8_t)(PageAddr << 2));
   #endif 

   spi_send(0xFF); 

   spi_ss(1);
}

 /*---------------------------  AT45DB_Compare   --------------------------------------*/
uint8_t AT45DB_Compare(uint8_t nbuf, uint16_t PageAddr) 
{
	AT45DB_WaitOK();
	
	spi_ss(0);
   
    switch(nbuf) 
	{
       case 1:
	   spi_send(0x60);
	   break;

	   case 2:
	   spi_send(0x61);
	   break;

	   default:
	   break;
    } 
       
	#if  FLASH_TYPE == AT45DB081D    
    spi_send((uint8_t)(PageAddr >> 7));   
    spi_send((uint8_t)(PageAddr << 1));
    #elif FLASH_TYPE == AT45DB161D
    spi_send((uint8_t)(PageAddr >> 6));   
    spi_send((uint8_t)(PageAddr << 2));
    #endif 

    spi_send(0xFF); 

    spi_ss(1);

	AT45DB_WaitOK();

	return(AT45DB_ReadStatus() & 0x40);
 }

 /*---------------------------  AT45DB_ReadPage   --------------------------------------*/
void AT45DB_ReadPage(uint16_t PageAddr, uint8_t *pbuf) 
{
   	AT45DB_ReadByte(PageAddr, 0, PAGESIZE, pbuf);
}

 /*---------------------------  AT45DB_WritePage   --------------------------------------*/
uint8_t AT45DB_WritePage(uint16_t PageAddr, uint8_t *pbuf) 
{
   	uint8_t i;

	//AT45DB_WriteByte(1, PageAddr, 0, PAGESIZE, pbuf);
	//return 0;
	for(i = 0; i < 3; i++) {
	   AT45DB_WriteByte(1, PageAddr, 0, PAGESIZE, pbuf);

  	   AT45DB_WriteBuffer(2, 0, PageAddr, pbuf);

	   if(AT45DB_Compare(2, PageAddr)) {	 
	   	   AT45DB_WriteByte(2, PageAddr, 0, PAGESIZE, pbuf);
		   AT45DB_WriteBuffer(1, 0, PageAddr, pbuf);

		   if(AT45DB_Compare(1, PageAddr)) {
		      continue;
		   } else {
		      SYSPara1.FlashStat = 0;
			  return 0;
		   }
	   } else {
	      SYSPara1.FlashStat = 0;
		  return 0;
	   }
	}

	SYSPara1.FlashStat = 1; 
	return 1;	
}

 /*---------------------------      AT45DB_Test     --------------------------------------*/
void AT45DB_Test(void)
{
  /*  uint8_t    flashbuf[PAGESIZE];
	uint16_t   i;

	for(i = 0; i < PAGESIZE; i++) {
	    flashbuf[i] = i;
	}
	AT45DB_WritePage(500,flashbuf);
	for(i = 0; i < PAGESIZE; i++) {
	    flashbuf[i] = 0;
	}
	AT45DB_ReadPage(500,flashbuf);
	for(i = 0; i < PAGESIZE; i++) {
	    if(flashbuf[i] != (i&0xFF)) {
		    while(1);
		}
	}*/
}

