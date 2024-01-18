/*
 * ssd1306.c
 */

#include "ssd1306.h"
#include <msp430.h>
#include <stdint.h>
#include "i2c.h"

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

