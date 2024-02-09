/*
 * Joystick Lib Example.
 * Populate x0(left), x1(right), y0(up), y1(down) ac(clicked=0) with values from 0 to 500 with function call: interpret_joy_0
 */
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ajoystick.h>

unsigned int adc[8];

void main(void)
{
    WDTCTL     = WDTPW | WDTHOLD;
    BCSCTL1    = CALBC1_1MHZ;
    DCOCTL     = CALDCO_1MHZ;
    ADC10CTL1  = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
    ADC10CTL0  = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;
    ADC10AE0   = BIT5 + BIT4 + BIT3 + BIT2 + BIT1 + BIT0;
    ADC10DTC1  = 8;
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        interpret_joy_0(adc[5], adc[4], adc[3]);
        // do stuff with x0,x1,y0,y1,ac...
        }
}
