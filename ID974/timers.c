#include "timers.h"

//Interrupt handler when TIMER0
ISR(TIMER0_COMP_vect)
{
	button_key = fReadKeyAndLedindUpdate(ledind_bufer, signal_bufer);	
	TCNT0 = 0x00;														//Reset Timer/Counter0
}
//Interrupt handler when TIMER1 matches A
ISR(TIMER1_COMPA_vect)
{
	//0.5 sec
	if((PORTA & OUT_COMPRESSOR) == OUT_COMPRESSOR) { comp_on_time++; }
	temperature_bufer[0] = fCalculateTemperature(out_adc_result[0]);	//Calculation temperature from the first sensor
	temperature_bufer[1] = fCalculateTemperature(out_adc_result[1]);	//Calculation temperature from the second sensor
	TCNT1 = 0x00;														//Reset Timer/Counter1
}
//Setting Timers
void fTimersInit(void)
{
	TCCR0  = 0b00000101;												//Frequency MC/1024 Timer/Counter0
	TCNT0  = 0x00;														//Reset Timer/Counter0
	OCR0   = 0x28;														//Write 40 in compare registry Timer/Counter0
	TIMSK |= 0b00000010;												//IRQ compare on Timer/Counter0
	
	TCCR1B = 0b00000101;												//Frequency MC/1024 Timer/Counter1
	TCNT1  = 0x00;														//Reset Timer/Counter1
	OCR1A  = 0x7A1;														//Write 7813 (0x1E85) in compare registry Timer/Counter1A
	TIMSK |= 0b00010000;												//IRQ compare on Timer/Counter1A
}
