#include "adc.h"

uint8_t count_adc_conv = 0;													//Counter ADC conversion
uint16_t out_adc_result[] = { 0, 0 };										//ADC conversion result
uint16_t out_adc_temp_result = 0;											//Temporary ADC conversion result
int8_t temperature_bufer[]  = { 0, 0 };										//Temperature buffer
//Eliwell NTS 10kOm
uint16_t termo_table[] = {64671, 64284, 63757, 63052, 62130, 60948, 59466, 57649, 55476, 52944, 50072, 46902, 43500, 39949,
						  36341, 32768, 29312, 26040, 22999, 20220, 17714, 15480, 13506, 11775, 10265,  8954,  7817,  6834,  
						  5984,  5250,  4615};
//Interrupt handler at the end of ADC conversion
ISR(ADC_vect)
{
	if(count_adc_conv >= 64)
	{
		count_adc_conv = 0;													//Reset ADC counter conversion
		//Checking at which input the transformation was performed
		if(ADMUX & (1 << 0)) { out_adc_result[0] = out_adc_temp_result; }	//Saved ADC conversion result from ADC5
		else { out_adc_result[1] = out_adc_temp_result; }					//Saved ADC conversion result from ADC6
		out_adc_temp_result = 0;											//Reset temporary ADC conversion result
		ADMUX ^=  0b00000011;												//Input ADC5 or ADC6, U ref - AREF, right-aligned result.
	}
	else
	{
		out_adc_temp_result += ADCW;	
		count_adc_conv++;
	}
	ADCSRA |= 0b01000000;													//Start a single ADC conversion.
}
//ADC init and begin
void fADCStart(void)
{
	ADCSRA = 0b10001111;							//ADC on, enable interrupt at the end of conversion, frequency divider 1/128.
	ADMUX =  0b00000101;							//Input ADC5, U ref - AREF, right-aligned result.
	ADCSRA |= 0b01000000;							//Start a single ADC conversion.
}
//ADC stop and off
void fADCStop(void)
{
	ADCSRA = 0b00000111;							
}
//Calculation of temperature from the result of ADC conversion
int fCalculateTemperature(uint16_t in_adc_result)
{
	uint8_t l = 0;
	uint8_t r = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;
	uint16_t thigh = termo_table[r];
	
	if (in_adc_result <= thigh)
	{
		#ifdef TEMPERATURE_UNDER
		if (in_adc_result < thigh)
		return TEMPERATURE_UNDER;
		#endif
		return TEMPERATURE_TABLE_STEP * r + TEMPERATURE_TABLE_START;
	}
	
	uint16_t tlow = termo_table[0];
	
	if (in_adc_result >= tlow)
	{
		#ifdef TEMPERATURE_OVER
		if (in_adc_result > tlow)
		return TEMPERATURE_OVER;
		#endif
		return TEMPERATURE_TABLE_START;
	}

	while ((r - l) > 1)
	{
		uint8_t m = (l + r) >> 1;
		uint16_t mid = termo_table[m];
		if (in_adc_result > mid) { r = m; }
		else { l = m; }
	}
	
	uint16_t vl = termo_table[l];
	if (in_adc_result >= vl) { return l * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START; }
	uint16_t vr = termo_table[r];
	uint16_t vd = vl - vr;
	int16_t res = TEMPERATURE_TABLE_START + r * TEMPERATURE_TABLE_STEP;
	if (vd) { res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(in_adc_result - vr) + (vd >> 1)) / vd); }
	return res;
}