#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "Helper/helper.h"
#include "Helper/motor.h"
#include "Helper/gpio.h"
#include "Helper/led.h"
#include "Helper/audio.h"
#include "Helper/core.h"
#include "Helper/ultrasonic.h"

volatile uint8_t rx_data = 0;
volatile char isMoving = 0;
osMutexId_t selfDriveMutex;

const osThreadAttr_t thread_attr = {
	.priority = osPriorityHigh
};

/**
 * Obtains data from the ESP32
 * UART Status Register 1 (UARTx_S1): Page 729 - 731
 * UART Data Register (UARTx_D): Page 734 - 735
 */
void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//Receive Case
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;
	}
}

/*----------------------------------------------------------------------------
 * Application brain thread
 *---------------------------------------------------------------------------*/
void tBrain (void *argument) {

	for (;;) {	
		if (rx_data == V_TUNE) {
			victoryAudio();
		} 
	}
}

/*----------------------------------------------------------------------------
 * Application motor control thread
 *---------------------------------------------------------------------------*/
void tMotorControl (void *argument) {
 
	for (;;) {
		osMutexAcquire(selfDriveMutex, osWaitForever);
		if (rx_data != SD_CMD)
			isMoving = move(rx_data);
		osMutexRelease(selfDriveMutex);
	}
}

/*----------------------------------------------------------------------------
 * Application red led thread
 *---------------------------------------------------------------------------*/
void tRedLED (void *argument) {
 
	for (;;) {
		if (isMoving)
			blinkRedLEDS(BLINK_1HZ);
		else
			blinkRedLEDS(BLINK_2HZ);
	}
	
}

/*----------------------------------------------------------------------------
 * Application green led thread
 *---------------------------------------------------------------------------*/
void tGreenLED (void *argument) {
 
	for (;;) {
		if (isMoving)
			movingGreenLEDS();
		else
			onALLGreenLEDS();
	}
}

/*----------------------------------------------------------------------------
 * Application audio thread
 *---------------------------------------------------------------------------*/
void tAudio (void *argument) {
 
	for (;;) {
		raiderMoveAudio();
	}
}

/*----------------------------------------------------------------------------
 * Application self driving thread
 *---------------------------------------------------------------------------*/
void tSelfDrive (void *argument) {
	
	for	(;;) {
		if (rx_data == SD_CMD) {
			osMutexAcquire(selfDriveMutex, osWaitForever);
			isMoving = 1;
			selfDriveSequence();
			osMutexRelease(selfDriveMutex);
		}
	}
}


void tTest (void *argument) {
	
	for (;;) {
		activateUltrasonic();
	}
	
}


int main (void) {
 
	SystemCoreClockUpdate();
	initLEDS();
	initAudioPWM();
	initMotorPWM();
	initUART2(BAUD_RATE);
	initUltrasonic();

	osKernelInitialize();    
	
	musicSem = osSemaphoreNew(1, 1, NULL);
	selfDriveMutex = osMutexNew(NULL);
	osThreadNew(tMotorControl, NULL, NULL);  // Create application motor control thread
	osThreadNew (tSelfDrive, NULL, NULL);    // Create application self drive thread
	osThreadNew(tRedLED, NULL, NULL);        // Create application red led thread
	osThreadNew(tGreenLED, NULL, NULL);      // Create application green led thread
	osThreadNew(tBrain, NULL, NULL);         // Create application brain thread
	osThreadNew(tAudio, NULL, NULL);         // Create application audio thread
	//osThreadNew(tTest, NULL, NULL);
	osKernelStart();
	
	for (;;) {}
}
