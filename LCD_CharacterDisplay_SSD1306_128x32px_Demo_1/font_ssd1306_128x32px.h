#include <stdio.h>
#include <string.h>
#include "msp430.h"
#include "SSD1306.h"
#include "i2c.h"

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

void top5(x, y)
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

void lrhl4(x, y)
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

void lhl(x, y)
{
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
}

void rhl(x, y)
{
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    ssd1306_setPosition(x+4, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
}

void chl(x, y)
{
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  centre high
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  centre low
}

void tail4(x, y)
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

void mid4(x, y)
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
void oledWrite(dict_entry dict[], int size, char* str, int pos_x, int pos_y){
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
    top5(x, y);
    lrhl4(x, y);
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
    rhl(x, y);
}

void C2(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+4, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  right side
    mid4(x,y);
    ssd1306_setPosition(x+0, y+2);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    tail4(x,y);
}

void C3(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+4, y+1);
    rhl(x, y);
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
    top5(x, y);
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
    top5(x, y);
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
    top5(x, y);
    ssd1306_setPosition(x+4, y+1);
    rhl(x, y);
}

void C8(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    lrhl4(x, y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void C9(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+0, y+1);
    i2c_write(0x3C, 0xFFF00, 2);    // vertical pixel column x5  left side
    rhl(x, y);
}

void CA(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    lrhl4(x, y);
}

void CB(int x, int y)
{
    top5(x, y);
    ssd1306_setPosition(x+1, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+2, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid
    ssd1306_setPosition(x+3, y+1);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  mid


    lrhl4(x, y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CC(int x, int y)
{
    top5(x, y);
    lhl(x, y);
    tail4(x,y);
}

void CD(int x, int y)
{
    top5(x, y);
    lrhl4(x, y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CE(int x, int y)
{
    top5(x, y);
    mid4(x,y);
    lhl(x, y);
    tail4(x,y);
}

void CF(int x, int y)
{
    top5(x, y);
    mid4(x,y);
    lhl(x, y);
}

void CG(int x, int y)
{
    top5(x, y);
    lhl(x, y);
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
    lrhl4(x, y);
}

void CI(int x, int y)
{
    top5(x, y);
    chl(x, y);
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
    top5(x, y);
    rhl(x, y);
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
    lhl(x, y);
}

void CL(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    lhl(x, y);
    tail4(x,y);
}

void CM(int x, int y)
{
    lhl(x, y);
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+1, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+2, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    chl(x, y);
    ssd1306_setPosition(x+3, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    rhl(x, y);
}

void CN(int x, int y)
{
    lhl(x, y);
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
    rhl(x, y);
}

void CO(int x, int y)
{
    top5(x, y);
    lrhl4(x, y);
    ssd1306_setPosition(x+1, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+2, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
    ssd1306_setPosition(x+3, y+2);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  bottom
}

void CP(int x, int y)
{
    top5(x, y);
    lhl(x, y);
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
    top5(x, y);
    lrhl4(x, y);
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
    top5(x, y);
    lhl(x, y);
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
    top5(x, y);
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
    top5(x, y);
    chl(x, y);
}

void CU(int x, int y)
{
    ssd1306_setPosition(x+0, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    ssd1306_setPosition(x+4, y+0);
    i2c_write(0x3C, 0xFFFFF, 2);    // one pixel  top
    lrhl4(x, y);
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
    lrhl4(x, y);
    chl(x, y);
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
    top5(x, y);
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
};
