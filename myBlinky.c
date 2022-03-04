#include "MKL25Z4.h"                    // Device header
#include "helper.h"

volatile color_t led_color = RED;
volatile uint8_t rx_data = 0;
void PORTD_IRQHandler(void); 

/* MAIN FUNCTION */

int main(void)
{
	SystemCoreClockUpdate();
	initLEDs();
	clearLEDs();
	initUART2(BAUD_RATE);
	
	while(1) {
		
		
		switch (rx_data) {
			case 0x31:
				led_control(RED);
				break;
			case 0x33:
				led_control(GREEN);
				break;
			case 0x35:
				led_control(BLUE);
				break;
			case 0x30:
			case 0x32:
			case 0x34:
				clearLEDs();
				break;
			default:
				clearLEDs();
				break;
		}
		delay(0x80000);
		
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
	
	//Transmit Case
	if (UART2->S1 & UART_S1_TDRE_MASK) {
		UART2->D = 0x69;
	}
	
	//Receive Case
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;
	}
	
	//To-do: Add error handling case
}
