/* Configure UART to transmit and receive characters over the serial port at 9600 baud
 * This uses pins P1.1 and P1.2. On the Launchpad these are connected to a USB bridge chip to allow
 * for communication between the board and a Computer using a USB connection.
 * The code transmits "Hello world" and then waits to receive strings terminated with a carriage return.
 * The received strings are echoed back.
 * These following UART functions were not written by myself: Benjamin Jack Cullen
*/

#include <msp430.h>

void UARTConfigure(void);

volatile char UARTRxData[20];
volatile char UARTRxFlag = 0;
volatile char i=0;

void UARTConfigure(void){
    P1SEL = BIT1 + BIT2 ;               // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;              // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;               // Use SMCLK
    UCA0BR0 = 104;                      // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
    UCA0BR1 = 0;                        // Set baud rate to 9600 with 1MHz clock
    UCA0MCTL = UCBRS0;                  // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;               // Initialise USCI state machine
    IE2 |= UCA0RXIE;                    // Enable USCI_A0 RX interrupt
}

void UARTSendChar(unsigned char character)
{
    while(!(IFG2 & UCA0TXIFG));         // Wait for TX buffer to be ready for new data
    UCA0TXBUF = character;              //Write the character at the location specified by the pointer and increment pointer
}

void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength)
{
    while(ArrayLength--)
    {                                   // Loop until StringLength == 0 and post decrement
        while(!(IFG2 & UCA0TXIFG));     // Wait for TX buffer to be ready for new data
        UCA0TXBUF = *TxArray++;         // Write the character at the location specified by the pointer and increment pointer
    }
}

void UARTSendString(char *TxArray)
{
    while(*TxArray)
    {
        while(!(IFG2 & UCA0TXIFG));     // Wait for TX buffer to be ready for new data
        UCA0TXBUF = *TxArray++;         // Write the character at the location specified by the pointer and increment pointer
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    UARTRxData[i] = UCA0RXBUF;          //Read USART data register
    if(UARTRxData[i++]=='\r')
    {
        UARTRxFlag=--i;                 //Set String received flag to length of received string
        i=0;                            //Reset buffer index
        __bic_SR_register_on_exit(CPUOFF);
    }
}
