// GameEngine.c

#include "GameEngine.h"
#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "../inc/wave.h"
#include "Timer1.h"
#include "Timer0.h"
#include "Images.h"
#include "Buttons.h"
#include "LeftJoystick.h"
#include "Random.h"

const int16_t SkyBlue = 0xFDE0;
const int16_t HillGreen = 0x0666;
const int16_t WoodBrown = 0x43D4;
const int16_t GrassGreen = 0x0120;
const int16_t BloodRed = 0xFDE0;
const int16_t StripeBrown = 0x1988;
const int16_t LeafGreen = 0x0342;


void showTitleScreen(void){
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, TitleScreen, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ST7735_DrawBitmap(50, 30, TitleCredits, TITLECREDITS_WIDTH, TITLECREDITS_HEIGHT);
	ST7735_DrawBitmap(0, 55, TitleText, TITLETEXT_WIDTH, TITLETEXT_HEIGHT);
	ST7735_DrawBitmap(105, 85, TitleSimba, TITLESIMBA_WIDTH, TITLESIMBA_HEIGHT);
	while ((GPIO_PORTE_DATA_R & 0x03) == 0x00){}
	while ((GPIO_PORTE_DATA_R & 0x03) == 0x01){}
}


uint8_t language;
void languageSelect(void){
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, TitleScreen, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ST7735_DrawBitmap(0, 55, TitleText, TITLETEXT_WIDTH, TITLETEXT_HEIGHT);
	ST7735_DrawBitmap(105, 85, TitleSimba, TITLESIMBA_WIDTH, TITLESIMBA_HEIGHT);
	ST7735_DrawStringBG(1, 7, "R1:ENGLISH", ST7735_WHITE, SkyBlue);
	ST7735_DrawStringBG(1, 8, "R2:FRANCAIS", ST7735_WHITE, SkyBlue);
	while ((GPIO_PORTE_DATA_R & 0x0C) == 0x00){}
	if ((GPIO_PORTE_DATA_R & 0x08) == 0x08){
		language = 0;
	} else if ((GPIO_PORTE_DATA_R & 0x04) == 0x04){
		language = 1;
	}
	switch (language){
		case 0:
			GPIO_PORTB_DATA_R |= 0x80; 	// toggle PB7
			ST7735_DrawStringBG(1, 7, "R1:ENGLISH", ST7735_RED, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(1, 7, "R1:ENGLISH", ST7735_WHITE, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(1, 7, "R1:ENGLISH", ST7735_RED, SkyBlue);
			break;
		case 1:
			GPIO_PORTB_DATA_R |= 0x40; 	// toggle PB6
			ST7735_DrawStringBG(1, 8, "R2:FRANCAIS", ST7735_GREEN, SkyBlue); //FIGURE OUT ACCENT FRAN�AIS
			delay100ms(2);
			ST7735_DrawStringBG(1, 8, "R2:FRANCAIS", ST7735_WHITE, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(1, 8, "R2:FRANCAIS", ST7735_GREEN, SkyBlue);
			break;
	}
	delay100ms(5);
	ST7735_DrawStringBG(1, 10, "L2:==>", ST7735_WHITE, HillGreen);
	while ((GPIO_PORTE_DATA_R & 0x03) == 0x00){}
	while ((GPIO_PORTE_DATA_R & 0x03) == 0x01){}
	ST7735_DrawStringBG(1, 10, "L2:==>", ST7735_YELLOW, HillGreen);
	delay100ms(2);
	ST7735_DrawStringBG(1, 10, "L2:==>", ST7735_WHITE, HillGreen);
	delay100ms(2);
	ST7735_DrawStringBG(1, 10, "L2:==>", ST7735_YELLOW, HillGreen);
	GPIO_PORTB_DATA_R &= ~0xC0; 	// Turn LEDS off
}


struct Entity{
	uint8_t x;
	uint8_t y;
	uint8_t lives;
	uint8_t width;
	uint8_t height;
} typedef Entity_t;
	
Entity_t Simba;
Entity_t Hyena1;
Entity_t Hyena2;
Entity_t Hyena3;
Entity_t Scar;
Entity_t Friend1;
Entity_t Friend2;
Entity_t Friend3;
Entity_t Home;
static uint8_t orient;
static uint8_t lxNew;
static uint8_t rxNew;
static uint8_t uyNew;
static uint8_t dyNew;
void mazeInit(void){
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, JungleMaze, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	
	Simba.x = 20;
	Simba.y = 28;
	Simba.lives = 1;
	Simba.width = SPRITE_WIDTH;
	Simba.height = SPRITE_WIDTH;
	
	Hyena1.x = 84;
	Hyena1.y = 43;
	Hyena1.lives = 1;
	Hyena1.width = SPRITE_WIDTH;
	Hyena1.height = SPRITE_WIDTH;
	
	Hyena2.x = 84;
	Hyena2.y = 75;
	Hyena2.lives = 1;
	Hyena2.width = SPRITE_WIDTH;
	Hyena2.height = SPRITE_WIDTH;
	
	Hyena3.x = 116;
	Hyena3.y = 60;
	Hyena3.lives = 1;
	Hyena3.width = SPRITE_WIDTH;
	Hyena3.height = SPRITE_WIDTH;
	
	Scar.x = 110;
	Scar.y = 107;
	Scar.lives = 1;
	Scar.width = SPRITE_WIDTH;
	Scar.height = SPRITE_WIDTH;
	
	Friend1.x = 51;
	Friend1.y = 43;
	Friend1.lives = 1;
	Friend1.width = SPRITE_WIDTH;
	Friend1.height = SPRITE_WIDTH;
	
	Friend2.x = 51;
	Friend2.y = 92;
	Friend2.lives = 1;
	Friend2.width = SPRITE_WIDTH;
	Friend2.height = SPRITE_WIDTH;
	
	Friend3.x = 116;
	Friend3.y = 43;
	Friend3.lives = 1;
	Friend3.width = SPRITE_WIDTH;
	Friend3.height = SPRITE_WIDTH;
	
	Home.x = 132;
	Home.y = 107;
	Home.lives = 1;
	Home.width = SPRITE_WIDTH;
	Home.height = SPRITE_WIDTH;
	
	ST7735_DrawBitmap(Simba.x, Simba.y, SpriteSimbaD, Simba.width, Simba.height);
	ST7735_DrawBitmap(Hyena1.x, Hyena1.y, SpriteHyenaU, Hyena1.width, Hyena1.height);
	ST7735_DrawBitmap(Hyena2.x, Hyena2.y, SpriteHyenaL, Hyena2.width, Hyena2.height);
	ST7735_DrawBitmap(Hyena3.x, Hyena3.y, SpriteHyenaD, Hyena3.width, Hyena3.height);
	ST7735_DrawBitmap(Scar.x, Scar.y, SpriteScarL, Scar.width, Scar.height);
	ST7735_DrawBitmap(Friend1.x, Friend1.y, SpriteFriend, Friend1.width, Friend1.height);
	ST7735_DrawBitmap(Friend2.x, Friend2.y, SpriteFriend, Friend2.width, Friend2.height);
	ST7735_DrawBitmap(Friend3.x, Friend3.y, SpriteFriend, Friend3.width, Friend3.height);
	ST7735_DrawBitmap(Home.x, Home.y, SpriteHome, Home.width, Home.height);
	
	orient = 1;
	lxNew = 20;
	rxNew = (20 + SPRITE_WIDTH - 1);
	uyNew = (28 - SPRITE_WIDTH - 1);
	dyNew = 28;
}


bool checkOverlap(Entity_t sprite1, Entity_t sprite2) {
    // calculate the coordinates of the top-right corner of each sprite
    uint8_t x1Max = sprite1.x + sprite1.width;
    uint8_t y1Max = sprite1.y + sprite1.height;
    uint8_t x2Max = sprite2.x + sprite2.width;
    uint8_t y2Max = sprite2.y + sprite2.height;

    // check if the sprites overlap
    if (sprite1.x < x2Max && x1Max > sprite2.x && sprite1.y < y2Max && y1Max > sprite2.y) {
        // the sprites overlap
        return true;
    } else {
        // the sprites don't overlap
        return false;
    }
}


void mazeBattle(Entity_t *player, Entity_t *enemy, uint8_t enemyType){
	Wave_Stop();
	Song_Battle();
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, BattleArena, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ST7735_DrawBitmap(16, 96, BattleSimba, BATTLESIMBA_WIDTH, BATTLESIMBA_HEIGHT);
	switch (enemyType){
		case 0: 
			ST7735_DrawBitmap(110, 96, BattleHyena, BATTLEHYENA_WIDTH, BATTLEHYENA_HEIGHT);
			break;
		case 1:
			ST7735_DrawBitmap(109, 96, BattleScar, BATTLESCAR_WIDTH, BATTLESCAR_HEIGHT);
			break;
	}
	delay100ms(5);
	Wave_Roar();
	int16_t attack = -5;
	int16_t defend = +4;
	uint8_t playerMove;
	uint8_t enemyMove;
	int16_t playerHP = 50;
	int16_t enemyHP = 50;
	ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
	ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
	while ((playerHP > 0) && (enemyHP > 0)){
		switch (language){
			case 0:
				ST7735_DrawStringBG(5, 1, "LIVES:", ST7735_GREEN, LeafGreen);
				ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
				ST7735_DrawStringBG(2, 11, "R1:ATTACK", ST7735_WHITE, GrassGreen);
				ST7735_DrawStringBG(12, 12, "R2:DEFEND", ST7735_WHITE, GrassGreen);
				while ((GPIO_PORTE_DATA_R & 0x0C) == 0x00){}
				if ((GPIO_PORTE_DATA_R & 0x08) == 0x08){
					GPIO_PORTB_DATA_R |= 0x80; 	// toggle PB7
					ST7735_DrawStringBG(2, 11, "R1:ATTACK", ST7735_RED, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(2, 11, "R1:ATTACK", ST7735_WHITE, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(2, 11, "R1:ATTACK", ST7735_RED, GrassGreen);
					playerMove = 0;
				} else if ((GPIO_PORTE_DATA_R & 0x04) == 0x04){
					GPIO_PORTB_DATA_R |= 0x40; 	// toggle PB6
					ST7735_DrawStringBG(12, 12, "R2:DEFEND", ST7735_GREEN, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(12, 12, "R2:DEFEND", ST7735_WHITE, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(12, 12, "R2:DEFEND", ST7735_GREEN, GrassGreen);
					playerMove = 1;
				}
				break;
			
			case 1:
				ST7735_DrawStringBG(5, 1, "VIES:", ST7735_GREEN, LeafGreen);
				ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
				ST7735_DrawStringBG(2, 11, "R1:ATTAQUER", ST7735_WHITE, GrassGreen);
				ST7735_DrawStringBG(8, 12, "R2:DEFENDRE", ST7735_WHITE, GrassGreen); //FIGURE OUT ACCENT D�FENDRE
				while ((GPIO_PORTE_DATA_R & 0x0C) == 0x00){}
				if ((GPIO_PORTE_DATA_R & 0x08) == 0x08){
					GPIO_PORTB_DATA_R |= 0x80; 	// toggle PB7
					ST7735_DrawStringBG(2, 11, "R1:ATTAQUER", ST7735_RED, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(2, 11, "R1:ATTAQUER", ST7735_WHITE, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(2, 11, "R1:ATTAQUER", ST7735_RED, GrassGreen);
					playerMove = 0;
				} else if ((GPIO_PORTE_DATA_R & 0x04) == 0x04){
					GPIO_PORTB_DATA_R |= 0x40; 	// toggle PB6
					ST7735_DrawStringBG(8, 12, "R2:DEFENDRE", ST7735_GREEN, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(8, 12, "R2:DEFENDRE", ST7735_WHITE, GrassGreen);
					delay100ms(1);
					ST7735_DrawStringBG(8, 12, "R2:DEFENDRE", ST7735_GREEN, GrassGreen);
					playerMove = 1;
				}
				break;
			}
			enemyMove = (Random32()%5 < 3)? 0 : 1;
			if ((playerMove == 0) && (enemyMove == 0)){
				enemyHP += attack;
				playerHP += attack;
				ST7735_SetRotation(1);
				ST7735_DrawBitmap(0, 127, BattleArena, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
				switch (language){
					case 0:
						ST7735_DrawStringBG(5, 1, "LIVES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
					case 1:
						ST7735_DrawStringBG(5, 1, "VIES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
				}
				ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
				ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
				ST7735_DrawBitmap(16, 96, BattleSimba, BATTLESIMBA_WIDTH, BATTLESIMBA_HEIGHT);
				switch (enemyType){
					case 0: 
						ST7735_DrawBitmap(110, 96, BattleHyena, BATTLEHYENA_WIDTH, BATTLEHYENA_HEIGHT);
						break;
					case 1:
						ST7735_DrawBitmap(109, 96, BattleScar, BATTLESCAR_WIDTH, BATTLESCAR_HEIGHT);
						break;
				}
				delay100ms(1);
				ST7735_DrawBitmap(60, 86, BattleSwordL, BATTLESWORD_WIDTH, BATTLESWORD_HEIGHT);
				Wave_Sword();
				delay100ms(3);
				ST7735_DrawBitmap(86, 86, BattleSwordR, BATTLESWORD_WIDTH, BATTLESWORD_HEIGHT);
				Wave_Sword();
			} else if ((playerMove == 0) && (enemyMove == 1)){
				enemyHP += attack + defend;
				ST7735_SetRotation(1);
				ST7735_DrawBitmap(0, 127, BattleArena, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
				switch (language){
					case 0:
						ST7735_DrawStringBG(5, 1, "LIVES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
					case 1:
						ST7735_DrawStringBG(5, 1, "VIES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
				}
				ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
				ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
				ST7735_DrawBitmap(16, 96, BattleSimba, BATTLESIMBA_WIDTH, BATTLESIMBA_HEIGHT);
				switch (enemyType){
					case 0: 
						ST7735_DrawBitmap(110, 96, BattleHyena, BATTLEHYENA_WIDTH, BATTLEHYENA_HEIGHT);
						break;
					case 1:
						ST7735_DrawBitmap(109, 96, BattleScar, BATTLESCAR_WIDTH, BATTLESCAR_HEIGHT);
						break;
				}
				delay100ms(1);
				ST7735_DrawBitmap(60, 86, BattleSwordL, BATTLESWORD_WIDTH, BATTLESWORD_HEIGHT);
				Wave_Sword();
				delay100ms(3);
				ST7735_DrawBitmap(88, 86, BattleShieldR, BATTLESHIELD_WIDTH, BATTLESHIELD_HEIGHT);
				Wave_Shield();
			} else if ((playerMove == 1) && (enemyMove == 0)){
				playerHP += defend + attack;
				ST7735_SetRotation(1);
				ST7735_DrawBitmap(0, 127, BattleArena, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
				switch (language){
					case 0:
						ST7735_DrawStringBG(5, 1, "LIVES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
					case 1:
						ST7735_DrawStringBG(5, 1, "VIES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
				}
				ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
				ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
				ST7735_DrawBitmap(16, 96, BattleSimba, BATTLESIMBA_WIDTH, BATTLESIMBA_HEIGHT);
				switch (enemyType){
					case 0: 
						ST7735_DrawBitmap(110, 96, BattleHyena, BATTLEHYENA_WIDTH, BATTLEHYENA_HEIGHT);
						break;
					case 1:
						ST7735_DrawBitmap(109, 96, BattleScar, BATTLESCAR_WIDTH, BATTLESCAR_HEIGHT);
						break;
				}
				delay100ms(1);
				ST7735_DrawBitmap(62, 86, BattleShieldL, BATTLESHIELD_WIDTH, BATTLESHIELD_HEIGHT);
				Wave_Shield();
				delay100ms(3);
				ST7735_DrawBitmap(86, 86, BattleSwordR, BATTLESWORD_WIDTH, BATTLESWORD_HEIGHT);
				Wave_Sword();
			} else if ((playerMove == 1) && (enemyMove == 1)){
				ST7735_SetRotation(1);
				ST7735_DrawBitmap(0, 127, BattleArena, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
				switch (language){
					case 0:
						ST7735_DrawStringBG(5, 1, "LIVES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
					case 1:
						ST7735_DrawStringBG(5, 1, "VIES:", ST7735_GREEN, LeafGreen);
						ST7735_OutUDecBG(11, 1, Simba.lives, ST7735_GREEN, LeafGreen);
						break;
				}
				ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
				ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
				ST7735_DrawBitmap(16, 96, BattleSimba, BATTLESIMBA_WIDTH, BATTLESIMBA_HEIGHT);
				switch (enemyType){
					case 0: 
						ST7735_DrawBitmap(110, 96, BattleHyena, BATTLEHYENA_WIDTH, BATTLEHYENA_HEIGHT);
						break;
					case 1:
						ST7735_DrawBitmap(109, 96, BattleScar, BATTLESCAR_WIDTH, BATTLESCAR_HEIGHT);
						break;
				}
				delay100ms(1);
				ST7735_DrawBitmap(62, 86, BattleShieldL, BATTLESHIELD_WIDTH, BATTLESHIELD_HEIGHT);
				Wave_Shield();
				delay100ms(3);
				ST7735_DrawBitmap(88, 86, BattleShieldR, BATTLESHIELD_WIDTH, BATTLESHIELD_HEIGHT);
				Wave_Shield();
			}
			
			if (playerHP <= 0){
				playerHP = 0;
			} else if (playerHP > 50){
				playerHP = 50;
				ST7735_FillRect(10, 55, 50, 3, ST7735_GREEN);
			} else {
				ST7735_FillRect(10, 55, playerHP, 3, ST7735_GREEN);
			}
			if (enemyHP <= 0){
				enemyHP = 0;
			} else if (enemyHP > 50){
				enemyHP = 50;
				ST7735_FillRect(100, 55, 50, 3, ST7735_GREEN);
			} else {
				ST7735_FillRect(100, 55, enemyHP, 3, ST7735_GREEN);
			}
			GPIO_PORTB_DATA_R &= ~0xC0; 	// TURN OFF LEDS
	}
	
	if (playerHP <= 0){
		(*player).lives--;
	} else if (enemyHP <= 0){
		(*enemy).lives = 0;
	}
	Song_Stop();
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, JungleMaze, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
}


uint8_t randomizePosition(Entity_t *sprite, uint8_t lowLim, uint8_t upLim,){
	
}


int8_t gameStatus = 0;
uint32_t LStick[2];
void playMaze(void){
	Song_Jungle();
	switch (language){
		case 0:
			ST7735_DrawStringBG(10, 1, "LIVES:", ST7735_GREEN, WoodBrown);
			ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, WoodBrown);
			break;
		case 1:
			ST7735_DrawStringBG(10, 1, "VIES:", ST7735_GREEN, WoodBrown);
			ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, WoodBrown);
	}
	LeftJoystick_In(LStick);
	if (LStick[0] > 3000){ 
		dyNew = Simba.y - 1;
		uyNew = dyNew - (Simba.height - 1);
		orient = 0; //FACING UP
	} else if (LStick[0] < 1000){
		dyNew = Simba.y + 1;
		uyNew = dyNew + (Simba.height - 1);
		orient = 1; //FACING DOWN
	}
	if (LStick[1] > 3000){
		lxNew = Simba.x - 1;
		rxNew = lxNew + (Simba.width - 1);
		orient = 2; //FACING LEFT
	} else if (LStick[1] < 1000){
		lxNew = Simba.x + 1;
		rxNew = lxNew + (Simba.width - 1);
		orient = 3; //FACING RIGHT
	}
	
	
	if (lxNew < 17){
		lxNew = 17;
		rxNew = lxNew + SPRITE_WIDTH - 1;
	}
	if (rxNew > 142){
		rxNew = 142;
		lxNew = rxNew - SPRITE_WIDTH + 1;
	}
	if (uyNew < 17){
		uyNew = 17;
		dyNew = uyNew + SPRITE_WIDTH - 1;
	}
	if (dyNew > 110){
		dyNew = 110;
		uyNew = uyNew - SPRITE_WIDTH + 1;
	}
	
	
	Simba.x = lxNew;
	Simba.y = dyNew;
	
	switch (orient){
		case 0:
			ST7735_DrawBitmap(Simba.x, Simba.y, SpriteSimbaU, Simba.width, Simba.height);
			break;
		case 1:
			ST7735_DrawBitmap(Simba.x, Simba.y, SpriteSimbaD, Simba.width, Simba.height);
			break;
		case 2:
			ST7735_DrawBitmap(Simba.x, Simba.y, SpriteSimbaL, Simba.width, Simba.height);
			break;
		case 3:
			ST7735_DrawBitmap(Simba.x, Simba.y, SpriteSimbaR, Simba.width, Simba.height);
			break;
	}
	
	
	if (Hyena1.lives > 0){
		ST7735_DrawBitmap(Hyena1.x, Hyena1.y, SpriteHyenaU, Hyena1.width, Hyena1.height);
		if (checkOverlap(Simba, Hyena1)){
			Song_Stop();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaSmile, FLASHHYENASMILE_WIDTH, FLASHHYENASMILE_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaSmile, FLASHHYENASMILE_WIDTH, FLASHHYENASMILE_HEIGHT);
			delay100ms(5);
			mazeBattle(&Simba, &Hyena1, 0);
		}
	}
	
	if (Hyena2.lives > 0){
		ST7735_DrawBitmap(Hyena2.x, Hyena2.y, SpriteHyenaR, Hyena2.width, Hyena2.height);
		if (checkOverlap(Simba, Hyena2)){
			Song_Stop();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaTongue, FLASHHYENATONGUE_WIDTH, FLASHHYENATONGUE_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaTongue, FLASHHYENATONGUE_WIDTH, FLASHHYENATONGUE_HEIGHT);
			delay100ms(5);
			mazeBattle(&Simba, &Hyena2, 0);
		}
	}
	
	if (Hyena3.lives > 0){
		ST7735_DrawBitmap(Hyena3.x, Hyena3.y, SpriteHyenaD, Hyena3.width, Hyena3.height);
		if (checkOverlap(Simba, Hyena3)){
			Song_Stop();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaSmile, FLASHHYENASMILE_WIDTH, FLASHHYENASMILE_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashHyenaSmile, FLASHHYENASMILE_WIDTH, FLASHHYENASMILE_HEIGHT);
			delay100ms(5);
			mazeBattle(&Simba, &Hyena3, 0);
		}
	}
	
	if (Scar.lives > 0){
		ST7735_DrawBitmap(Scar.x, Scar.y, SpriteScarL, Scar.width, Scar.height);
		if (checkOverlap(Simba, Scar)){
			Song_Stop();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashScar, FLASHSCAR_WIDTH, FLASHSCAR_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 80, FlashScar, FLASHSCAR_WIDTH, FLASHSCAR_HEIGHT);
			delay100ms(5);
			mazeBattle(&Simba, &Scar, 1);
		}
	}
	
	if (Friend1.lives > 0){
		ST7735_DrawBitmap(Friend1.x, Friend1.y, SpriteFriend, Friend1.width, Friend1.height);
		if (checkOverlap(Simba, Friend1)){
			Song_Stop();
			Wave_Fanfare();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 65, FlashTimon, FLASHTIMON_WIDTH, FLASHTIMON_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 65, FlashTimon, FLASHTIMON_WIDTH, FLASHTIMON_HEIGHT);
			delay100ms(5);
			Simba.lives++;
			Friend1.lives--;
			ST7735_SetRotation(1);
			switch (language){
				case 0:
					ST7735_DrawStringBG(10, 1, "LIVES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(9, 9, "YOU FIND", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(10, 10, "TIMON!", ST7735_GREEN, GrassGreen);
					break;
				case 1:
					ST7735_DrawStringBG(10, 1, "VIES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(8, 9, "TU TROUVES", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(10, 10, "TIMON!", ST7735_GREEN, GrassGreen);
			}
			delay100ms(10);
			ST7735_DrawBitmap(0, 127, JungleMaze, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		}
	}
	
	if (Friend2.lives > 0){
		ST7735_DrawBitmap(Friend2.x, Friend2.y, SpriteFriend, Friend2.width, Friend2.height);
		if (checkOverlap(Simba, Friend2)){
			Song_Stop();
			Wave_Fanfare();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 65, FlashPumba, FLASHPUMBA_WIDTH, FLASHPUMBA_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(65, 65, FlashPumba, FLASHPUMBA_WIDTH, FLASHPUMBA_HEIGHT);
			delay100ms(5);
			Simba.lives++;
			Friend2.lives--;
			ST7735_SetRotation(1);
			switch (language){
				case 0:
					ST7735_DrawStringBG(10, 1, "LIVES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(9, 9, "YOU FIND", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(10, 10, "PUMBA!", ST7735_GREEN, GrassGreen);
					break;
				case 1:
					ST7735_DrawStringBG(10, 1, "VIES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(8, 9, "TU TROUVES", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(10, 10, "PUMBA!", ST7735_GREEN, GrassGreen);
			}
			delay100ms(10);
			ST7735_DrawBitmap(0, 127, JungleMaze, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		}
	}
	
	if (Friend3.lives > 0){
		ST7735_DrawBitmap(Friend3.x, Friend3.y, SpriteFriend, Friend3.width, Friend3.height);
		if (checkOverlap(Simba, Friend3)){
			Song_Stop();
			Wave_Fanfare();
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(60, 65, FlashZazu, FLASHZAZU_WIDTH, FLASHZAZU_HEIGHT);
			delay100ms(5);
			ST7735_FillScreen(GrassGreen);
			ST7735_DrawBitmap(60, 65, FlashZazu, FLASHZAZU_WIDTH, FLASHZAZU_HEIGHT);
			delay100ms(5);
			Simba.lives++;
			Friend3.lives--;
			ST7735_SetRotation(1);
			switch (language){
				case 0:
					ST7735_DrawStringBG(10, 1, "LIVES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(9, 9, "YOU FIND", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(11, 10, "ZAZU!", ST7735_GREEN, GrassGreen);
					break;
				case 1:
					ST7735_DrawStringBG(10, 1, "VIES:", ST7735_GREEN, GrassGreen);
					ST7735_OutUDecBG(16, 1, Simba.lives, ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(8, 9, "TU TROUVES", ST7735_GREEN, GrassGreen);
					ST7735_DrawStringBG(11, 10, "ZAZU!", ST7735_GREEN, GrassGreen);
			}
			delay100ms(10);
			ST7735_DrawBitmap(0, 127, JungleMaze, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		}
	}
	
	if (Home.lives > 0){
		ST7735_DrawBitmap(Home.x, Home.y, SpriteHome, Home.width, Home.height);
		if (checkOverlap(Simba, Home)){
			Wave_Fanfare();
			gameStatus = 1;
			Home.lives--;
			delay100ms(2);
		}
	}
	
	if (Simba.lives <= 0){
		gameStatus = -1;
		delay100ms(2);
	}
}


void youWin(void){
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, TitleScreen, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ST7735_DrawBitmap(22, 39, YouWinText, YOUWINTEXT_WIDTH, YOUWINTEXT_HEIGHT);
	ST7735_DrawBitmap(67, 84, YouWinIcon, YOUWINICON_WIDTH, YOUWINICON_HEIGHT);
	switch (language){
		case 0:
			ST7735_DrawStringBG(3, 8, "LIVES:", ST7735_GREEN, SkyBlue);
			ST7735_OutUDecBG(9, 8, Simba.lives, ST7735_GREEN, SkyBlue);
			break;
		case 1:
			ST7735_DrawStringBG(3, 8, "VIES:", ST7735_GREEN, SkyBlue);
			ST7735_OutUDecBG(9, 8, Simba.lives, ST7735_GREEN, SkyBlue);
	}
	GPIO_PORTB_DATA_R &= ~0xC0; 	// TURN OFF LEDS
}


void gameOver(void){
	ST7735_SetRotation(1);
	ST7735_DrawBitmap(0, 127, TitleScreen, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ST7735_DrawBitmap(0, 38, GameOverText, GAMEOVERTEXT_WIDTH, GAMEOVERTEXT_HEIGHT);
	ST7735_DrawBitmap(67, 80, GameOverIcon, GAMEOVERICON_WIDTH, GAMEOVERICON_HEIGHT);
	switch (language){
		case 0:
			ST7735_DrawStringBG(3, 8, "LIVES:", ST7735_RED, SkyBlue);
			ST7735_OutUDecBG(9, 8, Simba.lives, ST7735_RED, SkyBlue);
			break;
		case 1:
			ST7735_DrawStringBG(3, 8, "VIES:", ST7735_RED, SkyBlue);
			ST7735_OutUDecBG(9, 8, Simba.lives, ST7735_RED, SkyBlue);
	}
	GPIO_PORTB_DATA_R &= ~0xC0; 	// TURN OFF LEDS
}


void mainEngine(void){
	showTitleScreen();
	delay100ms(5);
	languageSelect();
	delay100ms(5);
	mazeInit();
	while (gameStatus == 0){
		playMaze();
	}
	if (gameStatus == 1){
		youWin();
	} else if (gameStatus == -1){
		gameOver();
	}
	Song_Stop();
	gameStatus = 0;
	switch (language){
		case 0:
			ST7735_DrawStringBG(3, 9, "L2:RESTART", ST7735_WHITE, SkyBlue);
			while ((GPIO_PORTE_DATA_R & 0x03) == 0x00){}
			while ((GPIO_PORTE_DATA_R & 0x03) == 0x01){}
			ST7735_DrawStringBG(3, 9, "L2:RESTART", ST7735_YELLOW, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(3, 9, "L2:RESTART", ST7735_WHITE, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(3, 9, "L2:RESTART", ST7735_YELLOW, SkyBlue);
			break;
		case 1:
			ST7735_DrawStringBG(3, 9, "L2:RECOMMENCER", ST7735_WHITE, SkyBlue);
			while ((GPIO_PORTE_DATA_R & 0x03) == 0x00){}
			while ((GPIO_PORTE_DATA_R & 0x03) == 0x01){}
			ST7735_DrawStringBG(3, 9, "L2:RECOMMENCER", ST7735_YELLOW, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(3, 9, "L2:RECOMMENCER", ST7735_WHITE, SkyBlue);
			delay100ms(2);
			ST7735_DrawStringBG(3, 9, "L2:RECOMMENCER", ST7735_YELLOW, SkyBlue);
			break;
	}
	delay100ms(5);
}