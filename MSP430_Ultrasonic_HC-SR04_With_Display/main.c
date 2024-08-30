#include <msp430.h>
#include "lcd.h"

#define TRIG_PIN BIT6
#define ECHO_PIN BIT7

volatile unsigned long pulse_duration = 0;
unsigned int distance =0;

void measureDistance(void)
{
    P1OUT |= TRIG_PIN;
    __delay_cycles(20);
    P1OUT &= ~TRIG_PIN;
    while (!(P1IN & ECHO_PIN));
    pulse_duration = 0;
    while (P1IN & ECHO_PIN)
    {
        pulse_duration++;
        __delay_cycles(5);
    }
    distance = pulse_duration / 3;
    displayNum(distance,2);
    displayMessage("CM");
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= TRIG_PIN;
    P1OUT &= ~TRIG_PIN;
    P1DIR &= ~ECHO_PIN;
    PM5CTL0 &= ~LOCKLPM5;
    lcd_init();

    while (1)
    {
        measureDistance();
        __delay_cycles(500000);
        lcd_clear();

    }
}
