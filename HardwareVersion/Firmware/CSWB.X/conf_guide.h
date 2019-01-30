/*
 THIS IS JUST AN INFORMATIVE FILE
 * |---------------------------------------------------------------------------|
 * |                         PIN SETUP INFORMATIONS                            |
 * |---------------------------------------------------------------------------|
 * To setup a pin, you need to set 8 registers.
 * PORTX - LATX - TRISX - ANSELX - WPUX - INLVLX - SLRCONX - ODCONX
 * 
 * PORTX is usually used to read value on a pin. #value = PORTAbits.RA0
 * 
 * LATX is the output latch. It controls a pin when it is in OUPUT mode.
 * WRITING on the LATX as the same effect than writing on PORTX. You can only
 * LATX when the pin is an output. #LATAbits.LATA0 = 1; sets pin A0 to HIGH.
 * 
 * TRISX is the data direction selector. 0 is output. 1 is input.
 * #TRISAbits.TRISA0 = 0; sets pin A0 as an output.
 * 
 * ANSELX is the analogRead register. It only matters if the pin is in input
 * mode. If on, it will act like an ADC, if not it will return HIGH or LOW.
 * 
 * WPUX is the weak pull-up resistor settings.
 * It connects a pull-up with an high resistance to allow a low current to pass.
 * 
 * INLVLX is the input threshold control. If inputs are not ANALOG, this
 * register will set the input mode to TTL or to CMOS.
 * 
 * SLRCONX is the speed of data transfer through this pin. it's fastest by default.
 * 
 * ODCONX is a drain control pin. If this bit is set (1), the input becomes
 *  an open drain that can only sink current. it cannot source.
 
 
 
 * |---------------------------------------------------------------------------|
 * |                       INTERUPT SETUP INFORMATIONS                         |
 * |---------------------------------------------------------------------------|
 
 
 * 
 * 
 * |---------------------------------------------------------------------------|
 * |                         ADC SETUP INFORMATIONS                            |
 * |---------------------------------------------------------------------------|
 * P.603 of PIC18F47K42 microcontroller. (ADC2).
 * 
 * We need to configure all of the following things:
 * -(1)PORT CONFIGURATION
 * -(2)CHANNEL SELECTION
 * -(3)VOLTAGE REFERENCES
 * -(4)CLOCK SOURCE
 * -(5)INTERRUPT CONTROL
 * -(6)FORMATTING
 * -(7)CONVERSION TRIGGER
 * -(8)ACQUISITION TIME
 * -(9)PRECHARGE TIME
 * -(10)OTHER THINGS WE WONT TOUCH
 * 
 * (1)PORT CONFIGURATION
 * The ADC can convert digital and analog signals. The I/O pins should be
 * configured to analog inputs by setting the TRIS and ANSEL bits.
 * For input analog: 