#define	COUNTDOWN_H
int numCountdownInitialTime[6] = {0,1,0,0,0,0};
volatile int numCountdownNowTime[6] = {0,1,0,0,0,0};

char strCountdownNowTime[8] = {'0','1',':','0','0',':','0','0'};
char strCountdownInitialTime[8] = {'0','1',':','0','0',':','0','0'};

void COUNTDOWN_Initialize(void);

void countdownStart(void);

void countdownSet(void);

void countdownDisplay(void);

void countdownStop(void);

void countdown_now_reset_to_initial(void);

void countdown_initial_str2num(void);

void countdown_now_num2str(void);

void countdown_drop_timer(void);
