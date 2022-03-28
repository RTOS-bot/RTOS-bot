#ifndef MOTORS_H
#define MOTORS_H
#include "PWM.h"

#define PORT_MOTOR_RIGHT 			PORTD
#define NUM_PINS_MOTOR_RIGHT 2
#define PTD0_PIN 0
#define PTD1_PIN 1

#define PORT_MOTOR_LEFT 		PORTD
#define NUM_PINS_MOTOR_LEFT	2
#define PTD2_PIN 2
#define PTD3_PIN 3

#define TPM_MOTOR_RIGHT	TPM0
#define TPM_MOTOR_LEFT	TPM0
#define TPM_CHANNEL0 0
#define TPM_CHANNEL1 1
#define TPM_CHANNEL2 2
#define TPM_CHANNEL3 3
#define NUM_CHANNELS_MOTOR_RIGHT 2
#define NUM_CHANNELS_MOTOR_LEFT	 2

#define MOTOR_MOD 7500
#define MOTOR_ALT 4

#define LF_CMD	0x40
#define RF_CMD	0x41
#define F_CMD	0x42
#define B_CMD	0x43
#define LB_CMD 0x45
#define RB_CMD 0x46

#define RIGHT_SPEED_0 2000
#define RIGHT_SPEED_1 3750 
#define RIGHT_SPEED_2 5000
#define RIGHT_SPEED_3 7500
#define LEFT_SPEED_0  2000
#define LEFT_SPEED_1  3750 
#define LEFT_SPEED_2  5000
#define LEFT_SPEED_3  7500

typedef enum dir_t {
	LEFT, 
	RIGHT, 
	FORWARD, 
	BACKWARD, 
	STOP
} dir_t;

void initMotorPWM(void);

void move(uint8_t rx_data);

void movementControl(dir_t direction, uint16_t speedRight, uint16_t speedLeft);

void clearTPM(void);

#endif
