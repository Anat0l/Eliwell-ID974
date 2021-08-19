#include "output.h"


uint8_t main_mode_out = 0;
uint8_t error_mode_out = 0;
uint8_t out_status = 0;
int unload_count = 0;

void fOutInit(void)
{
	DDRA   = 0b00011100;																//Registry 2, 3, 4 PORTA - relay control outputs
	PORTA  = 0b00000000;
	_delay_ms(1);
}

uint8_t fSetOut(int8_t temperature_bufer[], int8_t set_temp, int8_t set_diff, uint8_t in_command)
{
	if(in_command != 0)
	{
		if(in_command == COMMAND_OUT_OFF)
		{
			main_mode_out = MAIN_MODE_OUT_NONE;
			PORTA = OUT_NONE;
			out_status = OUT_SET_OK;
		}
		if(in_command == COMMAND_MANUAL_DEFROST)
		{
			main_mode_out = MAIN_MODE_OUT_DEFROST;
			out_status = OUT_SET_OK;
		}
	}
	
	switch(main_mode_out)
	{
		case MAIN_MODE_OUT_NONE:														//Outputs off, check work mode
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_NONE;
				out_status = OUT_SET_OK;
			}
			
			if(comp_on_time != 0)
			{
				if(service_state == SERVICE_OK)
				{
					eeprom_write_dword(&eep_comp_on, (comp_on_time + comp_on)); 
					comp_on_time = 0;
				}
				if(service_state == SERVICE_NO)
				{
					eeprom_write_dword(&eep_comp_over_on, (comp_on_time + comp_over_on));
					comp_on_time = 0;
				}
			}
			
			if(in_command != 1)
			{
				if(temperature_bufer[0] > (set_temp + set_diff))						//Frost
				{
					main_mode_out = MAIN_MODE_OUT_UNLOAD_FROST;	
				}
				if(temperature_bufer[0] < (set_temp - set_diff))						//Heat
				{
					main_mode_out = MAIN_MODE_OUT_UNLOAD_HEAT;	
				}
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_UNLOAD_FROST:												//Unloading before starting the compressor
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_DEFROST;
				unload_count = 0;
				out_status = OUT_SET_OK;
			}
			if(unload_count > 20000)
			{
				if(temperature_bufer[0] > (set_temp + set_diff))						//Frost
				{
					main_mode_out = MAIN_MODE_OUT_FROST;
				}
				if(temperature_bufer[0] <= set_temp)
				{
					main_mode_out = MAIN_MODE_OUT_NONE;
				}
				out_status = OUT_SET_NONE;
				unload_count = 0;
			}
			else
			{
				unload_count++;
			}
			break;
		case MAIN_MODE_OUT_FROST:														//Frost and fan off
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_COMPRESSOR;
				out_status = OUT_SET_OK;
			}
			else
			{
				if((temperature_bufer[0] - temperature_bufer[1]) >= 4)
				{
					main_mode_out = MAIN_MODE_OUT_FROST_AND_FAN;
				}
				else
				{
					if(temperature_bufer[0] <= set_temp) 
					{ 
						main_mode_out = MAIN_MODE_OUT_NONE; 
					}
				}
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_FROST_AND_FAN:												//Frost and fan on
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_COMPRESSOR | OUT_FAN;
				out_status = OUT_SET_OK;
			}
			else
			{
				if(temperature_bufer[1] <= -25)											//Check defrost
				{
					main_mode_out = MAIN_MODE_OUT_DEFROST;
				}
				if(temperature_bufer[0] - temperature_bufer[1] >= 20)					//Evaporator purge
				{
					main_mode_out = MAIN_MODE_OUT_FAN;
				}
				out_status = OUT_SET_NONE;
			}
			if(temperature_bufer[0] <= set_temp)
			{
				main_mode_out = MAIN_MODE_OUT_NONE;
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_UNLOAD_HEAT:													//Unloading before starting the compressor
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_DEFROST;
				unload_count = 0;
				out_status = OUT_SET_OK;
			}
			if(unload_count > 20000)
			{
				if(temperature_bufer[0] < (set_temp - set_diff))						//Heat
				{
					main_mode_out = MAIN_MODE_OUT_HEAT;
				}
				if(temperature_bufer[0] >= set_temp)
				{
					main_mode_out = MAIN_MODE_OUT_NONE;
				}
				out_status = OUT_SET_NONE;
				unload_count = 0;
			}
			else
			{
				unload_count++;
			}
			break;
		case MAIN_MODE_OUT_HEAT:														//Heat and fan off
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_COMPRESSOR | OUT_DEFROST;
				out_status = OUT_SET_OK;
			}
			else
			{
				if(temperature_bufer[0] >= set_temp)
				{
					main_mode_out = MAIN_MODE_OUT_NONE;
				}
				else
				{
					if((temperature_bufer[1] - temperature_bufer[0]) >= 4)
					{
						main_mode_out = MAIN_MODE_OUT_HEAT_AND_FAN;
					}
				}
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_HEAT_AND_FAN:												//Heat and fan on
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_COMPRESSOR | OUT_FAN | OUT_DEFROST;
				out_status = OUT_SET_OK;
			}
			else
			{
				if(temperature_bufer[0] >= set_temp)
				{
					main_mode_out = MAIN_MODE_OUT_NONE;
				}
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_FAN:															//Purge
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_FAN;
				out_status = OUT_SET_OK;
			}
			if(temperature_bufer[0] <= set_temp)
			{
				main_mode_out = MAIN_MODE_OUT_NONE;
				out_status = OUT_SET_NONE;
			}
			else
			{
				if(temperature_bufer[0] - temperature_bufer[1] <= 4)					
				{
					main_mode_out = MAIN_MODE_OUT_FROST_AND_FAN;
				}
				out_status = OUT_SET_NONE;
			}
			break;
		case MAIN_MODE_OUT_DEFROST:														//Defrost
			if(out_status == OUT_SET_NONE)
			{
				PORTA = OUT_COMPRESSOR | OUT_DEFROST;
				out_status = OUT_SET_OK;
			}
			if(temperature_bufer[1] >= 25)
			{
				main_mode_out = MAIN_MODE_OUT_NONE;
				out_status = OUT_SET_NONE;
			}
			break;
	}
	return main_mode_out;
}

uint8_t fSetOutErrorMode(int8_t temperature_bufer[], int8_t set_temp, int8_t set_diff)
{
	switch(error_mode_out)
	{
		case ERROR_MODE_OUT_NONE:
			break;
		case ERROR_MODE_OUT_FROST_AND_FAN:
			break;
		case ERROR_MODE_OUT_HEAT_AND_FAN:
			break;
	}
	return error_mode_out;
}