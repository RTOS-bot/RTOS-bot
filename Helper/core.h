#ifndef CORE_H
#define CORE_H

#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include <stdbool.h>

#define MASK(x) (1 << (x))

extern osSemaphoreId_t musicSem;
extern osSemaphoreId_t selfDrivingSem;

uint32_t getPortMask(PORT_Type *port);

static void delay(volatile uint32_t nof);

#endif

