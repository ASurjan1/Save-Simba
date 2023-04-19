// ADC.h
// Runs on TM4C123
// Provide functions that initialize ADC0
// Student names: Arnav Surjan, Qiuyang Lu
// Last modification date: 3/26/2023

#ifndef ADC_H
#define ADC_H
#include <stdint.h>

// ADC initialization function 
// Input: # of samples to avg, as power of 2
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(uint32_t power);

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void);
#endif
