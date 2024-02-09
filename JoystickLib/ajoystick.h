/*
 * Analogue Joy Stick.
 * Map:
 *    j0_x0 = left  = range(0,500)
 *    j0_x1 = right = range(0,500)
 *    j0_y0 = up    = range(0,500)
 *    j0_y1 = down  = range(0,500)
 *    j0_ac = click =            0 = clicked
 *
 * j0: Joy stick zero. This library can be reproduced with j0 replaced with jN for more Joy Sticks.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int j0_ax = 0;
int j0_ay = 0;
int j0_ac = 0;
int j0_x0 = 0;
int j0_x1 = 0;
int j0_y0 = 0;
int j0_y1 = 0;

void interpret_joy_0(int j0_ax, int j0_ay, int j0_ac){
    // Zero to 500 converter: converts two analogue values to four 0-500 numbers.
    // An offset of 10 is also applied that accounts for a fluctuation range of 10 when my particular joy stick is idle (x=500-510, y=535-545). (A software patch for hardware problem).
    if       ((j0_ay  <=  535) && (j0_ax  <=  500))  {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=abs((500-j0_ay));  j0_y1=0;}
    else if  ((j0_ay  <=  535) && (j0_ax  >=  510))  {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=abs(500-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  >=  545) && (j0_ax  <=  500))  {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else if  ((j0_ay  >=  545) && (j0_ax  >=  510))  {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else if  (j0_ax   <=  500)                       {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=0;                 j0_y1=0;}
    else if  (j0_ax   >=  510)                       {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=0;                 j0_y1=0;}
    else if  (j0_ay   <=  535)                       {j0_x0=0;               j0_x1=0;                  j0_y0=abs(500-j0_ay);    j0_y1=0;}
    else if  (j0_ay   >=  545)                       {j0_x0=0;               j0_x1=0;                  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else                                             {j0_x0=0;               j0_x1=0;                  j0_y0=0;                 j0_y1=0;}
    // sanitise the normalisation (again joy stick specific, may not be necessary depending on quality of joy stick). this does not report corrections, only corrects:
    if (j0_x0 > 500 ){j0_x0=500;}  // ensure 500 max
    if (j0_y0 > 500 ){j0_y0=500;}  // ensure 500 max
    if (j0_x1 > 500 ){j0_x1=500;}  // ensure 500 max
    if (j0_y1 > 500 ){j0_y1=500;}  // ensure 500 max
    if (j0_x0 < 0 ){j0_x0=0;}      // ensure   0 minimum
    if (j0_y0 < 0 ){j0_y0=0;}      // ensure   0 minimum
    if (j0_x1 < 0 ){j0_x1=0;}      // ensure   0 minimum
    if (j0_y1 < 0 ){j0_y1=0;}      // ensure   0 minimum
    if (j0_y0 >= 496){j0_y0=500;}  // in my case y goes no lower than 4 on analogue value so i will force 500 at from 496
}
