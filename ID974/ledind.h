#ifndef LEDIND_H_
#define LEDIND_H_

#include "main.h"

#define LEDIND_DIGIT_0  0b00000100				//0 indicator symbol						PORTC2
#define LEDIND_DIGIT_1  0b00001000				//1 indicator symbol						PORTC3
#define LEDIND_DIGIT_2  0b00010000				//2 indicator symbol						PORTC4
#define LEDIND_DIGIT_3  0b00100000				//3 indicator symbol						PORTC5

#define LEDIND_NONE		0b11111111				//NULL

#ifdef IDPLUS974	//IDPlus974
	#define LEDIND_SOUND    0b11111101			//Led: sound beep							DIGIT_1
	#define LEDIND_FAN      0b11111011			//Led: fan									DIGIT_3
	#define LEDIND_DEFROST  0b11111011			//Led: defrost								DIGIT_1
	#define LEDIND_FROST    0b11111011			//Led: frost								DIGIT_2
	#define LEDIND_HEAT     0b11111011			//Led: heat									DIGIT_4
	#define LEDIND_FAR      0b10111111			//Led: temp in F					        DIGIT_1
	#define LEDIND_CEL      0b11110111			//Led: temp in C				            DIGIT_1
	#define LEDIND_AUX      0b01111111			//Led: AUX									DIGIT_1
#endif

#ifdef ID974LX		//ID974lx
	#define LEDIND_SOUND    0b11111101			//Led: sound beep							DIGIT_1
	#define LEDIND_FAN      0b11111110			//Led: fan									DIGIT_1
	#define LEDIND_DEFROST  0b11101111			//Led: defrost								DIGIT_1
	#define LEDIND_FROST    0b11111011			//Led: frost								DIGIT_2
	#define LEDIND_HEAT     0b11111011			//Led: heat									DIGIT_3
#endif
#define LEDIND_POINT    0b11111110				//Point										DIGIT_4
#define LEDIND_MINUS    0b11011111				//Minus
#define LEDIND_NUMBER_0 0b00100100				//Number 0
#define LEDIND_NUMBER_1 0b10110111				//Number 1
#define LEDIND_NUMBER_2 0b01010100				//Number 2
#define LEDIND_NUMBER_3 0b00010101				//Number 3
#define LEDIND_NUMBER_4 0b10000111				//Number 4
#define LEDIND_NUMBER_5 0b00001101				//Number 5
#define LEDIND_NUMBER_6 0b00001100				//Number 6
#define LEDIND_NUMBER_7 0b10110101				//Number 7
#define LEDIND_NUMBER_8 0b00000100				//Number 8
#define LEDIND_NUMBER_9 0b00000101				//Number 9

#define LEDIND_LETTER_A 0b11111111				//Letter A -
#define LEDIND_LETTER_B 0b00001110				//Letter B +
#define LEDIND_LETTER_C 0b01011110				//Letter C +
#define LEDIND_LETTER_D 0b00010110				//Letter D +
#define LEDIND_LETTER_E 0b01001100				//Letter E +
#define LEDIND_LETTER_F 0b11001100				//Letter F +
#define LEDIND_LETTER_G 0b11111111				//Letter G -
#define LEDIND_LETTER_H 0b10000110				//Letter H -
#define LEDIND_LETTER_I 0b10111111				//Letter I +
#define LEDIND_LETTER_J 0b11111111				//Letter J -
#define LEDIND_LETTER_K 0b11111111				//Letter K -
#define LEDIND_LETTER_L 0b01101110				//Letter L +
#define LEDIND_LETTER_M 0b11111111				//Letter M -
#define LEDIND_LETTER_N 0b10011110				//Letter N +
#define LEDIND_LETTER_O 0b00011110				//Letter O +
#define LEDIND_LETTER_P 0b11000100				//Letter P +
#define LEDIND_LETTER_Q 0b11111111				//Letter Q -
#define LEDIND_LETTER_R 0b11011110				//Letter R +
#define LEDIND_LETTER_S 0b00001101				//Letter S +
#define LEDIND_LETTER_T 0b01001110				//Letter T +
#define LEDIND_LETTER_U 0b00111110				//Letter U +
#define LEDIND_LETTER_V 0b11111111				//Letter V -
#define LEDIND_LETTER_W 0b11111111				//Letter W -
#define LEDIND_LETTER_X 0b11111111				//Letter X -
#define LEDIND_LETTER_Y 0b00000111				//Letter Y +
#define LEDIND_LETTER_Z 0b11111111				//Letter Z -

#define A LEDIND_LETTER_A						//Letter A
#define B LEDIND_LETTER_B						//Letter B
#define C LEDIND_LETTER_C						//Letter C
#define D LEDIND_LETTER_D						//Letter D
#define E LEDIND_LETTER_E						//Letter E
#define F LEDIND_LETTER_F						//Letter F
#define G LEDIND_LETTER_G						//Letter G
#define H LEDIND_LETTER_H						//Letter H
#define I LEDIND_LETTER_I						//Letter I
#define J LEDIND_LETTER_J						//Letter J
#define K LEDIND_LETTER_K						//Letter K
#define L LEDIND_LETTER_L						//Letter L
#define M LEDIND_LETTER_M						//Letter M
#define N LEDIND_LETTER_N						//Letter N
#define O LEDIND_LETTER_O						//Letter O
#define P LEDIND_LETTER_P						//Letter P
#define Q LEDIND_LETTER_Q						//Letter Q
#define R LEDIND_LETTER_R						//Letter R
#define S LEDIND_LETTER_S						//Letter S
#define T LEDIND_LETTER_T						//Letter T
#define U LEDIND_LETTER_U						//Letter U
#define V LEDIND_LETTER_V						//Letter V
#define W LEDIND_LETTER_W						//Letter W
#define X LEDIND_LETTER_X						//Letter X
#define Y LEDIND_LETTER_Y						//Letter Y
#define Z LEDIND_LETTER_Z						//Letter Z

uint8_t digits[4];
uint8_t numbers[10];
uint8_t sumbols[2];
uint8_t signals[9];
uint8_t letters[4];

uint8_t ledind_bufer[4];
uint8_t signal_bufer[4];
uint8_t in_letters[4];

void fLedindInit(void);
void fLedindNumber(int in_number, uint8_t out_ledind_bufer[]);
void fLedindSignal(uint8_t in_signal, uint8_t out_ledind_bufer[]);
void fLedindLetter(uint8_t in_sumbols[], uint8_t out_ledind_bufer[]);
uint8_t fReadKeyAndLedindUpdate(uint8_t out_ledind_bufer[], uint8_t in_signal_bufer[]);
void fLoadAnimation(void);
void fOnAnimation(void);
void fOffAnimation(uint8_t sound_beep);
void fServiceAnimation(uint8_t service_state);
void fErrorAnimation(void);
void fDisplayError(uint8_t code_error);

#endif /* LEDIND_H_ */