/**
 * main.c
 */
#include <stdio.h>
#include <msp430.h>
#include "SSD1306.h"
#include "i2c.h"
#include <font_ssd1306_128x32px.h>
#include <manipulate_leds.h>

int smode = 0;
int execution=0;

void setup()
{
    WDTCTL = WDTPW | WDTHOLD;                   // stop watch dog timer
    __delay_cycles(100);                        // wait n clock cycles/
    P2DIR |= BIT1;                              // configure P2.1 as output R
    P2DIR |= BIT3;                              // configure P2.3 as output G
    P2DIR |= BIT5;                              // configure P2.5 as output B
    RGB_0_BLUE();
    BCSCTL1 = CALBC1_16MHZ;                     // Set DCO to 16MHz (for better OLED performance)
    DCOCTL = CALDCO_16MHZ;                      // Set DCO to 16MHz (for better OLED performance)
    i2c_init();                                 // initialise I2C to use with OLED
    ssd1306_init();                             // Initialise SSD1306 OLED
    ssd1306_clearDisplay();
    P1DIR &= ~BIT3;                     // set switch pin as input
    P1OUT |= BIT3;                      // select pull up for switch pin
    P1REN |= BIT3;                      // enable resistor for switch pin
    P1IES &= ~BIT3;                     // select interrupt on rising edge
    P1IE |= BIT3;                       // enable interrupt on switch pin
    RGB_0_OFF();
}

void lauch_zero()
{
    RGB_0_RED();
    char cstr[50];
    int n = 5;
    while (n>=0) {
        char int_str[36];
        sprintf(int_str, "%d", n);
        strcpy(cstr, "T-");
        strcat(cstr, int_str);
        ssd1306_clearDisplay2(0, 4);
        oledWrite(dict, 39, "-LAUNCH SEQUENCE-", 0, 0);
        oledWrite(dict, 39, cstr, 0, 4);
        __delay_cycles(10000000);
        n--;
    }
    ssd1306_clearDisplay();
    oledWrite(dict, 39, "LAUNCHING!", 0, 0);
    RGB_0_RED_FLASH(10);
    __delay_cycles(10000000);
    ssd1306_clearDisplay();
    execution = 1;
}

int main(void)
{
    setup();
    __bis_SR_register(GIE);         // enable CPU interrupt
    while(1)
    {
        RGB_0_GREEN();
        __delay_cycles(1000000);
        RGB_0_OFF();
        __delay_cycles(1000000);
        if(execution==1)
        {
            lauch_zero();
            execution=0;
        }
    }
}

/* entry point for switch interrupt */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG & BIT3)    // if switch is pressed and interrupt flag logic one for switch
    {
        execution=1;
    }
    P1IFG &= ~BIT3;     // clear switch interrupt flag
}
