#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float  VOLTADC10MEM = 0;
float  volts0    = 3.3;
float  volts1    = 3.3;
float  volts2    = 3.3;
float  volts3    = 3.3;
float  volts4    = 3.3;
float  volts5    = 5.0;
float  volts6    = 5.0;
float  volts7    = 5.0;
char   volta0[]  = "  A0: ";
char   volta1[]  = "  A1(RXD): ";
char   volta2[]  = "  A2(TXD): ";
char   volta3[]  = "  A3: ";
char   volta4[]  = "  A4: ";
char   volta5[]  = "  A5: ";
char   volta6[]  = "  A6: ";
char   volta7[]  = "  A7: ";
char   newline[] = " \r\n";
char vt_charv0[16];
char vt_charv1[16];
char vt_charv2[16];
char vt_charv3[16];
char vt_charv4[16];
char vt_charv5[16];
char vt_charv6[16];
char vt_charv7[16];
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
void pads(char *str, int sl);
void svolt(char *vstr, unsigned int av, float v);
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
    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
        ltoa(adc[7],vt_chara0); ltoa(adc[6],vt_chara1); ltoa(adc[5],vt_chara2);
        ltoa(adc[4],vt_chara3); ltoa(adc[3],vt_chara4); ltoa(adc[2],vt_chara5);
        ltoa(adc[1],vt_chara6); ltoa(adc[0],vt_chara7);
        pads(vt_chara0,4); pads(vt_chara1,4); pads(vt_chara2,4);
        pads(vt_chara3,4); pads(vt_chara4,4); pads(vt_chara5,4);
        pads(vt_chara6,4); pads(vt_chara7,4);
        svolt(vt_charv0, adc[7],volts0);  svolt(vt_charv1,adc[6],volts1);  svolt(vt_charv2,adc[5],volts2);
        svolt(vt_charv3, adc[4],volts3);  svolt(vt_charv4,adc[3],volts4);  svolt(vt_charv5,adc[2],volts5);
        svolt(vt_charv6, adc[1],volts6);  svolt(vt_charv7,adc[0],volts7);
        ser_output(volta0); ser_output(vt_chara0); ser_output(vt_charv0); ser_output(volta1); ser_output(vt_chara1); ser_output(vt_charv1);
        ser_output(volta2); ser_output(vt_chara2); ser_output(vt_charv2); ser_output(volta3); ser_output(vt_chara3); ser_output(vt_charv3);
        ser_output(volta4); ser_output(vt_chara4); ser_output(vt_charv4); ser_output(volta5); ser_output(vt_chara5); ser_output(vt_charv5);
        ser_output(volta6); ser_output(vt_chara6); ser_output(vt_charv6); ser_output(volta7); ser_output(vt_chara7); ser_output(vt_charv7);
        ser_output(newline);
        __delay_cycles(100000);
    }
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
