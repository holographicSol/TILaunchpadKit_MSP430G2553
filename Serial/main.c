#include <msp430.h>
#include <uart.h>
//optional: includes below if using charcon.h for microcode type conversions (can use up to around 3KB)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    UARTConfigure();
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    __bis_SR_register(GIE);

    printtxd("\n[MSP] Serial communication.\n");

    while(1)
    {}
}

