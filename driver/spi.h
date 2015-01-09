/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               spi.h
** Latest modified Date:    2010-07-22
** Latest Version:           
** Descriptions:            spiÇý¶¯³ÌÐòÉùÃ÷
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
#ifndef __SPI_H 
#define __SPI_H

/* SSPxSR - bit definitions. */
#define TFE     0x01
#define TNF     0x02
#define RNE     0x04
#define RFF     0x08
#define BSY     0x10

/*----------------------------------------------------------------------------
 *      SPI Driver Functions
 *---------------------------------------------------------------------------*/
void spi_init(void);
void spi_ss(uint8_t ss);
uint8_t  spi_send(uint8_t outb);
void spi_hi_speed(uint8_t on);
void spi_sziku(uint8_t ss);

#endif /* end __SPI_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
