#include "Buttons.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"

void Buttons_Init(void){   
	SYSCTL_RCGCGPIO_R |= 0x10; // (a) activate clock for port E
	while((SYSCTL_PRGPIO_R&0x10) != 0x10){};  // 3 for stabilization
	GPIO_PORTE_DIR_R &= ~0x0F;	// (c) make PE3-0 in (built-in button)
	GPIO_PORTE_DEN_R |= 0x0F; 	// 	enable digital I/O on PE3-0
	/*
	GPIO_PORTE_IS_R &= ~0x0F; 	// (d) PE3-0 is edge-sensitive
	GPIO_PORTE_IBE_R &= ~0x0F;	// 	PE3-0 is not both edges
	GPIO_PORTE_IEV_R &= ~0x0F;	// 	PE3-0 falling edge event
	GPIO_PORTE_ICR_R = 0x0F;  	// (e) clear flag3,2
	GPIO_PORTE_IM_R  |= 0x0F;  	// (f) arm interrupt on PE3-0
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFFF1F)|0x00000020; // (g) priority 1 (bits 7-5)
	NVIC_EN0_R = 0x00000010;  	// (h) enable interrupt 4 in NVIC
	*/
}

void GPIOPortE_Handler(void){ static uint32_t last0 = 0;
	/*
	uint32_t now0 = (GPIO_PORTE_DATA_R & 0x0F);
	
	if (((now0 & 0x04) == 0x04)){ // Just pressed
		GPIO_PORTB_DATA_R ^= 0x40; 	// toggle PB6   
		
	} else if (((now0 & 0x08) == 0x08)){
		GPIO_PORTB_DATA_R ^= 0x80; 	// toggle PB7
	}
	
	if (((now0 & 0x04) == 0x00) && ((last0 & 0x04) == 0x04)){ // Just released
  
	} else if (((now0 & 0x08) == 0x00) && ((last0 & 0x08) == 0x08)){

	} 
	
	
	last0 = now0;
	GPIO_PORTE_ICR_R = 0x0F;  	// acknowledge flag
	*/
}
