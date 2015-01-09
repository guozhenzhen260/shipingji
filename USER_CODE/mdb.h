/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           mdb.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        MDB协议相关函数声明
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
#ifndef __MDB_H 
#define __MDB_H

#define MAX_BLOCK_SIZE 36 		    //最大数据块
#define MDB_ACK 0x00				//应答
#define MDB_RET 0xAA				//重试
#define MDB_NAK 0xFF				//无应答

#define  MDB_RCV_OK       0x00		//MDB数据接收正确
#define  MDB_RCV_INIT     0x80		//开始接收MDB数据
#define  MDB_RCV_DATA     0x81		//正在接收MDB数据
#define  MDB_OUT_RANGE    0x82		//MDB数据接收超界
#define  MDB_CHKSUM_ERR   0x83		//MDB数据接收校验出错

#define  MDB_ADD          1			//MDB地址
#define  MDB_DAT          0			//MDB数据

void Mdb_TransmitByte(uint8_t dat,uint8_t mode);
uint8_t Mdb_Session(uint8_t add,uint8_t dat[],uint8_t count);
uint8_t Mdb_ErrJudge(uint8_t x);
void Mdb_ReadBuf(uint8_t *buf, uint8_t len);

#endif /* end __MDB_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
