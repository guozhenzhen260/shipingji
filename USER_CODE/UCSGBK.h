#ifndef _UCSGBK_H
#define _UCSGBK_H
#include "stdint.h"



uint32_t gsmEncodeUcs2(const uint8_t* src, uint8_t* dst, uint32_t srclen);


uint32_t gsmDecodeUcs2(const uint8_t * src, uint8_t * dst,uint32_t srclen);

#endif

