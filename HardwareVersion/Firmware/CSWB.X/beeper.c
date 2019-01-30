#include <stdlib.h>
#include <stdio.h>
#include "mcc.h"
#include "beeper.h"
#include "lcd.h"
#include "countdown.h"
#include <xc.h>

/*=========================================================================
                              SETUP BEEPER
=========================================================================*/

void BEEPER_Initialize()
{
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;       //A1 is an output for PWM5
    
    RA1PPS = 0b001101;         //sets the PWM5 on pin A1
    
    /*----------------------PWM FSETUP WITH TMR2--------------------------|
    |          Period = (T2PR+1)*4*(1/Fosc)*T2prescale                    |
    |          Duty cycle = (PWM5DCH:PWM5DCL)/(4*(T2PR+1))                |
    |--------------------------------------------------------------------*/
    
    CCPTMRS1bits.P5TSEL = 0b01; //PWM5 uses TMR2
    //Weird, PWM5DCH/L don't match the calculations.
    PWM5DCH = 0x39;             //sets high byte duty-cycle to 50%.
    PWM5DCL = 0x10;             //sets low byte duty-cycle to 50%.
    PWM5CONbits.EN = 0;         //close the PWM output
    T2CLKbits.CS = 0b0001;      // sets T2clk at F/4 as required by PWM module
    T2PR = 0x72;                // sets T2PR at 114 as required for 4.3kHz
    T2CONbits.CKPS = 0b010;     //sets prescaler of TMR2 to 4.
    T2CONbits.ON = 1;           //turns ON the TMR2
    
    /*-------------------------TMR 1 SETUP--------------------------------|
    |           Timer 1 is used do make the LED and BEEP interrupt        |
    |--------------------------------------------------------------------*/
    T1CON = 0b00110000;             //sets prescaler to 8 and 16bits counter
    T1CLK = 0b00001;                //F_OSC/4 
    PIE4bits.TMR1IE = 1;            //ENABLE interrupt flag
    PIR4bits.TMR1IF = 0;            //Sets flag to 0.    
}  


void interrupt ISR(void)
{
    //BEEPER
    if (PIR4bits.TMR1IF == 1)
    {
        BEEPER1 = 0;
        LED1 = 0;
        T1CONbits.ON = 0;
        PIR4bits.TMR1IF = 0;
        T2PR = 0x72;
    }
    
    
    if (PIR3bits.TMR0IF == 1)
    {
        PIR3bits.TMR0IF = 0;
        T0CON0bits.EN = 0;
        
        countdown_drop_timer();
        
        if (status == armed)
        {
            TMR0H = 0x63;//65535-40000 = 25535 (0x63BF))
            TMR0L = 0xBF;
            T0CON0bits.EN = 1;
        }
        
        else if (status == exploded)
        {
            LCD_Set_Cursor(1,1);
            LCD_Write_String("DONE  ");
            LCD_Set_Cursor(2,1);
            LCD_Write_String("    EXPLODED    ");
        }
        
     }
     
}

void quickBeep()
{
    TMR1H = 0xCE;   //approx. 50ms
    TMR1L = 0x57;
    BEEPER1 = 1;
    LED1 = 1;
    T1CONbits.ON = 1;
}

void quickBlink()
{
    TMR1H = 0xCE;   //approx. 50ms
    TMR1L = 0x57;
    LED1 = 1;
    T1CONbits.ON = 1;
}

void longBeep()
{
    TMR1H = 0x54;   //approx. 200ms
    TMR1L = 0x1F;
    BEEPER1 = 1;
    LED1 = 1;
    T1CONbits.ON = 1;
}

void normalBeep()
{
    TMR1H = 0xAE;   //approx. 100ms
    TMR1L = 0x57;
    BEEPER1 = 1;
    LED1 = 1;
    T1CONbits.ON = 1;
}

void normalBlink()
{
    TMR1H = 0xAE;
    TMR1L = 0x57;
    LED1 = 1;
    T1CONbits.ON = 1;
}


void veryLowBeep()
{
    T2PR = 0xFF;
    TMR1H = 0xCE;   //approx. 50ms
    TMR1L = 0x57;
    BEEPER1 = 1;
    LED1 = 1;
    T1CONbits.ON = 1;
}

void armBeep()
{
    veryLowBeep();
    __delay_ms(110);
    veryLowBeep();
}

void disarmBeep()
{
    veryLowBeep();
    __delay_ms(110);
    veryLowBeep();
    __delay_ms(110);
    veryLowBeep();
}

void longLowBeep()
{   
    T2PR = 0xF2;
    TMR1H = 0x54;   //approx. 200ms
    TMR1L = 0x1F;
    BEEPER1 = 1;
    LED1 = 1;
    T1CONbits.ON = 1;
}


void endBeep()
{
    BEEPER1 = 1;
    //T0CON0bits.EN = 1;
    T2PR = 0xB4;
    for (int i=0;i<32;i++)
    {
        T2PR = T2PR-1;
        __delay_ms(32);}
    
    T2PR = 0xF4;
    
    for (int i=0;i<12;i++)
    {
        BEEPER1=0;
        __delay_ms(52);
        BEEPER1=1;
        __delay_ms(29);
    }
    BEEPER1 = 0;
    T2PR = 0x72;
}