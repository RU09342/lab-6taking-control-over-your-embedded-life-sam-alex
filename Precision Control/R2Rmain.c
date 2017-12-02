#include <msp430.h> 


/**
 * main.c
 */

int i = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P6DIR |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6; // initalizing inputs
	P2DIR |= BIT0;


	    while(1)
	    {


	    for (i = 0; i < 128; i++)    // increment i when value is less than 128
	    {
	      P6OUT = i;                 // pin 6 output is equal to i
	    }
	    P2OUT ^= BIT0;

	    }
	return 0;
}
