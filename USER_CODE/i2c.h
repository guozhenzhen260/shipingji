/*****************************************************************************
 *   i2c.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __I2C_H 
#define __I2C_H

#define I2C_BUFSIZE		0x04
#define I2C_MAX_TIMEOUT 0x00FFFFFF

#define I2C_MASTER		0x01
#define I2C_SLAVE		0x02

#define I2C_WRITE		0x00
#define I2C_READ 		0x01

#define I2C_RD_BIT			0x01

#define I2C_IDLE			0
#define I2C_STARTED			1
#define I2C_RESTARTED		2
#define I2C_REPEATED_START	3
#define I2C_DATA_ACK		4
#define I2C_DATA_NACK		5

#define I2CONSET_I2EN		0x00000040  /* I2C Control Set Register */
#define I2CONSET_AA			0x00000004
#define I2CONSET_SI			0x00000008
#define I2CONSET_STO		0x00000010
#define I2CONSET_STA		0x00000020

#define I2CONCLR_AAC		0x00000004  /* I2C Control clear Register */
#define I2CONCLR_SIC		0x00000008
#define I2CONCLR_STAC		0x00000020
#define I2CONCLR_I2ENC		0x00000040

#define I2DAT_I2C			0x00000000  /* I2C Data Reg */
#define I2ADR_I2C			0x00000000  /* I2C Slave Address Reg */
#define I2SCLH_SCLH			0x80  /* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL			0x80  /* I2C SCL Duty Cycle Low Reg */


void    I2C0_IRQHandler( void );
uint8_t I2C0_Init( uint8_t I2cMode );
uint8_t I2C0_Start( void );
uint8_t I2C0_Stop( void );
uint8_t I2C0_Engine( void );
uint8_t I2C0_Write(uint8_t DevAddr, uint8_t SubAddr, uint8_t *dat, uint8_t wrlen);
uint8_t I2C0_Read(uint8_t DevAddr, uint8_t SubAddr, uint8_t *dat, uint8_t rdlen);
#endif /* end __I2C_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
