#include "main.h"

#ifndef ERROR_H_
#define ERROR_H_

#define ERROR_NONE			0b00000000
#define ERROR_E10			0b00000001
#define ERROR_E11			0b00000010
#define ERROR_E20			0b00000100
#define ERROR_E21			0b00001000

#define NO_ERROR			0

uint8_t error_code;
uint8_t error_bufer[4];
int8_t error_count;
int8_t error_pos;

uint8_t fCheckError(int8_t temperature_bufer[]);
int8_t fGetError(uint8_t error_code, uint8_t error_bufer[]);

#endif /* ERROR_H_ */