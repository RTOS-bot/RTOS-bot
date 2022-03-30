#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "MKL25Z4.h"
#include "PWM.h"
#include "gpio.h"
#include "core.h"

#define PORT_US  PORTB
#define NUM_PWM_PINS 1
#define TRIG_PIN 2
#define ECHO_PIN 3

#define TPM_US TPM2
#define TRIG_TPM_CH 0
#define NUM_TPM_CH  1

#define US_MOD 7500
#define US_ALT 3

void initUltrasonic();
void initUltrasonicTrig();
void initUltrasonicEcho();
void activateTrig();


#endif