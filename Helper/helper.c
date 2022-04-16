#include "helper.h"
#include "MKL25Z4.h"                    // Device header


/* UTILITY FUNCTIONS */

void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	//Enable Clock Gating for UART2
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	
	//Enable Clock Gating for PORTE
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//Connect UART (RX) to port E pins
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	//Disable TX and RX + TX/RX Interrupts before configurations
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK) | (UART_C2_TIE_MASK) |(UART_C2_RIE_MASK));
	
	//Set Baud Rate
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->C2 = 0;
	UART2->C3 = 0;
	
	NVIC_SetPriority(UART2_IRQn, 128); //Set UART IRQ to priority 2
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	//Enable RX with RX Interrupts
	UART2->C2 |= ((UART_C2_RE_MASK)|(UART_C2_RIE_MASK));
	
}









