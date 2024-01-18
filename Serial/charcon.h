/*
 * Designed for MSP430G2553
 * Provides functionality otherwise lost in partial printf library due to limited FRAM and RAM
 * on the MSP430G2553.
 * Functionality converts int's, floats to strings and strings to int's and floats.
 * Probably not as efficient as it could be but it fits on the MCU unlike many parts of
 * various libraries needed to achieve the same effects.
 * Allows for concise powerful and precise setting of values in MCU from RXD for example.
 * This was worth squeezing into the MCU because a variable(s) may have a range of accepted values,
 * like for volume in range of 0-100 for one example, this can be more direct without iteration and or
 * large command lists.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

float stof(char *str)
{
    float res = 0;
    float sign = 1;
    float factor = 1.0;
    bool decimal = false;
    int i = 0;
    if (str[0] == '-')          // if number negative, update sign
    {sign = -1; i++;}           // update index of first digit
    for (; str[i] != '\0'; ++i) // iterate digits and update result
    {
        if (str[i] == '.')
        {decimal = true; continue;}
        int digit = str[i] - '0';
        if (decimal){factor /= 10.0f;}
        res = res*10.0f + (float)digit;
    }
    return sign*res/factor; // return result with sign
}

int stoi(char *str)
{
    int res = 0;
    int sign = 1;
    int i = 0;
    if (str[0] == '-')               // if number negative, update sign
    {sign = -1; i++;}                // update index of first digit
    for (; str[i] != '\0'; ++i)      // iterate digits and update result
        res = res*10 + str[i] - '0';
    return sign*res;                 // return result with sign
}

void itoa(int num, char *str, int radix)
{
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int spos;
    int sign;
    int count;
    count = 0;
    if ((sign = num) < 0)
    num = -num;
    do {str[count++] = digits[num % radix];}
    while ((num /= radix) > 0);
    if (sign < 0)
    str[count++] = '-';
    str[count] = '\0';
    // reverse the string
    for (spos = 0 , count--; spos < count; spos++, count--) {
    sign = str[spos];
    str[spos] = str[count];
    str[count] = (char) sign;
    }
}

typedef enum {
    INT,
    FLOAT
} NumType;
void ifstring(float num, char* str, size_t size, NumType type) {
    int int_part = (int)num;
    char int_str[20];
    itoa(int_part, int_str, 10);
    if (type == FLOAT) {
        int frac_part = (int)((num - int_part) * 1000000); // 6 decimal places
        char frac_str[20];
        itoa(frac_part, frac_str, 10);
        snprintf(str, size, "%s.%s", int_str, frac_str);
    } else if (type == INT) {
        snprintf(str, size, "%s", int_str);
    }
}

void slice_inplace(char* str, int start, int end) {
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

void strip(char *s) { // leading spaces
    while(isspace((unsigned char) *s)) s++;

    if(*s) { // trailing spaces
        char *p = s + strlen(s) - 1;
        while(p > s && isspace((unsigned char) *p)) p--;
        *(p+1) = '\0';
    }
}
