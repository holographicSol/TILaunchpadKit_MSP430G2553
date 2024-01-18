#include <rxdexec.h>

#define                UART_RXD BIT1
#define                UART_TXD BIT2
#define                MAX_BUFFER_SIZE 56
char                   UARTRxData[MAX_BUFFER_SIZE];
volatile char          UARTRxFlag = 0;
char                   UARTRxChar;
volatile unsigned int  buffer_index = 0;

void UARTConfigure(void){
    P1SEL = UART_RXD + UART_TXD ;       // P1.1 = RXD, P1.2=TXD
    P1SEL2 = UART_RXD + UART_TXD ;      // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;               // Use SMCLK
    UCA0BR0 = 104;                      // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
    UCA0BR1 = 0;                        // Set baud rate to 9600 with 1MHz clock
    UCA0MCTL = UCBRS0;                  // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;               // Initialise USCI state machine
    IE2 |= UCA0RXIE;                    // Enable USCI_A0 RX interrupt
}

void printtxd(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}

void RESET_UCA0RXBUF(void){memset(UARTRxData, 0, sizeof UARTRxData); buffer_index = 0;}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    while(!(IFG2 & UCA0RXIFG));
    UARTRxChar = UCA0RXBUF;
    if(UARTRxChar == '\n') {printtxd("[MSP_RXD] Received: "); printtxd(UARTRxData); printtxd("\n");
    RXD_Exec(UARTRxData);
    RESET_UCA0RXBUF();}
    else if (buffer_index < MAX_BUFFER_SIZE - 1) {UARTRxData[buffer_index++] = UARTRxChar;}
    else if (buffer_index == MAX_BUFFER_SIZE - 1){printtxd("[MSP_RXD] [OVER_MAX] Max buffer size is 56 bytes.\n");
        RESET_UCA0RXBUF();
        __delay_cycles(100000);
    }
    IFG2 &= ~UCA0RXIFG;
}
