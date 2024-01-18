#include <msp430.h>
#include "TM1637.h"

// Change TM1637 configuration within the TM1637.h file.

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

void setup(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TM1637_init(1/*enable*/, 7/*brightness*/);
    TM1637_set_brightness(7);
    TM1637_display_colon(0);
}

int main(void)
{
    setup();
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
            _delay_us(5000);
            item = item + 1;
        }
        run_circle();
        int item2 = 0;
        while(item2 < 19)
        {
            TM1637_display_segments(4, _alpha2segments[item2]);
            TM1637_display_segments(3, _alpha2segments[item2]);
            TM1637_display_segments(2, _alpha2segments[item2]);
            TM1637_display_segments(1, _alpha2segments[item2]);
            _delay_us(5000);
            item2 = item2 + 1;
        }
        run_circle();
        int item3 = 0;
        while(item3 < 8)
        {
            TM1637_display_segments(4, _truesegments[item3]);
            TM1637_display_segments(3, _truesegments[item3]);
            TM1637_display_segments(2, _truesegments[item3]);
            TM1637_display_segments(1, _truesegments[item3]);
            _delay_us(5000);
            item3 = item3 + 1;
        }
        run_circle();
        int item4 = 0;
        while(item4 < 2)
        {
            TM1637_display_segments(4, _shape2segments[item4]);
            TM1637_display_segments(3, _shape2segments[item4]);
            TM1637_display_segments(2, _shape2segments[item4]);
            TM1637_display_segments(1, _shape2segments[item4]);
            _delay_us(5000);
            item4 = item4 + 1;
        }
    }
}
