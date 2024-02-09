/*
 * Joystick Analyzer: This setup increases/decreases values from approximately 0 to 500 for each direction.
 * All calls to ssd1306_printText can be removed outside of testing purposes, leaving only desired values being populated to remain.
 */
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SSD1306.h>
#include <i2c.h>
#include <ajoystick.h>

unsigned int adc[8];

void joystick_0(void);
void main(void)
{
    WDTCTL     = WDTPW | WDTHOLD;
    BCSCTL1    = CALBC1_1MHZ;
    DCOCTL     = CALDCO_1MHZ;
    i2c_init();
    ssd1306_init();
    ADC10CTL1  = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
    ADC10CTL0  = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;
    ADC10AE0   = BIT5 + BIT4 + BIT3 + BIT2 + BIT1 + BIT0;
    ADC10DTC1  = 8;
    __delay_cycles(10000);
    ssd1306_clearDisplay();
    ssd1306_printText(0, 1, "[ Joystick Analyzer ]");
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;

        j0_ax = adc[5]; j0_ay = adc[4]; j0_ac = adc[3];
        interpret_joy_0(adc[5], adc[4], adc[3]);

        ssd1306_printText(0,  2, "X      ");   ssd1306_printUI32(15,  2, j0_ax, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  3, "X0     ");  ssd1306_printUI32(15,  3, j0_x0, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  4, "X1     ");  ssd1306_printUI32(15,  4, j0_x1, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 2, "Y      ");   ssd1306_printUI32(60,  2, j0_ay, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 3, "Y0     ");  ssd1306_printUI32(60,  3, j0_y0, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 4, "Y1     ");  ssd1306_printUI32(60,  4, j0_y1, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(90, 2, "C      ");   ssd1306_printUI32(100, 2, j0_ac, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(90, 3, "C0     ");    ssd1306_printUI32(110, 3, j0_c0, HCENTERUL_OFF, ComSep=0);
        }
}
