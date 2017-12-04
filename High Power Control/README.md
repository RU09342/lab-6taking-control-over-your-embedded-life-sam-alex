High Power Control


# Purpose:
The purpose of this lab was to use the msp430g2553 to control a higher voltage using a switch. The output signal from the MSP will control whether or not the 12V from the power supply is sent to the 10 ohm 1 watt power resistor.  A PWM signal is sent to the mosfet and the relay to control the switching. 

# Mosfet: 
The mosfet has 3 pins. These pins are going to be connected to 12V, the msp430g2553 gpio pin and a 100 ohm power resistor. When the voltage is ‘high’ (3.3V) at the gate of the mosfet, then the 12 V from the power supply will be able to pass to the power resistor. When the voltage is low “0V’ then the voltage will not be able to go to the power resistor.  The mosfet needs a small current to activate the switching.

# Relay: 
The relay uses a contact to switch between an off and on state. Because of this, bouncing is a problem and switching is not as fast as the relay.  When a voltage is applied to the pin of the relay, it will switch to an on state and allow the 12V to pass to the mosfet.
