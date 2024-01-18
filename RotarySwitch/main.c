#include <msp430.h>
#include "rotary-encoder.h"

#define LED1 BIT0

void stepCCW(){
	P2OUT ^= BIT1;
}
void stepCW(){
	P2OUT ^= BIT1;

}

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	P2DIR |= BIT1;
    P2OUT &= ~(BIT1);

	encoderInit();

	__bis_SR_register(GIE);             // enable CPU interrupt

}
