#include "rotary-encoder.h"

void encoderInit(){

	P2OUT |= (ENCODER_A+ENCODER_B);	//enable pull-up resistor
	P2REN |= ENCODER_A+ENCODER_B;	//enable pull-up resistor
	P2IFG &= ~ENCODER_A;			//clear interrupt flag
	P2IE |= ENCODER_A;				//enable interrupt for encoder

	__enable_interrupt();
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	if(P2IN & ENCODER_B){stepCCW();
	}
	else{stepCW();}

	P2IFG &= ~ENCODER_A;	//clear interupt flag
}
