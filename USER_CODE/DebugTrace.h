#ifndef __DEBUGTRACE_H 
#define __DEBUGTRACE_H

void Uart0_Init(void);
void Uart0_SendByte(uint8_t byte);
void Uart0_Write(uint8_t *buf,uint8_t lenth);
void Trace( uint8_t *format , ... ); 

#endif
