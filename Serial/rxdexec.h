#include <msp430.h>

volatile char *com0 = "EXEC0";

void RXD_Exec(char *received_chars)
{
    if(memcmp(com0, received_chars, strlen(received_chars)+1) == 0)
    {printtxd("[MSP] Running command EXEC0\n");}

    else{printtxd("[MSP] Received unknown command\n");}
};
