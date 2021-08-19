#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "main.h"

#define OUT_NONE		0b00000000			//All output off							PORTA
#define OUT_FAN			0b00000100			//Output fan								PORTA2
#define OUT_DEFROST		0b00001000			//Output defrost							PORTA3
#define OUT_COMPRESSOR	0b00010000			//Output compressor							PORTA4

#define OUT_SET_NONE	0
#define OUT_SET_OK		1

#define MAIN_MODE_OUT_NONE				0
#define MAIN_MODE_OUT_FROST				1
#define MAIN_MODE_OUT_FROST_AND_FAN		2
#define MAIN_MODE_OUT_HEAT				3
#define MAIN_MODE_OUT_HEAT_AND_FAN		4
#define MAIN_MODE_OUT_FAN				5
#define MAIN_MODE_OUT_DEFROST			6
#define MAIN_MODE_OUT_UNLOAD_FROST		7
#define MAIN_MODE_OUT_UNLOAD_HEAT		8
#define MAIN_MODE_OUT_NONE_SIGNAL_OFF	9

#define ERROR_MODE_OUT_NONE				0
#define ERROR_MODE_OUT_FROST_AND_FAN	2
#define ERROR_MODE_OUT_HEAT_AND_FAN		4

#define COMMAND_OUT_OFF					1
#define COMMAND_MANUAL_DEFROST			2

uint32_t eep_comp_on;							//Compressor work time
uint32_t eep_comp_over_on;						//Compressor work overtime

uint32_t comp_on_time;							//Counter compressor work time
uint8_t  service_state;							//Check service
uint32_t comp_on;								//Saved compressor work time
uint32_t comp_over_on;							//Saved compressor work overtime

void fOutInit(void);
uint8_t fSetOut(int8_t temperature_bufer[], int8_t set_temp, int8_t set_diff, uint8_t in_command);
uint8_t fSetOutErrorMode(int8_t temperature_bufer[], int8_t set_temp, int8_t set_diff);

#endif /* OUTPUT_H_ */