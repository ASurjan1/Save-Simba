// SaveSimba.c
// Runs on TM4C123

#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "../inc/ADC.h"
#include "../inc/wave.h"
#include "Timer1.h"
#include "Timer0.h"
#include "Buttons.h"
#include "LeftJoystick.h"
#include "PortInits.h"
#include "../inc/DAC.h"
#include "GameEngine.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}

int main(void){ char l;
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
	Buttons_Init();
	LeftJoystick_Init(6);
	PortB_Init();
	ST7735_InitR(INITR_BLACKTAB);
	Wave_Init();
	Song_Init();
	Timer1_Init(80000000/1000, 3);
	EnableInterrupts();
  Random_Init(243671);
	while(1){
		mainEngine();
	}
}

