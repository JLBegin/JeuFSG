#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include "mcc.h"
#include "keypad.h"
#include "beeper.h"
#include "countdown.h"
#include "lcd.h"
#include "UART.h"


void COUNTDOWN_Initialize()
{
    /*SET THE TIMER0 REGISTER AND INETRRUPT PARAMETERS*/
    PIE3bits.TMR0IE = 1; //ENABLES TIMER1 INTERRUPT
    IPR3bits.TMR0IP = 1; //HIGH PRIORITY;
    PIR3bits.TMR0IF = 0;    // Sets interrupt flag to 0.
    T0CON0bits.EN = 0; //Stops timer
    T0CON0bits.MD16 = 1; //16 bits mode
    T0CON1bits.CS = 0b011; //Internal clock
    T0CON1bits.ASYNC = 1; //Not synchronous with F/4
    T0CON1bits.CKPS = 0b0001;//1:2 prescaler 0.01s = 40 000 counts
}

void countdownStart()
{  
    TMR0H = 0x63;//65535-40000 = 25535 (0x63BF))
    TMR0L = 0xBF;
    T0CON0bits.EN = 1;
}

void countdownStop()
{
    T0CON0bits.EN = 0;
}

void countdown_now_reset_to_initial()
{
   for(int i=0;i<=5;i++)
   {
        numCountdownNowTime[i] = numCountdownInitialTime[i];
        if(2>i){strCountdownNowTime[i] = strCountdownInitialTime[i];}
        if(2<=i && i<4){strCountdownNowTime[i+1] = strCountdownInitialTime[i+1];}
        if(4<=i && i<6){strCountdownNowTime[i+2] = strCountdownInitialTime[i+2];}
   }
}

void countdown_initial_str2num()
{
    for(int i=0;i<=7;i++)
    {
        if(i==0||i==1)
        {
            if(strCountdownInitialTime[i]=='0'){numCountdownInitialTime[i] = 0;}
            if(strCountdownInitialTime[i]=='1'){numCountdownInitialTime[i] = 1;}
            if(strCountdownInitialTime[i]=='2'){numCountdownInitialTime[i] = 2;}
            if(strCountdownInitialTime[i]=='3'){numCountdownInitialTime[i] = 3;}
            if(strCountdownInitialTime[i]=='4'){numCountdownInitialTime[i] = 4;}
            if(strCountdownInitialTime[i]=='5'){numCountdownInitialTime[i] = 5;}
            if(strCountdownInitialTime[i]=='6'){numCountdownInitialTime[i] = 6;}
            if(strCountdownInitialTime[i]=='7'){numCountdownInitialTime[i] = 7;}
            if(strCountdownInitialTime[i]=='8'){numCountdownInitialTime[i] = 8;}
            if(strCountdownInitialTime[i]=='9'){numCountdownInitialTime[i] = 9;}
        }
        
        if(i==3||i==4)
        {
            if(strCountdownInitialTime[i]=='0'){numCountdownInitialTime[i-1] = 0;}
            if(strCountdownInitialTime[i]=='1'){numCountdownInitialTime[i-1] = 1;}
            if(strCountdownInitialTime[i]=='2'){numCountdownInitialTime[i-1] = 2;}
            if(strCountdownInitialTime[i]=='3'){numCountdownInitialTime[i-1] = 3;}
            if(strCountdownInitialTime[i]=='4'){numCountdownInitialTime[i-1] = 4;}
            if(strCountdownInitialTime[i]=='5'){numCountdownInitialTime[i-1] = 5;}
            if(strCountdownInitialTime[i]=='6'){numCountdownInitialTime[i-1] = 6;}
            if(strCountdownInitialTime[i]=='7'){numCountdownInitialTime[i-1] = 7;}
            if(strCountdownInitialTime[i]=='8'){numCountdownInitialTime[i-1] = 8;}
            if(strCountdownInitialTime[i]=='9'){numCountdownInitialTime[i-1] = 9;}
        }
        
        if(6==i||i==7)
        {
            if(strCountdownInitialTime[i]=='0'){numCountdownInitialTime[i-2] = 0;}
            if(strCountdownInitialTime[i]=='1'){numCountdownInitialTime[i-2] = 1;}
            if(strCountdownInitialTime[i]=='2'){numCountdownInitialTime[i-2] = 2;}
            if(strCountdownInitialTime[i]=='3'){numCountdownInitialTime[i-2] = 3;}
            if(strCountdownInitialTime[i]=='4'){numCountdownInitialTime[i-2] = 4;}
            if(strCountdownInitialTime[i]=='5'){numCountdownInitialTime[i-2] = 5;}
            if(strCountdownInitialTime[i]=='6'){numCountdownInitialTime[i-2] = 6;}
            if(strCountdownInitialTime[i]=='7'){numCountdownInitialTime[i-2] = 7;}
            if(strCountdownInitialTime[i]=='8'){numCountdownInitialTime[i-2] = 8;}
            if(strCountdownInitialTime[i]=='9'){numCountdownInitialTime[i-2] = 9;}
        }
    }
}

