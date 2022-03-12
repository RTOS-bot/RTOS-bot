#include "helper.h"
#include "MKL25Z4.h"                    // Device header


/* UTILITY FUNCTIONS */

void initLEDs(void) {
	
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
	
}

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

void UART2_Transmit_Poll(uint8_t data) {
	
		while (!(UART2->S1 & UART_S1_TDRE_MASK));
		UART2->D = data;
}

uint8_t UART2_Receive_Poll(void) {
	
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}

void clearLEDs(void) {
	// Set Data Output Registers of PortB and PortD to all 1 (active low)
	PTB->PDOR |= ~(0);
	PTD->PDOR |= ~(0);
}

void led_control(color_t color) {
	// Clear before selecting register to write to, in the event that 2 ports are written together
	clearLEDs();
	switch (color) {
		case RED:
			PTB->PDOR = ~MASK(RED_LED);
			break;
		case GREEN:
			PTB->PDOR = ~MASK(GREEN_LED);
			break;
		case BLUE:
			PTD->PDOR = ~MASK(BLUE_LED);
			break;
	}
}

/*  UTILITY FUNCTION */
void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

/* PROBABLY NEVER USED FUNCTIONS */
void initSwitch(void) {
	// Enable Clock to PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Select GPIO, enable pull-up resistors and interrupts (falling edge)
	PORTD->PCR[SW_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) |
												PORT_PCR_PS_MASK |
	                      PORT_PCR_PE_MASK |
	                      PORT_PCR_IRQC(0x0a));
	
	// Set Switch as Input
	PTD->PDDR &= ~MASK(SW_POS);
	
	// Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn, 2); //priority can be any no. since its the only one w prio
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

