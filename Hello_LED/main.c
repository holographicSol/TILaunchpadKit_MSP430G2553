/*
 * Hello LED!
 */

#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0;                  // configure P1.0 as output (sets p1 direction register bit)

    volatile unsigned int i;        // volatile to prevent optimization
    while(1)
    {
        P1OUT ^= BIT0;              // toggle P1.6 (flips bit)
        for(i=10000; i>0; i--);     // delay
    }
}
