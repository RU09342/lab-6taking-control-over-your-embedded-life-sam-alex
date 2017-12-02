/* Lab 6 Precision Control PWM Part 2
 * Sam Toanone Alex Revolus
 * This code takes an input from UART and outputs the value as a PWM.
 * Pins 3.4 and 3.3 are Rx and Tx. The output PWM is sent on PIN 1.2.
 * The default PWM value is 50% and after the first value is sent
 * it becomes the TA0CCR1 value.
 */

#include <msp430.h>


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
/*******************Enable Inputs***********************/
  P1DIR |= BIT2;
  P1SEL |= BIT2;

 /**********************Timer Settings******************/
 //Max value for PWM = 1000. Set value in CCR0. Counts up to CCR0 the resets.
 // 1 MHZ clock, Upmode, reset, set.
  TA0CCR0 = 1000;
  TA0CCR1=500;
  TA0CCTL1 = OUTMOD_7;
  TA0CTL = TASSEL_2 + MC_1;         // SMCLK, upmode, clear TA
/***********************Uart Settings**********************/
 // UART Inputs 3.3 and 3.4. 9600 Baud Rate.SMCLK for UCA0CTL1 1MHZ/9600=104. Set UCA0BRO to 104.
  //No modulation, enable interrupt
  P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                              // 9600 Baud rate
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt




  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  __no_operation();   // For debugger


}

/********************Uart Interrupt*********************/
//Only case needed is case 2. If the receiver interrupt flag is on transmit back the data sent(to make sure UART is working)
//Uart can only send up to 255. Only send data between 0-100. Multiply Received data by 10 to have CCR1 values between 0-1000.
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)

{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
    TA0CCR1=(UCA0RXBUF *10);
    UCA0TXBUF = UCA0RXBUF;                  //Transmit Data back over UART
    break;
  case 4:break;
  default: break;
  }
}
