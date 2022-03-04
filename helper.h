#ifndef HELPER_H
#define HELPER_H
#define PTB0_PIN 0
#define PTB1_PIN 1
#define SW_POS 6 //PortD Pin 6
#define RED_LED 18  //PortB Pin 18
#define GREEN_LED 19  //PortB Pin 19
#define BLUE_LED 1  //PortD Pin 1
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128
#define BAUD_RATE 9600
#define MASK(x) (1 << (x))
#include <stdint.h>

typedef enum color_t {RED, GREEN, BLUE} color_t;

void initSwitch(void);
void initLEDs(void);
void clearLEDs(void);
void initPWM(void);
void initUART2(uint32_t baud_rate);

//Switches to the particular led color
void led_control(color_t color);

//Delay Function
void delay(volatile uint32_t nof);

// UART2 Transmit & Receive Polls
void UART2_Transmit_Poll(uint8_t data);
uint8_t UART2_Receive_Poll(void);

#endif