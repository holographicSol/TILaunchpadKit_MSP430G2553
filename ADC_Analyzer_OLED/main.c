#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SSD1306.h"
#include "i2c.h"

#define LONG_DELAY  __delay_cycles(1000000)
#define SHORT_DELAY __delay_cycles(50000)
const int    main_delay   = 0;
float        VOLTADC10MEM = 0;
int          digiathresh0 = 0;
int          digiathresh1 = 0;
int          digiathresh2 = 0;
int          digiathresh3 = 0;
int          digiathresh4 = 0;
int          digiathresh5 = 0;
int          digiathresh6 = 0;
int          digiathresh7 = 0;
const int    athresh0     = 50;   // tunable digital threshold
const int    athresh1     = 50;   // tunable digital threshold
const int    athresh2     = 50;   // tunable digital threshold
const int    athresh3     = 50;   // tunable digital threshold
const int    athresh4     = 1000;  // tunable digital threshold
const int    athresh5     = 50;   // tunable digital threshold
const int    athresh6     = 50;   // tunable digital threshold
const int    athresh7     = 50;   // tunable digital threshold
const float  volts0       = 3.3;  // set actual pin voltage
const float  volts1       = 3.3;  // set actual pin voltage
const float  volts2       = 3.3;  // set actual pin voltage
const float  volts3       = 3.3;  // set actual pin voltage
const float  volts4       = 3.3;  // set actual pin voltage
const float  volts5       = 3.0;  // set actual pin voltage
const float  volts6       = 5.0;  // set actual pin voltage
const float  volts7       = 3.0;  // set actual pin voltage
char *sdigiathresh0;
char *sdigiathresh1;
char *sdigiathresh2;
char *sdigiathresh3;
char *sdigiathresh4;
char *sdigiathresh5;
char *sdigiathresh6;
char *sdigiathresh7;
char vt_charv0[10];
char vt_charv1[10];
char vt_charv2[10];
char vt_charv3[10];
char vt_charv4[10];
char vt_charv5[10];
char vt_charv6[10];
char vt_charv7[10];
char vt_chara0[5];
char vt_chara1[5];
char vt_chara2[5];
char vt_chara3[5];
char vt_chara4[5];
char vt_chara5[5];
char vt_chara6[5];
char vt_chara7[5];
char STRVOLTADC10MEM[20];
char STRRMDRADC10MEM[20];
unsigned int adc[8];

void ser_output(char *str);
void svolt(char *vstr, unsigned int av, float v);
void set_digital_a_values(void);
void digital_execution(void);

void main(void)
{
    WDTCTL     = WDTPW | WDTHOLD;
    i2c_init();                                 // initialize I2C to use with OLED
    ssd1306_init();                             // Initialize SSD1306 OLED
    ssd1306_clearDisplay();                     // Clear OLED display
    ssd1306_printText(0, 1, "[ ADC Analyzer ]");
    BCSCTL1    = CALBC1_1MHZ;
    DCOCTL     = CALDCO_1MHZ;
    ADC10CTL1  = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
    ADC10CTL0  = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;
    ADC10AE0   = BIT5 + BIT4 + BIT3 + BIT2 + BIT1+ BIT0;
    ADC10DTC1  = 8;
    LONG_DELAY;
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        ltoa(adc[7],vt_chara0);
        ltoa(adc[6],vt_chara1);
        ltoa(adc[5],vt_chara2);
        ltoa(adc[4],vt_chara3);
        ltoa(adc[3],vt_chara4);
        ltoa(adc[2],vt_chara5);
        set_digital_a_values();
        svolt(vt_charv0, adc[7],volts3);
        svolt(vt_charv1, adc[6],volts3);
        svolt(vt_charv2, adc[5],volts3);
        svolt(vt_charv3, adc[4],volts3);
        svolt(vt_charv4, adc[3],volts4);
        svolt(vt_charv5, adc[2],volts5);
        ssd1306_printText(0, 2, "A0"); ssd1306_printText(20, 2, sdigiathresh0);
        ssd1306_printText(30, 2, vt_chara0); ssd1306_printText(55, 2, vt_charv0);
        ssd1306_printText(0, 3, "A1"); ssd1306_printText(20, 3, sdigiathresh1);
        ssd1306_printText(30, 3, vt_chara1); ssd1306_printText(55, 3, vt_charv1);
        ssd1306_printText(0, 4, "A2"); ssd1306_printText(20, 4, sdigiathresh2);
        ssd1306_printText(30, 4, vt_chara2); ssd1306_printText(55, 4, vt_charv2);
        ssd1306_printText(0, 5, "A3"); ssd1306_printText(20, 5, sdigiathresh3);
        ssd1306_printText(30, 5, vt_chara3); ssd1306_printText(55, 5, vt_charv3);
        ssd1306_printText(0, 6, "A4"); ssd1306_printText(20, 6, sdigiathresh4);
        ssd1306_printText(30, 6, vt_chara4); ssd1306_printText(55, 6, vt_charv4);
        ssd1306_printText(0, 7, "A5"); ssd1306_printText(20, 7, sdigiathresh5);
        ssd1306_printText(30, 7, vt_chara5); ssd1306_printText(55, 7, vt_charv5);
        digital_execution();
        if(main_delay==1){__delay_cycles(100000);}
    }
}
void digital_execution(void){
}
void set_digital_a_values(void){  // tunable operators
    if     (adc[7]  <=  athresh0){digiathresh0=1; sdigiathresh0="1";}  //P1.0
    else if(adc[7]  >   athresh0){digiathresh0=0; sdigiathresh0="0";}  //P1.0
    if     (adc[6]  <=  athresh1){digiathresh1=1; sdigiathresh1="1";}  //P1.1
    else if(adc[6]  >   athresh1){digiathresh1=0; sdigiathresh1="0";}  //P1.1
    if     (adc[5]  <=  athresh2){digiathresh2=1; sdigiathresh2="1";}  //P1.2
    else if(adc[5]  >   athresh2){digiathresh2=0; sdigiathresh2="0";}  //P1.2
    if     (adc[4]  <=  athresh3){digiathresh3=1; sdigiathresh3="1";}  //P1.3
    else if(adc[4]  >   athresh3){digiathresh3=0; sdigiathresh3="0";}  //P1.3
    if     (adc[3]  >=  athresh4){digiathresh4=1; sdigiathresh4="1";}  //P1.4
    else if(adc[3]  <   athresh4){digiathresh4=0; sdigiathresh4="0";}  //P1.4
    if     (adc[2]  <=  athresh5){digiathresh5=1; sdigiathresh5="1";}  //P1.5
    else if(adc[2]  >   athresh5){digiathresh5=0; sdigiathresh5="0";}  //P1.5
}
void svolt(char *vstr, unsigned int av, float v){
    VOLTADC10MEM        = 0;
    VOLTADC10MEM        = ((av*v)/1023);
    int VINTADC10MEM    = floor(VOLTADC10MEM);
    double RMDRADCMEM10 = floor((av-VINTADC10MEM)*1000);
    ltoa(VINTADC10MEM,STRVOLTADC10MEM);
    ltoa((long)RMDRADCMEM10,STRRMDRADC10MEM);
    memset(vstr, 0, sizeof vstr);
    strcpy(vstr, " ");
    strcat(vstr, STRVOLTADC10MEM);
    strcat(vstr, ".");
    strcat(vstr, STRRMDRADC10MEM);
}
