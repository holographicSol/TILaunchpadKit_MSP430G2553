/*
 * Hello Clock with Interrupts!
 *
 * This program provides 3 clock frequency modes accessible via the push button
 * Pushbutton cycles clock frequency mode via use of interrupts.
 *
 * Created by Benjamin Jack Cullen
 *
 */

#include <msp430.h> 

#define SW BIT3
#define LEDR BIT6
#define LEDG BIT0
int clock_mode = 0;

void register_settings_for_VLO()
{
   volatile unsigned long i;
   BCSCTL3 |= LFXT1S_2;                 // LFXT1 = VLO
   do{
       IFG1 &= ~OFIFG;                  // Clear oscillator fault flag
       for (i = 50000; i; i--);         // Delay
     } while (IFG1 & OFIFG);            // Test OSC fault flag
   BCSCTL2 |= SELM_3;                   // MCLK = VLO
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop Watchdog timer

    volatile unsigned long a;
    for(a = 0; a<20000; a++);        // startup delay to allow clocks to initialise

    // setup for serial transmission
    UARTConfigure();

    // setup LEDs
    P1DIR |= LEDR;      // set led pin as output
    P1DIR |= LEDG;      // set led pin as output

    // setup RGB
    P2DIR |= BIT1;                  // configure P2.1 as output R
    P2DIR |= BIT3;                  // configure P2.3 as output G
    P2DIR |= BIT5;                  // configure P2.5 as output B
    P2OUT &=~ (BIT8 + BIT1);        // clear P2.1  (RGB red off)
    P2OUT &=~ (BIT8 + BIT3);        // clear P2.3  (RGB green off)
    P2OUT &=~ (BIT8 + BIT5);        // clear P2.5  (RGB blue off)

    // setup switch
    P1DIR &= ~SW;       // set switch pin as input
    P1REN |= SW;        // enable resistor for switch pin
    P1OUT |= SW;        // select pull up for switch pin
    P1IES &= ~SW;       // select interrupt on rising edge
    P1IE |= SW;         // enable interrupt on switch pin

    __bis_SR_register(GIE);             // enable CPU interrupt

    volatile unsigned long n;
    register_settings_for_VLO();        //Register settings for VLO

    // set initial clock frequency and display clock frequency mode using RGB LED
    UARTSendArray("-- master clock signal using VLO as clock source at frequency: 1.5kHz (low power mode)", 86);
    UARTSendArray("\r\n", 2);
    P2OUT ^= BIT1;                  // toggle P2.1 (RGB red on: displays clock mode 0)
    P2OUT &=~ (BIT8 + BIT3);        // clear P2.3  (RGB green off)
    P2OUT &=~ (BIT8 + BIT5);        // clear P2.5  (RGB blue off)
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset VLO divider
    BCSCTL2 |= (BIT5 + BIT4);       // VLO = 12kHz/8 = 1.5kHz
    clock_mode = 1;                 // iterate clock mode

    while(1) {
        P1OUT ^= LEDG;                  // toggle led at approximately delay speed
        for (n = 100; n > 0; n--);      // very short delay
    }

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    // display clock frequency mode conveyed in ascending colour frequency (red->green->blue)
    if(clock_mode==0)
    {
        UARTSendArray("-- master clock signal using VLO as clock source at frequency: 1.5kHz (low power mode)", 86);
        UARTSendArray("\r\n", 2);
        P2OUT ^= BIT1;                  // toggle P2.1 (RGB red on: displays clock mode 0)
        P2OUT &=~ (BIT8 + BIT3);        // clear P2.3  (RGB green off)
        P2OUT &=~ (BIT8 + BIT5);        // clear P2.5  (RGB blue off)
        BCSCTL2 &=~ (BIT5 + BIT4);      // reset VLO divider
        BCSCTL2 |= (BIT5 + BIT4);       // VLO = 12kHz/8 = 1.5kHz
        clock_mode = 1;                 // iterate clock mode
    }
    else
    {
        if(clock_mode==1)
        {
            UARTSendArray("-- master clock signal using VLO as clock source at frequency: 3kHz", 69);
            UARTSendArray("\r\n", 2);
            P2OUT &=~ (BIT8 + BIT1);        // clear P2.1  (RGB red)
            P2OUT ^= BIT3;                  // toggle P2.3 (RGB green: displays clock mode 1)
            P2OUT &=~ (BIT8 + BIT5);        // clear P2.5  (RGB blue)
            BCSCTL2 &=~ (BIT5 + BIT4);      // reset VLO divider
            BCSCTL2 |= (BIT4);              // VLO = 12kHz/4 = 3kHz
            clock_mode = 2;                 // iterate clock mode
        }
        else
        {
            if(clock_mode==2)
            {
                UARTSendArray("-- master clock signal using VLO as clock source at frequency: 12kHz", 69);
                UARTSendArray("\r\n", 2);
                P2OUT &=~ (BIT8 + BIT1);        // clear P2.1  (RGB red)
                P2OUT &=~ (BIT8 + BIT3);        // clear P2.3  (RGB green off)
                P2OUT ^= BIT5;                  // toggle P2.5 (RGB blue: displays clock mode 2)
                BCSCTL2 &=~ (BIT5 + BIT4);      // VLO = 12kHz/1 = 12kHz
                clock_mode = 0;                 // reset clock mode
            }
        }
    }
    P1IFG &= ~SW;    // clear switch interrupt flag
}
