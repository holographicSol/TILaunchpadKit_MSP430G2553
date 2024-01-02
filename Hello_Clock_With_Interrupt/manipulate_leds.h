#include <msp430.h>

void RGB_0_OFF()
{
    P2OUT &=~ (BIT8 + BIT1);        // clear P2.1  (RGB red off)
    P2OUT &=~ (BIT8 + BIT3);        // clear P2.3  (RGB green off)
    P2OUT &=~ (BIT8 + BIT5);        // clear P2.3  (RGB green off)
}

void RGB_0_RED()
{
    RGB_0_OFF();
    P2OUT ^= BIT1;                  // toggle P2.1
}

void RGB_0_YELLOW()
{
    RGB_0_OFF();
    P2OUT ^= BIT1;                  // toggle P2.1
    P2OUT ^= BIT3;                  // toggle P2.3
}

void RGB_0_GREEN()
{
    RGB_0_OFF();
    P2OUT ^= BIT3;                  // toggle P2.3
}


void RGB_0_BLUE()
{
    RGB_0_OFF();
    P2OUT ^= BIT5;                  // toggle P2.5
}

void RGB_0_LIGHT_BLUE()
{
    RGB_0_OFF();
    P2OUT ^= BIT3;                  // toggle P2.3
    P2OUT ^= BIT5;                  // toggle P2.5
}

void RGB_0_PURPLE()
{
    RGB_0_OFF();
    P2OUT ^= BIT1;                  // toggle P2.1
    P2OUT ^= BIT5;                  // toggle P2.5
}

void RGB_0_WHITE()
{
    RGB_0_OFF();
    P2OUT ^= BIT1;                  // toggle P2.1
    P2OUT ^= BIT3;                  // toggle P2.3
    P2OUT ^= BIT5;                  // toggle P2.5
}
