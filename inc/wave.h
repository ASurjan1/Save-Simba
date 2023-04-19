// wave.h
// playing wav files.
//
// For use with the TM4C123
// Program written by: Arnav Surjan
#ifndef __WAVE_h
#define __WAVE_h
#include <stdint.h>
// initialize Wave functionality
void Wave_Init(void);

#define fanfaresize 16346
extern const uint8_t fanfare[16346];

#define roarsize 6954
extern const unsigned char roar[6954];
#define shieldsize 8269
extern const unsigned char shield[8269];
#define swordsize 2937
extern const unsigned char sword[2937];


//******* Wave_Start ************
// These functions do not output to the DAC. 
// Rather, they set a pointer and counter, and then enables the timer interrupt.
// It starts the sound, and the timer ISR does the output
// feel free to change the parameters
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Wave_Start(const uint8_t *pt, uint32_t count);
// stop wave output
void Wave_Stop(void);

void Wave_Fanfare(void);

void Wave_Roar(void);
void Wave_Shield(void);
void Wave_Sword(void); 


#endif
