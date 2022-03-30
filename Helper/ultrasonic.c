#include "ultrasonic.h"

/**
 * Initialise both the trigger and echo pins of the ultrasonic sensor
 * Trigger Pin: GPIO Output
 * Echo Pin: PWM Input
 */
void initUltrasonic() {
	initUltrasonicTrig();
	initUltrasonicEcho();
}

/**
 * Activates the ultrasonic sensor
 */
void activateUltrasonic() {
	
}

/**
 * Initialise the Trigger pin
 */
void initUltrasonicTrig() {
	initPortClockGating(PORT_US);
	initGPIOPin(PORT_US, TRIG_PIN);
	PTB->PDDR |= MASK(TRIG_PIN);
}

/**
 * Initialise PWM for the Echo pin
 */
void initUltrasonicEcho() {
	uint8_t pins[NUM_PWM_PINS] = {ECHO_PIN};
	uint8_t channels[NUM_TPM_CH] = {TRIG_TPM_CH};
	initPWM(PORT_US, US_ALT, pins, NUM_PWM_PINS, TPM_US, US_MOD, channels, NUM_TPM_CH);
	initInterrupt(TPM_US, channels, NUM_TPM_CH, 2);
}

/**
 * Outputs a momentary signal on the trigger pin
 * Port Data Output Register (FGPIOx_PDOR): Page 780
 */
void activateTrig() {
	PTD->PDOR &= ~MASK(TRIG_PIN);
	delay(24);
	PTD->PDOR |= MASK(TRIG_PIN);
	delay(120);
	PTD->PDOR &= ~MASK(TRIG_PIN);
}

/**
 * Handles the interrupt request
 */
void TPM2_IRQHandler() {
	
}



