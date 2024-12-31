#ifndef	__UART_H__
#define __UART_H__

#include "config.h"

void Uart1_Init(void);
void UART_SendByte(uint8_t Byte);
void UART_SendString(uint8_t *str);







#endif
