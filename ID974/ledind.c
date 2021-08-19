#include "ledind.h"

uint8_t digits[]   = { LEDIND_DIGIT_3, LEDIND_DIGIT_2, LEDIND_DIGIT_1, LEDIND_DIGIT_0 };
uint8_t numbers[]  = { LEDIND_NUMBER_0, LEDIND_NUMBER_1, LEDIND_NUMBER_2, LEDIND_NUMBER_3, LEDIND_NUMBER_4, 
					   LEDIND_NUMBER_5, LEDIND_NUMBER_6, LEDIND_NUMBER_7, LEDIND_NUMBER_8, LEDIND_NUMBER_9 };
uint8_t sumbols[]  = { LEDIND_MINUS, LEDIND_POINT};
	
#ifdef IDPLUS974	//IDPlus974
uint8_t signals[]  = { LEDIND_NONE, LEDIND_SOUND, LEDIND_FAN, LEDIND_DEFROST, LEDIND_FROST, LEDIND_HEAT, LEDIND_FAR, LEDIND_CEL, LEDIND_AUX };
#endif
#ifdef ID974LX		//ID974lx
uint8_t signals[]  = { LEDIND_NONE, LEDIND_SOUND, LEDIND_FAN, LEDIND_DEFROST, LEDIND_FROST, LEDIND_HEAT };
#endif

uint8_t letters[]  = { LEDIND_LETTER_D, LEDIND_LETTER_F, LEDIND_LETTER_I, LEDIND_LETTER_T };

uint8_t ledind_count_digits = 0;
uint8_t out_button = 0;
uint8_t ledind_bufer[] = { 0b11111111, 0b11111111, 0b11111111, 0b11111111 };	//Indicator buffer
uint8_t signal_bufer[] = { 0b11111111, 0b11111111, 0b11111111, 0b11111111 };	//Signal buffer
uint8_t in_letters[] = { 0b11111111, 0b11111111, 0b11111111, 0b11111111 };		//String symbols for the indicator
	
void fLedindInit(void)
{
	DDRB  = 0b11111111;
	DDRC  = 0b00111100;
	_delay_ms(1);
}

void fLedindNumber(int in_number, uint8_t out_ledind_bufer[])
{
	int temp = 0;
	int number = 0;
	int minus = 0;
	
	if(in_number < 0)
	{
		number = -in_number;
		minus = 1;
	}
	else
	{
		number = in_number;
		minus = 0;
	}
	
	for(int i = 0; i < 4; i++)
	{
		temp = number % 10;
		if(i == 0 || number > 0) { out_ledind_bufer[i] = numbers[temp]; }
		else
		{
			if(minus == 1)
			{
				out_ledind_bufer[i] = sumbols[0];
				minus = 0;
			}
			else
			{
				out_ledind_bufer[i] = 0b11111111;
			}
		}
		number /= 10;
	}
}

