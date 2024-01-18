#include <msp430.h>
#include <uart.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // stop WDT timer
    UARTConfigure();           // setup serial communication
    P1DIR |= BIT0;             // configure P1.0 as output (Green LED)

    __bis_SR_register(GIE);    //  Enter   LPM0,   interrupts  enabled

    volatile unsigned int i;   // volatile to prevent optimisation
    printtxd("[MSP] Serial communication\n");
    while(1)
    {
        P1OUT ^= BIT0;
        __delay_cycles(1);
    }
}
