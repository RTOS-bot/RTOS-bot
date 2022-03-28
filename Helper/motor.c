#include "motor.h"

/**
 * Initialise PWM for the 4 motors
 */
void initMotorPWM(void) {
	uint8_t pinsRight[NUM_PINS_MOTOR_RIGHT] = {PTB0_PIN, PTB1_PIN};	
	uint8_t pinsLeft[NUM_PINS_MOTOR_LEFT] = {PTB2_PIN, PTB3_PIN};
	uint8_t channelsRight[NUM_CHANNELS_MOTOR_RIGHT] = {TPM_CHANNEL0, TPM_CHANNEL1};
	uint8_t channelsLeft[NUM_CHANNELS_MOTOR_LEFT] = {TPM_CHANNEL0, TPM_CHANNEL1};
	
	initPWM(PORT_MOTOR_RIGHT, MOTOR_ALT, pinsRight, NUM_PINS_MOTOR_RIGHT, TPM_MOTOR_RIGHT, MOTOR_MOD, channelsRight, NUM_CHANNELS_MOTOR_RIGHT);
	initPWM(PORT_MOTOR_LEFT, MOTOR_ALT, pinsLeft, NUM_PINS_MOTOR_LEFT, TPM_MOTOR_LEFT, MOTOR_MOD, channelsLeft, NUM_CHANNELS_MOTOR_LEFT);
}

/**
 * Intepret data received from ESP32 to move the motors
 *
 * @param rx_data Hexadecimal command code received from ESP32
 */
char move(uint8_t rx_data) {
	switch (rx_data) {
		case LF_CMD:
			movementControl(FORWARD, RIGHT_SPEED_3, LEFT_SPEED_0);
			return 1;
		case RF_CMD:
			movementControl(FORWARD, RIGHT_SPEED_0, LEFT_SPEED_3);
			return 1;
		case F_CMD:
			movementControl(FORWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
			return 1;
		case B_CMD:
			movementControl(BACKWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
			return 1;
		case LB_CMD:
			movementControl(BACKWARD, RIGHT_SPEED_3, LEFT_SPEED_0);
			return 1;
		case RB_CMD:
			movementControl(BACKWARD, RIGHT_SPEED_0, LEFT_SPEED_3);
			return 1;
		default:
			movementControl(STOP, 0, 0);
			return 0;
	}
}

/**
 * Controls the movement of the wheels
 *
 * TPM1_C0V: Right wheels move forward
 * TPM1_C1V: Right wheels move back
 * TPM2_C0V: Left wheels move back
 * TPM2_C1V: Left wheels move forward
 * 
 * @param direction		Direction to move the robot
 * @param speedRight	Speed of the right wheels
 * @param speedLeft		Speed of the left wheels
 */
void movementControl(dir_t direction, uint16_t speedRight, uint16_t speedLeft) {
	
	switch(direction) {
		case FORWARD:
			TPM1_C1V = 0;
			TPM2_C0V = 0;
			TPM1_C0V = speedRight;
			TPM2_C1V = speedLeft;
			break;
		case BACKWARD:
			TPM1_C0V = 0;
		  TPM2_C1V = 0;
			TPM1_C1V = speedRight;
			TPM2_C0V = speedLeft;
			break;
		case LEFT:
			TPM1_C1V = 0;
			TPM2_C1V = 0;
			TPM1_C0V = speedRight;
			TPM2_C0V = speedLeft;
			break;
		case RIGHT:
			TPM1_C0V = 0;
		  TPM2_C0V = 0;
			TPM1_C1V = speedRight;
			TPM2_C1V = speedLeft;
			break;
		case STOP:
			TPM1_C0V = speedRight;
			TPM1_C1V = speedRight;
			TPM2_C0V = speedLeft;
			TPM2_C1V = speedLeft;
			break;
	}
}




