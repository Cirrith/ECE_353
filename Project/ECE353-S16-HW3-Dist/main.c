//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "TM4C123.h"

#include "boardUtil.h"
#include "draw.h"

extern void hw3_config(void);
extern void UART0_Handler(void);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

/******************************************************************************
 * Global Variables
 *****************************************************************************/ 
char ryan[] = "Ryan Bambrough";
char nick[] = "Nick Adams";
char team[3] = {0, 0, 0};
char person1[NAMELEN];
char person2[NAMELEN];
uint8_t highscore = 0;

bool ps2_Buf[5] = {0,0,0,0,0};
bool start_Buf[5] = {0,0,0,0,0};
bool a_Buf[5] = {0,0,0,0,0};
bool b_Buf[5] = {0,0,0,0,0};

bool a_press = false;
bool b_press = false;
bool start_press = false;
bool ps2_press = false;

bool konamiCodeEntered = false;

const char pokeNames[4][6] = {"Bulba", "Char", "Squirt", "Pika"};
const uint8_t pokeHP[4] = { 60, 40, 50, 75 };
const uint8_t pokePower[4] = { 10, 12, 8, 15 };
const uint8_t pokeSpeed[4] = { 8, 10, 12, 15 };
const uint16_t tempRanges[8] = { 1000, 2000, 3000, 4096, 0, 1000, 0, 4096};

uint8_t pets = 0;
uint8_t owned = 0xff;
double fullness = 10;
double happiness = 10;
double power = 10;
double speed = 10;
double hp = 10;

uint8_t ticksInState = 0;

/* 
	Push Button Debounce Functions 
  These are following the method described in ICE 14
 */

bool ps2_button_pressed(void)
{
	bool val = GPIOE->DATA & PS2BTN_PIN; 
	
	ps2_Buf[4] = ps2_Buf[3];
	ps2_Buf[3] = ps2_Buf[2];
  ps2_Buf[2] = ps2_Buf[1];
	ps2_Buf[1] = ps2_Buf[0];
	ps2_Buf[0] = val;
	
	if(~(ps2_Buf[0] | ps2_Buf[1] | ps2_Buf[2] | ps2_Buf[3]) & ps2_Buf[4])
		return true;
	else
		return false;
}

bool start_button_pressed (void)
{
	bool val = GPIOF->DATA & UP_PIN; 
	
	start_Buf[4] = start_Buf[3];
	start_Buf[3] = start_Buf[2];
  start_Buf[2] = start_Buf[1];
	start_Buf[1] = start_Buf[0];
	start_Buf[0] = val;
	
	if(~(start_Buf[0] | start_Buf[1] | start_Buf[2] | start_Buf[3]) & start_Buf[4])
		return true;
	else
		return false;
}

bool a_button_pressed (void)
{
	bool val = GPIOF->DATA & LEFT_PIN; 
	
	a_Buf[4] = a_Buf[3];
	a_Buf[3] = a_Buf[2];
  a_Buf[2] = a_Buf[1];
	a_Buf[1] = a_Buf[0];
	a_Buf[0] = val;
	
	if(~(a_Buf[0] | a_Buf[1] | a_Buf[2] | a_Buf[3]) & a_Buf[4])
		return true;
	else
		return false;
}

bool b_button_pressed (void)
{
	bool val = GPIOF->DATA & RIGHT_PIN; 
	
	b_Buf[4] = b_Buf[3];
	b_Buf[3] = b_Buf[2];
  b_Buf[2] = b_Buf[1];
	b_Buf[1] = b_Buf[0];
	b_Buf[0] = val;
	
	if(~(b_Buf[0] | b_Buf[1] | b_Buf[2] | b_Buf[3]) & b_Buf[4])
		return true;
	else
		return false;
}

