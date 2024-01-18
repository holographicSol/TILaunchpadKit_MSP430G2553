#include <msp430.h>
#include "TM1637.h"


#define _BV(val) 1<<val // Bit shift macro

// Change TM1637 configuration within the TM1637.h file.

/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* setup */
    TM1637_init(1/*enable*/, 7/*brightness*/);

    TM1637_display_colon(0);

    TM1637_display_digit(1,7);
    TM1637_display_digit(2,7);
    TM1637_display_digit(3,6);
    TM1637_display_digit(4,1);
}
