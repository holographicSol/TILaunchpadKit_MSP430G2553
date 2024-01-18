#include "Energia.h"

#line 1 "C:/Users/Benjamin/workspace_v9/Photoresistor/Photoresistor.ino"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TM1637.h"
#include <msp430.h>

void
_delay_us(const unsigned long us);
void
_delay_ms(const int ms);
void
TM1637_init(const int enable, const int brightness);
void
TM1637_enable(const int value);
void
TM1637_set_brightness(const int value);
void
TM1637_display_segments(const int position, const int segments);
void
TM1637_display_digit(const int position, const int digit);
void
TM1637_display_alpha(const int position, const int digit);
void
TM1637_display_colon(const int value);
void
TM1637_clear(void);
void
TM1637_send_config(const int enable, const int brightness);
void
TM1637_send_command(const int value);
void
TM1637_start(void);
void
TM1637_stop(void);
int
TM1637_write_byte(int value);
int* intToArray(int num, int* size);
void setup();
void loop();

#line 7
int digitalPin = 18;  
int analogPin = 6;    
int digitalVal;       
int v_input;          

#define _BV(val) 1<<val 

#define TM1637_CMD_SET_DATA         0x40
#define TM1637_CMD_SET_ADDR         0xC0
#define TM1637_CMD_SET_DSIPLAY      0x80


#define TM1637_SET_DATA_WRITE       0x00 
#define TM1637_SET_DATA_READ        0x02 
#define TM1637_SET_DATA_A_ADDR      0x00 
#define TM1637_SET_DATA_F_ADDR      0x04 
#define TM1637_SET_DATA_M_NORM      0x00 
#define TM1637_SET_DATA_M_TEST      0x10 


#define TM1637_SET_DISPLAY_OFF      0x00 
#define TM1637_SET_DISPLAY_ON       0x08 

#define TM1637_DIO_HIGH()       (P1OUT |= _BV(TM1637_DIO_PIN))  
#define TM1637_DIO_LOW()        (P1OUT &= ~_BV(TM1637_DIO_PIN))
#define TM1637_DIO_OUTPUT()     (P1DIR |= _BV(TM1637_DIO_PIN)) 
#define TM1637_DIO_INPUT()      (P1DIR &= ~_BV(TM1637_DIO_PIN))
#define TM1637_DIO_READ()       (((P1IN & _BV(TM1637_DIO_PIN)) > 0) ? 1 : 0) 
#define TM1637_CLK_HIGH()       (P1OUT |= _BV(TM1637_CLK_PIN)) 
#define TM1637_CLK_LOW()        (P1OUT &= ~_BV(TM1637_CLK_PIN))

 #define _BV(val) 1<<val 

static int _config = TM1637_SET_DISPLAY_ON | TM1637_BRIGHTNESS_MAX;
static int _config_off = TM1637_SET_DISPLAY_OFF | TM1637_BRIGHTNESS_MIN;
static int _segments = 0xff;

int previous_v_input;

static const int _digit_addr[] =
{
     0x3F, 
     0x06, 
     0x5B, 
     0x4F, 
     0x66, 
     0x6D, 
     0x7D, 
     0x07, 
     0x7F, 
     0x6F, 
};

void
_delay_us(const unsigned long us)
{
    unsigned long cycles = us * 16;
    while (0 < cycles)
    {
      __delay_cycles(1);
      --cycles;
    }
}

void
_delay_ms(const int ms)
{
    unsigned long us = ms * 1000;
    while (0 < us)
    {
        _delay_us(1);
        --us;
    }
}

void
TM1637_init(const int enable, const int brightness)
{
    P1SEL &= ~(_BV(TM1637_DIO_PIN)|_BV(TM1637_CLK_PIN)); 
    P1DIR |= (_BV(TM1637_DIO_PIN)|_BV(TM1637_CLK_PIN)); 
    P1OUT &= ~(_BV(TM1637_DIO_PIN)|_BV(TM1637_CLK_PIN)); 
    TM1637_send_config(enable, brightness);
}

void
TM1637_enable(const int value)
{

    TM1637_send_config(value, _config & TM1637_BRIGHTNESS_MAX);
}

void
TM1637_set_brightness(const int value)
{
    TM1637_send_config(_config & TM1637_SET_DISPLAY_ON,
        value & TM1637_BRIGHTNESS_MAX);
}

void
TM1637_display_segments(const int position, const int segments)
{

    TM1637_send_command(TM1637_CMD_SET_DATA | TM1637_SET_DATA_F_ADDR);
    TM1637_start();
    TM1637_write_byte(TM1637_CMD_SET_ADDR | (position & (TM1637_POSITION_MAX - 1)));
    TM1637_write_byte(segments);
    TM1637_stop();
}

void
TM1637_display_digit(const int position, const int digit)
{
    int segments = (digit < 10 ? _digit2segments[digit] : 0x00); 

    if (position == 0x01) {
        segments = segments | (_segments & 0x80);
        _segments = segments;
    }
    TM1637_display_segments(position, segments);
}

