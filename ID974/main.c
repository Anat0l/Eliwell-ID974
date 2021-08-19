#include "main.h"

uint8_t  eep_mode			EEMEM = 0;								//Saved work mode
uint8_t  eep_temp			EEMEM = 25;								//Saved temperature (C)
uint8_t  eep_diff			EEMEM = 2;								//Saved differential set (C)
uint32_t eep_comp_on		EEMEM = 0;								//Saved compressor work time
uint32_t eep_comp_over_on	EEMEM = 0;								//Saved compressor work overtime

int8_t temp_set = 0;
int8_t set_temp_min =	-18;
int8_t set_temp_max =	 50; //?
int8_t set_diff_min =	  1;
int8_t set_diff_max =	 10;

uint8_t menu_user_mode = 0;											//Menu user modes
uint8_t menu_adm_mode = 0;											//Menu admin modes
uint8_t button_state = 0;											//Button state
uint8_t in_signal = 0;												//Signal of indicator
uint8_t button_key = 0b00000000;									//Button id

int8_t off_bufer[] = { 0, 0 };

int main(void)
{
    uint8_t main_mode = eeprom_read_byte(&eep_mode);				
	int8_t set_temp = eeprom_read_byte(&eep_temp);					
    int8_t set_diff = eeprom_read_byte(&eep_diff);					
	comp_on = eeprom_read_dword(&eep_comp_on);						
	comp_over_on = eeprom_read_dword(&eep_comp_over_on);			
	
	int count_key_press = 0;										//Button press duration
	int count_change = 0;											//Parameter change counter
	int32_t count_disp_error = 0;									//Error display duration
	
	service_state = 0;												//Check service on
	comp_on_time = 0;												//Compressor operating time counter

	fTimersInit();
	fLedindInit();
	fOutInit();

#ifdef IDPLUS974
	fSoundInit();
#endif
	
	sei();															
	
	//Check compressor overtime 7192800/2 = time in sec.
	if(comp_on >= 7192800) { service_state = SERVICE_NO; }
	else { service_state = SERVICE_OK; }
	
	//Load animation
	if(main_mode != OFF_MODE) 
	{ 
		fADCStart(); 
		fLoadAnimation();
		//If 999 h work time, we display a warning when power is applied, if the unit is not turned off
		if(service_state == SERVICE_NO) { fServiceAnimation(service_state); } 
	}
	else { fOffAnimation(0); }
	
	while (1) 
    {
		switch(main_mode)
		{
			case NORMAL_MODE:													//Normal mode
				//Output setting ------------------------------------------------------------------------------------------------------------
				error_code = fCheckError(temperature_bufer);
				if(error_code == NO_ERROR)
				{
					//Check compressor overtime 7192800/2 = time in sec.
					if(comp_on >= 7192800) { service_state = SERVICE_NO; }
					else { service_state = SERVICE_OK; }
					in_signal = fSetOut(temperature_bufer, set_temp, set_diff, 0);
					fLedindSignal(in_signal, signal_bufer);	
				}
				else
				{
					in_signal = fSetOut(off_bufer, 0, 0, COMMAND_OUT_OFF);
					fLedindSignal(in_signal, signal_bufer);
					fErrorAnimation();
					main_mode = ERROR_MODE;
				}	
				//----------------------------------------------------------------------------------------------------------------------------
				switch(button_key)
				{
					case BUTTON_NONE:											//Show the current temperature in the volume
						fLedindNumber(temperature_bufer[0], ledind_bufer);
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:												//Show the set temperature
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 500)
							{
								//Defrost manual on
								if(temperature_bufer[1] <= -5)
								{
									in_signal = fSetOut(temperature_bufer, set_temp, set_diff, COMMAND_MANUAL_DEFROST);
								}
								else
								{
									for(int i = 0; i < 5; i++)
									{
										fLedindNumber(temperature_bufer[1], ledind_bufer);
										_delay_ms(1000);
										fSoundBeepOn();
										ledind_bufer[3] = 0b11111111; ledind_bufer[2] = 0b11111111; ledind_bufer[1] = 0b11111111; ledind_bufer[0] = 0b11111111;
										_delay_ms(1000);
										fSoundBeepOff();
									}
								}
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
							}
							else
							{
								_delay_ms(50);
								fLedindNumber(set_temp, ledind_bufer);
								count_key_press++;
							}
						}
						else
						{
							fLedindNumber(set_temp, ledind_bufer);
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_DOWN:											//Show the current temperature on the evaporator
						if(button_state == BUTTON_RELEASED)
						{
							fLedindNumber(temperature_bufer[1], ledind_bufer);	
							button_state = BUTTON_PRESSED;
						}
						else
						{
							fLedindNumber(temperature_bufer[1], ledind_bufer);
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_FNC:											//In OFF_MODE
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								main_mode = OFF_MODE;
								eeprom_write_byte(&eep_mode, main_mode);
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fADCStop();
								fOffAnimation(1);
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:											//In user mode
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								in_letters[3] = 0b11111111; in_letters[2] = T; in_letters[1] = 0b11111111; in_letters[0] = 0b11111111; //t
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fLedindLetter(in_letters, ledind_bufer);
								main_mode = MENU_USER_MODE;
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
				}
				break;
			case OFF_MODE:														//Sleep mode and save state
				in_signal = fSetOut(off_bufer, 0, 0, COMMAND_OUT_OFF);
				fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);		//Symbol (C) off
				switch(button_key)
				{
					case BUTTON_NONE:
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:
						break;
					case BUTTON_DOWN:	//MENU_SERVICE_MODE						//View service
						if(count_key_press > 1000)
						{
							//Animation in service
							fServiceAnimation(service_state);
							comp_on = eeprom_read_dword(&eep_comp_on);						//Reading saved running time of the compressor
							comp_over_on = eeprom_read_dword(&eep_comp_over_on);			//Reading saved compressor overtime
							main_mode = MENU_SERVICE_MODE;
							button_state = BUTTON_PRESSED;
							count_key_press = 0;
						}
						else
						{
							_delay_ms(50);
							count_key_press++;
						}
						break;
					case BUTTON_FNC:											//In normal mode
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								fADCStart();
								main_mode = NORMAL_MODE;
								eeprom_write_byte(&eep_mode, main_mode);
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fOnAnimation();
								//Check compressor overtime 7192800/2 = time in sec.
								//If 999 h work time, we display a warning when power is applied, if the unit is not turned off
								if(comp_on >= 7192800) { service_state = SERVICE_NO; fServiceAnimation(service_state); }
								else { service_state = SERVICE_OK; }
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:
						break;
				}
				break;
			case MENU_SERVICE_MODE:												//Service view and edit mode
				in_signal = fSetOut(off_bufer, 0, 0, COMMAND_OUT_OFF);
				fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);		//Symbol (C) off
				switch(button_key)
				{
					case BUTTON_NONE:
						if(service_state == SERVICE_OK) { fLedindNumber((comp_on / 7200), ledind_bufer); }
						if(service_state == SERVICE_NO) { fLedindNumber((comp_over_on / 7200), ledind_bufer); }
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:
						break;
					case BUTTON_DOWN:
						break;
					case BUTTON_FNC:
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								main_mode = OFF_MODE;
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fADCStop();
								fOffAnimation(1);
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:														//Reset service
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 1000)
							{
								comp_on = eeprom_read_dword(&eep_comp_on);						//Reading saved running time of the compressor
								comp_over_on = eeprom_read_dword(&eep_comp_over_on);			//Reading saved compressor overtime
								if(comp_on != 0) { eeprom_write_dword(&eep_comp_on, 0); }
								if(comp_over_on != 0) { eeprom_write_dword(&eep_comp_over_on, 0); }
								comp_on = 0;
								comp_over_on = 0;	
								comp_on_time = 0;
								fServiceAnimation(SERVICE_RESET);
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
				}
				break;
			case ERROR_MODE:													//Emergency mode
				//Output configure -----------------------------------------------------------------------------------------------------------
				error_code = fCheckError(temperature_bufer);
				if(error_code != NO_ERROR)
				{
					error_count = fGetError(error_code, error_bufer);
					error_pos = 0;
					in_signal = fSetOutErrorMode(temperature_bufer, set_temp, set_diff);
					fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
				}
				else
				{
					main_mode = NORMAL_MODE;
				}
				//----------------------------------------------------------------------------------------------------------------------------
				switch(button_key)
				{
					case BUTTON_NONE:
						in_letters[3] = 0b11111111; in_letters[2] = E; in_letters[1] = R; in_letters[0] = R; //Err
						fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
						fLedindLetter(in_letters, ledind_bufer);
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:
						if(button_state == BUTTON_RELEASED)
						{
							main_mode = ERROR_LIST_MODE;
							count_disp_error = 0;
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_DOWN:
						if(button_state == BUTTON_RELEASED)
						{
							main_mode = ERROR_LIST_MODE;
							count_disp_error = 0;
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_FNC:											//OFF_MODE
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								main_mode = OFF_MODE;
								eeprom_write_byte(&eep_mode, main_mode);		//Save state OFF_MODE
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fADCStop();										//ADC off
								fOffAnimation(1);								//Animation off
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:
						if(button_state == BUTTON_RELEASED)
						{
							fErrorAnimation();
							button_state = BUTTON_PRESSED;
						}
						break;
				}
				break;
			case ERROR_LIST_MODE:												//View error mode
				error_code = fCheckError(temperature_bufer);
				if(error_code == NO_ERROR)
				{
					main_mode = NORMAL_MODE;
					_delay_ms(1);	
				}
				switch(button_key)
				{
					case BUTTON_NONE:
						if(count_disp_error > 300000)
						{
							main_mode = ERROR_MODE;
							count_disp_error = 0;
						}
						else
						{
							fDisplayError(error_bufer[error_pos]);
							count_disp_error++;
						}
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:
						if(button_state == BUTTON_RELEASED)
						{
							if(error_pos > 0) { error_pos--; }
							else { error_pos = error_count - 1; }
							fDisplayError(error_bufer[error_pos]);
							count_disp_error = 0;
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_DOWN:
						if(button_state == BUTTON_RELEASED)
						{
							if(error_pos < (error_count - 1)) { error_pos++; }
							else { error_pos = 0; }
							fDisplayError(error_bufer[error_pos]);
							count_disp_error = 0;
							button_state = BUTTON_PRESSED;
						}
						break;
					case BUTTON_FNC:											//OFF_MODE
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								main_mode = OFF_MODE;
								eeprom_write_byte(&eep_mode, main_mode);		//Save state OFF_MODE
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fADCStop();													//ADC off
								fOffAnimation(1);											//Animation off
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:
						if(button_state == BUTTON_RELEASED)
						{
							fErrorAnimation();
							button_state = BUTTON_PRESSED;
						}
						break;
				}
				break;
			case MENU_USER_MODE:												//User mode
				in_signal = fSetOut(off_bufer, 0, 0, COMMAND_OUT_OFF);			//Output off
				switch(menu_user_mode)
				{
					case MENU_USER_MODE_SET_TEMP:								//Menu: temperature setting
						switch(button_key)
						{
							case BUTTON_NONE:
								in_letters[3] = 0b11111111; in_letters[2] = T; in_letters[1] = 0b11111111; in_letters[0] = 0b11111111; //t
								fLedindLetter(in_letters, ledind_bufer);
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								button_state = BUTTON_RELEASED;
								count_key_press = 0;
								count_change = 0;
								break;
							case BUTTON_UP:										//Switch menu sections in a circle
								if(button_state == BUTTON_RELEASED)
								{
									if(menu_user_mode == MENU_USER_MODE_SET_TEMP) { menu_user_mode = MENU_USER_MODE_SET_DIFF; }
									else { menu_user_mode = MENU_USER_MODE_SET_TEMP; }									
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_DOWN:									//Switch menu sections in a circle
								if(button_state == BUTTON_RELEASED)
								{
									if(menu_user_mode == MENU_USER_MODE_SET_TEMP) { menu_user_mode = MENU_USER_MODE_SET_DIFF; }
									else { menu_user_mode = MENU_USER_MODE_SET_TEMP; }
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_FNC:									//Out in normal mode
								if(button_state == BUTTON_RELEASED)
								{
									fLedindNumber(temperature_bufer[0], ledind_bufer);
									main_mode = NORMAL_MODE;
									menu_user_mode = MENU_USER_MODE_SET_TEMP;
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_SET:									//In temperature setting
								if(button_state == BUTTON_RELEASED)
								{
									menu_user_mode = MENU_USER_MODE_SET_TEMP_EDIT;
									temp_set = set_temp;
									button_state = BUTTON_PRESSED;
								}
								break;
						}
						break;
					case MENU_USER_MODE_SET_TEMP_EDIT:							//Temperature setting
						switch(button_key)
						{
							case BUTTON_NONE:
								fLedindNumber(temp_set, ledind_bufer);
								fLedindSignal(MAIN_MODE_OUT_NONE, signal_bufer);	//Symbol (C) on
								button_state = BUTTON_RELEASED;
								count_key_press = 0;
								count_change = 0;
								break;
							case BUTTON_UP:										//Set temperature up
								if(button_state == BUTTON_RELEASED)
								{
									if(temp_set < set_temp_max) { temp_set++; }
									fLedindNumber(temp_set, ledind_bufer);
									button_state = BUTTON_PRESSED;
								}
								else 
								{ 
									if(count_key_press > 100)
									{
										if(temp_set < set_temp_max) { temp_set++; }
										fLedindNumber(temp_set, ledind_bufer);
										count_key_press = 0;
										count_change++;
									}
									else
									{
										if(count_change < 2) { _delay_ms(50); }
										else { _delay_ms(10); }
										count_key_press++;
									}
								}
								break;
							case BUTTON_DOWN:									//Set temperature down
								if(button_state == BUTTON_RELEASED)
								{
									if(temp_set > set_temp_min) { temp_set--; }
									fLedindNumber(temp_set, ledind_bufer);
									button_state = BUTTON_PRESSED;
								}
								else 
								{ 
									if(count_key_press > 100)
									{
										if(temp_set > set_temp_min) { temp_set--; }
										fLedindNumber(temp_set, ledind_bufer);
										count_key_press = 0;
										count_change++;
									}
									else
									{
										if(count_change < 2) { _delay_ms(50); }
										else { _delay_ms(10); }
										count_key_press++;
									}
								}
								break;
							case BUTTON_FNC:									//Exit without saving to the menu item Temperature setting
								if(button_state == BUTTON_RELEASED)
								{
									temp_set = 0;
									menu_user_mode = MENU_USER_MODE_SET_TEMP;
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_SET:									//Exit and save to the Temperature setting menu section
								if(button_state == BUTTON_RELEASED)
								{
									if(set_temp != temp_set)
									{
										set_temp = temp_set;
										eeprom_write_byte(&eep_temp, set_temp);
									}
									menu_user_mode = MENU_USER_MODE_SET_TEMP;
									button_state = BUTTON_PRESSED;
								}
								break;
						}
						break;
					case MENU_USER_MODE_SET_DIFF:								//Menu: Differential setting
						switch(button_key)
						{
							case BUTTON_NONE:
								in_letters[3] = 0b11111111; in_letters[2] = D; in_letters[1] = I; in_letters[0] = F; //dif
								fLedindLetter(in_letters, ledind_bufer);
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								button_state = BUTTON_RELEASED;
								count_key_press = 0;
								count_change = 0;
								break;
							case BUTTON_UP:										//Switch menu sections in a circle
								if(button_state == BUTTON_RELEASED)
								{
									if(menu_user_mode == MENU_USER_MODE_SET_TEMP) { menu_user_mode = MENU_USER_MODE_SET_DIFF; }
									else { menu_user_mode = MENU_USER_MODE_SET_TEMP; }
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_DOWN:									//Switch menu sections in a circle
								if(button_state == BUTTON_RELEASED)
								{
									if(menu_user_mode == MENU_USER_MODE_SET_TEMP) { menu_user_mode = MENU_USER_MODE_SET_DIFF; }
									else { menu_user_mode = MENU_USER_MODE_SET_TEMP; }
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_FNC:									//Out in normal mode
								if(button_state == BUTTON_RELEASED)
								{
									fLedindNumber(temperature_bufer[0], ledind_bufer);
									fLedindSignal(MAIN_MODE_OUT_NONE, signal_bufer);	//Symbol (C) on
									main_mode = NORMAL_MODE;
									menu_user_mode = MENU_USER_MODE_SET_TEMP;
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_SET:									//In differential setting
								if(button_state == BUTTON_RELEASED)
								{
									menu_user_mode = MENU_USER_MODE_SET_DIFF_EDIT;
									temp_set = set_diff;
									button_state = BUTTON_PRESSED;
								}
								break;
						}
						break;
					case MENU_USER_MODE_SET_DIFF_EDIT:							//Differential set
					{
						switch(button_key)
						{
							case BUTTON_NONE:
								fLedindNumber(temp_set, ledind_bufer);
								fLedindSignal(MAIN_MODE_OUT_NONE, signal_bufer);	//Symbol (C) on
								button_state = BUTTON_RELEASED;
								count_key_press = 0;
								count_change = 0;
								break;
							case BUTTON_UP:										//Differential change up
								if(button_state == BUTTON_RELEASED)
								{
									if(temp_set < set_diff_max) { temp_set++; }
									fLedindNumber(temp_set, ledind_bufer);
									button_state = BUTTON_PRESSED;
								}
								else
								{
									if(count_key_press > 100)
									{
										if(temp_set < set_diff_max) { temp_set++; }
										fLedindNumber(temp_set, ledind_bufer);
										count_key_press = 0;
										count_change++;
									}
									else
									{
										if(count_change < 2) { _delay_ms(50); }
										else { _delay_ms(10); }
										count_key_press++;
									}
								}
								break;
							case BUTTON_DOWN:									//Differential change down
								if(button_state == BUTTON_RELEASED)
								{
									if(temp_set > set_diff_min) { temp_set--; }
									fLedindNumber(temp_set, ledind_bufer);
									button_state = BUTTON_PRESSED;
								}
								else
								{
									if(count_key_press > 100)
									{
										if(temp_set > set_diff_min) { temp_set--; }
										fLedindNumber(temp_set, ledind_bufer);
										count_key_press = 0;
										count_change++;
									}
									else
									{
										if(count_change < 2) { _delay_ms(50); }
										else { _delay_ms(10); }
										count_key_press++;
									}
								}
								break;
							case BUTTON_FNC:									//Exit without saving to the menu section Differential setting
								if(button_state == BUTTON_RELEASED)
								{
									temp_set = 0;
									menu_user_mode = MENU_USER_MODE_SET_DIFF;
									button_state = BUTTON_PRESSED;
								}
								break;
							case BUTTON_SET:									//Exit with saving to the menu section Differential setting
								if(button_state == BUTTON_RELEASED)
								{
									if(set_diff != temp_set)
									{
										set_diff = temp_set;
										eeprom_write_byte(&eep_diff, set_diff);
									}
									menu_user_mode = MENU_USER_MODE_SET_DIFF;
									button_state = BUTTON_PRESSED;
								}
								break;
						}
						break;
					}
				}
				break;
			case MENU_ADM_MODE:													//Admin work mode
				in_signal = fSetOut(off_bufer, 0, 0, COMMAND_OUT_OFF);
				fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);		//Symbol (C) off
				switch(button_key)
				{
					case BUTTON_NONE:
						button_state = BUTTON_RELEASED;
						count_key_press = 0;
						count_change = 0;
						break;
					case BUTTON_UP:
						break;
					case BUTTON_DOWN:
						break;
					case BUTTON_FNC:
						if(button_state == BUTTON_RELEASED)
						{
							if(count_key_press > 100)
							{
								main_mode = OFF_MODE;
								if(main_mode != OFF_MODE) { eeprom_write_byte(&eep_mode, main_mode); }
								button_state = BUTTON_PRESSED;
								count_key_press = 0;
								fLedindSignal(MAIN_MODE_OUT_NONE_SIGNAL_OFF, signal_bufer);	//Symbol (C) off
								fADCStop();
								fOffAnimation(1);
							}
							else
							{
								_delay_ms(50);
								count_key_press++;
							}
						}
						break;
					case BUTTON_SET:
						break;
				}
			break;
		}
    }
}

