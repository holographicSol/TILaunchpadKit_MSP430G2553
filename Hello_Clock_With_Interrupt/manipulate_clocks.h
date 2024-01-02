#include <msp430.h>

extern int clock_mode;

void register_settings_for_VLO()
{
   volatile unsigned long v;
   BCSCTL3 |= LFXT1S_2;             // LFXT1 = VLO
   do{
       IFG1 &= ~OFIFG;              // Clear oscillator fault flag
       for (v = 50000; v; v--);     // Delay
     } while (IFG1 & OFIFG);        // Test OSC fault flag
   BCSCTL2 |= SELM_3;               // MCLK = VLOCLK
}

void MCLK_VLO_1_5kHz()
{
    clock_mode = 1;                 // iterate clock mode
    BCSCTL3 |= LFXT1S_2;            // LFXT1 = VLOLCK 0.4-1MHz
    BCSCTL2 |= SELM_3;              // switch to VLOLCK as clock source for MCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset MCLK Divider
    BCSCTL2 |= (BIT5 + BIT4);       // MCLK Divider = 12kHz/8 = 1.5kHz
}

void MCLK_VLO_3kHz()
{
    clock_mode = 2;                 // iterate clock mode
    BCSCTL3 |= LFXT1S_2;            // LFXT1 = VLOLCK 0.4-1MHz
    BCSCTL2 |= SELM_3;              // switch to VLOLCK as clock source for MCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset MCLK Divider
    BCSCTL2 |= (BIT4);              // MCLK Divider = 12kHz/4 = 3kHz
}

void MCLK_VLO_12kHz()
{
    clock_mode = 3;                 // iterate clock mode
    BCSCTL3 |= LFXT1S_2;            // LFXT1 = VLOLCK 0.4-1MHz
    BCSCTL2 |= SELM_3;              // switch to VLOLCK as clock source for MCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset MCLK Divider
    BCSCTL2 &=~ (BIT5 + BIT4);      // MCLK Divider = 12kHz/1 = 12kHz
}

void MCLK_DCO_125kHz()
{
    clock_mode = 4;                 // iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // calibrate DCOCLK frequency
    BCSCTL2 = SELM0;                // switch to DCOCLK as clock source for MCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset MCLK divider
    BCSCTL2 |= (BIT5 + BIT4);       // MCLK Divider = 1MHz/8 = 125kHz
}

void MCLK_DCO_16MHz()
{
    clock_mode = 0;                 // iterate clock mode
    DCOCTL = CALDCO_16MHZ;          // calibrate DCOCLK frequency
    BCSCTL2 = SELM0;                // switch to DCOCLK as clock source for MCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // reset MCLK divider
    BCSCTL2 &=~ (BIT5 + BIT4);      // MCLK Divider = 16MHz/1 = 16MHz
}
