/*
 * Joy Stick Analyzer: For calibrating ajoystick.h library for a specific joy stick.
 *
 */
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SSD1306.h>
#include <i2c.h>
#include <ajoystick.h>

int px = 5;  // x adc[5] = p1.2
int py = 4;  // y adc[4] = p1.3
int pc = 3;  // c adc[3] = p1.4
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
        j0_ax = adc[px]; j0_ay = adc[py]; j0_ac = adc[pc];
        interpret_joy(adc[px], adc[py], adc[pc], 500, 490, 510, 530, 550, 10, 16, 0);
        ssd1306_printText(0,  2, "X      ");  ssd1306_printUI32(15,  2, j0_ax, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  3, "X0     ");  ssd1306_printUI32(15,  3, j0_x0, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  4, "X1     ");  ssd1306_printUI32(15,  4, j0_x1, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  5, "X2     ");  ssd1306_printUI32(15,  5, j0_x2, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(0,  6, "X3     ");  ssd1306_printUI32(15,  6, j0_x3, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 2, "Y      ");  ssd1306_printUI32(60,  2, j0_ay, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 3, "Y0     ");  ssd1306_printUI32(60,  3, j0_y0, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 4, "Y1     ");  ssd1306_printUI32(60,  4, j0_y1, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 5, "Y2     ");  ssd1306_printUI32(60,  5, j0_y2, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(45, 6, "Y3     ");  ssd1306_printUI32(60,  6, j0_y3, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(90, 2, "C      ");  ssd1306_printUI32(100, 2, j0_ac, HCENTERUL_OFF, ComSep=0);
        ssd1306_printText(90, 3, "C0     ");  ssd1306_printUI32(110, 3, j0_c0, HCENTERUL_OFF, ComSep=0);
        }
}
