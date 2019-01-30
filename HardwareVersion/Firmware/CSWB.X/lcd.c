#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include "mcc.h"
#include "beeper.h"
#include "lcd.h"
#include "countdown.h"

    /*COMMAND FUNCTIONS*/
    void LCD_Initialize()
    { 
        TRISD = 0x00;
        TRISC = 0x00;
        LCD_RS = 0;

        //Makes functions 8 bits
         __delay_ms(40); 
       
        LCD_Cmd(0b00110000);    
        __delay_ms(20);
        LCD_Cmd(0b00110000);    
        __delay_ms(2);
        LCD_Cmd(0b00110000);    
        __delay_ms(40);
      
        //Setup parameters
        LCD_Cmd(0b00111000);
        
        LCD_Cmd(0b00001000);
         
        LCD_Cmd(0b00000001);
        
        LCD_Cmd(0b00000110);
        
        LCD_Cmd(0b00001110);
        
        LCD_Cmd(0b00000010);
        
        LCD_Clear();
        
    }
    
    void LCD_Cmd(char b)
    {
        LCD_RS = 0;             // => RS = 0
        LCD_BUS = b;
        LCD_EN  = 1;             // => E = 1
        __delay_us(50);
        LCD_EN  = 0;             // => E = 0
    }
    
    void LCD_Clear()
    {
         LCD_RS = 0;             // => RS = 0
        LCD_BUS &= 0x01;
        LCD_EN  = 1;             // => E = 1
        __delay_ms(4);
        LCD_EN  = 0; 
        
    }

    void LCD_Set_Cursor(int c, int d)
    {
        char temp;
        if(c == 1)
        {
            temp = 0x80 + d - 1;
            LCD_Cmd(temp);
        }
        else if(c == 2)
        {
            temp = 0xC0 + d - 1;
            LCD_Cmd(temp);
        }
    }

    void LCD_Write_Char(char e)
    {
        LCD_RS = 1;
        LCD_BUS = 0;
        LCD_BUS |= e;
        LCD_EN = 1;
        __delay_us(40);
        LCD_EN = 0;
        LCD_BUS = 0;
        //Data transfer
    }

    void LCD_Write_String(char *f)
    {
        int i;
        for(i=0;f[i]!='\0';i++)
           LCD_Write_Char(f[i]);
    }

    void LCD_Cursor_Blink_On()
    {
        LCD_Cmd(0x0F);
    }
    
    void LCD_Cursor_Blink_Off()
    {
        LCD_Cmd(0x0C);
    }
    
    void LCD_Cursor_Right()
    {
        LCD_Cmd(0x14);
    }
    
    void LCD_Cursor_Left()
    {
        LCD_Cmd(0x10);
    }
    
    /*INTERFACE FUNCTIONS*/
    void LCD_printAccepted()
    {
        LCD_Cursor_Blink_Off();
        LCD_Set_Cursor(2,1);
        LCD_Write_String("                ");

        for (int i=0;i<=5;i++)
        {
            LCD_Set_Cursor(2,5);
            LCD_Write_String("ACCEPTED");
            longLowBeep();
            __delay_ms(150);
            LCD_Set_Cursor(2,5);
            LCD_Write_String("        ");
            __delay_ms(150);
        } 
    }
    
    void LCD_printDeclined()
    {
        LCD_Cursor_Blink_Off();
        LCD_Set_Cursor(2,1);
        LCD_Write_String("                ");

        for (int i=0;i<=5;i++)
        {
            LCD_Set_Cursor(2,5);
            LCD_Write_String("DECLINED");
            longLowBeep();
            __delay_ms(150);
            LCD_Set_Cursor(2,5);
            LCD_Write_String("        ");
            __delay_ms(150);
        } 
    }
     
    void LCD_printNowCountdown()
    {
        for(int i=0;i<=7;i++)
        {
            LCD_Set_Cursor(1,9+i);
            LCD_Write_Char(strCountdownNowTime[i]);   
        }
    }
    
    void LCD_printInitialCountdown()
    {
        for(int i=0;i<=7;i++)
        {
            LCD_Set_Cursor(1,9+i);
            LCD_Write_Char(strCountdownInitialTime[i]);
        }
        
    }

    void LCD_resetPasswordEntry()
    {
        LCD_Set_Cursor(2,1);
        LCD_Write_String("PASSWORD:       ");
        LCD_Set_Cursor(2,10);
       // if(cursorBlinkMem == ON){LCD_Cursor_Blink_On();}
        //else {LCD_Cursor_Blink_Off();}
        
    }