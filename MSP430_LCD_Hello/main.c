#include <msp430.h>
#include "lcd.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // Unlock ports from power manager
    lcd_init();
    while(1)
    {
        displayMessage("JAYDIP");
        __delay_cycles(1000000);
        lcd_clear();
        __delay_cycles(1000000);
    }
}
