#define	MCC_H

    /*GENERAL DEFINES*/
    #define _XTAL_FREQ 8000000 

    /*GLOBAL USER VARIABLES*/
    
    char password[8] = {'1','1','1','1','1','1','1'};//has to have a 0 at end for strcomp
 
    
    /*GLOBAL MAIN INTERFACE VARIABLES*/
    char mode = 0;
    char countdownMode = 0;
    char alarmMode = 1;
    
    int keyIterrator = 0;
    int isPasswordTrue = 0;
    
    int timerChange = 0;
    int passwordChange = 0;
    
    int status = 0;
    int stanby = 0;
    int armed = 1;
    int disarmed = 2;
    int exploded = 3;
    
    /*OPTIONS FLAGS*/
    int keyBeeps = 1;   int keyBeepsMem = 1;
    int countdownBeeps = 1; int countdownBeepsMem = 1;
    
    int cursorBlinkMem = 1;

    /*KEYWORDS*/
    int ON = 1;
    int OFF = 0;
    int TRUE = 1;
    int FALSE = 0;
    int NONE = 0;
    

void SYSTEM_Initialize(void);

void INTERRUPT_Initialize(void);

void TIMER0_Initialize(void);

void OSCILLATOR_Initialize(void);

void PMD_Initialize(void);

void CONFIGURATION_Initialize(void);

void PIN_MANAGER_Initialize (void);

void ADC_MANAGER_Initialize(void);
