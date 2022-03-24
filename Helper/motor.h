#ifndef MOTORS_H
#define MOTORS_H
#include "PWM.h"

#define PORT_MOTOR_RIGHT 			PORTB
#define NUM_PINS_MOTOR_RIGHT 2
#define PTB0_PIN 0
#define PTB1_PIN 1

#define PORT_MOTOR_LEFT 		PORTB
#define NUM_PINS_MOTOR_LEFT	2
#define PTB2_PIN 2
#define PTB3_PIN 3

#define TPM_MOTOR_RIGHT	TPM1
#define TPM_MOTOR_LEFT	TPM2
#define TPM_CHANNEL0 0
#define TPM_CHANNEL1 1
#define NUM_CHANNELS_MOTOR_RIGHT 2
#define NUM_CHANNELS_MOTOR_LEFT	 2

#define MOTOR_MOD 7500
#define MOTOR_ALT 3

#define L_CMD	0x40
#define R_CMD	0x41
#define F_CMD	0x42
#define B_CMD	0x43

#define RIGHT_SPEED_1 3750 
#define RIGHT_SPEED_2 5000
#define RIGHT_SPEED_3 7500
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
