#include "Energia.h"

#line 1 "C:/Users/Benjamin/workspace_v9/Light_Cup_OLED/Light_Cup_OLED.ino"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <msp430.h>
#include <stdint.h>
#include <string.h>

void i2c_init(void);
void i2c_write(long int slave_address, long int *DataBuffer, unsigned char ByteCtr);
__interrupt void USCIAB0TX_ISR(void);
void ssd1306_command(unsigned char command);
void ssd1306_clearDisplay2(int x, int y);
void ssd1306_setPosition(uint8_t column, uint8_t page);
void top5(int x, int y);
void lrhl4(int x, int y);
void lhl(int x, int y);
void rhl(int x, int y);
void chl(int x, int y);
void tail4(int x, int y);
void mid4(int x, int y);
void oledWrite(const dict_entry dict[], int size, char* str, int pos_x, int pos_y);
void CSPACE(int x, int y);
void C0(int x, int y);
void C1(int x, int y);
void C2(int x, int y);
void C3(int x, int y);
void C4(int x, int y);
void C5(int x, int y);
void C6(int x, int y);
void C7(int x, int y);
void C8(int x, int y);
void C9(int x, int y);
void CA(int x, int y);
void CB(int x, int y);
void CC(int x, int y);
void CD(int x, int y);
void CE(int x, int y);
void CF(int x, int y);
void CG(int x, int y);
void CH(int x, int y);
void CI(int x, int y);
void CJ(int x, int y);
void CK(int x, int y);
void CL(int x, int y);
void CM(int x, int y);
void CN(int x, int y);
void CO(int x, int y);
void CP(int x, int y);
void CQ(int x, int y);
void CR(int x, int y);
void CS(int x, int y);
void CT(int x, int y);
void CU(int x, int y);
void CV(int x, int y);
void CW(int x, int y);
void CX(int x, int y);
void CY(int x, int y);
void CZ(int x, int y);
void CEMARK(int x, int y);
void CTACK(int x, int y);
void setup();
void loop();

#line 8
int digitalPin = 5;
int analogPin = 6;
int digitalVal;       
int v_input;          



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


#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A





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






unsigned char buffer[17];                                                     
unsigned char *PTxData;                                                       
unsigned char TxByteCtr;                                                      

#define SDA BIT7                                                        
#define SCL BIT6                                                        

void i2c_init(void) {
    P1SEL    |= SCL + SDA;                                              
    P1SEL2   |= SCL + SDA;                                              
    UCB0CTL1 |= UCSWRST;                                                
    UCB0CTL0  = UCMST + UCMODE_3 + UCSYNC;                              
    UCB0CTL1  = UCSSEL_2 + UCSWRST;                                     
    UCB0BR0   = 10;                                                     
    UCB0BR1   = 0;
    UCB0CTL1 &= ~UCSWRST;                                               
    IE2      |= UCB0TXIE;                                               

} 

void i2c_write(long int slave_address, long int *DataBuffer, unsigned char ByteCtr) {
    UCB0I2CSA = slave_address;

    PTxData = DataBuffer;
    TxByteCtr = ByteCtr;

    while (UCB0CTL1 & UCTXSTP);                                         
    UCB0CTL1 |= UCTR + UCTXSTT;                                         
    while (UCB0CTL1 & UCTXSTP);                                         
    __bis_SR_register(CPUOFF + GIE);                                    
                                                                        
}






#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
    if (TxByteCtr) {                                                    
        UCB0TXBUF = *PTxData++;                                         
        TxByteCtr--;                                                    
    } else {
        UCB0CTL1 |= UCTXSTP;                                            
        IFG2 &= ~UCB0TXIFG;                                             
        __bic_SR_register_on_exit(CPUOFF);                              
  }
}

void ssd1306_init(void) {
    
    ssd1306_command(SSD1306_DISPLAYOFF);                                
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);                        
    ssd1306_command(0x80);                                              

    ssd1306_command(SSD1306_SETMULTIPLEX);                              
    ssd1306_command(SSD1306_LCDHEIGHT - 1);

    ssd1306_command(SSD1306_SETDISPLAYOFFSET);                          
    ssd1306_command(0x0);                                               
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);                        
    ssd1306_command(SSD1306_CHARGEPUMP);                                
    ssd1306_command(0x14);                                              
    ssd1306_command(SSD1306_MEMORYMODE);                                
    ssd1306_command(0x00);                                              
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);

    ssd1306_command(SSD1306_SETCOMPINS);                                
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                               
    ssd1306_command(0xCF);

    ssd1306_command(SSD1306_SETPRECHARGE);                              
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);                             
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);                       
    ssd1306_command(SSD1306_NORMALDISPLAY);                             

    ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

    ssd1306_command(SSD1306_DISPLAYON);                                 
} 