#ifdef IDPLUS974	//IDPlus974
void fLedindSignal(uint8_t in_signal, uint8_t out_ledind_bufer[])
{
	switch(in_signal)
	{
		case (MAIN_MODE_OUT_NONE):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[7]; 
			break;
		case MAIN_MODE_OUT_FROST:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[4]; 	//FROST
			out_ledind_bufer[3] = signals[7]; 
			break;
		case (MAIN_MODE_OUT_FROST_AND_FAN):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[2]; 	//FAN
			out_ledind_bufer[2] = signals[4]; 	//FROST
			out_ledind_bufer[3] = signals[7]; 
			break;	
		case MAIN_MODE_OUT_HEAT:
			out_ledind_bufer[0] = signals[5]; 	//HEAT
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[7]; 
			break;
		case (MAIN_MODE_OUT_HEAT_AND_FAN):
			out_ledind_bufer[0] = signals[5]; 	//HEAT
			out_ledind_bufer[1] = signals[2]; 	//FAN
			out_ledind_bufer[2] = signals[0]; 	//FROST
			out_ledind_bufer[3] = signals[7]; 
			break;
		case MAIN_MODE_OUT_FAN:
			out_ledind_bufer[0] = signals[0];
			out_ledind_bufer[1] = signals[2];	//FAN
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[7]; 
			break;
		case (MAIN_MODE_OUT_DEFROST):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[3] & signals[7];	//DEFROST
			break;
		case MAIN_MODE_OUT_UNLOAD_FROST:
			out_ledind_bufer[0] = signals[0];
			out_ledind_bufer[1] = signals[0];
			out_ledind_bufer[2] = signals[0];
			out_ledind_bufer[3] = signals[7];	//DEFROST
			break;
		case MAIN_MODE_OUT_UNLOAD_HEAT:
			out_ledind_bufer[0] = signals[0];
			out_ledind_bufer[1] = signals[0];
			out_ledind_bufer[2] = signals[0];
			out_ledind_bufer[3] = signals[7];	//DEFROST
			break;
		case MAIN_MODE_OUT_NONE_SIGNAL_OFF:
			out_ledind_bufer[0] = signals[0];
			out_ledind_bufer[1] = signals[0];
			out_ledind_bufer[2] = signals[0];
			out_ledind_bufer[3] = signals[0];
			break;
	}
}
#endif

#ifdef ID974LX //ID974lx
void fLedindSignal(uint8_t in_signal, uint8_t out_ledind_bufer[])
{
	switch(in_signal)
	{
		case (MAIN_MODE_OUT_NONE):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[0]; 
			break;
		case MAIN_MODE_OUT_FROST:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[4]; 	//FROST
			out_ledind_bufer[3] = signals[0]; 
			break;
		case (MAIN_MODE_OUT_FROST_AND_FAN):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[4]; 	//FROST
			out_ledind_bufer[3] = signals[2]; 	//FAN
			break;
		case MAIN_MODE_OUT_HEAT:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[5]; 	//HEAT
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[0]; 
			break;
		case (MAIN_MODE_OUT_HEAT_AND_FAN):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[5]; 	//HEAT
			out_ledind_bufer[2] = signals[0]; 	//FROST
			out_ledind_bufer[3] = signals[2]; 	//FAN
			break;
		case MAIN_MODE_OUT_FAN:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[2]; 	//FAN
			break;
		case (MAIN_MODE_OUT_DEFROST):
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[3]; 	//DEFROST
			break;
		case MAIN_MODE_OUT_UNLOAD_FROST:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[0]; 	//DEFROST
			break;
		case MAIN_MODE_OUT_UNLOAD_HEAT:
			out_ledind_bufer[0] = signals[0]; 
			out_ledind_bufer[1] = signals[0]; 
			out_ledind_bufer[2] = signals[0]; 
			out_ledind_bufer[3] = signals[0]; 	//DEFROST
			break;
		case MAIN_MODE_OUT_NONE_SIGNAL_OFF:
			out_ledind_bufer[0] = signals[0];
			out_ledind_bufer[1] = signals[0];
			out_ledind_bufer[2] = signals[0];
			out_ledind_bufer[3] = signals[0];
			break;
	}
}
#endif

void fLedindLetter(uint8_t in_string[], uint8_t out_ledind_bufer[])
{
	out_ledind_bufer[0] = in_string[0];
	out_ledind_bufer[1] = in_string[1];
	out_ledind_bufer[2] = in_string[2];
	out_ledind_bufer[3] = in_string[3];
}

uint8_t fReadKeyAndLedindUpdate(uint8_t out_ledind_bufer[], uint8_t in_signal_bufer[])
{
	uint8_t mask = 0;
	DDRB = 0b00000000;
	DDRC = 0b00000000;
	PORTC = 0b00000000;
	if(ledind_count_digits == 4) { ledind_count_digits = 0; }
	out_button = ~PINB;
	DDRB  = 0b11111111;
	PORTB = 0b11111111;
	PORTC = 0b00000000;
	mask = out_ledind_bufer[ledind_count_digits];
	mask &= in_signal_bufer[ledind_count_digits];
	PORTB = mask;
	DDRC  = digits[ledind_count_digits];
	PORTC = digits[ledind_count_digits];
	ledind_count_digits++;
	return out_button;
}

