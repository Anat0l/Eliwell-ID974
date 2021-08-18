# Eliwell-ID974
Automotive refrigeration controller program.
For Eliwell ID974lx and IDPlus974.

ATMega32A
Screen with color symbols for operating modes
Three outputs
Two sensors
Works on sensors included

Custom settings for desired temperature and differential
Power Loss Stateful Shutdown
Fully automatic operating mode of the unit (Cooling or heating)
Automatic mode of purging the evaporator when the unit is cold
Automatic defrost mode of the evaporator when the unit is operating in cold weather
Manual defrost start
Hot gas defrost required!
Automatic easy start of the compressor
Integrity monitoring of temperature sensors
Temperature sensor fault codes display
Compressor service interval with recycling hours, reminder and reset. Display in hours


PORTA0 - output in speeker

PORTA5 - input sensor 1
POTRA6 - input sensor 2

PORTA2 - relay fan evaporator
PORTA3 - relay defrost output (switchable relay)
PORTA4 - relay compressor output

PORTB0 - input button DOWN
PORTB1 - input button UP
PORTB2 - input button FNC
PORTB3 - input button SET

PORTB0 - output led segment ()
PORTB1 - output led segment () 
PORTB2 - output led segment () 
PORTB3 - output led segment () 
PORTB4 - output led segment ()
PORTB5 - output led segment ()
PORTB6 - output led segment ()
PORTB7 - output led segment ()

PORTC2 - output led digit (0)?
PORTC3 - output led digit (1)?
PORTC4 - output led digit (2)?
PORTC5 - output led digit (3)?