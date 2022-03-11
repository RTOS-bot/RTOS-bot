#include "MKL25Z4.h"                    // Device header
#include "helper.h"

volatile color_t led_color = RED;
volatile uint8_t rx_data = 0;
void PORTD_IRQHandler(void); 

/* MAIN FUNCTION */

int main(void)
{
	SystemCoreClockUpdate();
	initMotorPWM();
	initUART2(BAUD_RATE);
	
	while(1) {
		switch(rx_data) {
			case 0x40:
				dir_control(LEFT);
				break;
			case 0x41:
				dir_control(RIGHT);
				break;
			case 0x42:
				dir_control(FRONT);
				break;
			case 0x43:
				dir_control(BACK);
				break;
			case 0x44:
			default:
				dir_control(STOP);
				break;
			
		}
		dir_control(RIGHT);
	}
}

void PORTD_IRQHandler() {
	
	//Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	//Update led_color 
	switch(led_color) {
		case RED:
			led_color = GREEN;
			break;
		case GREEN:
			led_color = BLUE;
			break;
		case BLUE:
		default:
			led_color = RED;
			break;
	}
	
	//Clear INT flag
	PORTD->ISFR |= MASK(SW_POS);
}

void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//Receive Case
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;
	}
	
	//To-do: Add error handling case
}
