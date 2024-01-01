/*
 * Hello Clock!
 *
 * The RGB LED will be used to display the clock frequency mode:
 *      red:   VLO = 12kHz/8 = 1.5kHz
 *      green: VLO = 12kHz/4 = 3kHz
 *      blue:  VLO = 12kHz/1 = 12kHz
 * The Green LED will blink in a way which broadly conveys the slower/higher
 * clock speed.
 * This program also demonstrates how implementing interrupts would be more reliable
 * than using delays to debounce.
 *
 * The pushbutton changes the mode.
 *
 */

#include <msp430.h> 

#define LED BIT0                        // Green Led Pin

#define SW BIT3                         // Push Button

// 0 = 1.5Khz
// 1 = 3Khz
// 2 = 12Khz
int clock_mode = 0;
int pressed = 0;

volatile unsigned int i;                // Volatile to prevent removal

/**
 * @brief
 * Function to take input from 3 switches and change CPU clock accordingly
 **/
void switch_input()
{

    if(!(P1IN & SW) & clock_mode==0)     // If SW is Pressed
    {

        clock_mode = 1;

        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW));            // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        P2OUT ^= BIT1;                  // toggle P2.1
        P2OUT &=~ (BIT8 + BIT3);        // clear P2.3
        P2OUT &=~ (BIT8 + BIT5);        // clear P2.5

        BCSCTL2 &=~ (BIT5 + BIT4);      //Reset VLO divider
        BCSCTL2 |= (BIT5 + BIT4);       //VLO = 12kHz/8 = 1.5kHz
    }


    if(!(P1IN & SW) & clock_mode==1)     // If SW is Pressed
    {
        clock_mode = 2;

        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW));            // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        P2OUT &=~ (BIT8 + BIT1);        // clear P2.1
        P2OUT ^= BIT3;                  // toggle P2.3
        P2OUT &=~ (BIT8 + BIT5);        // clear P2.5

        BCSCTL2 &=~ (BIT5 + BIT4);      //Reset VLO divider
        BCSCTL2 |= (BIT4);              //VLO = 12kHz/4 = 3kHz
    }

    if(!(P1IN & SW) & clock_mode==2)     // If SW is Pressed
    {
        clock_mode = 0;

        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW));            // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        P2OUT &=~ (BIT8 + BIT1);        // clear P2.1
        P2OUT &=~ (BIT8 + BIT3);        // clear P2.3
        P2OUT ^= BIT5;                  // toggle P2.5

        BCSCTL2 &=~ (BIT5 + BIT4);      //VLO = 12kHz/1 = 12kHz

    }

}

/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= LED;                       //P1.7 is set as Output
    P1DIR &=~ SW;        //P1.3
}


/**
 * @brief
 * These settings are w.r.t enabling VLO  on Lunch Box
 **/
void register_settings_for_VLO()
{
   BCSCTL3 |= LFXT1S_2;                 // LFXT1 = VLO
   do{
       IFG1 &= ~OFIFG;                  // Clear oscillator fault flag
       for (i = 50000; i; i--);         // Delay
     } while (IFG1 & OFIFG);            // Test osc fault flag

   BCSCTL2 |= SELM_3;                   // MCLK = VLO
}

/**
 * main.c
 */
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    register_settings_for_VLO();        //Register settings for VLO
    register_settings_for_GPIO();       //Register settings for GPIO

    P2DIR |= BIT1;                  // configure P2.1 as output R
    P2DIR |= BIT3;                  // configure P2.3 as output G
    P2DIR |= BIT5;                  // configure P2.5 as output B

    while(1)
    {
        switch_input();                 //Switch Input

        P1OUT ^= LED;                   //LED Toggle
        for (i = 100; i > 0; i--);

    }
}
