#include "pwm.h"

/**
 * Initialises a PWM signal
 * 
 * @param port					1 of the ports from PORTA to PORTE
 * @param alt						ALT functionality
 * @param pins					List of pins belonging to a port
 * @param numOfPins			Number of pins in the pin list
 * @param timer					Timer/PWM module to enable clock gating (TPM0, TPM1, TPM2)			
 * @param mod						Determines clock frequency using 375000/mod Hz
 * @param channels			List of channels of Timer/PWM module (Channel 0 to 5)
 * @param numOfChannels	Number of channels in the channel list
 */
void initPWM(PORT_Type *port, uint8_t alt, uint8_t pins[], uint8_t numOfPins, 
	TPM_Type *timer, uint32_t mod, uint8_t channels[], uint8_t numOfChannels) {
	initPortClockGating(port);
	initPort(port, pins, numOfPins, alt);
	initTimerClockGating(timer);
	initModule(timer, mod);
	initChannel(timer, channels, numOfChannels);
}

/**
 * Enable clock gating for a Port
 * System Clock Gating Control Register 5 (SIM_SCGC5): Page 206 - 207
 * 
 * @param port	Port to enable clock gating (PORTA, PORTB, PORTC, PORTD, PORTE)
 */
void initPortClockGating(PORT_Type *port) {
	SIM->SCGC5 |= getPortMask(port);
}

/**
 * Initialises pin to a chosen ALT functionality 
 * KL25 Signal Multiplexing and Pin Assignments: Page 161 - 164
 *
 * @param port			Ports A to E
 * @param pins			List of pins of a port
 * @param numOfPins	Number of pins in the pin list
 * @param alt				ALT functionality
 */
void initPort(PORT_Type *port, uint8_t pins[], uint8_t numOfPins, uint8_t alt) {
  	for (uint8_t i = 0; i < numOfPins; i++) {
		port->PCR[pins[i]] &= ~PORT_PCR_MUX_MASK;
		port->PCR[pins[i]] |= PORT_PCR_MUX(alt);
  	}
}

/**
 * Enable clock gating for the Timer/PWM module
 * System Clock Gating Control Register 6 (SIM_SCGC6): Page 207 - 209
 * 
 * @param timer	Timer/PWM module to enable clock gating (TPM0, TPM1, TPM2)
 */
void initTimerClockGating(TPM_Type *timer) {
	if (timer == TPM0)
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	else if (timer == TPM1)
		SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	else
		SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
}

/**
 * Set up the Timer/PWM module as an edge-aligned PWM
 * System Options Register 2 (SIM_SOPT2): Page 195 - 197
 * Status and Control (TPMx_SC): Page 552 - 553
 * Modulo (TPMx_MOD): Page 554
 * 
 * @param timer	Timer/PWM module to initialise (TPM0, TPM1, TPM2)
 * @param mod		Determines clock frequency using 375000/mod Hz
 */
void initModule(TPM_Type *timer, uint32_t mod) {
	// Select 01: MCGFLLCLK clock or MCGPLLCLK/2 for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);	// 01: 
	
	// Set Modulo Value 48000000/128 = 375000/mod
	TPM_MOD_REG(timer) = mod;
	
	// Edge-Aligned PWM
	// Update SnC register:
	// CMOD = 01 (LPTPM counter increments on every LPTPM counter clock)
	// PS = 111 (Divide by 128)
	TPM_SC_REG(timer) &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM_SC_REG(timer) |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM_SC_REG(timer) &= ~(TPM_SC_CPWMS_MASK);
}

/**
 * Starts the PWM signal
 * Channel (n) Status and Control (TPMx_CnSC): Page 555 - 556
 * 
 * @param timer   			Timer/PWM module to initialise (TPM0, TPM1, TPM2)
 * @param channels			List of channels of Timer/PWM module (Channel 0 to 5)
 * @param numOfChannels	Number of channels in the list
 */
void initChannel(TPM_Type *timer, uint8_t channels[], uint8_t numOfChannels) {
	// MS0B:MS0A = '10' and ELS0B:ELS0A = '10': high when up-counting (high true pulses)
	for (uint8_t i = 0; i < numOfChannels; i++) {
		TPM_CnSC_REG(timer, channels[i]) &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
		TPM_CnSC_REG(timer, channels[i]) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	}	
}

/**
 * Enables channel interrupts for a timer
 * Channel (n) Status and Control (TPMx_CnSC): Page 555 - 556
 * 
 * @param timer   			Timer/PWM module to initialise (TPM0, TPM1, TPM2)
 * @param channels			List of channels of Timer/PWM module (Channel 0 to 5)
 * @param numOfChannels	Number of channels in the list
 * @param priority      Priority of the interrupt
 */
void initInterrupt(TPM_Type *timer, uint8_t channels[], uint8_t numOfChannels, uint8_t priority) {
	TPM_SC_REG(timer) |= (TPM_SC_TOIE_MASK);
	
	for (uint8_t i = 0; i < numOfChannels; i++) {
		TPM_CnSC_REG(timer, channels[i]) &= ~(TPM_CnSC_CHIE_MASK);
		TPM_CnSC_REG(timer, channels[i]) |= (TPM_CnSC_CHIE(1));
	}
	
	IRQn_Type interrupt;
	if (timer == TPM0)
		interrupt = TPM0_IRQn;
	else if (timer == TPM1)
		interrupt = TPM1_IRQn;
	else if (timer == TPM2)
		interrupt = TPM2_IRQn;
	
	NVIC_SetPriority(interrupt, priority);
	NVIC_ClearPendingIRQ(interrupt);
	NVIC_EnableIRQ(interrupt);
}


