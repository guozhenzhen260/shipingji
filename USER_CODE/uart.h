/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        串口驱动程序相关函数声明
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
#ifndef __UART_H 
#define __UART_H

#define  ERR_STX            0x81		//帧头出错
#define  ERR_ETX            0x82		//帧尾出错
#define  ERR_CHKSUM         0x83		//校验出错
#define  ERR_TIMEOUT        0x84		//超时

#define  PARITY_DIS         0
#define  PARITY_ODD         1
#define  PARITY_EVEN        2
#define  PARITY_F_1         3
#define  PARITY_F_0         4

void Uart2_Init(void);
void Uart2_ClrBuf(void);
void Uart2_IRQHandler(void);
void Uart2_SetParity(uint8_t mode);
uint8_t Uart2_ReadMdbStat(void);
void Uart2_SetMdbMode(void);
void Uart2_SetEvbMode(void);
void Uart2_TransmitWait(void);
void Uart2_SendByte(uint8_t byte);
void Uart2_Write(uint8_t *buf,uint8_t lenth);
uint8_t Uart2_Read(uint8_t *buf, uint8_t len);
void Uart3_Init(void);
void Uart3_ClrBuf(void);
uint8_t Uart3_Read(uint8_t *buf, uint8_t len);
void Uart3_SendByte(uint8_t byte);
void Uart3_Write(uint8_t *buf,uint8_t lenth);
void Uart3_IRQHandler(void);
#endif
