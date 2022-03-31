#include "motor.h"

/**
 * Initialise PWM for the 4 motors
 */
void initMotorPWM(void) {
	uint8_t pinsRight[NUM_PINS_MOTOR_RIGHT] = {PTD0_PIN, PTD1_PIN};	
	uint8_t pinsLeft[NUM_PINS_MOTOR_LEFT] = {PTD2_PIN, PTD3_PIN};
	uint8_t channelsRight[NUM_CHANNELS_MOTOR_RIGHT] = {TPM_CHANNEL0, TPM_CHANNEL1};
	uint8_t channelsLeft[NUM_CHANNELS_MOTOR_LEFT] = {TPM_CHANNEL2, TPM_CHANNEL3};
	
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
			movementControl(LEFT, RIGHT_SPEED_3, LEFT_SPEED_0);
			return 1;
		case RF_CMD:
			movementControl(RIGHT, RIGHT_SPEED_0, LEFT_SPEED_3);
			return 1;
		case F_CMD:
			movementControl(FORWARD, RIGHT_SPEED_3, LEFT_SPEED_3);
			return 1;
		case B_CMD:
			movementControl(BACKWARD, RIGHT_SPEED_3, LEFT_SPEED_3);
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

void selfDriveSequence(void) {
	//Ultrasonic Code
	movementControl(FORWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
	activateUltrasonic();
	
	//Movement Code
	movementControl(STOP, 0, 0);
	osDelay(500);
	movementControl(LEFT, RIGHT_SPEED_3, LEFT_SPEED_0);
	osDelay(300);
	movementControl(FORWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
	osDelay(600);
	movementControl(RIGHT, RIGHT_SPEED_0, LEFT_SPEED_3);
	osDelay(1500);
	movementControl(FORWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
	osDelay(400);
	movementControl(LEFT, RIGHT_SPEED_3, LEFT_SPEED_0);
	osDelay(300);
	movementControl(FORWARD, RIGHT_SPEED_3, LEFT_SPEED_3);
	osDelay(1000);
	movementControl(STOP, 0, 0);
	osDelay(1000);
	
	// Move forward until obstacle is detected
	movementControl(FORWARD, RIGHT_SPEED_2, LEFT_SPEED_2);
	activateUltrasonic();
	
	// Stop all movement
	movementControl(STOP, 0, 0);
}

/**
 * Controls the movement of the wheels
 *
 * TPM0_C0V: Right wheels move forward
 * TPM0_C1V: Right wheels move back
 * TPM0_C2V: Left wheels move back
 * TPM0_C3V: Left wheels move forward
 * 
 * @param direction		Direction to move the robot
 * @param speedRight	Speed of the right wheels
 * @param speedLeft		Speed of the left wheels
 */
void movementControl(dir_t direction, uint16_t speedRight, uint16_t speedLeft) {
	
	switch(direction) {
		case FORWARD:
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C0V = speedRight;
			TPM0_C3V = speedLeft;
			break;
		case BACKWARD:
			TPM0_C0V = 0;
		  TPM0_C3V = 0;
			TPM0_C1V = speedRight;
			TPM0_C2V = speedLeft;
			break;
		case LEFT:
			TPM0_C1V = 0;
			TPM0_C3V = 0;
			TPM0_C0V = speedRight;
			TPM0_C2V = speedLeft;
			break;
		case RIGHT:
			TPM0_C0V = 0;
		  TPM0_C2V = 0;
			TPM0_C1V = speedRight;
			TPM0_C3V = speedLeft;
			break;
		case STOP:
			TPM0_C0V = speedRight;
			TPM0_C1V = speedRight;
			TPM0_C2V = speedLeft;
			TPM0_C3V = speedLeft;
			break;
	}
}