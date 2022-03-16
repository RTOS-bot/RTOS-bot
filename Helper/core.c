#include "core.h"

/**
 * Obtains the port mask for SIM_SCGC5 register
 * 
 * @param port One of the ports from PORTA to PORTE
 * @return SIM_SCGC5 Mask corresponding to the port
 */
uint32_t getPortMask(PORT_Type *port) {
  if (port == PORTA)
    return SIM_SCGC5_PORTA_MASK;
  else if (port == PORTB)
    return SIM_SCGC5_PORTB_MASK;
  else if (port == PORTC)
    return SIM_SCGC5_PORTC_MASK;
  else if (port == PORTD)
    return SIM_SCGC5_PORTD_MASK;
  else
    return SIM_SCGC5_PORTE_MASK;
}

/**
 * Delay function
 *
 * @param nof Number of instruction cycles to delay
 */
static void delay (volatile uint32_t nof) {
	while (nof != 0) {
		__asm("NOP");
		nof--;
	}
}
