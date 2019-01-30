#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>
#include "beeper.h"
#include "mcc.h"

void UART1_Initialize(void)
{
    //P.499:
    U1CON0bits.BRGS = 0;      //Normal speed (F/16)
    U1CON0bits.ABDEN = 0;     //No auto Baud-rate detection
    U1CON0bits.TXEN = 1;        //Enable TX
    U1CON0bits.MODE = 0b0000;   // 8 bit operation

    U1CON1bits.ON = 1;          // Serial Port ON =1
    
    //PIE3bits.U1TXIE = 1;        //Transmit Interrupt ON
    
    U1BRGH = 0b0000;            // Set the Baud rate to 9600
    U1BRGL = 0b0011;

    U1RXPPS = 0b010000;         //P.278 PPS setting; Set UART1 RX on the RC0 PIN
    RC1PPS = 0b010011;          //Set UART1 TX on the RC1 PIN
    
    TRISCbits.TRISC1 = 0;       //output TX
    TRISCbits.TRISC0 = 1;       //input RX
}


void serialSend(char word)
{
    U1TXB = word; 
}

void serialPrint(char string[])
{
    int len = strlen(string);
    for(int i=0; i<=len;i++)
    {
        serialSend(string[i]);
        __delay_ms(50);
        
    }
    U1TXB = 0b00000000;
}