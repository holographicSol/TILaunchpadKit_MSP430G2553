#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <msp430.h>
#include <stdint.h>
#include <string.h>

int digitalPin = 5;
int analogPin = 6;
int digitalVal;       // digital readings
int v_input;          // analogue readings

//#define _BV(val) 1<<val // Bit shift macro

int previous_v_input;

#define HCENTERUL_OFF   0
#define HCENTERUL_ON    1

#define SSD1306_I2C_ADDRESS             0x3C

#define SSD1306_LCDWIDTH                128
#define SSD1306_LCDHEIGHT               64
#define SSD1306_128_64

#define SSD1306_SETCONTRAST             0x81
#define SSD1306_DISPLAYALLON_RESUME     0xA4
#define SSD1306_DISPLAYALLON            0xA5
#define SSD1306_NORMALDISPLAY           0xA6
#define SSD1306_INVERTDISPLAY           0xA7
#define SSD1306_DISPLAYOFF              0xAE
#define SSD1306_DISPLAYON               0xAF

#define SSD1306_SETDISPLAYOFFSET        0xD3
#define SSD1306_SETCOMPINS              0xDA

#define SSD1306_SETVCOMDETECT           0xDB

#define SSD1306_SETDISPLAYCLOCKDIV      0xD5
#define SSD1306_SETPRECHARGE            0xD9

#define SSD1306_SETMULTIPLEX            0xA8

#define SSD1306_SETLOWCOLUMN            0x00
#define SSD1306_SETHIGHCOLUMN           0x10

#define SSD1306_SETSTARTLINE            0x40

#define SSD1306_MEMORYMODE              0x20
#define SSD1306_COLUMNADDR              0x21
#define SSD1306_PAGEADDR                0x22

#define SSD1306_COMSCANINC              0xC0
#define SSD1306_COMSCANDEC              0xC8

#define SSD1306_SEGREMAP                0xA0

#define SSD1306_CHARGEPUMP              0x8D

#define SSD1306_EXTERNALVCC             0x1
#define SSD1306_SWITCHCAPVCC            0x2

// currently no scroll functionality, left for possible future use
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A


/* ====================================================================
 * SSD1306 OLED Prototype Definitions
 * ==================================================================== */
void ssd1306_init(void);
void ssd1306_command(unsigned char);
void ssd1306_clearDisplay(void);
void ssd1306_setPosition(uint8_t, uint8_t);
void ssd1306_printText(uint8_t, uint8_t, char *);
void ssd1306_printTextBlock(uint8_t, uint8_t, char *);
void ssd1306_printUI32(uint8_t, uint8_t, uint32_t, uint8_t);

uint8_t digits(uint32_t);
void ultoa(uint32_t, char *);
void reverse(char *);


/*
 * i2c.c
 */

unsigned char buffer[17];                                                     // buffer for data transmission to screen
unsigned char *PTxData;                                                       // Pointer to TX data
unsigned char TxByteCtr;                                                      // number of bytes to TX

#define SDA BIT7                                                        // i2c sda pin
#define SCL BIT6                                                        // i2c scl pin

void i2c_init(void) {
    P1SEL    |= SCL + SDA;                                              // Assign I2C pins to USCI_B0
    P1SEL2   |= SCL + SDA;                                              // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                                                // Enable SW reset
    UCB0CTL0  = UCMST + UCMODE_3 + UCSYNC;                              // I2C Master, synchronous mode
    UCB0CTL1  = UCSSEL_2 + UCSWRST;                                     // Use SMCLK, keep SW reset
    UCB0BR0   = 10;                                                     // fSCL = SMCLK/10 = ~100kHz with SMCLK = 1MHz
    UCB0BR1   = 0;
    UCB0CTL1 &= ~UCSWRST;                                               // Clear SW reset, resume operation
    IE2      |= UCB0TXIE;                                               // Enable TX interrupt

} // end i2c_init

void i2c_write(unsigned char slave_address, unsigned char *DataBuffer, unsigned char ByteCtr) {
    UCB0I2CSA = slave_address;

    PTxData = DataBuffer;
    TxByteCtr = ByteCtr;

    while (UCB0CTL1 & UCTXSTP);                                         // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;                                         // I2C TX, start condition
    while (UCB0CTL1 & UCTXSTP);                                         // Ensure stop condition got sent
    __bis_SR_register(CPUOFF + GIE);                                    // Enter LPM0 w/ interrupts
                                                                        // Remain in LPM0 until all data is TX'd
}

