/*
 * AJoyStick - Heavy - Written by Benjamin Jack Cullen
 *
 * Multiple Joy Sticks      Yes
 * Dynamic Range            Yes
 * Force                    Yes
 * Stabilisation            Yes
 *
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
int force_resolution;    // maps 0-xyresolution to a new specified range                (concept here is use a number that will result in a stable, consistent use of force when force is applied)
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

short map(short x, short in_min, short in_max, short out_min, short out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void interpret_joy(int jx_ax, int jx_ay, int jx_ac, int xyresolution, int xidlemin, int xidlemax, int yidlemin, int yidlemax, int xyminmaxstabalizer, int force_resolution, int joystickselect){
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
    // meaning degrees of force can be further divided or less divided depending on needs and hardware ability.
    jx_x2 = map(jx_x0, 0, xyresolution, 0, force_resolution);
    jx_x3 = map(jx_x1, 0, xyresolution, 0, force_resolution);
    jx_y2 = map(jx_y0, 0, xyresolution, 0, force_resolution);
    jx_y3 = map(jx_y1, 0, xyresolution, 0, force_resolution);

    // ----- set values for joy stick selected: copy/paste this statement as jN and joy stick selected to save more joy.
    if      (joystickselect==0){j0_x0=jx_x0; j0_x1=jx_x1; j0_y0=jx_y0; j0_y1=jx_y1; j0_x2=jx_x2; j0_x3=jx_x3; j0_y2=jx_y2; j0_y3=jx_y3; j0_c0=jx_c0;}
}
