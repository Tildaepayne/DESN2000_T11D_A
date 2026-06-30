#include <LPC24xx.H>          

#define I2C_AA      0x00000004
#define I2C_SI      0x00000008
#define I2C_STO     0x00000010
#define I2C_STA     0x00000020
#define I2C_I2EN    0x00000040

/* Import external IRQ handlers from IRQ.c file                               */
extern __irq void T0_IRQHandler  (void);
extern __irq void ADC_IRQHandler (void);

/* Import external variables from IRQ.c file                                  */
extern unsigned int leds;

void LED_Init (void);

/******************************************************************************/
/* Press F11 to step through all of these instructions one at a time          */
/* in order to return to the main loop. Or press Ctrl-F11 to                  */
/* step out of the function entirely.                                         */
/******************************************************************************/

/* Function that initializes LED Cycle                                        */
void LEDCycle_Init() {
  LED_Init();                                  /* LED Initialization          */

  /* Enable and setup timer interrupt, start timer                            */
  PCONP        |= (1 << 1);                    /* Enable power to TIM block   */
  T0MR0         = 18000-1;                     /* 1msec = 18000-1 at 18 MHz   */
  T0MCR         = 3;                           /* Interrupt and Reset on MR0  */
  T0TCR         = 1;                           /* Timer0 Enable               */
  VICVectAddr4  = (unsigned long)T0_IRQHandler;/* Set Interrupt Vector        */
  VICVectCntl4  = 15;                          /* use it for Timer0 Interrupt */
  VICIntEnable  = (1  << 4);                   /* Enable Timer0 Interrupt     */

  /* Power enable, Setup pin, enable and setup AD converter interrupt         */
  PCONP        |= (1 << 12);                   /* Enable power to AD block    */
  PINSEL1      &=~(0x03<<18);                  /* AD0.2 pin function select   */
  PINSEL1      |= (0x01<<18);
  AD0INTEN      = (1 <<  2);                   /* CH2 enable interrupt        */
  AD0CR         = 0x00200304;                  /* Power up, PCLK/4, sel AD0.2 */
  VICVectAddr18 = (unsigned long)ADC_IRQHandler;/* Set Interrupt Vector       */
  VICVectCntl18 = 14;                          /* use it for ADC Interrupt    */
  VICIntEnable  = (1  << 18);                  /* Enable ADC Interrupt        */
}

/* Function that initializes LEDs                                             */
void LED_Init (void) {
  PCONP    |=  (1 << 7);                /* Enable clock for I2C0              */

  /* Initialize pins for SDA (P0.27) and SCL (P0.28) functions                */
  PINSEL1  &= ~0x03C00000;
  PINSEL1  |=  0x01400000;

  /* Clear I2C state machine                                                  */
  I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_I2EN;

  /* Setup I2C clock speed                                                    */
  I20SCLL   =  0x80;
  I20SCLH   =  0x80;

  I20CONSET =  I2C_I2EN;
  I20CONSET =  I2C_STO;
}

/* Function that switches on a specific LED                                   */
void LEDCycle(unsigned int value) {
	
	// Press F11 to step through all of these instructions one at a time
	// in order to return to the main loop. Or press Ctrl-F11 to
	// step out of the function entirely.
	
	unsigned int val = value;

  I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_STO;
  I20CONSET =  I2C_STA;
  while (!(I20CONSET & I2C_SI));              
  I20DAT    =  0xC0;
  I20CONCLR =  I2C_SI | I2C_STA;
  while (!(I20CONSET & I2C_SI));    
  I20DAT    =  0x18;
  I20CONCLR =  I2C_SI;
  while (!(I20CONSET & I2C_SI));       
  I20DAT    =  (0x01 * ((val >> 0) & 1)) |
               (0x04 * ((val >> 1) & 1)) |
               (0x10 * ((val >> 2) & 1)) |
               (0x40 * ((val >> 3) & 1)) ;
  I20CONCLR =  I2C_SI;
  while (!(I20CONSET & I2C_SI));      
  I20CONSET =  I2C_STO;
  I20CONCLR =  I2C_SI;
  while (I20CONSET & I2C_STO);    
	
}

/* Function that creates a time delay by counted clock cycles                 */
void delay (void) {                     
  unsigned int count;
  unsigned int max = 0x1FFFFF;

	// Waste time counting up to max
	// Click Step-Out to skip over this. (Ctrl-F11)
  for (count = 0; count < max; count++);

}

