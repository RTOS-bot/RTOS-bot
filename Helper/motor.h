#ifndef MOTORS_H
#define MOTORS_H
#include "PWM.h"
#include "ultrasonic.h"

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

#define LF_CMD 0x40
#define RF_CMD 0x41
#define F_CMD	 0x42
#define B_CMD	 0x43
#define LB_CMD 0x45
#define RB_CMD 0x46
#define V_TUNE 0x47
#define SD_CMD 0x48

#define RIGHT_SPEED_0 300
#define RIGHT_SPEED_1 3750
#define RIGHT_SPEED_2 6000
#define RIGHT_SPEED_3 7500
#define LEFT_SPEED_0  300
#define LEFT_SPEED_1  3750
#define LEFT_SPEED_2  5700
#define LEFT_SPEED_3  7500

#define LT_SPD_DL 200
#define RT_SPD_DL 350
#define RT_SPD_DL2 450

typedef enum dir_t {
	LEFT, 
	RIGHT, 
	FORWARD, 
	BACKWARD, 
	STOP
} dir_t;

void initMotorPWM(void);

char move(uint8_t rx_data);

void selfDriveSequence(volatile char *isMoving);
void turnSlightLeft(void);
void turnSlightRight(void);
void moveStop(void);
void goForwardA(void);
void goForwardB(void);
void goForwardC(void);
void turn90RightA(void);
void turn90RightB(void);
void turn90LeftA(void);
void turn90LeftB(void);

void movementControl(dir_t direction, uint16_t speedRight, uint16_t speedLeft);


void clearTPM(void);

#endif
