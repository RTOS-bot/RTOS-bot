#include "gpio.h"

void initGPIOPin(PORT_Type *port, uint8_t pin) {
	port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
	port->PCR[pin] |= PORT_PCR_MUX(1);	
}



