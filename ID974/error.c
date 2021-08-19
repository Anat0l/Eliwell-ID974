#include "error.h"

uint8_t error_state = 0;
uint8_t error_code = 0b00000000;
uint8_t error_bufer[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
int8_t error_count = 0;
int8_t error_pos = 0;

uint8_t fCheckError(int8_t temperature_bufer[])
{
	error_code = 0;
	
	if(temperature_bufer[0] >= 100) { error_code |= ERROR_E10; }	//temperature sensor 1 open
	if(temperature_bufer[0] <= -50) { error_code |= ERROR_E11; }	//temperature sensor 1 short
	if(temperature_bufer[1] >= 100) { error_code |= ERROR_E20; }	//temperature sensor 2 open
	if(temperature_bufer[1] <= -50) { error_code |= ERROR_E21; }	//temperature sensor 2 short
	
	if(error_code == 0b00000000) { return NO_ERROR; }
	else { return error_code; }
}

int8_t fGetError(uint8_t error_code, uint8_t error_bufer[])
{
	uint8_t err_count = 0;
	error_bufer[0] = 0;
	error_bufer[1] = 0;
	error_bufer[2] = 0;
	error_bufer[3] = 0;
	
	if((error_code & ERROR_E10)) 
	{
		for(int i = 0; i < 4; i++)
		{
			if(error_bufer[i] == 0)
			{
				error_bufer[i] = ERROR_E10; 
				err_count++;
				break;
			}
		}
	}
	if((error_code & ERROR_E11))
	{
		for(int i = 0; i < 4; i++)
		{
			if(error_bufer[i] == 0)
			{
				error_bufer[i] = ERROR_E11;
				err_count++;
				break;
			}
		}
	}
	if((error_code & ERROR_E20))
	{
		for(int i = 0; i < 4; i++)
		{
			if(error_bufer[i] == 0)
			{
				error_bufer[i] = ERROR_E20;
				err_count++;
				break;
			}
		}
	}
	if((error_code & ERROR_E21))
	{
		for(int i = 0; i < 4; i++)
		{
			if(error_bufer[i] == 0)
			{
				error_bufer[i] = ERROR_E21;
				err_count++;
				break;
			}
		}
	}
	return err_count;
}