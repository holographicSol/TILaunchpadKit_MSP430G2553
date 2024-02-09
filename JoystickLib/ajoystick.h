/*
 * Analogue Joy Stick. Written by Benjamin Jack Cullen.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// ---------------------------------------------------- calibration start: account for lower end joy sticks by providing calibration for stabilisation.
const int xyresolution       = 500;                  // ensure resolution is approximately proportional to offset.  (high-end=512)
const int xidlemin           = 500;                  // stabilise no x input using an x idle minimum.               (high-end=512)
const int xidlemax           = 510;                  // stabilise no x input using an x idle minimum.               (high-end=512)
const int yidlemin           = 535;                  // stabilise no y input using an y idle minimum.               (high-end=512)
const int yidlemax           = 545;                  // stabilise no y input using an y idle max.                   (high-end=512)
const int x_offset           = (xidlemax-xidlemin);  // idle range between x idle minimum and x idle maximum.       (high-end=0)    (sets automatically)
const int y_offset           = (yidlemax-yidlemin);  // idle range between y idle minimum and y idle maximum.       (high-end=0)    (sets automatically)
const int xyminmaxstabalizer = 10;                   // stabilise x,y minimum and max input                         (high-end=0)
// ---------------------------------------------------- calibration end.
int j0_ax                    = 0;                    // raw analogue:   x axis
int j0_ay                    = 0;                    // raw analogue:   y axis
int j0_ac                    = 0;                    // raw analogue:   click
int j0_x0                    = 0;                    // mapped left:    0-500
int j0_x1                    = 0;                    // mapped right:   0-500
int j0_y0                    = 0;                    // mapped up:      0-500
int j0_y1                    = 0;                    // mapped down:    0-500
int j0_c0                    = 0;                    // mapped clicked: zero/one

void interpret_joy_0(int j0_ax, int j0_ay, int j0_ac){
    // mapping: converts two analogue values to four numbers between 0 and 500.
    if       ((j0_ay  <=  yidlemin) && (j0_ax  <=  xidlemin))  {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  <=  yidlemin) && (j0_ax  >=  xidlemax))  {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  >=  yidlemax) && (j0_ax  <=  xidlemin))  {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else if  ((j0_ay  >=  yidlemax) && (j0_ax  >=  xidlemax))  {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else if  (j0_ax   <=  xidlemin)                            {j0_x0=abs(xyresolution-j0_ax);  j0_x1=0;                        j0_y0=0;                          j0_y1=0;}
    else if  (j0_ax   >=  xidlemax)                            {j0_x0=0;                        j0_x1=abs((j0_ax/2)-x_offset);  j0_y0=0;                          j0_y1=0;}
    else if  (j0_ay   <=  yidlemin)                            {j0_x0=0;                        j0_x1=0;                        j0_y0=abs(xyresolution-j0_ay);    j0_y1=0;}
    else if  (j0_ay   >=  yidlemax)                            {j0_x0=0;                        j0_x1=0;                        j0_y0=0;                          j0_y1=abs((j0_ay/2)-y_offset);}
    else                                                       {j0_x0=0;                        j0_x1=0;                        j0_y0=0;                          j0_y1=0;}
    // sanitise: ensures values are between 0 and 500 and add stability for max and minimum when values in range of max and minimum.
    if (j0_x0 >= (xyresolution)){j0_x0=xyresolution;}                     // sanitise max:     n max
    if (j0_y0 >= (xyresolution-xyminmaxstabalizer)){j0_y0=xyresolution;}  // sanitise max:     n max
    if (j0_x1 >= (xyresolution-xyminmaxstabalizer)){j0_x1=xyresolution;}  // sanitise max:     n max
    if (j0_y1 >= (xyresolution-xyminmaxstabalizer)){j0_y1=xyresolution;}  // sanitise max:     n max
    if (j0_x0 <= (0+xyminmaxstabalizer))  {j0_x0=0;}                      // sanitise minimum: 0 minimum
    if (j0_y0 <= (0+xyminmaxstabalizer))  {j0_y0=0;}                      // sanitise minimum: 0 minimum
    if (j0_x1 <= (0+xyminmaxstabalizer))  {j0_x1=0;}                      // sanitise minimum: 0 minimum
    if (j0_y1 <= (0+xyminmaxstabalizer))  {j0_y1=0;}                      // sanitise minimum: 0 minimum
    if (j0_ac == 0)  {j0_c0=1;}                                           // clicked:          zero/one
    else{j0_c0=0;}
}
