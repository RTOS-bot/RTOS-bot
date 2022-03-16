#ifndef PWM_H
#define PWM_H

#include "core.h"

#define FREQ_2_MOD(x)	(375000 / x)

void initPWM(PORT_Type *port, uint8_t alt, uint8_t pins[], uint8_t numOfPins, 
	TPM_Type *timer, uint32_t mod, uint8_t channels[], uint8_t numOfChannels);

void initPortClockGating(PORT_Type *port);

void initPort(PORT_Type *port, uint8_t pins[], uint8_t numOfPins, uint8_t alt);

void initTimerClockGating(TPM_Type *timer);

void initModule(TPM_Type *timer, uint32_t mod);

void initChannel(TPM_Type *timer, uint8_t channels[], uint8_t numOfChannels);

#endif

