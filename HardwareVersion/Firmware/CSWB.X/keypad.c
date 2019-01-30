#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include "mcc.h"
#include "keypad.h"
#include "beeper.h"

/*               KEYPAD_C1 KEYPAD_C2 KEYPAD_C3 KEYPAD_C4
//               |---------|---------|---------|---------|
//KEYPAD_ROW1    |    1    |    2    |    3    |    A    |      
//               |---------|---------|---------|---------|
//KEYPAD_ROW2    |    4    |    5    |    6    |    B    |
//               |---------|---------|---------|---------|
//KEYPAD_ROW3    |    7    |    8    |    9    |    C    |
//               |---------|---------|---------|---------|
//KEYPAD_ROW4    |   *     |    0    |    #    |    D    |
                 |---------|---------|---------|---------| */
    
/*=========================================================================
                              SETUP KEYPAD
=========================================================================*/
void KEYPAD_Initialize()
{
   
    /*OUTPUTS PINS*/
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    /*INPUT PINS*/
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    /*DIGITALREAD*/
    ANSELBbits.ANSELB4 = 0;
    ANSELBbits.ANSELB5 = 0;
    ANSELBbits.ANSELB6 = 0;
    ANSELBbits.ANSELB7 = 0;
    /*PULL UP RESITORS FOR INPUTS*/
    
}

keypadStruct keypadRead()
{
    keypadStruct output;
    volatile unsigned char keyHeld = 0;
    volatile unsigned char keyPressed = 0;
    volatile unsigned char key = 0;
    
    unsigned int flagPressed = 0;
    unsigned int keyHoldTime = 0;
    
    while(key == NONE)
    {
        key = keypadScan();
        
        if(key != NONE)
        {
            keyPressed = key;
            flagPressed = TRUE;
            
            if (keyBeeps == ON)
            {
                normalBeep();
            }
            
            else
            {
                normalBlink();
            }
        }
    }
    
    while(flagPressed == TRUE)
    {
        __delay_ms(10);
        keyHoldTime += 10;
        key = keypadScan();
        
        if(key == NONE)
        {
            flagPressed = 0;
        }
        
        if (keyHoldTime == 1000)
        {
            keyHeld = key;
            
            if (keyBeeps == ON)
            {
                quickBeep();
                __delay_ms(100);
                quickBeep(); 
            }
            
            else
            {
                quickBlink();
                __delay_ms(100);
                quickBlink();
            }
        }
    }
    
    output.keyPressed = keyPressed;
    output.keyHeld = keyHeld;
    return output;
    
}

int keypadScan()
{
    //VALUES ARE IN UTF8 ENCODING
    KP1_R1 = 1;
    if (PORTBbits.RB4 == 1){KP1_R1=0;return '1';/*1*/}
    else if(PORTBbits.RB5 == 1){KP1_R1=0;return '2';/*2*/}
    else if(PORTBbits.RB6 == 1){KP1_R1=0;return '3';/*3*/}
    else if(PORTBbits.RB7 == 1){KP1_R1=0;return 'A';/*A*/}
    else {KP1_R1 = 0;}
    __delay_ms(1);
    
    KP1_R2 = 1;
    if (PORTBbits.RB4 == 1){KP1_R2=0;return '4';}
    else if(PORTBbits.RB5 == 1){KP1_R2=0;return '5';}
    else if(PORTBbits.RB6 == 1){KP1_R2=0;return '6';}
    else if(PORTBbits.RB7 == 1){KP1_R2=0;return 'B';}
    else {KP1_R2 = 0;}
    __delay_ms(1);
    
    KP1_R3 = 1;
    if (PORTBbits.RB4 == 1){KP1_R3=0;return '7';}
    else if(PORTBbits.RB5 == 1){KP1_R3=0;return '8';}
    else if(PORTBbits.RB6 == 1){KP1_R3=0;return '9';}
    else if(PORTBbits.RB7 == 1){KP1_R3=0;return 'C';}
    else {KP1_R3 = 0;}
    __delay_ms(1);
    
    KP1_R4 = 1;
    if (PORTBbits.RB4 == 1){KP1_R4=0;return '*';/***/}
    else if(PORTBbits.RB5 == 1){KP1_R4=0;return '0';/*0*/}
    else if(PORTBbits.RB6 == 1){KP1_R4=0;return '#';/*#*/}
    else if(PORTBbits.RB7 == 1){KP1_R4=0;return 'D';/*D*/}
    else {KP1_R4 = 0;}
    __delay_ms(1);
    
    return 0;
}

