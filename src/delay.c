/* Reused Lab 6 blocking delay for LCD and SDRAM initialization only. */

void mdelay (unsigned int ms) {                     
  unsigned int i, j;
  unsigned int max = 0xFFF;
  
  for (i = 0; i < ms; i++) {
		/* Delay approximately 1 ms. */
		for (j = 0; j < max; j++);
	}
}
