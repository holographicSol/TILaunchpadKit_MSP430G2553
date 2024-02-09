/*
 * Joystick Analyzer: This setup increases/decreases values from approximately 0 to 500 for each direction.
 * All calls to ssd1306_printText can be removed outside of testing purposes, leaving only desired values being populated to remain.
 */
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SSD1306.h"
#include "i2c.h"
#include <ajoystick.h>

char x0_char[5];
char x1_char[5];
char y0_char[5];
char y1_char[5];
char c0_char[5];
char vt_chara2[5];
char vt_chara3[5];
char vt_chara4[5];
unsigned int adc[8];

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
    __delay_cycles(1000);
    ssd1306_clearDisplay();
    ssd1306_printText(0, 1, "[ Joystick Analyzer ]");
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        ltoa(adc[5], vt_chara2); ltoa(adc[4], vt_chara3); ltoa(adc[3], vt_chara4);
        joystick_0(adc[5], adc[4], adc[3]);
        ltoa(x0, x0_char); ltoa(x1, x1_char); ltoa(y0, y0_char); ltoa(y1, y1_char); ltoa(ac, c0_char);
        ssd1306_printText(0,  2, "X      ");  ssd1306_printText(15,  2, vt_chara2);
        ssd1306_printText(0,  3, "X0     ");  ssd1306_printText(15,  3, x0_char);
        ssd1306_printText(0,  4, "X1     ");  ssd1306_printText(15,  4, x1_char);
        ssd1306_printText(45, 2, "Y      ");  ssd1306_printText(60,  2, vt_chara3);
        ssd1306_printText(45, 3, "Y0     ");  ssd1306_printText(60,  3, y0_char);
        ssd1306_printText(45, 4, "Y1     ");  ssd1306_printText(60,  4, y1_char);
        ssd1306_printText(90, 2, "C     ");   ssd1306_printText(100, 2, vt_chara4);
        }
}
