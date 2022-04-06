#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "MKL25Z4.h"
#include "PWM.h"
#include "gpio.h"
#include "core.h"
#include "motor.h"

#define PORT_US   PORTD
#define TRIG_PIN  6
#define ECHO_PIN  7
#define THRESHOLD 45000  // Change this to suit threshold required

void initUltrasonic(void);
void activateUltrasonic(void);

void initUltrasonicTrig(void);
void initUltrasonicEcho(void);
void activateTrig(void);
void PORTD_IRQHandler(void);

#endif