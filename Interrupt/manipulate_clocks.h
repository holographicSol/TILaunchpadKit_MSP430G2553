// Need oscilloscope to verify this

#include <msp430.h>

extern int clock_mode;

void register_settings_for_VLO()
{
   volatile unsigned long v;
   BCSCTL3 |= LFXT1S_2;             // BCSCTL3 Register select LFXTCLK or VLOCLK
   do{
       IFG1 &= ~OFIFG;              // Clear oscillator fault flag
       for (v = 50000; v; v--);     // Delay
     } while (IFG1 & OFIFG);        // Test OSC fault flag
   BCSCTL2 |= SELM_3;               // BCSCTL2 Register select master clock as LFXT1CLK or VLOCLK
}

void MCLK_VLO_1_5kHz()
{
    clock_mode = 1;                 // Iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL3 |= LFXT1S_2;            // BCSCTL3 Register select LFXT1CLK or VLOCLK
    BCSCTL2 |= SELM_3;              // BCSCTL2 Register select master clock as LFXT1CLK or VLOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 |= (BIT5 + BIT4);       // BCSCTL2 Register set MCLK Divider = 12kHz/8 = 1.5kHz
}

void MCLK_VLO_3kHz()
{
    clock_mode = 2;                 // Iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL3 |= LFXT1S_2;            // BCSCTL3 Register select LFXT1CLK or VLOCLK
    BCSCTL2 |= SELM_3;              // BCSCTL2 Register select master clock as LFXT1CLK or VLOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 |= (BIT4);              // BCSCTL2 Register set MCLK Divider = 12kHz/4 = 3kHz
}

void MCLK_VLO_12kHz()
{
    clock_mode = 3;                 // Iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL3 |= LFXT1S_2;            // BCSCTL3 Register select LFXT1CLK or VLOCLK
    BCSCTL2 |= SELM_3;              // BCSCTL2 Register select master clock as LFXT1CLK or VLOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register set MCLK Divider = 12kHz/1 = 12kHz
}

void MCLK_DCO_125kHz()
{
    clock_mode = 4;                 // Iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL2 = SELM0;                // BCSCTL2 Register select master clock as DCOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 |= (BIT5 + BIT4);       // BCSCTL2 Register set MCLK Divider = 1MHz/8 = 125kHz
}

void MCLK_DCO_250kHz()
{
    clock_mode = 5;                 // Iterate clock mode
    DCOCTL = CALDCO_1MHZ;           // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL2 = SELM0;                // BCSCTL2 Register select master clock as DCOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 |= (BIT4);              // BCSCTL2 Register set MCLK Divider = 1MHz/4 = 250kHz
}

void MCLK_DCO_16MHz()
{
    clock_mode = 0;                 // Iterate clock mode
    DCOCTL = CALDCO_16MHZ;          // DCOCTL Register calibrate DCOCLK frequency
    BCSCTL2 = SELM0;                // BCSCTL2 Register select master clock as DCOCLK
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register reset MCLK Divider
    BCSCTL2 &=~ (BIT5 + BIT4);      // BCSCTL2 Register set MCLK Divider = 16MHz/1 = 16MHz
}
