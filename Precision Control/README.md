Precision Control
Purpose:
The purpose of this lab was to 1) Receive a PWM duty cycle over UART and have the MSP430FR55259 output that same duty cycle, 2) Create an 8-bit Digital to Analog converter.
PWM Part 2:
The goal of PWM part 2 was to take a PWM value over UART and output that signal on a GPIO pin for the msp430fr5529. To do this, Uart must be configured correctly. To set up UART and the inputs and outputs, the following code was used. 
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


The output pin on the GPIO is Pin 1.2. The CCR0 value is 1000 and CCR1 initial value is 500.  The UART will transmit at a baud rate of 9600.  The divider for the clock must be 104 to achieve the baud Rate of 9600 with using smclk at 1MHZ.

The next code will be the UART interrupt. This code is shown below.
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

In this code, when the value sent over UART to the processor is between 0-100. This value will be multiplied by 10 to give the value to TA0CCR1. For example, if the value sent over is 100. The value of TA0CCR1 will be 100. This will be 100% duty cycle.

R2R DAC
The goal of this lab was to create an 8-bit analog to digital converter. The architecture used was an R2R ladder. The output of the R2R ladder was shown on the Oscilloscope. The F5529 was picked because of the amount of free GPIO pins available. With pins 6.0-6.6 available the for loop was easy to implement.


d by 10 to give the value to TA0CCR1. For example, if the value sent over is 100. The value of TA0CCR1 will be 100. This will be 100% duty cycle.

R2R DAC
The goal of this lab was to create an 8-bit analog to digital converter. The architecture used was an R2R ladder. The output of the R2R ladder was shown on the Oscilloscope. The F5529 was picked because of the amount of free GPIO pins available. With pins 6.0-6.6 available the for loop was easy to implement.

