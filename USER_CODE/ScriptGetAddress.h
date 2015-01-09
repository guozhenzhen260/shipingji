#ifndef _SCRIPTGETADDRESS_H
#define _SCRIPTGETADDRESS_H
#include "stdint.h"



#define ASCII5X7					(0)
#define ASCII7X8					(1)
#define ASCII6X12					(2)
#define ASCII8X16					(3)
#define ASCII8X16CU					(4)
#define ASCII12_FANGTOU				(5)
#define ASCII12_BAIZHENG			(6)
#define ASCII24_FANGTOU				(7)
#define ASCII16_FANGTOU				(8)
#define ASCII16_BAIZHENG			(9)
#define LADING8X16					(10)
#define XILA8X16					(11)
#define JILIER8X16					(12)
#define XIBOLAI8X16					(13)
#define TAIWEN8X16					(14)
#define DIANZHENBUDENGKUANG16		(15)


#define ZILEIXING_8X16
//#define ZILEIXING_12X12
//#define ZILEIXING_16X16
uint32_t Script_GetAddress(uint8_t *ScriptStr,uint32_t *GetAddress,uint8_t ziCount);

uint8_t GetScriptUnicode(uint8_t DispRow,uint8_t DispLCDFlag,const char *str,uint8_t strLen,uint8_t ChangeWhite);
void DispStrZiFu(uint8_t DispRow,uint8_t DispLCDFlag,uint32_t *ZiAddress,uint8_t DispCount,uint8_t ChangeWhite);

#endif

