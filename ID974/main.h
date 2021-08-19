#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 1000000UL
//#define ID974LX
#define IDPLUS974

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "timers.h"
#include "adc.h"
#include "ledind.h"
#include "output.h"
#include "error.h"
#include "sound.h"

//Work Modes
#define NORMAL_MODE						0
#define OFF_MODE						1
#define MENU_USER_MODE					2
#define MENU_ADM_MODE					3
#define ERROR_MODE						4
#define ERROR_LIST_MODE					5
#define MENU_SERVICE_MODE				6

#define MENU_USER_MODE_SET_TEMP			0
#define MENU_USER_MODE_SET_TEMP_EDIT	1
#define MENU_USER_MODE_SET_DIFF			2
#define MENU_USER_MODE_SET_DIFF_EDIT	3



//Buttons
#define BUTTON_NONE     0b00000000			//Button no press	PORTB
#define BUTTON_DOWN     0b00000001			//Button DOWN		PORTB0
#define BUTTON_UP       0b00000010			//Button UP			PORTB1
#define BUTTON_FNC      0b00000100			//Button FNC		PORTB2
#define BUTTON_SET      0b00001000			//Button SET		PORTB3

#define BUTTON_RELEASED	0
#define BUTTON_PRESSED	1

#define SERVICE_OK		0
#define SERVICE_NO		1
#define SERVICE_RESET   2

uint8_t  eep_mode;							//Saved work mode
uint8_t  eep_temp;							//Saved temperature set (C)
uint8_t  eep_diff;							//Saved differential (C)
uint8_t button_key;

#endif /* MAIN_H_ */