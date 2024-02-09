/*
 * Analogue Joy Stick. Written by Benjamin Jack Cullen.
 * j0: Joy stick zero. This library can be reproduced with j0 replaced with jN for more Joy Sticks.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int j0_ax = 0;  // analogue x axis
int j0_ay = 0;  // analogue y axis
int j0_ac = 0;  // analogue click
int j0_x0 = 0;  // left     0-500
int j0_x1 = 0;  // right    0-500
int j0_y0 = 0;  // up       0-500
int j0_y1 = 0;  // down     0-500
int j0_c0 = 0;  // clicked  zero/one

void interpret_joy_0(int j0_ax, int j0_ay, int j0_ac){
    // Zero to 500 converter: converts two analogue values to four 0-500 numbers.
    // An offset of 10 is also applied that accounts for a fluctuation range of 10 when my particular joy stick is idle (x=500-510, y=535-545). (A software patch for hardware problem).
    if       ((j0_ay  <=  535) && (j0_ax  <=  500))  {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=abs(500-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  <=  535) && (j0_ax  >=  510))  {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=abs(500-j0_ay);    j0_y1=0;}
    else if  ((j0_ay  >=  545) && (j0_ax  <=  500))  {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else if  ((j0_ay  >=  545) && (j0_ax  >=  510))  {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else if  (j0_ax   <=  500)                       {j0_x0=abs(500-j0_ax);  j0_x1=0;                  j0_y0=0;                 j0_y1=0;}
    else if  (j0_ax   >=  510)                       {j0_x0=0;               j0_x1=abs((j0_ax/2)-10);  j0_y0=0;                 j0_y1=0;}
    else if  (j0_ay   <=  535)                       {j0_x0=0;               j0_x1=0;                  j0_y0=abs(500-j0_ay);    j0_y1=0;}
    else if  (j0_ay   >=  545)                       {j0_x0=0;               j0_x1=0;                  j0_y0=0;                 j0_y1=abs((j0_ay/2)-10);}
    else                                             {j0_x0=0;               j0_x1=0;                  j0_y0=0;                 j0_y1=0;}
    // sanitise: ensures values are between 0 and 500 and also adds stability for max and minimum when values in range of max and minimum
    if (j0_x0 >= 490){j0_x0=500;}  // sanitise max:     500 max
    if (j0_y0 >= 490){j0_y0=500;}  // sanitise max:     500 max
    if (j0_x1 >= 490){j0_x1=500;}  // sanitise max:     500 max
    if (j0_y1 >= 490){j0_y1=500;}  // sanitise max:     500 max
    if (j0_x0 <= 10) {j0_x0=0;}    // sanitise minimum: 0 minimum
    if (j0_y0 <= 10) {j0_y0=0;}    // sanitise minimum: 0 minimum
    if (j0_x1 <= 10) {j0_x1=0;}    // sanitise minimum: 0 minimum
    if (j0_y1 <= 10) {j0_y1=0;}    // sanitise minimum: 0 minimum
    if (j0_ac == 0)  {j0_c0=1;}    // zero one logic for click
    else{j0_c0=0;}
}
