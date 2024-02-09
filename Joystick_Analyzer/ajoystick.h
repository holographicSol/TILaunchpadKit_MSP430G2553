/*
 * Analogue Joy Stick. Written by Benjamin Jack Cullen.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int xyresolution       = 500;                 // ideally 512 however for lower precision joy sticks we can trade some resolution for stabilisation. resolution should be approximately proportional to offset
const int xidlemin           = 500;                 // stabilise no x input using an x idle minimum
const int xidlemax           = 510;                 // stabilise no x input using an x idle minimum
const int yidlemin           = 535;                 // stabilise no y input using an y idle minimum
const int yidlemax           = 545;                 // stabilise no y input using an y idle max
const int x_offset           = (xidlemax-xidlemin); // idle range between x idle minimum and x idle maximum
const int y_offset           = (yidlemax-yidlemin); // idle range between y idle minimum and y idle maximum
const int xyminmaxstabalizer = 10;                  // stabilise x,y minimum and max input
int j0_ax = 0;  // analogue x axis
int j0_ay = 0;  // analogue y axis
int j0_ac = 0;  // analogue click
int j0_x0 = 0;  // left     0-500
int j0_x1 = 0;  // right    0-500
int j0_y0 = 0;  // up       0-500
int j0_y1 = 0;  // down     0-500
int j0_c0 = 0;  // clicked  zero/one

void interpret_joy_0(int j0_ax, int j0_ay, int j0_ac){
    // mapping: converts two analogue values to four 0-500 numbers.
    if       ((j0_ay  <=  yidlemin) && (j0_ax  <=  xidlemin))  {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  <=  yidlemin) && (j0_ax  >=  xidlemax))  {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  >=  yidlemax) && (j0_ax  <=  xidlemin))  {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else if  ((j0_ay  >=  yidlemax) && (j0_ax  >=  xidlemax))  {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else if  (j0_ax   <=  xidlemin)                            {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=0;                          j0_y1=0;}
    else if  (j0_ax   >=  xidlemax)                            {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=0;                          j0_y1=0;}
    else if  (j0_ay   <=  yidlemin)                            {j0_x0=0;                        j0_x1=0;                        j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  (j0_ay   >=  yidlemax)                            {j0_x0=0;                        j0_x1=0;                        j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else                                                       {j0_x0=0;                        j0_x1=0;                        j0_y0=0;                          j0_y1=0;}
    // sanitise: ensures values are between 0 and 500 and also adds stability for max and minimum when values in range of max and minimum
    if (j0_x0 >= (500-xyminmaxstabalizer)){j0_x0=xyresolution;}  // sanitise max:     n max
    if (j0_y0 >= (500-xyminmaxstabalizer)){j0_y0=xyresolution;}  // sanitise max:     n max
    if (j0_x1 >= (500-xyminmaxstabalizer)){j0_x1=xyresolution;}  // sanitise max:     n max
    if (j0_y1 >= (500-xyminmaxstabalizer)){j0_y1=xyresolution;}  // sanitise max:     n max
    if (j0_x0 <= (0+xyminmaxstabalizer))  {j0_x0=0;}             // sanitise minimum: 0 minimum
    if (j0_y0 <= (0+xyminmaxstabalizer))  {j0_y0=0;}             // sanitise minimum: 0 minimum
    if (j0_x1 <= (0+xyminmaxstabalizer))  {j0_x1=0;}             // sanitise minimum: 0 minimum
    if (j0_y1 <= (0+xyminmaxstabalizer))  {j0_y1=0;}             // sanitise minimum: 0 minimum
    if (j0_ac == 0)  {j0_c0=1;}    // zero one logic for click
    else{j0_c0=0;}
}