void fLoadAnimation(void)
{
	for(int i = 0; i < 5; i++)
	{
		ledind_bufer[0] = sumbols[0];
		ledind_bufer[1] = sumbols[0];
		ledind_bufer[2] = sumbols[0];
		ledind_bufer[3] = 0b11111111;
		_delay_ms(2000);
		ledind_bufer[0] = 0b11111111;
		ledind_bufer[1] = 0b11111111;
		ledind_bufer[2] = 0b11111111;
		ledind_bufer[3] = 0b11111111;
		_delay_ms(2000);
	}
}

void fOnAnimation(void)
{
	in_letters[3] = 0b11111111; in_letters[2] = 0b00100100; in_letters[1] = N; in_letters[0] = 0b11111111;
	fLedindLetter(in_letters, ledind_bufer);
	fKeyBeep();
	_delay_ms(20000);
	ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
}

void fOffAnimation(uint8_t sound_beep)
{
	in_letters[3] = 0b11111111; in_letters[2] = 0b00100100; in_letters[1] = F; in_letters[0] = F; //t
	fLedindLetter(in_letters, ledind_bufer);
	if(sound_beep != 0) { fKeyBeep(); }
	_delay_ms(20000);
	ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
}

void fServiceAnimation(uint8_t service_state)
{
	for(int i = 0; i < 5; i++)
	{
		if(service_state == SERVICE_OK)
		{
			in_letters[3] = 0b11111111; in_letters[2] = S; in_letters[1] = S; in_letters[0] = S; //SSS
			fLedindLetter(in_letters, ledind_bufer);
			_delay_ms(1000);
			//fSoundBeepOn();
			ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
			_delay_ms(1000);
			//fSoundBeepOff();
		}
		if(service_state == SERVICE_NO)
		{
			in_letters[3] = 0b11111111; in_letters[2] = 0b00000101; in_letters[1] = 0b00000101; in_letters[0] = 0b00000101; //999
			fLedindLetter(in_letters, ledind_bufer);
			_delay_ms(1000);
			fSoundBeepOn();
			ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
			_delay_ms(1000);
			fSoundBeepOff();
		}
		if(service_state == SERVICE_RESET)
		{
			in_letters[3] = 0b11111111; in_letters[2] = 0b00100100; in_letters[1] = 0b00100100; in_letters[0] = 0b00100100; //000
			fLedindLetter(in_letters, ledind_bufer);
			_delay_ms(1000);
			//fSoundBeepOn();
			ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
			_delay_ms(1000);
			//fSoundBeepOff();
		}
	}
}

void fErrorAnimation(void)
{
	for(int i = 0; i < 5; i++)
	{
		in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = R; in_letters[0] = R; //Err
		fLedindLetter(in_letters, ledind_bufer);
		_delay_ms(1000);
		fSoundBeepOn();		
		ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
		_delay_ms(1000);
		fSoundBeepOff();
	}
}

void fDisplayError(uint8_t code_error)
{
	if(code_error == ERROR_E10)									//temperature sensor 1 open
	{
		in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = 0b10110111; in_letters[0] = 0b00100100; //E10
		fLedindLetter(in_letters, ledind_bufer);
	}
	
	if(code_error == ERROR_E11)									//temperature sensor 1 short
	{
		in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = 0b10110111; in_letters[0] = 0b10110111; //E11
		fLedindLetter(in_letters, ledind_bufer);
	}
	
	if(code_error == ERROR_E20)									//temperature sensor 2 open
	{
		in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = 0b01010100; in_letters[0] = 0b00100100; //E20
		fLedindLetter(in_letters, ledind_bufer);
	}
	
	if(code_error == ERROR_E21)									//temperature sensor 2 short
	{
		in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = 0b01010100; in_letters[0] = 0b10110111; //E21
		fLedindLetter(in_letters, ledind_bufer);
	}
}