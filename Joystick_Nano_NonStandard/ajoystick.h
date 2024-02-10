/*
 * AJoyStick - Nano Non-Standard - Written by Benjamin Jack Cullen
 *
 * Multiple Joy Sticks      No
 * Dynamic Range            No
 * Force                    No
 * Stabilisation            Idle
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned short int x0 = 0;
unsigned short int x1 = 0;
unsigned short int y0 = 0;
unsigned short int y1 = 0;
unsigned short int c0 = 0;

void interpret_joy(int adc_a, int adc_b, int adc_c){
    if      ((adc_b  <=  535) && (adc_a  <=  500)) {x0=1; x1=0; y0=1; y1=0;}  // up left
    else if ((adc_b  <=  535) && (adc_a  >=  510)) {x0=0; x1=1; y0=1; y1=0;}  // up right
    else if ((adc_b  >=  545) && (adc_a  <=  500)) {x0=1; x1=0; y0=0; y1=1;}  // down left
    else if ((adc_b  >=  545) && (adc_a  >=  510)) {x0=0; x1=1; y0=0; y1=1;}  // down right
    else{x0=0; x1=0; y0=0; y1=0;}                                             // none
    if(adc_c ==  0){c0=1;} else{c0=0;}                                        // click
}