typedef enum
{
		KSTATE_IDLE,
		KSTATE_UP1,
		KSTATE_UP2,
		KSTATE_DOWN1,
		KSTATE_DOWN2,
		KSTATE_LEFT1,
		KSTATE_RIGHT1,
		KSTATE_LEFT2,
		KSTATE_RIGHT2,
		KSTATE_B,
		KSTATE_A,
		KSTATE_START,
		KSTATE_ERROR
} kState;

typedef enum
{
	STATE_MENU,
	STATE_EGG,
	STATE_RAISE,
	STATE_BATTLE
} State;

void eeprom_reset_values() {
	int i;
	char input[NAMELEN];
	
	#if WATCHDOG
		WATCHDOG0->CTL = 0;
	#endif
		
	printf("\n\rStudent 1: %s\n\r", person1);
	printf("Student 2: %s\n\r", person2);
	printf("Team Number: %s\n\r", team);
	
	printf("\n\rEnter Student 1: ");
	scanf("%79[^\n]", input);
	for (i = 0; i < NAMELEN; i++) {
		if (input[i] == '\n' || input[i] == '\r') {
			eeprom_byte_write(EEPROM_I2C_BASE, i+2, 0);
			break;
		}
		
		eeprom_byte_write(EEPROM_I2C_BASE, i+2, input[i]);
		person1[i] = input[i];
		if (person1[i] == 0) break;
	}
	
	printf("Enter Student 2: ");
	scanf("%79[^\n]", input);
	for (i = 0; i < NAMELEN; i++) {
		if (input[i] == '\n' || input[i] == '\r') {
			eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, 0);
			break;
		}
		
		eeprom_byte_write(EEPROM_I2C_BASE, i+NAMELEN+2, input[i]);
		person2[i] = input[i];
		if (person2[i] == 0) break;
	}
	
	printf("Enter Team: ");
	scanf("%2[^\n]", input);
	eeprom_byte_write(EEPROM_I2C_BASE, 0, input[0]);
	eeprom_byte_write(EEPROM_I2C_BASE, 1, input[1]);
	team[0] = input[0];
	team[1] = input[1];
	
	eeprom_byte_write(EEPROM_I2C_BASE, HSADDR, 0);
	
	// Goodbye!
	NVIC_SystemReset();
}

void saveHighScore(bool won) {
	uint8_t score;
	
	score = happiness * (hp + speed + power);
	if (won)
		score <<= 2;
	
	eeprom_byte_write(EEPROM_I2C_BASE, HSADDR, score);
}

void ledHappiness(void) {
	if (happiness > 20)
		ledSendData(0x14, 0x00);
	else if (happiness > 15)
		ledSendData(0x14, 0x01);
	else if (happiness > 12)
		ledSendData(0x14, 0x03);
	else if (happiness > 9)
		ledSendData(0x14, 0x07);
	else if (happiness > 6)
		ledSendData(0x14, 0x0f);
	else if (happiness > 4)
		ledSendData(0x14, 0x1f);
	else if (happiness < 4)
		ledSendData(0x14, 0x3f);
	
	ledSendData(0x15, 0x01);
}

void ledHunger(void) {
	if (fullness > 20)
		ledSendData(0x14, 0x00);
	else if (fullness > 15)
		ledSendData(0x14, 0x01);
	else if (fullness > 12)
		ledSendData(0x14, 0x03);
	else if (fullness > 9)
		ledSendData(0x14, 0x07);
	else if (fullness > 6)
		ledSendData(0x14, 0x0f);
	else if (fullness > 4)
		ledSendData(0x14, 0x1f);
	else if (fullness < 4)
		ledSendData(0x14, 0x3f);
	
	ledSendData(0x15, 0x2);
}

void ledHP(void) {
	if (hp > 70)
		ledSendData(0x14, 0x00);
	else if (hp > 60)
		ledSendData(0x14, 0x01);
	else if (hp > 50)
		ledSendData(0x14, 0x03);
	else if (hp > 40)
		ledSendData(0x14, 0x07);
	else if (hp > 30)
		ledSendData(0x14, 0x0f);
	else if (hp > 20)
		ledSendData(0x14, 0x1f);
	else if (hp < 20)
		ledSendData(0x14, 0x3f);
	
	ledSendData(0x15, 0x4);
}

