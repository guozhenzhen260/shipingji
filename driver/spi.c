/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               spi.c
** Latest modified Date:    2010-07-22
** Latest Version:           
** Descriptions:            spi驱动程序
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
#include "spi.h"

/*--------------------------- spi_init --------------------------------------*/

void spi_init(void) 
{
   /* Initialize and enable the SSP Interface module. */

  LPC_SC->PCONP        |= (1 << 21);              /* Enable power to SSPI0 block  */

  /* SSEL is GPIO, output set to high. */
  //LPC_PINCON->PINSEL1 &= ~(3<<0);                 /* P0.16 SSEL (used as GPIO) */ 
  LPC_GPIO0->FIODIR  |=  (1<<21);                 /* P0.16 is output */
  LPC_GPIO0->FIOPIN  |=  (1<<21);                 /* set P0.16 high (SSEL inactiv) */

  LPC_GPIO1->FIODIR  |=  (1<<18);                 /* P0.16 is output */
  LPC_GPIO1->FIOPIN  |=  (1<<18);                 /* set P0.16 high (SSEL inactiv) */

  /* SCK, MISO, MOSI are SSP pins. */
  LPC_PINCON->PINSEL0 &= ~(3UL<<30);              /* P0.15 cleared */
  LPC_PINCON->PINSEL0 |=  (2UL<<30);              /* P0.15 SCK0 */
  LPC_PINCON->PINSEL1 &= ~((3<<2) | (3<<4));      /* P0.17, P0.18 cleared */
  LPC_PINCON->PINSEL1 |=  ((2<<2) | (2<<4));      /* P0.17 MISO0, P0.18 MOSI0 */

  LPC_SC->PCLKSEL1 &= ~(3<<10);                   /* PCLKSP0 = CCLK/4 (12MHz) */
  LPC_SC->PCLKSEL1 |=  (1<<10);                   /* PCLKSP0 = CCLK   (48MHz) */

  LPC_SSP0->CPSR = 240;                           /* 96MHz / 240 = 400kBit */
                                              /* maximum of 18MHz is possible */    
  LPC_SSP0->CR0  = 0x00C7;                        /* 8Bit, CPOL=0, CPHA=0         */
  LPC_SSP0->CR1  = 0x0002;                        /* SSP0 enable, master          */
}


/*--------------------------- spi_hi_speed ----------------------------------*/
//设置spi的速度
void spi_hi_speed(uint8_t on) 
{
   /* Set a SPI clock speed to desired value. */

   if (on == 1) 
   {
      /* Max. 12 MBit used for Data Transfer. */
      LPC_SSP0->CPSR =  12;                         /* 96MHz / 12   = 8MBit */
   } 
   else 
   {
      /* Max. 400 kBit used in Card Initialization. */
      LPC_SSP0->CPSR = 240;                         /* 96MHz / 240 = 400kBit */
   }
}


/*--------------------------- spi_ss ----------------------------------------*/
//0禁能spi总线芯片，1使能spi总线芯片
void spi_ss(uint8_t ss) 
{
   /* Enable/Disable SPI Chip Select (drive it high or low). */

  if(ss) 
  {
	LPC_GPIO0->FIODIR |= (1ul << 21); 
	LPC_GPIO0->FIOSET |= (1ul << 21);
  } 
  else 
  {
	LPC_GPIO0->FIODIR |= (1ul << 21); 
	LPC_GPIO0->FIOCLR |= (1ul << 21);
  }
}


/*--------------------------- spi_send --------------------------------------*/
//收发数据
uint8_t spi_send(uint8_t outb) 
{
   /* Write and Read a byte on SPI interface. */
  LPC_SSP0->DR = outb;
  while (LPC_SSP0->SR & BSY);                     /* Wait for transfer to finish */
  return (LPC_SSP0->DR);                          /* Return received value */

}


/**************************************
**此部分使用spi总线，访问字库芯片
**************************************/
/*--------------------------- spi_sziku ----------------------------------------*/
//0使能spi访问字库的总线芯片，1禁能spi访问字库的总线芯片
void spi_sziku(uint8_t ss) 
{
   /* Enable/Disable SPI Chip Select (drive it high or low). */

  if(ss) 
  {
	LPC_GPIO1->FIODIR |= (1ul << 18); 
	LPC_GPIO1->FIOSET |= (1ul << 18);
  } 
  else 
  {
	LPC_GPIO1->FIODIR |= (1ul << 18); 
	LPC_GPIO1->FIOCLR |= (1ul << 18);
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
