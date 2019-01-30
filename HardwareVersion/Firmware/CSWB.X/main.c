#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>

#include "mcc.h"
#include "beeper.h"
#include "keypad.h"
#include "lcd.h"
#include "countdown.h"
#include "UART.h"

/*GLOBAL KEYPAD VARIABLES*/
    keypadStruct keypadOutput;
    unsigned char keyHeld = 0;
    unsigned char keyPressed = 0; 

    char userPswdEntry[8] = {0,0,0,0,0,0,0};
    int gameMode = 1;
    
void setup(void)
{  
    UART1_Initialize();
    SYSTEM_Initialize();
    BEEPER_Initialize(); 
    KEYPAD_Initialize();
    COUNTDOWN_Initialize();
    LCD_Initialize();
    //serialSend(0xAA);
}

/*=========================================================================
                            BASIC FUNCTIONS
=========================================================================*/    
void resetEntry(char str[])
{
    for(int i=0;i<=strlen(str);i++)
    {
        str[i]=0;
    }
}

void resetPasswordEntry()
{
    for(int i=0;i<=6;i++)
    {
    userPswdEntry[i] = 0;
    }
}

int verifyPassword(char user[],char pass[])
{
    if (strcmp(user, pass) == 0){return 1;}
    else {return 0;}
}

void stanbyPageReset()
{
    keyIterrator = 0;
    resetEntry(userPswdEntry);
    
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Write_String("STANBY  ");
    LCD_printNowCountdown();
    LCD_resetPasswordEntry();  
}

void armedPageReset()
{
    LCD_Set_Cursor(1,1);
    LCD_Write_String("ARMED ");
    keyIterrator = 0;
    resetEntry(userPswdEntry);
    LCD_resetPasswordEntry(); 
}

void disarmedPageReset()
{
    keyIterrator = 0;
    resetEntry(userPswdEntry);
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Write_String("STOPED  ");
    LCD_printNowCountdown();
    LCD_resetPasswordEntry(); 
}

void verifyInterfaceOptions()
{
    if (keyBeepsMem == ON && status != armed){keyBeeps = ON;}
    else {keyBeeps = OFF;}
    if (countdownBeepsMem == ON){countdownBeeps = ON;}
    else {countdownBeeps = OFF;}
    if (cursorBlinkMem == ON && status != armed){LCD_Cursor_Blink_On();}
    else {LCD_Cursor_Blink_Off();}
}



