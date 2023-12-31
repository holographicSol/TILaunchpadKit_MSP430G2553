/*
 * Hello Interrupt!
 * This program demonstrates at least several things:
 * 1: How to link an interrupt subroutine to a source of interrupt.
 * 2: By using interrupts instead of polling, we can jump to and from code blocks
 *    very quickly.
 * The MSP430G2553 has one CPU so tasks must be ran one after the other however
 * by using various techniques like interrupts, we can give the impression that
 * the system is multitasking:
 *      1: Notice the Green LED blink speed.
 *      2: Notice how the Green LED blink speed appears completely undisturbed when
 *         toggling the Red LED on/off using a push button interrupt.
 *
 */

#include <msp430.h> 

#define SW BIT3
#define LEDR BIT6
#define LEDG BIT0

/* main.c */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= LEDR;      // set led pin as output
	P1DIR |= LEDG;      // set led pin as output
	P1DIR &= ~SW;       // set switch pin as input
	P1REN |= SW;        // enable resistor for switch pin
	P1OUT |= SW;        // select pull up for switch pin

	P1IES &= ~SW;       // select interrupt on rising edge
	P1IE |= SW;         // enable interrupt on switch pin

	__bis_SR_register(GIE);             // enable CPU interrupt

	while(1) {
	    volatile unsigned long i;
	    for(i = 0; i<1000; i++);        // very short delay
	    P1OUT ^= LEDG;                  // toggle led at approximately delay speed
	}

}

/* entry point for switch interrupt */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG & SW)                      // if switch is pressed
    {
        P1OUT ^= LEDR;                  // toggle led (demo interrupt function)
        volatile unsigned long i;
        P1IFG &= ~SW;                   // clear switch interrupt flag
    }
    }
