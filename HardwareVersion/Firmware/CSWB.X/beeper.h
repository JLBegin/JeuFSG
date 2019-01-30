#define	BEEPER_H
//FOR BEEPER AND LED
#define LED1      LATAbits.LATA0
#define BEEPER1     PWM5CONbits.EN
 

void BEEPER_Initialize(void);

void interrupt ISR(void);

void quickBeep(void);

void quickBlink(void);

void normalBeep(void);

void normalBlink(void);

void longBeep(void);

void veryLowBeep(void);

void longLowBeep(void);

void endBeep(void);

void armBeep(void);

void disarmBeep(void);
