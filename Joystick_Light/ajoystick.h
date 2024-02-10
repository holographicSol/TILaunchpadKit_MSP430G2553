/*
 * AJoyStick - Light - Written by Benjamin Jack Cullen
 *
 * Multiple Joy Sticks      No
 * Dynamic Range            Yes
 * Force                    No
 * Stabilisation            Yes
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// ------------------------ calibration: account for lower end joy sticks by providing calibration for stabilisation.
const int xyresolution       = 500;                  // ensure resolution is approximately proportional to offset.  (high-end=512)
const int xidlemin           = 490;                  // stabilise no x input using an x idle minimum.               (high-end=512)
const int xidlemax           = 510;                  // stabilise no x input using an x idle minimum.               (high-end=512)
const int yidlemin           = 530;                  // stabilise no y input using an y idle minimum.               (high-end=512)
const int yidlemax           = 550;                  // stabilise no y input using an y idle max.                   (high-end=512)
const int xyminmaxstabalizer = 10;                   // stabilise x,y minimum and max input                         (high-end=0)
const int x_offset           = (xidlemax-xidlemin);  // idle range between x idle minimum and x idle maximum.       (high-end=0)    (sets automatically)
const int y_offset           = (yidlemax-yidlemin);  // idle range between y idle minimum and y idle maximum.       (high-end=0)    (sets automatically)
// ---------------------------------- joy stick zero: place holders
int jx_ax                    = 0;  // raw analogue:   x axis
int jx_ay                    = 0;  // raw analogue:   y axis
int jx_ac                    = 0;  // raw analogue:   click
int jx_x0                    = 0;  // mapped left:    0-500
int jx_x1                    = 0;  // mapped right:   0-500
int jx_y0                    = 0;  // mapped up:      0-500
int jx_y1                    = 0;  // mapped down:    0-500
int jx_c0                    = 0;  // mapped clicked: zero/one

void interpret_joy(int jx_ax, int jx_ay, int jx_ac){
    // mapping: converts two analogue values to four numbers between 0 and 500 and converts one analogue number to zero or one.
    if       ((jx_ay  <=  yidlemin) && (jx_ax  <=  xidlemin))  {jx_x0=abs(xyresolution-jx_ax);  jx_x1=0;                                   jx_y0=abs(xyresolution-jx_ay);    jx_y1=0;}                                   // left up
    else if  ((jx_ay  <=  yidlemin) && (jx_ax  >=  xidlemax))  {jx_x0=0;                        jx_x1=abs((jx_ax-xyresolution)-x_offset);  jx_y0=abs(xyresolution-jx_ay);    jx_y1=0;}                                   // right up
    else if  ((jx_ay  >=  yidlemax) && (jx_ax  <=  xidlemin))  {jx_x0=abs(xyresolution-jx_ax);  jx_x1=0;                                   jx_y0=0;                          jx_y1=abs((jx_ay-xyresolution)-y_offset);}  // left down
    else if  ((jx_ay  >=  yidlemax) && (jx_ax  >=  xidlemax))  {jx_x0=0;                        jx_x1=abs((jx_ax-xyresolution)-x_offset);  jx_y0=0;                          jx_y1=abs((jx_ay-xyresolution)-y_offset);}  // right down
    else if  (jx_ax   <=  xidlemin)                            {jx_x0=abs(xyresolution-jx_ax);  jx_x1=0;                                   jx_y0=0;                          jx_y1=0;}                                   // left
    else if  (jx_ax   >=  xidlemax)                            {jx_x0=0;                        jx_x1=(jx_ax-xyresolution)-x_offset;       jx_y0=0;                          jx_y1=0;}                                   // right
    else if  (jx_ay   <=  yidlemin)                            {jx_x0=0;                        jx_x1=0;                                   jx_y0=abs(xyresolution-jx_ay);    jx_y1=0;}                                   // up
    else if  (jx_ay   >=  yidlemax)                            {jx_x0=0;                        jx_x1=0;                                   jx_y0=0;                          jx_y1=abs((jx_ay-xyresolution)-y_offset);}  // down
    else                                                       {jx_x0=0;                        jx_x1=0;                                   jx_y0=0;                          jx_y1=0;}                                   // none
    if(jx_ac == 0){jx_c0=1;} else{jx_c0=0;}                                                                                                                                                                              // click
    // sanitise: ensures values are between 0 and 500 and add stability for max and minimum when values in range of max and minimum.
    if (jx_x0 >= (xyresolution))                     {jx_x0=xyresolution;}  // sanitise max:     n max
    if (jx_y0 >= (xyresolution-xyminmaxstabalizer))  {jx_y0=xyresolution;}  // sanitise max:     n max
    if (jx_x1 >= (xyresolution-xyminmaxstabalizer))  {jx_x1=xyresolution;}  // sanitise max:     n max
    if (jx_y1 >= (xyresolution-xyminmaxstabalizer))  {jx_y1=xyresolution;}  // sanitise max:     n max
    if (jx_x0 <= (0+xyminmaxstabalizer))             {jx_x0=0;}             // sanitise minimum: 0 minimum
    if (jx_y0 <= (0+xyminmaxstabalizer))             {jx_y0=0;}             // sanitise minimum: 0 minimum
    if (jx_x1 <= (0+xyminmaxstabalizer))             {jx_x1=0;}             // sanitise minimum: 0 minimum
    if (jx_y1 <= (0+xyminmaxstabalizer))             {jx_y1=0;}             // sanitise minimum: 0 minimum                                                                                                                                                                            // click
}
