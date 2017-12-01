# Purpose:
The purpose of this lab was to use open loop control to control the temperature of a voltage regulator. With 15-20V being the input to the voltage regulator and the output being 5v, the voltage regulator produces heat. To cool the regulator, a fan will be used.  The temperature that the voltage regulator needs to stay between is 30C and 40C. The C code will convert the voltage to a temperature in Celsius and output this temperature over Uart.

# Code:
The code is similar to past labs used ADC and Uart. The main loop that transmits and sends data is shown below.

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
In this while loop,  the temperature is sent over Uart every 1 second from the code:
__delay_cycles(1000000);
The ADC value is converted into a temperature from the code:
Temperature = (int)((ADC_value * 0.0032)/.01);  // Convert the ADC value to Degrees Celsius


The value is sent over Uart from the code:
UCA0TXBUF = Temperature;  //Transmit the data


# Control:
Since this lab is an open loop system, the output data will not be used for an input. The input to this system will be a PWM value that will be used to control the fan. The temperature of the voltage regulator will be dependent on the PWM value. For a lower PWM, the voltage regulator will become hotter. For a higher value PWM, the voltage regulator will be cooler. To control the PWM, the following code is used:


TA1CCR0 = 1000; //The value that the timer A1 will count up to.
           TA1CCTL1 = OUTMOD_7;
           TA1CCR1 = 0; //Start at 0% duty cycle. Value will change depending on the PWM needed for the fan
           TA1CTL = TASSEL_2 + MC_1; //SMCLK and up mode


When TA1CCR1 is 1000, the fan will be at 100% duty cycle and 100% PWM. When the value is 0, there is no duty cycle and no PWM. 
