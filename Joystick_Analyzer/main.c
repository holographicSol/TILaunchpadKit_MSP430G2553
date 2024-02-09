/*
 * Written by Benjamin Jack Cullen
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

int ax = 0;
int ay = 0;
int ac = 0;
int x0 = 0;
int x1 = 0;
int y0 = 0;
int y1 = 0;
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
        joystick_0();
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

void joystick_0(void){
    ax = adc[5];
    ay = adc[4];
    ac = adc[3];
    // Zero to 500 converter: converts two analogue values to four 0-500 numbers.
    // An offset of 10 is also applied that accounts for a fluctuation range of 10 when my particular joy stick is idle (x=500-510, y=535-545). (A software patch for hardware problem).
    if       ((ay  <=  535) && (ax  <=  500))  {x0=abs(500-ax);  x1=0;               y0=abs(500-ay);    y1=0;               ssd1306_printText(0, 7, "[ULEFT] ");}
    else if  ((ay  <=  535) && (ax  >=  510))  {x0=0;            x1=abs((ax/2)-10);  y0=abs(500-ay);    y1=0;               ssd1306_printText(0, 7, "[URIGHT]");}
    else if  ((ay  >=  545) && (ax  <=  500))  {x0=abs(500-ax);  x1=0;               y0=0;              y1=abs((ay/2)-10);  ssd1306_printText(0, 7, "[DLEFT] ");}
    else if  ((ay  >=  545) && (ax  >=  510))  {x0=0;            x1=abs((ax/2)-10);  y0=0;              y1=abs((ay/2)-10);  ssd1306_printText(0, 7, "[DRIGHT]");}
    else if  (ax  <=   500)                    {x0=abs(500-ax);  x1=0;               y0=0;              y1=0;               ssd1306_printText(0, 7, "[LEFT]  ");}
    else if  (ax  >=   510)                    {x0=0;            x1=abs((ax/2)-10);  y0=0;              y1=0;               ssd1306_printText(0, 7, "[RIGHT] ");}
    else if  (ay  <=   535)                    {x0=0;            x1=0;               y0=abs(500-ay);    y1=0;               ssd1306_printText(0, 7, "[UP]    ");}
    else if  (ay  >=   545)                    {x0=0;            x1=0;               y0=0;              y1=abs((ay/2)-10);  ssd1306_printText(0, 7, "[DOWN]  ");}
    else                                       {x0=0;            x1=0;               y0=0;              y1=0;               ssd1306_printText(0, 7, "[NONE]  ");}
    // sanitise the normalisation (again joy stick specific, may not be necessary depending on quality of joy stick). this does not report corrections, only corrects:
    if (x0 > 500 ){x0=500;}  // ensure 500 max
    if (y0 > 500 ){y0=500;}  // ensure 500 max
    if (x1 > 500 ){x1=500;}  // ensure 500 max
    if (y1 > 500 ){y1=500;}  // ensure 500 max
    if (x0 < 0 ){x0=0;}      // ensure   0 minimum
    if (y0 < 0 ){y0=0;}      // ensure   0 minimum
    if (x1 < 0 ){x1=0;}      // ensure   0 minimum
    if (y1 < 0 ){y1=0;}      // ensure   0 minimum
    if (y0 >= 496){y0=500;}  // in my case y goes no lower than 4 on analogue value so i will force 500 at from 496
}
