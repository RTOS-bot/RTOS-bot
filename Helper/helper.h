#ifndef HELPER_H
#define HELPER_H
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128
#define BAUD_RATE 9600
#define MASK(x) (1 << (x))
#include <stdint.h>


void initUART2(uint32_t baud_rate);


#endif