/*=========================================================================
                                MAIN PROGRAM
=========================================================================*/
void main(void)
{
   
    setup();
    stanbyPageReset();
    
    while (1)
    {
        
        if(mode == countdownMode)
        {
            verifyInterfaceOptions();
            keypadOutput = keypadRead();
            keyPressed = keypadOutput.keyPressed;
            keyHeld = keypadOutput.keyHeld;

            /*=================== STANBY STATUS =======================*/

            if(status == stanby)
            { 
                if(keyPressed != NONE && keyHeld == NONE)
                {
            
                    if(keyIterrator == 0 && keyBeeps == OFF)
                    {
                        armBeep();
                    }
                    if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                    {
                        
                        userPswdEntry[keyIterrator] = keyPressed;
                        keyIterrator++;

                        LCD_Set_Cursor(2,9+keyIterrator);
                    
                        LCD_Write_Char(keyPressed);
                    }
                    
                    if (keyPressed == '#' || keyPressed == '*'&& keyHeld == NONE)
                    {   
                        
                        isPasswordTrue = verifyPassword(userPswdEntry,password);
                        __delay_ms(10);  
                        resetPasswordEntry();
                        
                        if(isPasswordTrue == TRUE)
                        {
                            
                            LCD_printAccepted();
                            LCD_Set_Cursor(2,1);
                            if(gameMode == 1)
                            {
                                serialSend('S');
                                LCD_Write_Char('5');
                                LCD_Set_Cursor(2,1);
                                __delay_ms(1000);
                                
                                LCD_Write_Char('4');
                                LCD_Set_Cursor(2,1);
                                __delay_ms(1000);
                                
                                LCD_Write_Char('3');
                                LCD_Set_Cursor(2,1);
                                __delay_ms(1000);
                                
                                LCD_Write_Char('2');
                                LCD_Set_Cursor(2,1);
                                __delay_ms(1000);
                                
                                LCD_Write_Char('1');
                                LCD_Set_Cursor(2,1);
                                __delay_ms(1000);
                             
                            }
                            LCD_Set_Cursor(1,1);
                            LCD_Write_String("ARMED ");
                            armedPageReset();
                            countdownStart();
                            status = armed;
                            continue;
                        }

                        else if (isPasswordTrue == FALSE)
                        {
                            LCD_printDeclined();
                            __delay_ms(250);  
                            stanbyPageReset();
                            continue;
                        }
                    }
                }

                else if(keyHeld != NONE)
                {
                    
                    if(keyPressed == '#' && keyHeld == '#')
                    {
                        if(gameMode != 1){
                        keyIterrator = 0;
                        passwordChange = TRUE;
                        LCD_Set_Cursor(1,1);
                        LCD_Write_String("PASSWORD CHANGE ");
                        resetEntry(userPswdEntry);
                        LCD_resetPasswordEntry();
                        while (passwordChange == TRUE)
                        {
                            keypadOutput = keypadRead();
                            keyPressed = keypadOutput.keyPressed;
                            keyHeld = keypadOutput.keyHeld;
                            
                            if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                            {
                                userPswdEntry[keyIterrator] = keyPressed;
                                keyIterrator++;

                                LCD_Set_Cursor(2,9+keyIterrator);

                                LCD_Write_Char(keyPressed);
                            }

                                if (keyPressed == '#' || keyPressed == '*'&& keyHeld == NONE)
                                {
                                    { 
                                    __delay_ms(10);
                                    isPasswordTrue = verifyPassword(userPswdEntry,password);
                                    resetPasswordEntry();
                                    
                                    if(isPasswordTrue == TRUE)
                                        
                                    {
                                        serialSend('N');
                                        keyIterrator = 0;
                                        LCD_printAccepted();
                                        LCD_Set_Cursor(1,1);
                                        LCD_Write_String("NEW PASSWORD    ");
                                        LCD_resetPasswordEntry();
                                        while (passwordChange == TRUE)
                                        {
                                            keypadOutput = keypadRead();
                                            keyPressed = keypadOutput.keyPressed;
                                            keyHeld = keypadOutput.keyHeld;

                                            if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                                            {
                                                serialSend('N');
                                                serialSend(keyPressed);
                                                userPswdEntry[keyIterrator] = keyPressed;
                                                keyIterrator++;

                                                LCD_Set_Cursor(2,9+keyIterrator);

                                                LCD_Write_Char(keyPressed);
                                            }

                                                if (keyPressed == '#' || keyPressed == '*'&& keyHeld == NONE)
                                                {
                                                    serialSend('N');
                                                    serialSend('*');
                                                    for (int i=0;i<=6;i++){password[i] = userPswdEntry[i];}
                                                    passwordChange = FALSE;
                                                    resetPasswordEntry();
                                                    stanbyPageReset();
                                                    break;
                                                }
                                                
                                                else{continue;}
                                            
                                        }
                                    }
                                    else if (isPasswordTrue == FALSE)
                                    {
                                        passwordChange = FALSE;
                                        LCD_printDeclined();
                                        resetEntry(userPswdEntry);
                                        stanbyPageReset();
                                        continue;
                                    }
                                }
                            }
                        }
                        
                        }
                    
                        if (gameMode ==1)
                        {
                            passwordChange = TRUE;
                            serialSend('N');
                            keyIterrator = 0;
                            LCD_Set_Cursor(1,1);
                            LCD_Write_String("NEW PASSWORD    ");
                            LCD_resetPasswordEntry();
                            while (passwordChange == TRUE)
                            {
                                keypadOutput = keypadRead();
                                keyPressed = keypadOutput.keyPressed;
                                keyHeld = keypadOutput.keyHeld;

                                if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                                {
                                    serialSend('N');
                                    serialSend(keyPressed);
                                    userPswdEntry[keyIterrator] = keyPressed;
                                    keyIterrator++;

                                    LCD_Set_Cursor(2,9+keyIterrator);

                                    LCD_Write_Char(keyPressed);
                                }

                                    if (keyPressed == '#' || keyPressed == '*'&& keyHeld == NONE)
                                    {
                                        serialSend('N');
                                        serialSend('*');
                                        for (int i=0;i<=6;i++){password[i] = userPswdEntry[i];}
                                        passwordChange = FALSE;
                                        resetPasswordEntry();
                                        stanbyPageReset();
                                        break;
                                    }

                                    else{continue;}
                                            
                                        }}
                    }

                    else if(keyPressed == '*' && keyHeld == '*')
                    {
                        timerChange = TRUE;
                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("TIMER CHANGE");

                        int cursorPos = 0;
                        LCD_printInitialCountdown();
                        LCD_Set_Cursor(1,9);
                        LCD_Cursor_Blink_On();

                        while(timerChange == TRUE)
                        {
                            keypadOutput = keypadRead();
                            keyPressed = keypadOutput.keyPressed;
                            keyHeld = keypadOutput.keyHeld;

                            if(keyHeld == NONE && keyPressed == '#')
                            {
                                if(cursorPos==0){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==1){LCD_Cursor_Right();LCD_Cursor_Right();cursorPos++;cursorPos++;}
                                else if(cursorPos==3){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==4){LCD_Cursor_Right();LCD_Cursor_Right();cursorPos++;cursorPos++;}
                                else if(cursorPos==6){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==7){}
                            }

                            else if (keyHeld == NONE && keyPressed == '*')
                            {
                                if(cursorPos==0){}
                                else if(cursorPos==1){LCD_Cursor_Left();cursorPos--;}
                                else if(cursorPos==3){LCD_Cursor_Left();LCD_Cursor_Left();cursorPos--;cursorPos--;}
                                else if(cursorPos==4){LCD_Cursor_Left();cursorPos--;}
                                else if(cursorPos==6){LCD_Cursor_Left();LCD_Cursor_Left();cursorPos--;cursorPos--;}
                                else if(cursorPos==7){LCD_Cursor_Left();cursorPos--;}
                            }

                            else if (keyHeld == NONE)
                            {
                                strCountdownInitialTime[cursorPos] = keyPressed;
                                LCD_Write_Char(keyPressed);
                                cursorPos++;
                                if(cursorPos == 2 || cursorPos == 5){LCD_Cursor_Right();cursorPos++;}
                                if(cursorPos == 8){LCD_Cursor_Left();cursorPos--;}
                            }

                            else if (keyHeld == '*' && keyPressed == '*')
                            {
                                timerChange = FALSE;
                            }
                        } 
                        
                        countdown_initial_str2num();
                        countdown_now_reset_to_initial();
                        LCD_printNowCountdown();
                        LCD_resetPasswordEntry();
                        keyIterrator = 0;
                        
                    }


                    else if(keyPressed == '1' && keyHeld == '1')
                    {
                        keyBeepsMem = !keyBeepsMem;
                        keyBeeps = !keyBeeps;

                        LCD_Set_Cursor(2,1);
                        
                        if(keyBeeps == ON)
                        {
                            LCD_Write_String("Key Beep ON     ");
                        }
                        
                        else 
                        {
                            LCD_Write_String("Key Beep OFF    ");
                        }
                        
                        __delay_ms(1500);
                        stanbyPageReset();
                        continue;
                    }

                    else if(keyPressed == '2' && keyHeld == '2')
                    {
                        if(cursorBlinkMem == OFF)
                        {
                            cursorBlinkMem = ON;
                            LCD_Cursor_Blink_On();
                            LCD_Set_Cursor(2,1);
                            LCD_Write_String("Cursor ON       ");
                        }
                        
                        else if(cursorBlinkMem == ON)
                        {
                            cursorBlinkMem = OFF;
                            LCD_Cursor_Blink_Off();
                            LCD_Set_Cursor(2,1);
                            LCD_Write_String("Cursor OFF      ");
                        }

                        __delay_ms(1500);
                        stanbyPageReset();
                        continue;
                    }

                    else if(keyPressed == '3' && keyHeld == '3')
                    {
                        countdownBeeps = !countdownBeeps;
                        countdownBeepsMem = !countdownBeepsMem;
                        
                        LCD_Set_Cursor(2,1);
                        
                        if(countdownBeeps == ON)
                        {
                            LCD_Write_String("Timer Beep ON   ");
                        }
                        
                        else
                        {
                            LCD_Write_String("Timer Beep OFF  ");
                        }
                        
                        __delay_ms(1500);
                        stanbyPageReset();
                        continue;
                    }
                }
            }    



            /*=================== ARMED STATUS =======================*/

            else if(status == armed)
            {
                keyBeeps = OFF;

                 if(keyPressed != NONE && keyHeld == NONE)
                {
                    if(keyIterrator == 0 && keyBeeps == OFF)
                    {
                        armBeep();
                    }
                    if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                    {
                        serialSend('U');
                        serialSend(keyPressed);
                        
                        userPswdEntry[keyIterrator] = keyPressed;
                        keyIterrator++;

                        LCD_Set_Cursor(2,9+keyIterrator);
                    
                        LCD_Write_Char(keyPressed);
                    }

                    if (keyPressed == '#' || keyPressed == '*'&& keyHeld == NONE)
                    {   
                        serialSend('U');
                        serialSend('*');
                        isPasswordTrue = verifyPassword(userPswdEntry,password);
                        resetPasswordEntry();
                        
                        if(isPasswordTrue == TRUE)
                        {
                            
                            countdownStop();
                            serialSend('D');
                            serialPrint(strCountdownNowTime);
                            disarmBeep();

                            status = disarmed;
                            disarmedPageReset();
                            continue;
                        }
                        
                        else
                        {
                            armedPageReset();
                            continue;
                        }
                        
                    }
                }

                else if(keyHeld != NONE)
                {
                    if (gameMode == 1)
                    {
                        if(keyPressed == 'A' && keyHeld == 'A')
                        {
                            status = stanby;
                            countdown_now_reset_to_initial();
                            countdown_initial_str2num();
                            resetPasswordEntry();
                            stanbyPageReset();
                            break;
                        }
                    }
                    
                    if(keyPressed == '1' && keyHeld == '1')
                    {
                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("NOT AVAILABLE   ");
                        __delay_ms(1500);
                        armedPageReset();
                        continue;
                    }

                    else if(keyPressed == '2' && keyHeld == '2')
                    {
                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("NOT AVAILABLE   ");
                        __delay_ms(1500);
                        armedPageReset();
                        continue;
                    }

                    else if(keyPressed == '3' && keyHeld == '3')
                    {
                        countdownBeeps = !countdownBeeps;
                        countdownBeepsMem = !countdownBeepsMem;
                        
                        LCD_Set_Cursor(2,1);
                        
                        if(countdownBeeps == ON)
                        {
                            LCD_Write_String("Timer Beep ON   ");
                        }
                        
                        else
                        {
                            LCD_Write_String("Timer Beep OFF  ");
                        }
                        
                        __delay_ms(1500);
                        armedPageReset();
                        continue;
                    }
                }

            }


            /*=================== DISARMED CHECK =======================*/


            else if(status == disarmed)
            { 
                if(keyPressed != NONE && keyHeld == NONE)
                {
                    if(keyIterrator == 0 && keyBeeps == OFF)
                    {
                        armBeep();
                    }
                    if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                    {
                        userPswdEntry[keyIterrator] = keyPressed;
                        keyIterrator++;
                        LCD_Set_Cursor(2,9+keyIterrator);  
                        LCD_Write_Char(keyPressed);
                    }

                    if (keyPressed != '#' && keyPressed != '*' && keyIterrator<7)
                    {   
                        __delay_ms(250);  
                        isPasswordTrue = verifyPassword(userPswdEntry,password);
                        resetPasswordEntry();

                        if(isPasswordTrue == TRUE)
                        {
                            LCD_printAccepted();

                            armedPageReset();
                            countdownStart();
                            status = armed;
                            continue;
                        }

                        else if (isPasswordTrue == FALSE)
                        {
                            LCD_printDeclined();
                            disarmedPageReset();
                            continue;
                        }
                    }
                }


                else if(keyHeld != NONE)
                {
                       
                    if (gameMode == 1)
                    {
                        if(keyPressed == 'A' && keyHeld == 'A')
                        {
                            status = stanby;
                            countdown_now_reset_to_initial();
                            countdown_initial_str2num();
                            resetPasswordEntry();
                            stanbyPageReset();
                            break;
                        }
                    }
                    
                    if(keyPressed == '*' && keyHeld == '*')
                    {
                        timerChange = TRUE;
                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("TIMER CHANGE    ");

                        int cursorPos = 0;
                        LCD_printInitialCountdown();
                        LCD_Set_Cursor(1,9);
                        LCD_Cursor_Blink_On();

                        while(timerChange == TRUE)
                        {
                            keypadOutput = keypadRead();
                            keyPressed = keypadOutput.keyPressed;
                            keyHeld = keypadOutput.keyHeld;

                            if(keyHeld == NONE && keyPressed == '#')
                            {
                                if(cursorPos==0){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==1){LCD_Cursor_Right();LCD_Cursor_Right();cursorPos++;cursorPos++;}
                                else if(cursorPos==3){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==4){LCD_Cursor_Right();LCD_Cursor_Right();cursorPos++;cursorPos++;}
                                else if(cursorPos==6){LCD_Cursor_Right();cursorPos++;}
                                else if(cursorPos==7){}
                            }

                            else if (keyHeld == NONE && keyPressed == '*')
                            {
                                if(cursorPos==0){}
                                else if(cursorPos==1){LCD_Cursor_Left();cursorPos--;}
                                else if(cursorPos==3){LCD_Cursor_Left();LCD_Cursor_Left();cursorPos--;cursorPos--;}
                                else if(cursorPos==4){LCD_Cursor_Left();cursorPos--;}
                                else if(cursorPos==6){LCD_Cursor_Left();LCD_Cursor_Left();cursorPos--;cursorPos--;}
                                else if(cursorPos==7){LCD_Cursor_Left();cursorPos--;}
                            }

                            else if (keyHeld == NONE)
                            {
                                strCountdownInitialTime[cursorPos] = keyPressed;
                                LCD_Write_Char(keyPressed);
                                cursorPos++;
                                if(cursorPos == 2 || cursorPos == 5){LCD_Cursor_Right();cursorPos++;}
                                if(cursorPos == 8){LCD_Cursor_Left();cursorPos--;}
                            }
                            else if (keyHeld == '*' && keyPressed == '*')
                            {
                                timerChange = FALSE;
                            }

                        } 
                        countdown_initial_str2num();
                        LCD_printNowCountdown();
                        LCD_resetPasswordEntry();
                    }

                    else if(keyPressed == '0' && keyHeld == '0')
                    {
                        status = stanby;

                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("RESETTING       ");
                        LCD_Set_Cursor(2,10);
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);

                        countdown_now_reset_to_initial();
                        countdown_initial_str2num();
                        stanbyPageReset();
                        continue;
                    }

                    else if(keyPressed == '1' && keyHeld == '1')
                    {
                        keyBeeps = !keyBeeps;

                        LCD_Set_Cursor(2,1);

                        if(keyBeeps == ON)
                        {
                            LCD_Write_String("Key Beep ON     ");
                        }

                        else 
                        {
                            LCD_Write_String("Key Beep OFF    ");
                        }

                        __delay_ms(1500);
                        disarmedPageReset();
                        continue;
                    }

                    else if(keyPressed == '2' && keyHeld == '2')
                    {
                        if(cursorBlinkMem == OFF)
                        {
                            cursorBlinkMem = ON;
                            LCD_Cursor_Blink_On();
                            LCD_Set_Cursor(2,1);
                            LCD_Write_String("Cursor ON       ");
                        }

                        else if(cursorBlinkMem == ON)
                        {
                            cursorBlinkMem = OFF;
                            LCD_Cursor_Blink_Off();
                            LCD_Set_Cursor(2,1);
                            LCD_Write_String("Cursor OFF      ");
                        }

                        __delay_ms(1500);
                        disarmedPageReset();
                        continue;
                    }

                    else if(keyPressed == '3' && keyHeld == '3')
                    {
                        countdownBeepsMem = !countdownBeepsMem;
                        countdownBeeps = !countdownBeeps;

                        LCD_Set_Cursor(2,1);

                        if(countdownBeeps == ON)
                        {
                            LCD_Write_String("Timer Beep ON   ");
                        }

                        else
                        {
                            LCD_Write_String("Timer Beep OFF  ");
                        }

                        __delay_ms(1500);
                        disarmedPageReset();
                        continue;
                    }
                }
            }
        
        // ====================== STATUS EXPLODED ==============================

            else if(status == exploded)
            {
                serialSend('E');
                verifyInterfaceOptions();

                if(keyHeld != 0)
                {
                    if(keyPressed == '0' && keyHeld == '0')/*RESET THE TIMER*/
                    {
                        status = stanby;

                        LCD_Set_Cursor(2,1);
                        LCD_Write_String("RESETTING       ");
                        LCD_Set_Cursor(2,10);
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);
                        LCD_Write_Char('.');
                        __delay_ms(800);
                        countdown_now_reset_to_initial();
                        countdown_initial_str2num();
                        stanbyPageReset();
                        continue;
                    }
                }    
            }       
        }
    
        
        
        if(mode == alarmMode)
        {

        }
      
    }
    
}




