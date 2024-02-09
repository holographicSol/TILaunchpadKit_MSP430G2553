/*
 * Analogue Joy Stick.
 * Map:
 *    x0=left,x1=right,y0=up,y1=down: range=(0,500)
 *    ac=click                            0=clicked
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int ax = 0;
int ay = 0;
int ac = 0;
int x0 = 0;
int x1 = 0;
int y0 = 0;
int y1 = 0;

int joystick_0(int ax, int ay, int ac){
    // Zero to 500 converter: converts two analogue values to four 0-500 numbers.
    // An offset of 10 is also applied that accounts for a fluctuation range of 10 when my particular joy stick is idle (x=500-510, y=535-545). (A software patch for hardware problem).
    if       ((ay  <=  535) && (ax  <=  500))  {x0=abs(500-ax);  x1=0;               y0=abs((500-ay));  y1=0;}
    else if  ((ay  <=  535) && (ax  >=  510))  {x0=0;            x1=abs((ax/2)-10);  y0=abs(500-ay);    y1=0;}
    else if  ((ay  >=  545) && (ax  <=  500))  {x0=abs(500-ax);  x1=0;               y0=0;              y1=abs((ay/2)-10);}
    else if  ((ay  >=  545) && (ax  >=  510))  {x0=0;            x1=abs((ax/2)-10);  y0=0;              y1=abs((ay/2)-10);}
    else if  (ax  <=   500)                    {x0=abs(500-ax);  x1=0;               y0=0;              y1=0;}
    else if  (ax  >=   510)                    {x0=0;            x1=abs((ax/2)-10);  y0=0;              y1=0;}
    else if  (ay  <=   535)                    {x0=0;            x1=0;               y0=abs(500-ay);    y1=0;}
    else if  (ay  >=   545)                    {x0=0;            x1=0;               y0=0;              y1=abs((ay/2)-10);}
    else                                       {x0=0;            x1=0;               y0=0;              y1=0;}
    // sanitise the normalisation (again joy stick specific, may not be necessary depending on quality of joy stick). this does not report corrections, only corrects:
    if (x0 > 500 ){x0=500;}  // ensure 500 max
    if (y0 > 500 ){y0=500;}  // ensure 500 max
    if (x1 > 500 ){x1=500;}  // ensure 500 max
    if (y1 > 500 ){y1=500;}  // ensure 500 max
    if (x0 < 0 ){x0=0;}      // ensure   0 minimum
    if (y0 < 0 ){y0=0;}      // ensure   0 minimum
    if (x1 < 0 ){x1=0;}      // ensure   0 minimum
    if (y1 < 0 ){y1=0;}      // ensure   0 minimum
    if (y0 >= 496){y0=500;}  // in my case y goes no lower than 4 on analogue value so i will force 500 at from 496
}
