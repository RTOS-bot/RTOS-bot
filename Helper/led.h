#ifndef LED_H
#define LED_H
#include "gpio.h"
#include "cmsis_os2.h"

#define NUM_PINS_A 4
#define NUM_PINS_B 1
#define NUM_PINS_C 5

#define RED_LEDS 8

#define BLINK_1HZ 500
#define BLINK_2HZ 250
#define BLINK_5HZ 100


void initLEDS(void);
void blinkRedLEDS (uint32_t delayTime);
void onALLGreenLEDS(void);
void offALLGreenLEDS(void);
void movingGreenLEDS (void) ;

#endif


