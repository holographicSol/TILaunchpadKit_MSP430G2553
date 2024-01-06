#include <msp430.h>
#include "TM1637.h"


#define _BV(val) 1<<val // Bit shift macro

// Change TM1637 configuration within the TM1637.h file.

/**
 * main.c
 */

void run_circle()
{
    TM1637_display_segments(4, SEG_TOP);
    TM1637_display_segments(1, SEG_TOP);
    TM1637_display_segments(1, SEG_TOP);
    TM1637_display_segments(2, SEG_TOP);
    TM1637_display_segments(2, SEG_TOP);
    TM1637_display_segments(3, SEG_TOP);
    TM1637_display_segments(3, SEG_TOP + SEG_TOP_RIGHT);
    TM1637_display_segments(3, SEG_TOP_RIGHT + SEG_BOTTOM_RIGHT);
    TM1637_display_segments(3, SEG_BOTTOM_RIGHT + SEG_BOTTOM);
    TM1637_display_segments(3, SEG_BOTTOM);
    TM1637_display_segments(2, SEG_BOTTOM);
    TM1637_display_segments(2, SEG_BOTTOM);
    TM1637_display_segments(1, SEG_BOTTOM);
    TM1637_display_segments(1, SEG_BOTTOM);
    TM1637_display_segments(4, SEG_BOTTOM);
    TM1637_display_segments(4, SEG_BOTTOM + SEG_BOTTOM_LEFT);
    TM1637_display_segments(4, SEG_BOTTOM_LEFT + SEG_TOP_LEFT);
    TM1637_display_segments(4, SEG_TOP_LEFT + SEG_TOP);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* setup */
    TM1637_init(1/*enable*/, 7/*brightness*/);

    TM1637_set_brightness(7);
    TM1637_display_colon(0);

    while(1)
    {
        run_circle();

        int item = 0;
        while(item < 10)
        {
            TM1637_display_segments(4, _digit2segments[item]);
            TM1637_display_segments(3, _digit2segments[item]);
            TM1637_display_segments(2, _digit2segments[item]);
            TM1637_display_segments(1, _digit2segments[item]);
            _delay_us(1000);
            item = item + 1;
        }

        int item2 = 0;
        while(item2 < 36)
        {
            TM1637_display_segments(4, _alpha2segments[item2]);
            TM1637_display_segments(3, _alpha2segments[item2]);
            TM1637_display_segments(2, _alpha2segments[item2]);
            TM1637_display_segments(1, _alpha2segments[item2]);
            _delay_us(1000);
            item2 = item2 + 1;
        }
    }

}
