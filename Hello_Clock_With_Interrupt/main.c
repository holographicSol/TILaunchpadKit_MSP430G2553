/*
 * Hello Clock with Interrupts!
 *
 * This program provides 3 clock frequency modes accessible via the push button.
 *
 * 1: Pushbutton cycles clock frequency mode via use of interrupts.
 * 2: RGB LED: intends to display current clock frequency mode (red=15kHz, green=3kHz, blue=12kHz).
 * 3: Green LED: broadly conveys current clock frequency via blink rate.
 *
 * Created by Benjamin Jack Cullen
 *
 */

#include <msp430.h>
#include <serialprint.h>
#include <manipulate_clocks.h>
#include <manipulate_leds.h>


int clock_mode = 0;

void setup(){
    // direction led's
    P1DIR |= BIT6;      // set red led pin as output
    P1DIR |= BIT0;      // set green led pin as output

    // direction RGB
    P2DIR |= BIT1;      // configure P2.1 as output R
    P2DIR |= BIT3;      // configure P2.3 as output G
    P2DIR |= BIT5;      // configure P2.5 as output B

    // direction switch
    P1DIR &= ~BIT3;     // set switch pin as input

    // startup states RGB
    P1OUT &=~ (BIT8 + BIT6);    // clear P2.1  (RGB red off)
    P1OUT &=~ (BIT8 + BIT0);    // clear P2.1  (RGB red off)

    // startup states switch
    P1OUT |= BIT3;              // select pull up for switch pin
    P1REN |= BIT3;              // enable resistor for switch pin
    P1IES &= ~BIT3;             // select interrupt on rising edge
    P1IE |= BIT3;               // enable interrupt on switch pin
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop Watchdog timer

    setup();                            // initial setup

    UARTConfigure();                    // setup UART RXD & TXD

    __bis_SR_register(GIE);             // enable CPU interrupt

    register_settings_for_VLO();        // initial VLO setup
    MCLK_VLO_1_5kHz();                  // setup initial master clock configuration (low power mode)
    RGB_0_RED();

    volatile unsigned long n;
    while(1) {
        P1OUT ^= BIT0;                  // toggle green led to broadly convey current master clock frequency
        for (n = 100; n > 0; n--);      // very short delay
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(clock_mode==0)
    {
        MCLK_VLO_1_5kHz();
        UARTSendArray("\r\n", 2);
        UARTSendArray("-- master clock signal using VLO as clock source at frequency: 1.5kHz (low power mode)", 86);
        UARTSendArray("\r\n", 2);
        RGB_0_RED();
    }
    else
    {
        if(clock_mode==1)
        {
            MCLK_VLO_3kHz();
            UARTSendArray("-- master clock signal using VLO as clock source at frequency: 3kHz", 68);
            UARTSendArray("\r\n", 2);
            RGB_0_GREEN();
        }
        else
        {
            if(clock_mode==2)
            {
                MCLK_VLO_12kHz();
                UARTSendArray("-- master clock signal using VLO as clock source at frequency: 12kHz", 69);
                UARTSendArray("\r\n", 2);
                RGB_0_BLUE();
            }
            else
            {
                if(clock_mode==3)
                {
                    MCLK_DCO_125kHz();
                    UARTSendArray("-- master clock signal using DCO as clock source at frequency: 125kHz", 69);
                    UARTSendArray("\r\n", 2);
                    RGB_0_PURPLE();
                }
                else
                {
                    if(clock_mode==4)
                    {
                        UARTSendArray("-- master clock signal using DCO as clock source at frequency: 16MHz (high power mode)", 88);
                        RGB_0_WHITE();
                        MCLK_DCO_16MHz();
                    }
                }
            }
        }
    }
    P1IFG &= ~BIT3;    // clear switch interrupt flag
}
