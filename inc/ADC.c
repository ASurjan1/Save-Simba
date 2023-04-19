// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Student names: Arnav Surjan
// Last modification date: 3/26/2023
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

//LEFT JOYSTICK PD0-1

// ADC initialization function using PD2 
// Input: number of samples to take and average, as a power of 2
// Output: none
void ADC_Init(uint32_t power){ 
	SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x08;    // 2) activate clock for Port D
  while((SYSCTL_PRGPIO_R&0x08) != 0x08){};  // 3 for stabilization
  GPIO_PORTD_DIR_R &= ~0x04;    // 4) make PD2 input
  GPIO_PORTD_AFSEL_R |= 0x04;   // 5) enable alternate function on PD2
  GPIO_PORTD_DEN_R &= ~0x04;    // 6) disable digital I/O on PD2
  GPIO_PORTD_AMSEL_R |= 0x04;   // 7) enable analog functionality on PD2
// while((SYSCTL_PRADC_R&0x0001) != 0x0001){}; // good code, but not implemented in simulator
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 5;           // 12) set channel (5)
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
	ADC0_SAC_R = power;
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	// 1) initiate SS3
  // 2) wait for conversion done
  // 3) read result
  // 4) acknowledge completion
	ADC0_PSSI_R = 0x0008;            					// 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   					// 2) wait for conversion done
  uint32_t result = ADC0_SSFIFO3_R & 0xFFF;	// 3) read result
  ADC0_ISC_R = 0x0008;             					// 4) acknowledge completion
  return result;
}


