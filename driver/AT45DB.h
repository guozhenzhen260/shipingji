/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               AT45DB.h
** Latest modified Date:    2010-07-22
** Latest Version:           
** Descriptions:            AT45DBÇý¶¯³ÌÐòÉùÃ÷
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
#ifndef __AT45DB_H 
#define __AT45DB_H

/*----------------------------------------------------------------------------
 *      AT45DB Driver Functions
 *---------------------------------------------------------------------------*/
 void AT45DB_Init(void);
 uint8_t AT45DB_ReadStatus(void);
 void AT45DB_GetFlashPara(void);
 uint8_t AT45DB_WaitOK(void);
 void AT45DB_ReadByte(uint16_t PageAddr, uint16_t start, uint16_t len, uint8_t *pbuf);
 void AT45DB_WriteBuffer(uint8_t nbuf, uint16_t start, uint16_t len, uint8_t *pbuf);
 void AT45DB_WriteByte(uint8_t nbuf, uint16_t PageAddr, uint16_t start, uint16_t len, uint8_t *pbuf);
 uint8_t AT45DB_Compare(uint8_t nbuf, uint16_t PageAddr);
 void AT45DB_ReadPage(uint16_t PageAddr, uint8_t *pbuf);
 uint8_t AT45DB_WritePage(uint16_t PageAddr, uint8_t *pbuf);
 void AT45DB_Test(void);

#endif /* end __AT45DB_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