void countdown_now_num2str()
{
    for(int i=0;i<=5;i++)
    {
        if(i==0||i==1)
        {
            if(numCountdownNowTime[i]==0){strCountdownNowTime[i] = '0';}
            if(numCountdownNowTime[i]==1){strCountdownNowTime[i] = '1';}
            if(numCountdownNowTime[i]==2){strCountdownNowTime[i] = '2';}
            if(numCountdownNowTime[i]==3){strCountdownNowTime[i] = '3';}
            if(numCountdownNowTime[i]==4){strCountdownNowTime[i] = '4';}
            if(numCountdownNowTime[i]==5){strCountdownNowTime[i] = '5';}
            if(numCountdownNowTime[i]==6){strCountdownNowTime[i] = '6';}
            if(numCountdownNowTime[i]==7){strCountdownNowTime[i] = '7';}
            if(numCountdownNowTime[i]==8){strCountdownNowTime[i] = '8';}
            if(numCountdownNowTime[i]==9){strCountdownNowTime[i] = '9';}
        }
        
        if(i==2||i==3)
        {
            if(numCountdownNowTime[i]==0){strCountdownNowTime[i+1] = '0';}
            if(numCountdownNowTime[i]==1){strCountdownNowTime[i+1] = '1';}
            if(numCountdownNowTime[i]==2){strCountdownNowTime[i+1] = '2';}
            if(numCountdownNowTime[i]==3){strCountdownNowTime[i+1] = '3';}
            if(numCountdownNowTime[i]==4){strCountdownNowTime[i+1] = '4';}
            if(numCountdownNowTime[i]==5){strCountdownNowTime[i+1] = '5';}
            if(numCountdownNowTime[i]==6){strCountdownNowTime[i+1] = '6';}
            if(numCountdownNowTime[i]==7){strCountdownNowTime[i+1] = '7';}
            if(numCountdownNowTime[i]==8){strCountdownNowTime[i+1] = '8';}
            if(numCountdownNowTime[i]==9){strCountdownNowTime[i+1] = '9';}
        }
        
        if(i==4||i==5)
        {
            if(numCountdownNowTime[i]==0){strCountdownNowTime[i+2] = '0';}
            if(numCountdownNowTime[i]==1){strCountdownNowTime[i+2] = '1';}
            if(numCountdownNowTime[i]==2){strCountdownNowTime[i+2] = '2';}
            if(numCountdownNowTime[i]==3){strCountdownNowTime[i+2] = '3';}
            if(numCountdownNowTime[i]==4){strCountdownNowTime[i+2] = '4';}
            if(numCountdownNowTime[i]==5){strCountdownNowTime[i+2] = '5';}
            if(numCountdownNowTime[i]==6){strCountdownNowTime[i+2] = '6';}
            if(numCountdownNowTime[i]==7){strCountdownNowTime[i+2] = '7';}
            if(numCountdownNowTime[i]==8){strCountdownNowTime[i+2] = '8';}
            if(numCountdownNowTime[i]==9){strCountdownNowTime[i+2] = '9';}
        }
    }
}

void countdown_drop_timer()
{
    if(numCountdownNowTime[5]!=0){numCountdownNowTime[5]--;}
    else
    {
        if  (numCountdownNowTime[4]!=0)
        {
            
            numCountdownNowTime[5]=9;
            numCountdownNowTime[4]--;
        }
        else
        {
            
            if  (numCountdownNowTime[3]!=0)
            {
                
                if (countdownBeeps == ON){normalBeep();serialSend('T');}
                numCountdownNowTime[3]--;
                numCountdownNowTime[5]=9;
                numCountdownNowTime[4]=9;
            }
            else
            {
                if (countdownBeeps == ON){normalBeep();serialSend('T');}
                if  (numCountdownNowTime[2]!=0)
                {
                    
                    numCountdownNowTime[2]--;
                    numCountdownNowTime[5]=9;
                    numCountdownNowTime[4]=9;
                    numCountdownNowTime[3]=9;
                }
                else
                {
                    
                    if  (numCountdownNowTime[1]!=0)
                    {
                        
                        numCountdownNowTime[1]--;
                        numCountdownNowTime[5]=9;
                        numCountdownNowTime[4]=9;
                        numCountdownNowTime[3]=9;
                        numCountdownNowTime[2]=5;
                    }
                    else
                    {
                        
                        if  (numCountdownNowTime[0]!=0)
                        {
                            numCountdownNowTime[0]--;
                            numCountdownNowTime[5]=9;
                            numCountdownNowTime[4]=9;
                            numCountdownNowTime[3]=9;
                            numCountdownNowTime[2]=5;
                            numCountdownNowTime[1]=9;
                        }
                        else {countdownStop(); status = exploded; endBeep();}

                    }
                }
            }
        }
    }
    
    countdown_now_num2str();
    LCD_printNowCountdown();
}