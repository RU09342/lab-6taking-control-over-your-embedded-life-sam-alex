/* Sam Toanone and Alexander Revolus
 * This code uses a timer to control the switching of mosfets
 * and a relay. The TA1CCR1 value can change to switch faster and slower.
 *
 */
#include <msp430.h>
void main(void)
{
    {
        WDTCTL = WDTPW + WDTHOLD;        // Stop WDT
        P1OUT &= ~BIT1;
        P1DIR |= BIT1;

        TA1CTL=TASSEL_2 + MC_1;
         TA1CCR0=1024;
          TA1CCTL1 |= OUTMOD_3;
           TA1CCR1 = 500;


        __enable_interrupt();            // Enable interrupts.
    }
}
