#include <msp430.h>


/**
 * blink.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR |= BIT0;                  // configure P1.0 as output G
    P1DIR |= BIT6;                  // configure P1.6 as output R

    P2DIR |= BIT1;                  // configure P2.1 as output R
    P2DIR |= BIT3;                  // configure P2.3 as output G
    P2DIR |= BIT5;                  // configure P2.5 as output B

    volatile unsigned int i;        // volatile to prevent optimization
    while(1)
    {
        P1OUT ^= BIT0;              // toggle P1.6
        P1OUT ^= BIT6;              // toggle P1.6

        P2OUT ^= BIT1;              // toggle P2.1
        P2OUT ^= BIT3;              // toggle P2.3
        P2OUT ^= BIT5;              // toggle P2.5

        for(i=10000; i>0; i--);     // delay
    }
}
