#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <uart.h>

volatile unsigned  IDXBUFFADC10MEM  = 0;    // iterating ADC10MEM buffer index
int                VOLTADC10MEM     = 0;    // hard voltage read from ADC10MEM
float              SVOLTADC10MEM    = 3.3;  // soft voltage for equations with ADC10MEM voltage
char               STRADC10MEM[5];          // string type of ADC10MEM
char               STRVOLTADC10MEM[5];      // string type of ADC10MEM converted to voltage
char               STRRMDRADC10MEM[5];      // string type of ADC10MEM remainder from voltage conversion

void VOLTAGESTABALIZATIONADC10MEM(){P1REN |= BIT4; P1OUT &= ~BIT4;}  // optionally enable resistor pull-down (reduce flicker when no sensor)

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                                           // Stop WDT
    UARTConfigure();
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE + MSC;  // Set ADC module
    ADC10CTL1 = CONSEQ_0 + INCH_4 + ADC10DIV_0;                         // Set ADC module
    ADC10DTC0 = ADC10CT;                                                // Set ADC module
    ADC10AE0 = BIT4;                                                    // P1.4 ADC option select
    ADC10CTL0 |= ENC + ADC10SC;                                         // Start Sampling and conversion
    __bis_SR_register(GIE);                                             // Enable Interrupts
    while(1){
        ADC10CTL0 |= ADC10SC;
    }
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_IRS(void)
{
    while (ADC10CTL1 & ADC10BUSY);
    if(IDXBUFFADC10MEM < 16){
        IDXBUFFADC10MEM++;
    }
    else{
        VOLTADC10MEM = ((ADC10MEM*SVOLTADC10MEM)/1023);
        int VINTADC10MEM = floor(VOLTADC10MEM);
        int RMDRADCMEM10 = floor((VOLTADC10MEM-VINTADC10MEM)*1000);
        ltoa(ADC10MEM, STRADC10MEM);
        ltoa(VINTADC10MEM,STRVOLTADC10MEM);
        ltoa(RMDRADCMEM10,STRRMDRADC10MEM);
        printtxd("\n[MSP ADC10MEM] "); printtxd(STRADC10MEM); printtxd("\n");
        printtxd("[MSP ADC10MEM V] "); printtxd(STRVOLTADC10MEM); printtxd("."); printtxd(STRRMDRADC10MEM); printtxd("\n");
        VOLTADC10MEM = 0;
        memset(STRVOLTADC10MEM, 0, sizeof STRVOLTADC10MEM);
        memset(STRADC10MEM, 0, sizeof STRADC10MEM);
        memset(STRRMDRADC10MEM, 0, sizeof STRRMDRADC10MEM);
        IDXBUFFADC10MEM = 0;
    }
    ADC10CTL0 &= ~(ADC10IFG);
}
