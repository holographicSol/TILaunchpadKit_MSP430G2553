#include <msp430.h>
#include <stdio.h>
#include <string.h>

void UARTConfigure(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    /* Load calibrated DCO values to set CPU clock to 1MHz */
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz

    UARTConfigure();
    volatile unsigned int n;        // volatile to prevent optimisation
    while(1){

        UARTSendArray("Hello Universe!", 16);
        UARTSendArray("\r\n", 2);

        for(n=100000; n>0; n--);     // delay
    }
}
