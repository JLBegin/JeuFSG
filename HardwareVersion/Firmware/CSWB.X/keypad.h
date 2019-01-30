#define KEYPAD_H4x4

#define KP1_R1  LATBbits.LATB0
#define KP1_R2  LATBbits.LATB1
#define KP1_R3  LATBbits.LATB2
#define KP1_R4  LATBbits.LATB3


typedef struct keypadStruct{char keyPressed; char keyHeld;}keypadStruct;

void KEYPAD_Initialize(void);

keypadStruct keypadRead(void);

int keypadScan(void);

