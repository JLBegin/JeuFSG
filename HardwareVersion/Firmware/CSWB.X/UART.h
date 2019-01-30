#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>

void UART1_Initialize(void);
void serialSend(char word);
void serialPrint(char string[]);
void serialTimerstart();
void serialTimerstop();