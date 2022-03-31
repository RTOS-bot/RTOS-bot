#include "ultrasonic.h"

volatile bool isUltrasonicActivated = false;
volatile bool hasTimerStart = false;
volatile uint8_t collisionWarningFlag = 0;
volatile uint32_t startTime = 0;
volatile uint32_t currTime = 0;

/**
 * Initialise both the trigger and echo pins of the ultrasonic sensor
 * Trigger Pin: GPIO Output
 * Echo Pin: PWM Input
 */
void initUltrasonic(void) {
	initUltrasonicTrig();
	initUltrasonicEcho();
}

/**
 * Activates the ultrasonic sensor by sending a pulse every 60ms
 * Terminates only when a obstacle is within a set threshold
 */
void activateUltrasonic(void) {
	isUltrasonicActivated = true;
	while (collisionWarningFlag == 0) {
		activateTrig();
		osDelay(60);
	}
	isUltrasonicActivated = false;
}

/**
 * Initialise the Trigger pin to a GPIO output
 */
void initUltrasonicTrig(void) {
	initPortClockGating(PORT_US);
	initGPIOPin(PORT_US, TRIG_PIN);
	PTB->PDDR |= MASK(TRIG_PIN);
}

/**
 * Initialise the Echo pin and a corresponding interrupt
 * Pin Control Register n (PORTx_PCRn): Page 183 - 185
 */
void initUltrasonicEcho(void) {
	initPortClockGating (PORT_US);
	initGPIOPin(PORT_US, ECHO_PIN);
	PORT_US->PCR[ECHO_PIN] |= PORT_PCR_IRQC(0x0b);  // Enable interrupt on either edge
	PORT_US->PCR[ECHO_PIN] &= ~PORT_PCR_PS_MASK;		// Enable pull select
	PORT_US->PCR[ECHO_PIN] |= PORT_PCR_PE_MASK;     // Enable pull enable
	NVIC_SetPriority(PORTD_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

/**
 * Outputs a momentary signal on the trigger pin
 * Port Data Output Register (FGPIOx_PDOR): Page 780
 */
void activateTrig(void) {
	PTD->PDOR |= MASK(TRIG_PIN);
	osDelay(1);
	PTD->PDOR &= ~MASK(TRIG_PIN);
}

/**
 * Handles the interrupt request:
 * (1) Checks if trigger pulse has been sent
 * (2) If sent check if timer has started
 *     --> Timer not started: starts timer
 *     --> Timer has started: proceed with (3)
 * (3) Compares the difference between current time and start time against threshold
 *     --> Difference less than threshold: set collision warning flag
 *     --> Difference more than threshold: does nothing for this trigger-echo cycle
 * (4) Sets logic of timer start back to false
 * (5) Clear the interrupt status flag
 */
void PORTD_IRQHandler(void) {
	
	if (isUltrasonicActivated) {
		if (!hasTimerStart) {
			hasTimerStart = true;
			startTime = osKernelGetSysTimerCount();
		} else if (hasTimerStart) {
			currTime = osKernelGetSysTimerCount();
			if ((currTime - startTime) <= THRESHOLD)
				collisionWarningFlag = 1;
		}
		hasTimerStart = false;
	}
	PORT_US->ISFR |= MASK(ECHO_PIN);
}



