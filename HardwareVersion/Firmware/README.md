# PIC-keypad-LCD-interface-FSG-game
A program that makes an interface on an LCD screen +keyPad to interact with a MasterMind game. The interface is controlled via a 4x4 keypad. The microcontroller must be a PIC18F47K42 or equivalent. The schematic is included in the Hardware directory.  If you find something you might need, I'm glad you did. upvote the directory!

## Derivatives:
* Compiler: XC8 v2.0
* IDE: MPLABX v5.0
* LCD: 1602-A (SPLC780D1 Controller)
* Keypad: STANDARD 4x4
* Programmer/Debugger: PICkit4

## Version 0.8:
This version is a version improved to communicate with a MasterMind software. 
### These bugs have been fixed:
* Countdown reset would only reset on the screen, but the real time didn't change.
* When changing scope, the countdown time would not update if changed by the user.
* Some beeping incoherence
* Overall 'main.c' spring-cleaning.
* End of countdown doesn't go into infinite loop anymore

### These features have been added:
* Start typing beeps and disarming beeps
* initial values of parameters to 0
* Parameters do not change beween scopes
* hold '3' makes the countdown beep OFF or ON.
* removed possibility to change parameters while countdown running.
* The user Input are sent through UART1 on pins RC0=RX and RC1=TX at a baudrate speed of 124380Hz (weird frequency synchronization of clocks), 8bits, no parity, 1 stopbit.

### These should be done in a next version:

- Find the problem on the baudrate-generator! (high priority)

* Add the accelerating beeping after the 45 second barrier
* Add the alarm Clock Mode
* Manage EEPROM to save settings or passwords even after shutdown
* Correct weird beeping while typing when countdown is running (low priority)
* Manage to make a sleep function in case left open
* Clean the other libraries of the code
* Look for a UnitTest library in Embedded C compatile with the XC8.

