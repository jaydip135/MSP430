#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT0;              // Set P1.0 to output direction
	PM5CTL0 &= ~LOCKLPM5;       // Clear LOCKLPM5 to enable GPIO pins
	
	while(1)
    {
        P1OUT ^= BIT0;          // Toggle P1.0 (LED)
        __delay_cycles(100000);  // Delay
    }
}
