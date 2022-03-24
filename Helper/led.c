#include "MKL25Z4.h" 

#include "led.h"
#include "core.h"




uint8_t portAPins[] = {1,2,12,4,5};
uint8_t portBPins[] = {8};  // red
uint8_t portCPins[] = {7, 0, 3, 4, 5};


void initLEDS(void) {
	// Enable clock 
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | 
									SIM_SCGC5_PORTB_MASK | 
									SIM_SCGC5_PORTC_MASK );
	
	// initGPIO and set direction as output
	for (uint8_t i = 0; i < NUM_PINS_A ; i++) {
		initGPIOPin(PORTA, portAPins[i]);
		PTA->PDDR |= (MASK(portAPins[i]));
	}

	for (uint8_t i = 0; i < NUM_PINS_B; i++) {
		initGPIOPin(PORTB, portBPins[i]);
		PTB->PDDR |= (MASK(portBPins[i]));
	}	
	
	for (uint8_t i = 0; i < NUM_PINS_C; i++) {
		initGPIOPin(PORTC, portCPins[i]);
		PTC->PDDR |= (MASK(portCPins[i]));
	}	
}
	
	
void blinkRedLEDS (uint32_t delayTime) {
		PTB->PSOR = MASK(RED_LEDS);
		osDelay(delayTime);
		PTB->PCOR = MASK(RED_LEDS);
	  osDelay(delayTime);
}

void onALLGreenLEDS (void) {

	for (uint8_t i = 0; i < NUM_PINS_A ; i++) {
		PTA->PSOR = (MASK(portAPins[i]));
	}
	
	for (uint8_t i = 0; i < NUM_PINS_C ; i++) {
		PTC->PSOR = (MASK(portCPins[i]));
	}	
}

void offALLGreenLEDS (void) {

	for (uint8_t i = 0; i < NUM_PINS_A ; i++) {
		PTA->PCOR = (MASK(portAPins[i]));
	}
	
	for (uint8_t i = 0; i < NUM_PINS_C ; i++) {
		PTC->PCOR = (MASK(portCPins[i]));
	}
}


void movingGreenLEDS (void) {
		offALLGreenLEDS();
	
		for (uint8_t i = 0; i < NUM_PINS_A ; i++) {
			PTA->PSOR = (MASK(portAPins[i]));
			osDelay(BLINK_5HZ);
			PTA->PCOR = (MASK(portAPins[i]));
		}
	
		for (uint8_t i = 0; i < NUM_PINS_C ; i++) {
			PTC->PSOR = (MASK(portCPins[i]));
			osDelay(BLINK_5HZ);
			PTC->PCOR = (MASK(portCPins[i]));
		}	

}


