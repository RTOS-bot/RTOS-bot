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

/* Temporary Interrupt to toggle isMoving for testing */
void PORTD_IRQHandler() {
	
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	delay(0x80000);
	isMoving ^= 1;
	
	//Clear INT flag
	PORTD->ISFR |= MASK(SW_POS);
}

void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//Receive Case
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;
	}
	
	//To-do: Add error handling case
}


void tBrain (void *argument) {

  for (;;) {	
		if (rx_data == V_TUNE) {
			victoryAudio();
		} 
	}
}

void tMotorControl (void *argument) {
 
  for (;;) {
		osMutexAcquire(selfDriveMutex, osWaitForever);
		if (rx_data != SD_CMD)
			isMoving = move(rx_data);
		osMutexRelease(selfDriveMutex);
	}
}

void tRedLED (void *argument) {
 
  for (;;) {
		if (isMoving)
			blinkRedLEDS(BLINK_1HZ);
		else
			blinkRedLEDS(BLINK_2HZ);
	}
	
}

void tGreenLED (void *argument) {
 
  for (;;) {
		if (isMoving)
			movingGreenLEDS();
		else
			onALLGreenLEDS();
	}
}

void tAudio (void *argument) {
 
  for (;;) {
		raiderMoveAudio();
	}
}

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

const osThreadAttr_t thread_attr = {
	.priority = osPriorityHigh
};


int main (void) {
 
  SystemCoreClockUpdate();
	initLEDS();
	initAudioPWM();
	initMotorPWM();
	initUART2(BAUD_RATE);
	initUltrasonic();

  osKernelInitialize();    

	
	musicSem = osSemaphoreNew(1,1,NULL);
	selfDriveMutex = osMutexNew(NULL);
	osThreadNew(tMotorControl, NULL, NULL);   
	osThreadNew (tSelfDrive, NULL, NULL);
  osThreadNew(tRedLED, NULL, NULL);   
  osThreadNew(tGreenLED, NULL, NULL);    
  osThreadNew(tBrain, NULL, NULL);    
  osThreadNew(tAudio, NULL, NULL);   
	

  osKernelStart();            
	
  for (;;) {}
}
