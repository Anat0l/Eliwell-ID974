#include "sound.h"

//Interrupt handler when TIMER2
ISR(TIMER2_COMP_vect)
{
	//TCNT2 = 0x00;				//Reset Timer/Counter2
}

void fSoundInit(void)
{
	DDRD  = 0b00000000;
	PORTD = 0b00000000;
	TCCR2  = 0b00011110;		//Frequency MC/256 Timer/Counter0 CTC OC2 PIND7 Toggle OC2 on Compare Match
	OCR2   = 0x00;  			//Write 0 in registry compare Timer/Counter2
}

void fSoundBeepOn(void)
{
	#ifdef IDPLUS974
		OCR2 = 0x02;
		DDRD  = 0b10000000;
	#endif
	#ifdef ID974LX
		DDRA |= 0b00000001;
		PORTA = 0b00000001;
	#endif
}

void fSoundBeepOff(void)
{
	#ifdef IDPLUS974
		DDRD  = 0b00000000;
	#endif
	#ifdef ID974LX
		DDRA |= 0b00000000;
		PORTA = 0b00000000;
	#endif
}

void fKeyBeep(void)
{
	#ifdef IDPLUS974
		fSoundBeepOn();
		_delay_ms(1000);
		fSoundBeepOff();
	#endif
	#ifdef ID974LX
		DDRA |= 0b00000001;
		PORTA = 0b00000001;
		_delay_ms(1000);
		DDRA |= 0b00000000;
		PORTA = 0b00000000;
	#endif
}