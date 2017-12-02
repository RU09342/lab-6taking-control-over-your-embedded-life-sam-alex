/* Lab 6 Open Loop Systems for the MSP430G2553
 * Sam Toanone & Alexander Revolus
 * This code converts a voltage applied to pin 1.3 into an binary value. This value is the converted into
 * a temperature. The temperature  value will be output through Uart
 * every 1 second. The Uart pins are 1.1 for RXD and 1.2 for TXD. The baudrate for the UART is 9600. The reference voltage for the
 * ADC pin will be Vcc(3.3V) and Gnd(0V). This code will be used for all three circuits.
 */

#include <msp430.h> //Include msp430 libraries


unsigned int ADC_value=0; // variable assigned to store the ADC value



void ConfigureAdc(void);  //Function call to set ADC settings

void main(void)    //Main function
{
    {
        WDTCTL = WDTPW + WDTHOLD;        // Stop WDT

        /***********CONFIGURE I/0 and Timers******************/
        P2DIR |= BIT1; //Pin 2.1 is set to the output direction
           P2SEL |= BIT1; //Pin 2.1 is selected for PWM output for the fan
           TA1CCR0 = 1000; //The value that the timer A1 will count up to.
           TA1CCTL1 = OUTMOD_7;
           TA1CCR1 = 0; //Start at 0% duty cycle. Value will change depending on the PWM needed for the fan
           TA1CTL = TASSEL_2 + MC_1; //SMCLK and up mode

/*********************Configure Uart Communications**************************/
 // 1MHZ/104=9,615 Baud Rate


        if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
        {
            while(1);                               // do not load, trap CPU!!
        }
        DCOCTL = 0;                               // Select lowest DCOx and MODx settings
        BCSCTL1 = CALBC1_1MHZ;                    // Set range
        DCOCTL = CALDCO_1MHZ;
        P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
        P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
        UCA0CTL1 |= UCSSEL_2;                     // SMCLK
        UCA0BR0 = 104;                            // 1MHz 9600
        UCA0BR1 = 0;                              // 1MHz 9600
        UCA0MCTL = UCBRS0;                        // No Modulation
        UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
        IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

        BCSCTL2 &= ~(DIVS_3);            // SMCLK = DCO = 1MHz
 /*********** Configure ADC control******************/
        P1SEL |= BIT3;                    // ADC input pin P1.3
        ConfigureAdc();                    // ADC set-up function call
        __enable_interrupt();            // Enable interrupts.

        unsigned int Temperature; //Variable to store degrees celsius from conversion


 /********************** Convert and send data**********************************/
        while(1)
        {
            __delay_cycles(1000000);                // Wait 1 second before sending data over uart
            ADC10CTL0 |= ENC + ADC10SC;            // start sampling and data conversion
            __bis_SR_register(CPUOFF + GIE);    //Low power mode with interrupt enabled
            ADC_value = ADC10MEM;    // Store ADC value into the variable ADC_value
            Temperature = (int)((ADC_value * 0.0032)/.01);  // Convert the ADC value to Degrees Celsius
            UCA0TXBUF = Temperature;  //Transmit the data

        }

    }
}
/*******************************End of Main******************************************/

/********************************ADC10 Interrupt*************************************/
#pragma vector=ADC10_VECTOR     //ADC interrupot vector
__interrupt void ADC10_ISR (void)
{

    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}

/**************************** Function to set ADC values*******************************/
void ConfigureAdc(void)
{

    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         //channel A3 , ADC10 divide by 3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    ADC10AE0 |= BIT3;                         //ADC input enabled on 1.3
