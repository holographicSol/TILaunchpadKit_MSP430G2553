/*
 * Joystick Analyzer: This setup increases/decreases values from approximately 0 to 500 for each direction.
 */

#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SSD1306.h"
#include "i2c.h"

int ax = 0;
int ay = 0;
int ac = 0;
int x0 = 0;
int x1 = 0;
int y0 = 0;
int y1 = 0;
int c0 = 0;
char x0_char[5];
char x1_char[5];
char y0_char[5];
char y1_char[5];
char c0_char[5];
char vt_chara2[5];
char vt_chara3[5];
char vt_chara4[5];
unsigned int adc[8];

void joystick_0(void);
void main(void)
{

    WDTCTL     = WDTPW | WDTHOLD;
    i2c_init();
    ssd1306_init();
    ssd1306_clearDisplay();
    ssd1306_printText(0, 1, "[ Joystick Analyzer ]");
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
        ltoa(adc[5], vt_chara2);
        ltoa(adc[4], vt_chara3);
        ltoa(adc[3], vt_chara4);
        joystick_0();
        ltoa(x0, x0_char);
        ltoa(x1, x1_char);
        ltoa(y0, y0_char);
        ltoa(y1, y1_char);
        ltoa(c0, c0_char);
        ssd1306_printText(0,  2, "X      ");  ssd1306_printText(15,  2, vt_chara2);
        ssd1306_printText(0,  3, "X0     ");  ssd1306_printText(15,  3, x0_char);
        ssd1306_printText(0,  4, "X1     ");  ssd1306_printText(15,  4, x1_char);
        ssd1306_printText(45, 2, "Y      ");  ssd1306_printText(60,  2, vt_chara3);
        ssd1306_printText(45, 3, "Y0     ");  ssd1306_printText(60,  3, y0_char);
        ssd1306_printText(45, 4, "Y1     ");  ssd1306_printText(60,  4, y1_char);
        ssd1306_printText(90, 2, "C     ");   ssd1306_printText(100, 2, vt_chara4);
        ssd1306_printText(90, 3, "C0    ");   ssd1306_printText(105, 3, c0_char);
        }
}

void joystick_0(void){

    ax = adc[5];
    ay = adc[4];
    ac = adc[3];

    /* Zero to 500 converter: converts two analogue values to four 0-500 numbers.
     * This setup enables normalisation of the four values derived from the 2 analogue values.
     * TODO -> Offset calibration (my joy stick fluctuates at idle (x=505-507, y=540-541) so in my case my idle buffer range is about 10, (x=500-510, y=535-545).
     */
    if      ((ay  <=  535) && (ax  <=  500)) {x0=(500-ax);  x1=0;       y0=(500-ay);  y1=0;       ssd1306_printText(0, 7, "[ULEFT] ");}
    else if ((ay  <=  535) && (ax  >=  510)) {x0=0;         x1=(ax/2);  y0=(500-ay);  y1=0;       ssd1306_printText(0, 7, "[URIGHT]");}
    else if ((ay  >=  545) && (ax  <=  500)) {x0=(500-ax);  x1=0;       y0=0;         y1=(ay/2);  ssd1306_printText(0, 7, "[DLEFT] ");}
    else if ((ay  >=  545) && (ax  >=  510)) {x0=0;         x1=(ax/2);  y0=0;         y1=(ay/2);  ssd1306_printText(0, 7, "[DRIGHT]");}
    else if (ax  <=   500)                   {x0=(500-ax);  x1=0;       y0=0;         y1=0;       ssd1306_printText(0, 7, "[LEFT]  ");}
    else if (ax  >=   510)                   {x0=0;         x1=(ax/2);  y0=0;         y1=0;       ssd1306_printText(0, 7, "[RIGHT] ");}
    else if (ay  <=   535)                   {x0=0;         x1=0;       y0=(500-ay);  y1=0;       ssd1306_printText(0, 7, "[UP]    ");}
    else if (ay  >=   545)                   {x0=0;         x1=0;       y0=0;         y1=(ay/2);  ssd1306_printText(0, 7, "[DOWN]  ");}
    else                                     {x0=0; x1=0; y0=0; y1=0;                             ssd1306_printText(0, 7, "[NONE]  ");}
    if      (ac  ==     0)                   {c0=1;                                               }
    else                                     {c0=0;                                               }
}
