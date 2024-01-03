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
    volatile unsigned long s;
    for (s = 50000; s; s--);     // Delay

    // direction led's
    P1DIR |= BIT6;                      // set red led pin as output
    P1DIR |= BIT0;                      // set green led pin as output

    // direction RGB
    P2DIR |= BIT1;                      // configure P2.1 as output R
    P2DIR |= BIT3;                      // configure P2.3 as output G
    P2DIR |= BIT5;                      // configure P2.5 as output B

    // direction switch
    P1DIR &= ~BIT3;                     // set switch pin as input

    // startup states RGB
    P1OUT &=~ (BIT8 + BIT6);            // clear P2.1  (RGB red off)
    P1OUT &=~ (BIT8 + BIT0);            // clear P2.1  (RGB red off)

    // startup states switch
    P1OUT |= BIT3;                      // select pull up for switch pin
    P1REN |= BIT3;                      // enable resistor for switch pin
    P1IES &= ~BIT3;                     // select interrupt on rising edge
    P1IE |= BIT3;                       // enable interrupt on switch pin
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop Watchdog timer

    setup();                            // initial setup

    UARTConfigure();                    // setup UART RXD & TXD

    __bis_SR_register(GIE);             // enable CPU interrupt

    register_settings_for_VLO();
    MCLK_VLO_1_5kHz();                  // setup initial master clock configuration (low power mode)
    RGB_0_RED();

    char txd[] = "\r\n-- MCLK=VLOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=8 12kHz/8=1.5kHz\r\n";
    UARTSendArray(txd, strlen(txd));

    volatile unsigned long n;
    while(1) {
        P1OUT ^= BIT0;                  // toggle green led to broadly convey current master clock frequency
        for (n = 100; n > 0; n--);      // very short delay for user blink rate visibility
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(clock_mode==0)
    {
        register_settings_for_VLO();
        MCLK_VLO_1_5kHz();
        volatile unsigned long c;
        char txd[] = "-- MCLK=VLOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=8 12kHz/8=1.5kHz\r\n";
        UARTSendArray(txd, strlen(txd));
        RGB_0_RED();
    }
    else
    {
        if(clock_mode==1)
        {
            MCLK_VLO_3kHz();
            char txd[] = "-- MCLK=VLOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=4 12kHz/4=3kHz\r\n";
            UARTSendArray(txd, strlen(txd));
            RGB_0_GREEN();
        }
        else
        {
            if(clock_mode==2)
            {
                MCLK_VLO_12kHz();
                char txd[] = "-- MCLK=VLOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=1 12kHz/1=12kHz\r\n";
                UARTSendArray(txd, strlen(txd));
                RGB_0_BLUE();
            }
            else
            {
                if(clock_mode==3)
                {
                    MCLK_DCO_125kHz();
                    char txd[] = "-- MCLK=DCOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=8 1MHz/8=125kHz\r\n";
                    UARTSendArray(txd, strlen(txd));
                    RGB_0_LIGHT_BLUE();
                }
                else
                {
                    if(clock_mode==4)
                    {
                        char txd[] = "-- MCLK=DCOCLK DCOCLK=CALDCO_1MHZ MCLK_Divider=4 1MHz/4=250kHz\r\n";
                        UARTSendArray(txd, strlen(txd)+2);
                        RGB_0_PURPLE();
                        MCLK_DCO_250kHz();
                    }
                    else
                    {
                        if(clock_mode==5)
                        {
                            char txd[] = "-- MCLK=DCOCLK DCOCLK=CALDCO_16MHZ MCLK_Divider=8 16MHz/1=16MHz\r\n";
                            UARTSendArray(txd, strlen(txd)+2);
                            RGB_0_WHITE();
                            MCLK_DCO_16MHz();
                        }
                    }
                }
            }
        }
    }
    P1IFG &= ~BIT3;    // clear switch interrupt flag
}
