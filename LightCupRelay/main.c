/*
 * Light Cup Sensor
 *
 * Rotary encoder for changing MCLK frequency. (mode 0: MCLK 1MHz, mode 1: MCLK 16MHz, mode 2: MCLK 16MHz and clock delays are removed)
 * Button enables/disables threshold breached code execution.
 * RGB broadly conveys MCLK frequency mode manipulated by rotary encoder.
 *
 * Light Cup:      +3v top    | -3v bottom | S P1.4   | L P1.3
 * Button:         +3v bottom | -3v bottom | S P1. 5
 * Rotary Encoder: +5v bottom | -v5 bottom | CLK P2.4 | DT P2.3 | SW P2.5
 * RGB LED:        + P1.6     | -5v bottom
 * Relay:          +5v top    | -5v top    | S P2.0
 *
 *
 */

#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <uart.h>
#include "rotary-encoder.h"

int datatosend[] = {0b00000001, 0b00000011, 0b00000111, 0b00001111, 0b00011111, 0b00111111, 0b01111111, 0b11111111};
int vthreshold[] = {0, 0, 0, 0, 0, 1, 1, 1};
int voltage = 0;
char vt_char[5];
char rm_char[5];
char mv_char[5];
char TAG_ADC10MEM[] = "[ ADC10MEM ] ";
char TAG_VOLT[] =     "[ VOLTAGE  ] ";
char TAG_DIGITAL[] =  "[ DIGITAL  ] ";
char newline[] = " \r\n";
char dot[] = ".";
int fmode;
int tmode;
#define GREENLED BIT0
#define SRCLK BIT3
#define RCLK BIT5
#define SW BIT5
#define CLKLED BIT6;
#define RELAY BIT0;

void SRCLK_Pulse (void);  //To create a clock pulse for Shift Reg
void RCLK_Pulse (void);   //To create a clock pulse for Storage Reg
void ZeroOneCondition (unsigned int value);     //For sending 1 or zero
void ser_output(char *str);

void setup(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1DIR |= GREENLED + SRCLK + RCLK;
    P1OUT &= ~GREENLED;

    P1DIR |= CLKLED;
    P1OUT &= ~CLKLED;

    P1SEL = BIT1|BIT2;
    P1SEL2 = BIT1|BIT2;
    P1DIR &= ~SW;       // set switch pin as input
    P1REN |= SW;        // enable resistor for switch pin
    P1OUT |= SW;        // select pull up for switch pin
    P1IES &= ~SW;       // select interrupt on rising edge
    P1IE |= SW;         // enable interrupt on switch pin
    encoderInit();
    __bis_SR_register(GIE);             // enable CPU interrupt
    fmode = 0;
    tmode = 0;
    UARTConfigure();
    volatile float voltage;
    ADC10CTL0 = SREF_0|ADC10SHT_2|ADC10ON;
    ADC10CTL1 = INCH_3|SHS_0|ADC10DIV_0|ADC10SSEL_0|CONSEQ_0;
    ADC10AE0 = BIT3;  //P1.3 also above, INCH_3
    ADC10CTL0 |= ENC;
}

void main(void)

{
    setup();
    int x;
    int y;
    int i;
    while(1){
        ADC10CTL0 |= ADC10SC;
        while(ADC10CTL1 & ADC10BUSY);
        int memval = ADC10MEM;
        voltage = ((ADC10MEM*3.3)/1023);
        int volt_int = floor(voltage);
        int rmndr = floor((voltage-volt_int)*1000);
        ltoa(memval, mv_char);
        ltoa(volt_int,vt_char);
        ltoa(rmndr,rm_char);

//        ser_output(TAG_ADC10MEM); ser_output(mv_char); ser_output(newline);
//        ser_output(TAG_VOLT); ser_output(vt_char); ser_output(dot); ser_output(rm_char); ser_output(newline);

        if ( voltage <= 0.412 ) y=0;
        else if ( voltage <= 0.825 ) y=1;
        else if ( voltage <= 1.236 ) y=2;
        else if ( voltage <= 1.648 ) y=3;
        else if ( voltage <= 2.06 ) y=4;
        else if ( voltage <= 2.472 ) y=5;
        else if ( voltage <= 2.884 ) y=6;
        else y=7;
        if(tmode==1){
            P1OUT ^= CLKLED;      // display t mode is enabled and broadly convey master clock speed
            x = vthreshold[y];    // get tunable voltage threshold breached as one or zero
            ZeroOneCondition(x);  // turn on/off relay if x zero or one
            SRCLK_Pulse();
        }
        else{P1OUT &= ~CLKLED;}  // t mode disabled, conveyed in loop

        RCLK_Pulse();

        if(fmode!=2){
            __delay_cycles(1000000);
        }
    }
}
void selectMode(){
    if(fmode==0){ BCSCTL1= CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;}
    else if(fmode==1){ BCSCTL1= CALBC1_16MHZ; DCOCTL = CALDCO_16MHZ;}
}
void stepCCW(){
    fmode = fmode-1;
    if(fmode<0){fmode=2;}
//    char int_str_mccw[4];
//    sprintf(int_str_mccw, "%d", fmode);
//    ser_output("[MODE] "); ser_output(int_str_mccw); ser_output(newline);
    selectMode();
}

void stepCW(){
    fmode = fmode+1;
    if(fmode>2){fmode=0;}
//    char int_str_mcw[4];
//    sprintf(int_str_mcw, "%d", fmode);
//    ser_output("[MODE] "); ser_output(int_str_mcw); ser_output(newline);
    selectMode();
}

void ser_output(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}
void SRCLK_Pulse (void)
{
  P1OUT |= SRCLK;
  P1OUT ^= SRCLK;
}
void RCLK_Pulse (void)
{
  P1OUT |= RCLK;
  P1OUT ^= RCLK;
}
void ZeroOneCondition (unsigned int value)
{
    if (value != 0){
//        ser_output(TAG_DIGITAL); ser_output("1"); ser_output(newline);  // send tagged non zero
        P1OUT |= GREENLED;   // set P1.0 as output

        P2DIR |= RELAY;
        P2OUT |= RELAY;
        __delay_cycles(10000000);
    }
    else {
//        ser_output(TAG_DIGITAL); ser_output("0"); ser_output(newline);  // send tagged zero
        P1OUT &= ~GREENLED;   // clear P1.0

        P2DIR |= RELAY;
        P2OUT &= ~RELAY;
    }
}

/* entry point for switch interrupt */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG & SW)  // if switch is pressed and P1 interrupt flag is true
    {
        if(tmode==0){tmode=1;}
        else if(tmode==1){tmode=0;}
        P1IFG &= ~SW;                   // clear switch interrupt flag
    }
    }
