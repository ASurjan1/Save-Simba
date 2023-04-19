//PortInits.c
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x20;      // activate port F
  while((SYSCTL_PRGPIO_R&0x20) != 0x20){};
  GPIO_PORTF_DIR_R |=  0x0E;   // output on PF3,2,1 (built-in LED)
  GPIO_PORTF_PUR_R |= 0x10;
  GPIO_PORTF_DEN_R |=  0x1E;   // enable digital I/O on PF
}

// Initialize Port B so PB6 and PB7 are heartbeats
void PortB_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x02;      // activate port B
  while((SYSCTL_PRGPIO_R&0x02) != 0x02){};
  GPIO_PORTB_DIR_R |=  0xC0;   // output on PB6,7
  GPIO_PORTB_DEN_R |=  0xC0;   // enable digital I/O on PB
}