void ssd1306_command(unsigned char command) {
    buffer[0] = 0x80;
    buffer[1] = command;

    i2c_write(SSD1306_I2C_ADDRESS, buffer, 2);
} 

void ssd1306_clearDisplay2(int x, int y) {

    ssd1306_setPosition(x, y);
    uint8_t icd;
    for (icd = 64; icd > 0; icd--) {                                          
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
} 

void ssd1306_clearDisplay(void) {

    ssd1306_setPosition(0, 0);
    uint8_t icd;
    for (icd = 64; icd > 0; icd--) {                                          
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
} 

void ssd1306_setPosition(uint8_t column, uint8_t page) {
    if (column > 128) {
        column = 0;                                                     
    }

    if (page > 8) {
        page = 0;                                                       
    }

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(column);                                            
    ssd1306_command(SSD1306_LCDWIDTH-1);                                

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(page);                                              
    ssd1306_command(7);                                                 
} 


typedef void (*func_ptr)(int, int);


typedef struct {
    char key[16];
    func_ptr value;
} dict_entry;


volatile unsigned int i_char_function_find = 0;
volatile unsigned int j = 0;

void top5(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void lrhl4(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void lhl(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void rhl(int x, int y)
{
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void chl(int x, int y)
{
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void tail4(int x, int y)
{
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void mid4(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
}


void oledWrite(const dict_entry dict[], int size, char* str, int pos_x, int pos_y){
    for(i_char_function_find = 0; i_char_function_find < strlen(str); i_char_function_find++) {
        for(j = 0; j < size; j++) {
            if(dict[j].key[0] == str[i_char_function_find]) {
                dict[j].value(pos_x, pos_y);  
                pos_x = pos_x + 7;           
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
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void C1(int x, int y)
{
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    rhl(int x, int y);
}

void C2(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    mid4(x,y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    tail4(x,y);
}

void C3(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    rhl(int x, int y);
    mid4(x,y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void C4(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void C5(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void C6(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
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
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void C9(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    rhl(int x, int y);
}

void CA(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lrhl4(int x, int y);
}

void CB(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    


    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
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
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
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
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CH(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lrhl4(int x, int y);
}

void CI(int x, int y)
{
    top5(int x, int y);
    chl(int x, int y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CJ(int x, int y)
{
    top5(int x, int y);
    rhl(int x, int y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CK(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lhl(int x, int y);
}

void CL(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lhl(int x, int y);
    tail4(x,y);
}

void CM(int x, int y)
{
    lhl(int x, int y);
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    chl(int x, int y);
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    rhl(int x, int y);
}

void CN(int x, int y)
{
    lhl(int x, int y);
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    rhl(int x, int y);
}

void CO(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CP(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void CQ(int x, int y)
{
    top5(int x, int y);
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CR(int x, int y)
{
    top5(int x, int y);
    lhl(int x, int y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CS(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CT(int x, int y)
{
    top5(int x, int y);
    chl(int x, int y);
}

void CU(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lrhl4(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CV(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    

    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    

    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void CW(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    lrhl4(int x, int y);
    chl(int x, int y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    

}

void CX(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CY(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
}

void CZ(int x, int y)
{
    top5(int x, int y);
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CEMARK(int x, int y)
{
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    
}

void CTACK(int x, int y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    
}


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
    WDTCTL = WDTPW | WDTHOLD;                   
    pinMode(digitalPin,  INPUT);
    pinMode(analogPin, INPUT);
    Serial.begin(9600);
    i2c_init();                                 
    ssd1306_init();                             
    ssd1306_clearDisplay();                     

}

void loop()
{

    
    digitalVal = digitalRead(digitalPin);

    
    int v_input = analogRead(analogPin);

    int size = 0;

    
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

    
    oledWrite(dict, 39, "ANALOGUE", 0, 1);
    oledWrite(dict, 39, "DIGITAL", 0, 5);

    previous_v_input = v_input;


}



