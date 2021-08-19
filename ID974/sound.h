#include "main.h"

#ifndef SOUND_H_
#define SOUND_H_

//PORTA PIN0 - output speaker	(ID974lx)
//PORTD PIN7 - output speaker	(IDPlus974)

void fSoundInit(void);
void fSoundBeepOn(void);
void fSoundBeepOff(void);
void fKeyBeep(void);

#endif /* SOUND_H_ */