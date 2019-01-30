#define	LCD_H

//FOR LCD
#define LCD_RS LATCbits.LATC6
#define LCD_EN LATCbits.LATC7

#define LCD_D0 LATDbits.LATD0
#define LCD_D1 LATDbits.LATD1
#define LCD_D2 LATDbits.LATD2
#define LCD_D3 LATDbits.LATD3
#define LCD_D4 LATDbits.LATD4
#define LCD_D5 LATDbits.LATD5
#define LCD_D6 LATDbits.LATD6
#define LCD_D7 LATDbits.LATD7

#define LCD_BUS  LATD

    //BASIC LCD FUNCTIONS
    void LCD_Initialize(void);

    void LCD_Cmd(char b);
    
    void LCD_Clear(void);

    void LCD_Set_Cursor(int c, int d);

    void LCD_Write_Char(char e);

    void LCD_Write_String(char *f);

    void LCD_Cursor_Blink_On(void);
    void LCD_Cursor_Blink_Off(void);
    void LCD_Cursor_Right(void);
    void LCD_Cursor_Left(void);
    
    // COUNTDOWN RELATED FUNCTIONS
    void LCD_printAccepted(void);
    
    void LCD_printDeclined(void);
    
    void LCD_printNowCountdown(void);
    
    void LCD_printInitialCountdown(void);
    
    void LCD_resetPasswordEntry(void);
    