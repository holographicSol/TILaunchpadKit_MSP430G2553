// Credits to drselim MSP430 & Multiple ADC (https://youtu.be/Ev871bhGFt0).
// Update includes value padding for alignment and voltage readings. 
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int    main_delay   = 1;
float        VOLTADC10MEM = 0;
int          digiathresh0 = 0;
int          digiathresh3 = 0;
int          digiathresh4 = 0;
int          digiathresh5 = 0;
int          digiathresh6 = 0;
int          digiathresh7 = 0;
const int    athresh0     = 50;   // tunable digital threshold
const int    athresh3     = 800;  // tunable digital threshold
const int    athresh4     = 50;   // tunable digital threshold
const int    athresh5     = 50;   // tunable digital threshold
const int    athresh6     = 50;   // tunable digital threshold
const int    athresh7     = 50;   // tunable digital threshold
const float  volts0       = 3.3;  // set actual pin voltage
const float  volts3       = 3.3;  // set actual pin voltage
const float  volts4       = 3.3;  // set actual pin voltage
const float  volts5       = 3.0;  // set actual pin voltage
const float  volts6       = 3.0;  // set actual pin voltage
const float  volts7       = 3.0;  // set actual pin voltage
char         volta0[]     = "  A0: ";
char         volta3[]     = "  A3: ";
char         volta4[]     = "  A4: ";
char         volta5[]     = "  A5: ";
char         volta6[]     = "  A6: ";
char         volta7[]     = "  A7: ";
char         newline[]    = " \r\n";
char *sdigiathresh0;
char *sdigiathresh3;
char *sdigiathresh4;
char *sdigiathresh5;
char *sdigiathresh6;
char *sdigiathresh7;
char vt_charv0[16];
char vt_charv3[16];
char vt_charv4[16];
char vt_charv5[16];
char vt_charv6[16];
char vt_charv7[16];
char vt_chara0[5];
char vt_chara3[5];
char vt_chara4[5];
char vt_chara5[5];
char vt_chara6[5];
char vt_chara7[5];
char STRVOLTADC10MEM[20];
char STRRMDRADC10MEM[20];
unsigned int adc[8];
#define RELAY BIT3;

void ser_output(char *str);
void pads(char *str, int sl);
void svolt(char *vstr, unsigned int av, float v);
void set_digital_a_values(void);
void main(void)
{
    WDTCTL     = WDTPW | WDTHOLD;
    P1SEL      = BIT1 + BIT2;
    P1SEL2     = BIT1 + BIT2 ;
    UCA0CTL1  |= UCSSEL_2;
    UCA0BR0    = 104;
    UCA0BR1    = 0;
    UCA0MCTL   = UCBRS0;
    UCA0CTL1  &= ~UCSWRST;
    IE2       |= UCA0RXIE;
    BCSCTL1    = CALBC1_1MHZ;
    DCOCTL     = CALDCO_1MHZ;
    P1SEL      = BIT1|BIT2;
    P1SEL2     = BIT1|BIT2;
    ADC10CTL1  = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
    ADC10CTL0  = SREF_0 + ADC10SHT_2 + MSC + ADC10ON;
    ADC10AE0   = BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT0;
    ADC10DTC1  = 8;
    P2OUT &= ~RELAY;
    P2DIR |= RELAY;
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        ltoa(adc[7],vt_chara0); ltoa(adc[4],vt_chara3);
        ltoa(adc[3],vt_chara4); ltoa(adc[0],vt_chara7);
        ltoa(adc[1],vt_chara6); ltoa(adc[2],vt_chara5);
        pads(vt_chara0,4); pads(vt_chara3,4); pads(vt_chara4,4);
        pads(vt_chara5,4); pads(vt_chara6,4); pads(vt_chara7,4);
        set_digital_a_values();
        svolt(vt_charv0, adc[7],volts0); svolt(vt_charv3, adc[4],volts3);
        svolt(vt_charv4,adc[3],volts4);  svolt(vt_charv5,adc[2],volts5);
        svolt(vt_charv6, adc[1],volts6);  svolt(vt_charv7,adc[0],volts7);
        ser_output(volta0); ser_output(sdigiathresh0); ser_output(" "); ser_output(vt_chara0); ser_output(vt_charv0);
        ser_output(volta3); ser_output(sdigiathresh3); ser_output(" "); ser_output(vt_chara3); ser_output(vt_charv3);
        ser_output(volta4); ser_output(sdigiathresh4); ser_output(" "); ser_output(vt_chara4); ser_output(vt_charv4);
        ser_output(volta5); ser_output(sdigiathresh5); ser_output(" "); ser_output(vt_chara5); ser_output(vt_charv5);
        ser_output(volta6); ser_output(sdigiathresh6); ser_output(" "); ser_output(vt_chara6); ser_output(vt_charv6);
        ser_output(volta7); ser_output(sdigiathresh7); ser_output(" "); ser_output(vt_chara7); ser_output(vt_charv7);
        ser_output(newline);
        if(main_delay==1){__delay_cycles(100000);}
    }
}
void set_digital_a_values(void){  // tunable operators
    if     (adc[7]  <=  athresh0){digiathresh0=1; sdigiathresh0="1";}  //P1.0
    else if(adc[7]  >   athresh0){digiathresh0=0; sdigiathresh0="0";}  //P1.0
    if     (adc[4]  >=  athresh3){digiathresh3=1; sdigiathresh3="1";}  //P1.3
    else if(adc[4]  <   athresh3){digiathresh3=0; sdigiathresh3="0";}  //P1.3
    if     (adc[3]  <=  athresh4){digiathresh4=1; sdigiathresh4="1";}  //P1.4
    else if(adc[3]  >   athresh4){digiathresh4=0; sdigiathresh4="0";}  //P1.4
    if     (adc[2]  <=  athresh5){digiathresh5=1; sdigiathresh5="1";}  //P1.5
    else if(adc[2]  >   athresh5){digiathresh5=0; sdigiathresh5="0";}  //P1.5
    if     (adc[1]  <=  athresh6){digiathresh6=1; sdigiathresh6="1";}  //P1.6
    else if(adc[1]  >   athresh6){digiathresh6=0; sdigiathresh6="0";}  //P1.6
    if     (adc[0]  <=  athresh7){digiathresh7=1; sdigiathresh7="1";}  //P1.7
    else if(adc[0]  >   athresh7){digiathresh7=0; sdigiathresh7="0";}  //P1.7
}
void ser_output(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}
void pads(char *str, int sl) {
    int i = 0;
    int length = 0;
    length = strlen(str);
    for(i = length; i < sl; i++){str[i] = ' ';}
    str[sl] = '\0';
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
    pads(vstr, 9);
}

