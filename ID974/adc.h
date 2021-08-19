#ifndef ADC_H_
#define ADC_H_

#include "main.h"

#define TEMPERATURE_UNDER		-50
#define TEMPERATURE_OVER		100
#define TEMPERATURE_TABLE_START -50
#define TEMPERATURE_TABLE_STEP	  5

uint8_t count_adc_conv;								//ADC conversion counter
uint16_t out_adc_result[2];							//ADC conversion result
uint16_t out_adc_temp_result;						//Temporary ADC conversion result
int8_t temperature_bufer[2];						//Temperature buffer

void fADCStart(void);								//ADC init and begin
void fADCStop(void);								//ADC stop and off
int fCalculateTemperature(uint16_t in_adc_result);	//Calculation of temperature from the result of ADC conversion

#endif /* ADC_H_ */