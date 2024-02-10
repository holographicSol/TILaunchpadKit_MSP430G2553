/*
 * Analogue Joy Stick. Written by Benjamin Jack Cullen.
 * Designed for multiple joy sticks.
 * 1: copy/paste joy stick value place holders.
 * 2: copy/paste set joy stick value place holders.
 * 3: call interpret_joy with adc values for a joy stick, joy stick calibration and joy stick selected.
 * This library is setup for different requirements like: (a) one joy stick (b) multiple joy sticks (c) memory requirements,
 * (d) precision as in N on or N off or is input exactly N or in range or N?
 * by facilitating the following:
 *                           (w) a single analogue joy stick.
 *                           (x) multiple analogue joy sticks, each overwriting stick zero place holder values.
 *                           (y) multiple analogue joy sticks using their own place holder values.
 *                           (z) 0-N input values that can be used as 'if not zero' (is left/right/up/down) and or as
 *                               'if input value for left/right/up/down specifically in N range or equal to N'.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// ------------------------ calibration: account for lower end joy sticks by providing calibration for stabilisation.
int xyresolution;        // ensure resolution is approximately proportional to offset.  (high-end=512)
int xidlemin;            // stabilise no x input using an x idle minimum.               (high-end=512)
int xidlemax;            // stabilise no x input using an x idle minimum.               (high-end=512)
int yidlemin;            // stabilise no y input using an y idle minimum.               (high-end=512)
int yidlemax;            // stabilise no y input using an y idle max.                   (high-end=512)
int xyminmaxstabalizer;  // stabilise x,y minimum and max input                         (high-end=0)
int x_offset;            // idle range between x idle minimum and x idle maximum.       (high-end=0)    (sets automatically)
int y_offset;            // idle range between y idle minimum and y idle maximum.       (high-end=0)    (sets automatically)

// ---------------------------------- system place holders.
int jx_ax                    = 0;  // raw analogue:   x axis
int jx_ay                    = 0;  // raw analogue:   y axis
int jx_ac                    = 0;  // raw analogue:   click
int jx_x0                    = 0;  // mapped left:    0-500
int jx_x1                    = 0;  // mapped right:   0-500
int jx_y0                    = 0;  // mapped up:      0-500
int jx_y1                    = 0;  // mapped down:    0-500
int jx_x2                    = 0;  // force:          left
int jx_x3                    = 0;  // force:          right
int jx_y2                    = 0;  // force:          up
int jx_y3                    = 0;  // force:          down
int jx_c0                    = 0;  // mapped clicked: zero/one

// ---------------------------------- joy stick zero: copy/paste these place holders as jN to save more joy.
int j0_ax                    = 0;  // raw analogue:   x axis
int j0_ay                    = 0;  // raw analogue:   y axis
int j0_ac                    = 0;  // raw analogue:   click
int j0_x0                    = 0;  // mapped left:    0-500
int j0_x1                    = 0;  // mapped right:   0-500
int j0_y0                    = 0;  // mapped up:      0-500
int j0_y1                    = 0;  // mapped down:    0-500
int j0_x2                    = 0;  // force:          left
int j0_x3                    = 0;  // force:          right
int j0_y2                    = 0;  // force:          up
int j0_y3                    = 0;  // force:          down
int j0_c0                    = 0;  // mapped clicked: zero/one

void interpret_joy(int jx_ax, int jx_ay, int jx_ac, int xyresolution, int xidlemin, int xidlemax, int yidlemin, int yidlemax, int xyminmaxstabalizer, int joystickselect){
    // mapping: converts two analogue values to four numbers between 0 and 500 and converts one analogue number to zero or one.
    x_offset = (xidlemax-xidlemin);
    y_offset = (yidlemax-yidlemin);
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
    if (jx_y1 <= (0+xyminmaxstabalizer))             {jx_y1=0;}             // sanitise minimum: 0 minimum

    // set force: third abstraction layer creates predefined ranges of 'force'. relative degrees of force are predicated upon joy stick stability, reasonable user ability and resolution divisibility,
    // meaning degrees of force can be further divided or less divided. update may result in degrees of force range 20 instead of 50, providing further degrees of force.
    if     ((jx_x0 > 0)  &&(jx_x0 <= 50 )){jx_x2=1;} else if((jx_x0 > 50) &&(jx_x0 <= 100)){jx_x2=1;} else if((jx_x0 > 100)&&(jx_x0 <= 150)){jx_x2=2;} else if((jx_x0 > 150)&&(jx_x0 <= 200)){jx_x2=3;}
    else if((jx_x0 > 200)&&(jx_x0 <= 250)){jx_x2=4;} else if((jx_x0 > 250)&&(jx_x0 <= 300)){jx_x2=5;} else if((jx_x0 > 300)&&(jx_x0 <= 350)){jx_x2=6;} else if((jx_x0 > 350)&&(jx_x0 <= 400)){jx_x2=7;}
    else if((jx_x0 > 400)&&(jx_x0 <= 450)){jx_x2=8;} else if((jx_x0 > 450)&&(jx_x0 <= 500)){jx_x2=8;} else if((jx_x0 > 500)&&(jx_x0 <= 550)){jx_x2=9;} else if(jx_x0==0){jx_x2=0;}
    if     ((jx_x1 > 0)  &&(jx_x1 <= 50 )){jx_x3=1;} else if((jx_x1 > 50) &&(jx_x1 <= 100)){jx_x3=1;} else if((jx_x1 > 100)&&(jx_x1 <= 150)){jx_x3=2;} else if((jx_x1 > 150)&&(jx_x1 <= 200)){jx_x3=3;}
    else if((jx_x1 > 200)&&(jx_x1 <= 250)){jx_x3=4;} else if((jx_x1 > 250)&&(jx_x1 <= 300)){jx_x3=5;} else if((jx_x1 > 300)&&(jx_x1 <= 350)){jx_x3=6;} else if((jx_x1 > 350)&&(jx_x1 <= 400)){jx_x3=7;}
    else if((jx_x1 > 400)&&(jx_x1 <= 450)){jx_x3=8;} else if((jx_x1 > 450)&&(jx_x1 <= 500)){jx_x3=8;} else if((jx_x1 > 500)&&(jx_x1 <= 550)){jx_x3=9;} else if(jx_x1==0){jx_x3=0;}
    if     ((jx_y0 > 0)  &&(jx_y0 <= 50 )){jx_y2=1;} else if((jx_y0 > 50) &&(jx_y0 <= 100)){jx_y2=1;} else if((jx_y0 > 100)&&(jx_y0 <= 150)){jx_y2=2;} else if((jx_y0 > 150)&&(jx_y0 <= 200)){jx_y2=3;}
    else if((jx_y0 > 200)&&(jx_y0 <= 250)){jx_y2=4;} else if((jx_y0 > 250)&&(jx_y0 <= 300)){jx_y2=5;} else if((jx_y0 > 300)&&(jx_y0 <= 350)){jx_y2=6;} else if((jx_y0 > 350)&&(jx_y0 <= 400)){jx_y2=7;}
    else if((jx_y0 > 400)&&(jx_y0 <= 450)){jx_y2=8;} else if((jx_y0 > 450)&&(jx_y0 <= 500)){jx_y2=8;} else if((jx_y0 > 500)&&(jx_y0 <= 550)){jx_y2=9;} else if (jx_y0==0){jx_y2=0;}
    if     ((jx_y1 > 0)  &&(jx_y1 <= 50 )){jx_y3=1;} else if((jx_y1 > 50) &&(jx_y1 <= 100)){jx_y3=1;} else if((jx_y1 > 100)&&(jx_y1 <= 150)){jx_y3=2;} else if((jx_y1 > 150)&&(jx_y1 <= 200)){jx_y3=3;}
    else if((jx_y1 > 200)&&(jx_y1 <= 250)){jx_y3=4;} else if((jx_y1 > 250)&&(jx_y1 <= 300)){jx_y3=5;} else if((jx_y1 > 300)&&(jx_y1 <= 350)){jx_y3=6;} else if((jx_y1 > 350)&&(jx_y1 <= 400)){jx_y3=7;}
    else if((jx_y1 > 400)&&(jx_y1 <= 450)){jx_y3=8;} else if((jx_y1 > 450)&&(jx_y1 <= 500)){jx_y3=8;} else if((jx_y1 > 500)&&(jx_y1 <= 550)){jx_y3=9;} else if (jx_y1==0){jx_y3=0;}

    // ----- set values for joy stick selected: copy/paste this statement as jN and joy stick selected to save more joy.
    if      (joystickselect==0){j0_x0=jx_x0; j0_x1=jx_x1; j0_y0=jx_y0; j0_y1=jx_y1; j0_x2=jx_x2; j0_x3=jx_x3; j0_y2=jx_y2; j0_y3=jx_y3; j0_c0=jx_c0;}
}
