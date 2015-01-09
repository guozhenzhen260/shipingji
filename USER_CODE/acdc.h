#ifndef _ACDC_H
#define _ACDC_H

#define ACDCLEDCTR_UP    0x01
#define ACDCLEDCTR_DOWN  0x00
#define ACDCCOMPER_UP    0x01
#define ACDCCOMPER_DOWN  0x00

#define ACDCDIST_TIME     2

extern uint8_t ACDCModuleQuery(uint8_t ACDCLedCtr, uint8_t ACDCDistCtr);
extern uint8_t ACDCModuleTest();

#endif