/* =============================================================================
 * The USCIAB0TX_ISR is structured such that it can be used to transmit any
 * number of bytes by pre-loading TXByteCtr with the byte count. Also, TXData
 * points to the next byte to transmit.
 * ============================================================================= */
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
    if (TxByteCtr) {                                                    // Check TX byte counter
        UCB0TXBUF = *PTxData++;                                         // Load TX buffer
        TxByteCtr--;                                                    // Decrement TX byte counter
    } else {
        UCB0CTL1 |= UCTXSTP;                                            // I2C stop condition
        IFG2 &= ~UCB0TXIFG;                                             // Clear USCI_B0 TX int flag
        __bic_SR_register_on_exit(CPUOFF);                              // Exit LPM0
  }
}

void ssd1306_init(void) {
    // SSD1306 init sequence
    ssd1306_command(SSD1306_DISPLAYOFF);                                // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);                        // 0xD5
    ssd1306_command(0x80);                                              // the suggested ratio 0x80

    ssd1306_command(SSD1306_SETMULTIPLEX);                              // 0xA8
    ssd1306_command(SSD1306_LCDHEIGHT - 1);

    ssd1306_command(SSD1306_SETDISPLAYOFFSET);                          // 0xD3
    ssd1306_command(0x0);                                               // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);                        // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                                // 0x8D
    ssd1306_command(0x14);                                              // generate high voltage from 3.3v line internally
    ssd1306_command(SSD1306_MEMORYMODE);                                // 0x20
    ssd1306_command(0x00);                                              // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);

    ssd1306_command(SSD1306_SETCOMPINS);                                // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                               // 0x81
    ssd1306_command(0xCF);

    ssd1306_command(SSD1306_SETPRECHARGE);                              // 0xd9
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);                             // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);                       // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                             // 0xA6

    ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

    ssd1306_command(SSD1306_DISPLAYON);                                 //--turn on oled panel
} // end ssd1306_init

void ssd1306_command(unsigned char command) {
    buffer[0] = 0x80;
    buffer[1] = command;

    i2c_write(SSD1306_I2C_ADDRESS, buffer, 2);
} // end ssd1306_command

void ssd1306_clearDisplay2(int x, int y) {

    ssd1306_setPosition(x, y);
    uint8_t icd;
    for (icd = 64; icd > 0; icd--) {                                          // count down for loops when possible for ULP
        uint8_t xcd;
        for(xcd = 32; xcd > 0; xcd--) {
            if (xcd == 1) {
                buffer[xcd-1] = 0x40;
            } else {
                buffer[xcd-1] = 0x0;
            }
        }

        i2c_write(SSD1306_I2C_ADDRESS, buffer, 17);
    }
} // end ssd1306_clearDisplay

void ssd1306_clearDisplay(void) {

    ssd1306_setPosition(0, 0);
    uint8_t icd;
    for (icd = 64; icd > 0; icd--) {                                          // count down for loops when possible for ULP
        uint8_t xcd;
        for(xcd = 32; xcd > 0; xcd--) {
            if (xcd == 1) {
                buffer[xcd-1] = 0x40;
            } else {
                buffer[xcd-1] = 0x0;
            }
        }

        i2c_write(SSD1306_I2C_ADDRESS, buffer, 17);
    }
} // end ssd1306_clearDisplay

void ssd1306_setPosition(uint8_t column, uint8_t page) {
    if (column > 128) {
        column = 0;                                                     // constrain column to upper limit
    }

    if (page > 8) {
        page = 0;                                                       // constrain page to upper limit
    }

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(column);                                            // Column start address (0 = reset)
    ssd1306_command(SSD1306_LCDWIDTH-1);                                // Column end address (127 = reset)

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(page);                                              // Page start address (0 = reset)
    ssd1306_command(7);                                                 // Page end address
} // end ssd1306_setPosition

// Define the function types you want to store
typedef void (*func_ptr)(int, int);

// Define a struct to act as a dictionary entry
typedef struct {
    char key[16];
    func_ptr value;
} dict_entry;

// Define a function to call the function associated with a key
volatile unsigned int i_char_function_find = 0;
volatile unsigned int j = 0;

void top5(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
}