void ledPower(void) {
	if (power > 20)
		ledSendData(0x14, 0x00);
	else if (power > 15)
		ledSendData(0x14, 0x01);
	else if (power > 12)
		ledSendData(0x14, 0x03);
	else if (power > 9)
		ledSendData(0x14, 0x07);
	else if (power > 6)
		ledSendData(0x14, 0x0f);
	else if (power > 4)
		ledSendData(0x14, 0x1f);
	else if (power < 4)
		ledSendData(0x14, 0x3f);
	
	ledSendData(0x15, 0x8);
}

void ledSpeed(void) {
	if (speed > 20)
		ledSendData(0x14, 0x00);
	else if (speed > 15)
		ledSendData(0x14, 0x01);
	else if (speed > 12)
		ledSendData(0x14, 0x03);
	else if (speed > 9)
		ledSendData(0x14, 0x07);
	else if (speed > 6)
		ledSendData(0x14, 0x0f);
	else if (speed > 4)
		ledSendData(0x14, 0x1f);
	else if (speed < 4)
		ledSendData(0x14, 0x3f);
	
	ledSendData(0x15, 0x10);
}

int 
main(void)
{
	State gameState = STATE_MENU;
	State gameStateNext = STATE_MENU;
	
	kState konamiState = KSTATE_IDLE;
	kState konamiStateNext = KSTATE_IDLE;
	
	char buffer[10];
	uint32_t data;
	
	uint8_t totalTicks;
	uint8_t frame = 1;
	uint8_t ledframe = 0;
	uint8_t cracks = 0;
	bool frameskip = false;
	
	uint16_t temp;
	bool petted = false;
	bool gaveCandy = false;
	bool gaveMeat = false;
	bool play = false;
	bool train = false;
	
	bool didStarve = false;
  
	initialize_board();
  drawMenu();
	
  // Infinite Loop
  while(1)
  {
		konamiState = konamiStateNext;
		
		if (ALERT_WIRELESS) {
			ALERT_WIRELESS = false;
			
			if ( wireless_get_32(false, &data) == NRF24L01_RX_SUCCESS )
			{
					if (data == PINGPACKET)
						printf("PING\n\r");
					else
						printf("Received: %d\n\r", data);
			}

			PETWATCHDOG();
		}
		
		// 5 Sec Timeout
		if(ALERT_5_SEC)
		{
			ALERT_5_SEC = false;
			
			printf("Packets Rx: %d Tx: %d\n\r", wirelessPacketsReceived, wirelessPacketsSent);
			wireless_send_32(false, false, PINGPACKET);
		}
		
		// 2 Sec Timeout, only active if we're in the middle of waiting for konami
		if(ALERT_2_SEC)
		{
			ALERT_2_SEC = false;
			
			if (konamiState != KSTATE_IDLE && konamiState != KSTATE_ERROR) {
				// Start over if timeout
				printf("Timeout\n\r");
				konamiState = KSTATE_IDLE;
			}
		}

		// Debounce all the buttons on the timer
		if(ALERT_DEBOUNCE)
		{
			ALERT_DEBOUNCE = false;
			
			a_press = a_button_pressed();
			b_press = b_button_pressed();
			start_press = start_button_pressed();
			ps2_press = ps2_button_pressed();
		}
		else { // Buttons weren't pressed
			a_press = false;
			b_press = false;
			start_press = false;
			ps2_press = false;
		}
		
		if (ALERT_LED) {
			ALERT_LED = false;

			if (gameState == STATE_BATTLE) {
				// TODO draw both players health bars
			}
			else if (gameState == STATE_RAISE) {
				if (ledframe)
					ledHunger();
				else
          ledHappiness();
				
				ledframe = (ledframe + 1) % 2;
			}
			/*
			else if (gameState == STATE_RAISE) {
				if (ledframe == 0)
          ledHappiness();
				else if (ledframe == 1)
          ledHunger();
				if (ledframe == 2)
          ledHP();
				if (ledframe == 3)
          ledPower();
				else
					ledSpeed();
				
				ledframe = (ledframe + 1) % 5;
			}
			*/
			else if (gameState == STATE_EGG) {
				ledHappiness();
			}
		}
		
		if (ps2_press) {
			eeprom_reset_values();
		}
		
		if (ALERT_GAME_TICK) {
			ALERT_GAME_TICK = false;
			
			switch (gameState)
			{
				case STATE_MENU: break;
				case STATE_EGG: {
					
					if (owned == 0xff) {
						if (konamiCodeEntered)
							owned = 3;
						else
							owned = SysTick->VAL % 3;
						
						hp = pokeHP[owned];
						power = pokePower[owned];
						speed = pokeSpeed[owned];
						Dprintf("Egg is %s\n\r", pokeNames[owned]);
						Dprintf("STATUS  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
				  }

					temp = getADCValue(ADC1_BASE, POT_CHANNEL);
					if (temp < tempRanges[owned*2] || temp > tempRanges[(owned*2)+1])
						happiness -= 0.5;
					
					if (petted) {
						Dprintf("PET\n\r");
						petted = false;
						pets++;
					}
					break;
				}
				case STATE_RAISE: {
					fullness--;
					
					temp = getADCValue(ADC1_BASE, POT_CHANNEL);
					if (temp < tempRanges[owned*2] || temp > tempRanges[(owned*2)+1])
						happiness -= 0.5;
					
					if (petted) {
						petted = false;
						Dprintf("PET\n\r");
						pets++;
					}
					
					if (gaveCandy) {
						gaveCandy = false;
						happiness += 1;
						fullness += 0.5;
						hp -= 0.25;
						power -= 0.5;
						speed += 0.25;
						Dprintf("Candy  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
					}
					
					if (gaveMeat) {
						gaveMeat = false;
						happiness -= 0.25;
						fullness += 2;
						hp += 0.5;
						power += 0.5;
						speed -= 0.25;
						Dprintf("Meat  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
					}
					
					if (play) {
						play = false;
						happiness += 1;
						fullness -= 0.5;
						power += 0;
						speed += 0.5;
						Dprintf("Play  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
					}
					
					if (train) {
						train = false;
						happiness -= 0.5;
						fullness -= 1;
						power += 0.5;
						speed += 1;
						Dprintf("Train  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
					}
					
					if (fullness <= 0) {
						happiness -= 3;
						Dprintf("STARVE  Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
						
						fullness = 2;
						
						if (!didStarve)
							didStarve = true;
						else {
							gameStateNext = STATE_MENU;
							lcd_clear();
							sprintf(buffer, "   DEAD");
							lcd_writeString(1, buffer);
							sprintf(buffer, " GAMEOVER");
							lcd_writeString(2, buffer);
							// TODO  Tell other board
						}
					}
					
					if (fullness > 10)
						fullness = 10;
					if (happiness < 0)
						happiness = 0;
					if (power < 1)
						power = 1;
					if (speed < 1)
						speed = 1;
					if (hp < 2)
						hp = 2;
					break;
				}
				case STATE_BATTLE: {
					break;
				}
			}

			ticksInState++;
			totalTicks--;
			gameState = gameStateNext;
		}
		
		switch (gameState)
		{
			case STATE_MENU:
			{
				if (start_press) {
					gameStateNext = STATE_EGG;
					
					ticksInState = 0;
					pets = 0;
					totalTicks = 51;
          owned = 0xff;
          fullness = 10;
          happiness = 10;
					
					Dprintf("EGG\n\r");
					
					lcd_clear();
					sprintf(buffer, " Previous");
					lcd_writeString(0, buffer);
					sprintf(buffer, "   High");
					lcd_writeString(1, buffer);
					sprintf(buffer, " Score %d", highscore);
					lcd_writeString(2, buffer);
				}
				break;
			}
			case STATE_EGG:
			{
				if (ALERT_GAME_DRAW) {
					ALERT_GAME_DRAW = false;
					
					if (ticksInState >= 2 && ticksInState < 18 && !frameskip) {
						lcd_clear();
						drawEgg(cracks, frame);
						
						if (ticksInState > 16 && ticksInState < 18)
							frame = (frame + 1) % 4;
						else if (ticksInState == 16) {
							cracks = 2;
							frame = 1;
						}
						else if (ticksInState > 10 && ticksInState < 13)
							frame = (frame + 1) % 4;
						else if (ticksInState == 10) {
							cracks = 1;
							frame = 1;
						}
						else if (ticksInState > 6 && ticksInState < 8) {
							frame = (frame + 1) % 4;
							frameskip = true;
						}
					}
					else {
						frameskip = !frameskip;
					}
				}
				
				if (b_press)
					petted = true;
				
				if (ticksInState == 18) {
					lcd_clear();
					sprintf(buffer, " The Egg");
					lcd_writeString(0, buffer);
					sprintf(buffer, " Hatched!");
					lcd_writeString(1, buffer);
				}
				else if (ticksInState == 20) {
					gameStateNext = STATE_RAISE;
					ticksInState = 0;
					frame = 0;
					frameskip = false;
					
					if (pets < 2 || pets > 5)
						happiness -= 2;
					
					if (happiness <= 0)
						happiness = 1;
					
					Dprintf("RAISE\n\r");
					Dprintf("STATUS Hap: %f Full: %f HP: %f Pow:%f Spd: %f\n\r",
											happiness, fullness, hp, power, speed);
				}
				break;
			}
			case STATE_RAISE:
			{
				if (ALERT_GAME_DRAW) {
					ALERT_GAME_DRAW = false;
					
					if (!frameskip) {
						lcd_clear();
						switch (owned) {
							case 0: {
								drawBulba(frame);
								break;
							}
							case 1: {
								drawChar(frame);
								break;
							}
							case 2: {
								drawSquirt(frame);
								break;
							}
							case 3: {
								drawPika(frame);
								break;
							}
						}
						frameskip = true;
						frame = (frame + 1) % 2;
					}
					else frameskip = !frameskip;
				}
				
				if (b_press)
					petted = true;
				
				if (UP) {
					UP = false;
					gaveCandy = true;
				}
				if (DOWN) {
					DOWN = false;
					gaveMeat = true;
				}
				if (LEFT) {
					LEFT = false;
					play = true;
				}
				if (RIGHT) {
					RIGHT = false;
					train = true;
				}
				
				if (totalTicks == 0 && ticksInState > 1) {
					gameStateNext = STATE_BATTLE;
					ticksInState = 0;
					
					if (pets < 2 || pets > 5)
						happiness -= 2;
					
					if (happiness <= 0)
						happiness = 1;
					
					Dprintf("BATTLE\n\r");
				}
				break;
			}
			case STATE_BATTLE:
			{
				
				/*if (done) {
					gameStateNext = STATE_MENU;
					Dprintf("MENU\n\r");
				}*/
				break;
			}
		}
		
		if (gameState == STATE_EGG || gameState == STATE_RAISE) {
			sprintf(buffer, " Time: %d", totalTicks);
			lcd_writeString(3, buffer);
		}
		
		if (gameState != STATE_MENU)
			continue;
		
		/* Konami Code Detection from HW3 */
		switch (konamiState)
		{
		case KSTATE_IDLE: // Wait for UP, restart the timer once it was
		{			
			if(a_press | b_press | start_press | LEFT | DOWN | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				LEFT = false;
				DOWN = false;
				RIGHT = false;
			}
			else if (UP)
			{
				konamiStateNext = KSTATE_UP2;
				UP = false;
				printf("UP\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_IDLE;
			}
			break;
		}
		
		case KSTATE_UP2: // Wait for UP, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | DOWN | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				LEFT = false;
				DOWN = false;
				RIGHT = false;
			}
			else if (UP)
			{
				konamiStateNext = KSTATE_DOWN1;
				UP = false;
				printf("UP\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_UP2;
			}
			break;
		}
		
		case KSTATE_DOWN1: // Wait for DOWN, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | UP | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				LEFT = false;
				UP = false;
				RIGHT = false;
			}
			else if (DOWN)
			{
				konamiStateNext = KSTATE_DOWN2;
				DOWN = false;
				printf("DOWN\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_DOWN1;
			}
			break;
		}
		
		case KSTATE_DOWN2: // Wait for DOWN, restart the timer once it was
		{
			if(a_press | b_press | start_press | LEFT | UP | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				LEFT = false;
				UP = false;
				RIGHT = false;
			}
			else if (DOWN)
			{
				konamiStateNext = KSTATE_LEFT1;
				DOWN = false;
				printf("DOWN\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_DOWN2;
			}
			break;
		}
		
		case KSTATE_LEFT1: // Wait for LEFT, restart the timer once it was
		{
			hw3_timer2_init();
			if(a_press | b_press | start_press | DOWN | UP | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				RIGHT = false;
			}
			else if (LEFT)
			{
				konamiStateNext = KSTATE_RIGHT1;
				LEFT = false;
				printf("LEFT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_LEFT1;
			}
			break;
		}
		
		case KSTATE_RIGHT1: // Wait for RIGHT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | LEFT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
			}
			else if (RIGHT)
			{
				konamiStateNext = KSTATE_LEFT2;
				RIGHT = false;
				printf("RIGHT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_RIGHT1;
			}
			break;
		}
		
		case KSTATE_LEFT2: // Wait for LEFT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				RIGHT = false;
			}
			else if (LEFT)
			{
				konamiStateNext = KSTATE_RIGHT2;
				LEFT = false;
				printf("LEFT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_LEFT2;
			}
			break;
		}
		
		case KSTATE_RIGHT2: // Wait for RIGHT, restart the timer once it was
		{
			if(a_press | b_press | start_press | DOWN | UP | LEFT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
			}
			else if (RIGHT)
			{
				konamiStateNext = KSTATE_B;
				RIGHT = false;
				printf("RIGHT\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_RIGHT2;
			}
			break;
		}
		
		case KSTATE_B: // Wait for B to be pressed, restart the timer once it was
		{
			if(a_press | start_press | DOWN | UP | LEFT | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (b_press)
			{
				konamiStateNext = KSTATE_A;
				printf("B\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_B;
			}
			break;
		}
		
		case KSTATE_A: // Wait for A to be pressed, restart the timer once it was
		{
			if(b_press | start_press | DOWN | UP | LEFT | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (a_press)
			{
				konamiStateNext = KSTATE_START;
				printf("A\n\r");
			  hw3_timer2_init();
			}
			else
			{
				konamiStateNext = KSTATE_A;
			}
			break;
		}
		
		case KSTATE_START: // Wait for START to be pressed, restart the timer once it was
		{
			if( b_press| a_press | DOWN | UP | LEFT | RIGHT)
			{
				konamiStateNext = KSTATE_ERROR;
				DOWN = false;
				UP = false;
				LEFT = false;
				RIGHT = false;
			}
			else if (start_press)
			{
				konamiStateNext = KSTATE_IDLE;
				printf("START\n\r");
				printf("Konami Code Detected!\n\r");
				konamiCodeEntered = true;
			}
			else
			{
				konamiStateNext = KSTATE_START;
			}
			break;
		}
		
		case KSTATE_ERROR: // All States come here if incorrect key was pressed
		{
			konamiStateNext = KSTATE_IDLE;
			printf("Error in Sequence\n\r");
			break;
		}

		default:
		{
			printf("UNKNOWN ERROR\n\r");
			while(1){}; // Infinite loop
		}
		}
	}
}
