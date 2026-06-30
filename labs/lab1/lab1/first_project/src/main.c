//These numbers are the bit patterns for lighting up the respective LED    
#define LED1	1
#define LED2	2
#define LED3	4
#define LED4	8              

extern void LEDCycle_Init(void);
extern void LEDCycle(unsigned int value);
extern void delay(void);

/*
	main(void)
	
	This function lights each LED on the QVGA Base Board
	one after the other in a cyclical fashion.
	
	The function LEDCycle lights up a single LED at a time, and accepts
	one argument, one of the constants defined above.
	
	The delay function halts the processor's execution for a set amount of time.
	
	These functions are called in an endless loop, implemented using while(1).
*/


int main (void) {

	// Step Over (F10) these functions.
	LEDCycle_Init();
	
	// Loop forever
	while (1) {
			LEDCycle(LED1);
			delay();
			LEDCycle(LED2);
			delay();
			LEDCycle(LED3);
			delay();
			LEDCycle(LED4);
			delay();
	}
}
