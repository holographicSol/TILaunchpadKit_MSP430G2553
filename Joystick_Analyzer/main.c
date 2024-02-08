#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SSD1306.h"
#include "i2c.h"

int YU = 0;
int YD = 0;
int XL = 0;
int XR = 0;
int CLICKED = 0;
char vt_chara2[5];
char vt_chara3[5];
char vt_chara4[5];
unsigned int adc[8];

void ser_output(char *str);
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
        ssd1306_printText(0, 2, "            ");
        ssd1306_printText(0, 2, "A2"); ssd1306_printText(20, 2, vt_chara2);
        ssd1306_printText(0, 3, "            ");
        ssd1306_printText(0, 3, "A3"); ssd1306_printText(20, 3, vt_chara3);
        ssd1306_printText(0, 4, "            ");
        ssd1306_printText(0, 4, "A4"); ssd1306_printText(20, 4, vt_chara4);
        }
}

void joystick_0(void){  // tunable operators
    if      ((adc[5] <=  500) &&  (adc[5] >=  400))  {YU=0; YD=0; XR=0; XL=1; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[LEFT]     <");}
    else if ((adc[5] <=  399) &&  (adc[5] >=  300))  {YU=0; YD=0; XR=0; XL=1; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[LEFT]    <<");}
    else if ((adc[5] <=  299) &&  (adc[5] >=  200))  {YU=0; YD=0; XR=0; XL=1; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[LEFT]   <<<");}
    else if ((adc[5] <=  199) &&  (adc[5] >=  100))  {YU=0; YD=0; XR=0; XL=1; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[LEFT]  <<<<");}
    else if ((adc[5] <=  99)  &&  (adc[5] >=    0))  {YU=0; YD=0; XR=0; XL=1; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[LEFT] <<<<<");}

    else if((adc[5]  >=  600)  && (adc[5] <=  699))  {YU=0; YD=0; XR=1; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[RIGHT] >    ");}
    else if((adc[5]  >=  700)  && (adc[5] <=  799))  {YU=0; YD=0; XR=1; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[RIGHT] >>   ");}
    else if((adc[5]  >=  800)  && (adc[5] <=  899))  {YU=0; YD=0; XR=1; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[RIGHT] >>>  ");}
    else if((adc[5]  >=  900)  && (adc[5] <=  999))  {YU=0; YD=0; XR=1; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[RIGHT] >>>> ");}
    else if((adc[5]  >=  1000) && (adc[5] <=  1024)) {YU=0; YD=0; XR=1; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[RIGHT] >>>>>");}

    else if((adc[4] <=  500) &&  (adc[4] >=  400)) {YU=1; YD=0; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[UP] >    ");}
    else if((adc[4] <=  399) &&  (adc[4] >=  300)) {YU=1; YD=0; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[UP] >>   ");}
    else if((adc[4] <=  299) &&  (adc[4] >=  200)) {YU=1; YD=0; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[UP] >>>  ");}
    else if((adc[4] <=  199) &&  (adc[4] >=  100)) {YU=1; YD=0; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[UP] >>>> ");}
    else if((adc[4] <=  99)  &&  (adc[4] >=  0))   {YU=1; YD=0; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[UP] >>>>>");}


    else if((adc[4]  >=  600)   && (adc[4] <=  699))  {YU=0; YD=1; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[DOWN]     <");}
    else if((adc[4]  >=  700)   && (adc[4] <=  799))  {YU=0; YD=1; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[DOWN]    <<");}
    else if((adc[4]  >=  800)   && (adc[4] <=  899))  {YU=0; YD=1; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[DOWN]   <<<");}
    else if((adc[4]  >=  900)   && (adc[4] <=  999))  {YU=0; YD=1; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[DOWN]  <<<<");}
    else if((adc[4]  >=  1000)  && (adc[4] <=  1024)) {YU=0; YD=1; XR=0; XL=0; ssd1306_printText(0, 7, "       "); ssd1306_printText(0, 7, "[DOWN] <<<<<");}


    else{YU=0; YD=0; XR=0; XL=0; ssd1306_printText(0, 7,       "         "); ssd1306_printText(0, 7, "[NONE]       ");}

    if      (adc[3] ==  0) {CLICKED=1; ssd1306_printText(0, 6, "[CLICKED]");}
    else                   {CLICKED=0; ssd1306_printText(0, 6, "         ");}
}
