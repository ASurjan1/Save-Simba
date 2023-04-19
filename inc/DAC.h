// DAC.h
// This software configures DAC output - 6-bit
// Runs on TM4C123
// Program written by: Arnav Surjan
// Last Modified: 4/19/23
// Hardware connections: PORTB 0-5

#ifndef DAC_H
#define DAC_H
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint8_t data);

#endif
