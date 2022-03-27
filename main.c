#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "Helper/helper.h"
#include "Helper/motor.h"
#include "Helper/gpio.h"
#include "Helper/led.h"
#include "Helper/audio.h"


volatile uint8_t rx_data = 0;
char isMoving = 0;

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
		/*
		switch(rx_data) {
			case 0x31:
				led_control(RED);
				break;
			case 0x33:
				led_control(GREEN);
				break;
			case 0x35:
				led_control(BLUE);
				break;
			default:
				clearLEDs();
				break;
		}
		*/
		isMoving = move(rx_data);
	}
}

void tMotorControl (void *argument) {
 
  for (;;) {
		movementControl(LEFT, RIGHT_SPEED_1, LEFT_SPEED_1);
		movementControl(RIGHT, RIGHT_SPEED_1, LEFT_SPEED_1);
		movementControl(FORWARD, RIGHT_SPEED_1, LEFT_SPEED_1);
		movementControl(BACKWARD, RIGHT_SPEED_1, LEFT_SPEED_1);
		movementControl(STOP, 0, 0);
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
		moveAudioRaider();

	}
}


const osThreadAttr_t thread_attr = {
	.priority = osPriorityHigh
};


int main (void) {
 
  SystemCoreClockUpdate();
	initLEDs();
	initLEDS();
	initAudioPWM();
	initMotorPWM();
	initUART2(BAUD_RATE);
	//initSwitch();
	

  osKernelInitialize();                 
	//osThreadNew(tMotorControl, NULL, NULL);    
  osThreadNew(tRedLED, NULL, NULL);   
  osThreadNew(tGreenLED, NULL, NULL);    
  osThreadNew(tAudio, NULL, NULL);    
	osThreadNew(tBrain, NULL, NULL);
  osKernelStart();                      
  for (;;) {}
		
}
