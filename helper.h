#ifndef HELPER_H
#define HELPER_H
#define PTB0_PIN 0
#define PTB1_PIN 1
#define PTB2_PIN 2
#define PTB3_PIN 3
#define SPEED_1 3750 
#define SPEED_2 5000
#define SPEED_3 7500
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
typedef enum dir_t {LEFT, RIGHT, FRONT, BACK, STOP} dir_t;

void initSwitch(void);
void initLEDs(void);
void clearLEDs(void);
void initMotorPWM(void);
void initUART2(uint32_t baud_rate);

//Control Functions
void led_control(color_t color);
void dir_control(dir_t dir);
void forward_move(int speed);
void backward_move(int speed);
void left_rotate(int speed);
void right_rotate(int speed);
void stop_move(void);

//Delay Function
void delay(volatile uint32_t nof);

// UART2 Transmit & Receive Polls
void UART2_Transmit_Poll(uint8_t data);
uint8_t UART2_Receive_Poll(void);

#endif