void
TM1637_display_alpha(const int position, const int digit)
{
    int segments = (digit < 10 ? _alpha2segments[digit] : 0x00); 

    if (position == 0x01) {
        segments = segments | (_segments & 0x80);
        _segments = segments;
    }
    TM1637_display_segments(position, segments);
}

void
TM1637_display_colon(const int value)
{

    if (value) {
        _segments |= 0x80;
    } else {
        _segments &= ~0x80;
    }
    TM1637_display_segments(0x01, _segments);
}

void
TM1637_clear(void)
{
    int i;

    for (i = 0; i < TM1637_POSITION_MAX; ++i) {
        TM1637_display_segments(i, 0x00);
    }
}


void
TM1637_send_config(const int enable, const int brightness)
{

    _config = (enable ? TM1637_SET_DISPLAY_ON : TM1637_SET_DISPLAY_OFF) |
        (brightness > TM1637_BRIGHTNESS_MAX ? TM1637_BRIGHTNESS_MAX : brightness);

    TM1637_send_command(TM1637_CMD_SET_DSIPLAY | _config);
}

void
TM1637_send_command(const int value)
{

    TM1637_start();
    TM1637_write_byte(value);
    TM1637_stop();
}

void
TM1637_start(void)
{

    TM1637_DIO_HIGH();
    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);
    TM1637_DIO_LOW();
}

void
TM1637_stop(void)
{

    TM1637_CLK_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_HIGH();
}

int
TM1637_write_byte(int value)
{
    int i, ack;

    for (i = 0; i < 8; ++i, value >>= 1) {
        TM1637_CLK_LOW();
        _delay_us(TM1637_DELAY_US);

        if (value & 0x01) {
            TM1637_DIO_HIGH();
        } else {
            TM1637_DIO_LOW();
        }

        TM1637_CLK_HIGH();
        _delay_us(TM1637_DELAY_US);
    }

    TM1637_CLK_LOW();
    TM1637_DIO_INPUT();
    TM1637_DIO_HIGH();
    _delay_us(TM1637_DELAY_US);

    ack = TM1637_DIO_READ();
    if (ack) {
        TM1637_DIO_OUTPUT();
        TM1637_DIO_LOW();
    }
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH();
    _delay_us(TM1637_DELAY_US);

    TM1637_CLK_LOW();
    _delay_us(TM1637_DELAY_US);

    TM1637_DIO_OUTPUT();

    return ack;
}

int* intToArray(int num, int* size) {
    int temp = num;
    *size = 0;

    
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;  
    }

    
    while (temp != 0) {
        temp /= 10;
        (*size)++;
    }

    int* arr = (int*)malloc(*size * sizeof(int));
    for (int i = *size - 1; i >= 0; i--) {
        arr[i] = num % 10;
        num /= 10;
    }








    return arr;
}

void setup()
{
    TM1637_clear();
    TM1637_init(1, 7);
    TM1637_set_brightness(7);
    TM1637_display_colon(0);
    TM1637_display_segments(4, 0x40);
    TM1637_display_segments(1, 0x40);
    TM1637_display_segments(2, 0x40);
    TM1637_display_segments(3, 0x40);

    delay(2000);

    pinMode(digitalPin,  INPUT);
    pinMode(analogPin, INPUT);
    Serial.begin(9600);

    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);

    pinMode(9,  OUTPUT);
    pinMode(11,  OUTPUT);
    pinMode(13,  OUTPUT);
    digitalWrite(9, LOW);
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);

    TM1637_clear();
}

void loop()
{
    
    digitalVal = digitalRead(digitalPin);

    
    int v_input = analogRead(analogPin);

    int size = 0;
    int* arr = intToArray(v_input, &size);

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

    TM1637_clear();

    
    int a_0 = 0;
    if(v_input == 0)
    {
        TM1637_display_segments(4, _digit_addr[0]);
        a_0 = 1;
    }
    if(arr[0] >=0 && arr[0] <= 9)
    {
        TM1637_display_segments(4, _digit_addr[arr[0]]);
    }

    
    if(arr[1] >=0 && arr[1] <= 9 && v_input >= 10)
    {
        TM1637_display_segments(1, _digit_addr[arr[1]]);
    }

    
    if(arr[2] >=0 && arr[2] <= 9 && v_input >= 100)
    {
        TM1637_display_segments(2, _digit_addr[arr[2]]);
    }

    
    if(arr[3] >=0 &&
    arr[3] <= 9 && v_input >= 100)
    {
        TM1637_display_segments(3, _digit_addr[arr[3]]);
    }

    if(v_input==1023)
    {
        




        
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);

        digitalWrite(8, HIGH);
    }
    else
    {
        




        
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);

        digitalWrite(8, LOW);
    }

    free(arr);

    delay(2000);
}