void lrhl4(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void lhl(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
}

void rhl(int x, int y)
{
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void chl(int x, int y)
{
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  centre high
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  centre low
}

void tail4(int x, int y)
{
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void mid4(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
}

// Define a function to call the function associated with a key for each character in a string
void oledWrite(const dict_entry dict[], int size, char* str, int pos_x, int pos_y){
    for(i_char_function_find = 0; i_char_function_find < strlen(str); i_char_function_find++) {
        for(j = 0; j < size; j++) {
            if(dict[j].key[0] == str[i_char_function_find]) {
                dict[j].value(pos_x, pos_y);  // call associated function
                pos_x = pos_x + 7;           // increment pos_x by char width + 1
                break;
            }
        }
    }
}

void CSPACE(int x, int y)
{
}

void C0(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C1(int x, int y)
{
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+1);
    rhl(int x, int y);
}

void C2(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    mid4(x,y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    tail4(x,y);
}

void C3(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    rhl(int x, int y);
    mid4(x,y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C4(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void C5(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C6(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C7(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    rhl(int x, int y);
}

void C8(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C9(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    rhl(int x, int y);
}

void CA(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    lrhl4(int x, int y);
}

void CB(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid


    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CC(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    tail4(x,y);
}

void CD(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CE(int x, int y)
{
    top5(int x, int y);
    mid4(x,y);
    lhl(int x, int y);
    tail4(x,y);
}

void CF(int x, int y)
{
    top5(int x, int y);
    mid4(x,y);
    lhl(int x, int y);
}

void CG(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CH(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    lrhl4(int x, int y);
}

void CI(int x, int y)
{
    top5(int x, int y);
    chl(int x, int y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CJ(int x, int y)
{
    top5(int x, int y);
    rhl(int x, int y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CK(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  diagonal up
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  diagonal up
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // one pixel  diagonal up
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // one pixel  diagonal up
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  diagonal up
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  diagonal up
    lhl(int x, int y);
}

void CL(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    lhl(int x, int y);
    tail4(x,y);
}

void CM(int x, int y)
{
    lhl(int x, int y);
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    chl(int x, int y);
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    rhl(int x, int y);
}

void CN(int x, int y)
{
    lhl(int x, int y);
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  centre
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixels
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // vertical pixels
    rhl(int x, int y);
}

void CO(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CP(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void CQ(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CR(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
}

void CS(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CT(int x, int y)
{
    top5(int x, int y);
    chl(int x, int y);
}

void CU(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CV(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side

    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom

    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void CW(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    lrhl4(int x, int y);
    chl(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom

}

void CX(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  centre
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CY(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void CZ(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CEMARK(int x, int y)
{
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel top
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CTACK(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel
}

// Create your dictionary
const dict_entry dict[] = {
    {" ", CSPACE},
    {"!", CEMARK},
    {"-", CTACK},
    {"0", C0},
    {"1", C1},
    {"2", C2},
    {"3", C3},
    {"4", C4},
    {"5", C5},
    {"6", C6},
    {"7", C7},
    {"8", C8},
    {"9", C9},
    {"A", CA},
    {"B", CB},
    {"C", CC},
    {"D", CD},
    {"E", CE},
    {"F", CF},
    {"G", CG},
    {"H", CH},
    {"I", CI},
    {"J", CJ},
    {"K", CK},
    {"L", CL},
    {"M", CM},
    {"N", CN},
    {"O", CO},
    {"P", CP},
    {"Q", CQ},
    {"R", CR},
    {"S", CS},
    {"T", CT},
    {"U", CU},
    {"V", CV},
    {"W", CW},
    {"X", CX},
    {"Y", CY},
    {"Z", CZ},
};




void setup()
{
    WDTCTL = WDTPW | WDTHOLD;                   // stop watch dog timer
    pinMode(digitalPin,  INPUT);
    pinMode(analogPin, INPUT);
    Serial.begin(9600);
    i2c_init();                                 // initialise I2C to use with OLED
    ssd1306_init();                             // Initialise SSD1306 OLED
    ssd1306_clearDisplay();                     // Clear OLED display

}

void loop()
{

    // read the digital interface
    digitalVal = digitalRead(digitalPin);

    // read the analogue interface
    int v_input = analogRead(analogPin);

    int size = 0;

    // modes of operation (t=threshold(enable threshold function), a=analogue(enable analogue is zero function, verbose(enable verbosity at performance cost)))
    int t = 1;
    int t_n = 20;
    int a = 1;
    int a_n = 0;
    int verbose = 0;

    if(verbose == 1)
    {
        Serial.println("---------------------------");
        Serial.print("[DIGITAL VALUE] ");
        Serial.println(digitalVal);
        Serial.print("[RAW V_INPUT]   ");
        Serial.println(v_input);
        Serial.print("[RAW V_INPUT 2] ");
        Serial.println(previous_v_input);
        Serial.print("[RAW V_INPUT THRESH HIGH] ");
        Serial.println(previous_v_input+20);
        Serial.print("[RAW V_INPUT THRESH LOW]  ");
        Serial.println(previous_v_input-20);
    }

    // display
    oledWrite(dict, 39, "ANALOGUE", 0, 1);
    oledWrite(dict, 39, "DIGITAL", 0, 5);

    previous_v_input = v_input;

//    delay(2000);
}
