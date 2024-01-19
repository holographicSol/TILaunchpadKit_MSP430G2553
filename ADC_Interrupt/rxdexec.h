#include <string.h>
#include <charcon.h>

void printtxd(char *str);

char *com0 = "EXEC0";
char *com1 = "VAR0";
int Example = 0;

void RXD_Exec(char *received_chars)
{
    // example function 1: do something
    if(memcmp(com0, received_chars, strlen(received_chars)+1) == 0)
    {printtxd("[MSP] Running command EXEC0\n");}

    /* example function 2: variably modify specified value.
     * concise when a variable may have a range of accepted values, set the value precisely without
     * having a huge command list per expected value and or iteration. like for volume in range of 0-100 for one example.
     */
    else if(memcmp(com1, received_chars, strlen(com1)) == 0)
    {
        slice_inplace(received_chars, 5, strlen(received_chars)-1);
        int EXAMPLE_VAR = (int)stoi(received_chars); // convert to integer and set variable
        char b4[50];
        ifstring(EXAMPLE_VAR, b4, sizeof(b4), INT);  // convert to string, TXD the updated value
        printtxd("[MSP] [Setting Internal Integer] "); printtxd(b4); printtxd("\n");
        free(b4);
    }

    else{printtxd("[MSP] Received unknown command\n");}
};
