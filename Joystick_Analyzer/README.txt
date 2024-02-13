1306 XTVTX 0.96 Inch OLED Module 12864 128x64 Driver IIC I2C Serial Self-Luminous Display Board (Blue and Yellow):
SCK P1.7
SDA P1.6
+   5v

Joystick: XY Dual-axis
X Axis P1.2
Y Axis P1.3
Click  P1.4
+ 	 3.3v


Manual calibration of ajoystick.h is performed by observing the output of JoyStick Analyzer to see if X0,X1,X2,X3
and Y0,Y1,Y2,Y3 are stable, if they are not stable then adjust the following values when calling interpret_joy()
to re-calibrate, then once again observe the output from JoyStick Analyzer and repeat the process until the joy
stick can be used in a stable way:

// ------------------------ calibration: stabilisation
int xyresolution;        // ensure resolution is approximately proportional to offset.  (high-end=512)
int xidlemin;            // stabilise no x input using an x idle minimum.               (high-end=512)
int xidlemax;            // stabilise no x input using an x idle minimum.               (high-end=512)
int yidlemin;            // stabilise no y input using an y idle minimum.               (high-end=512)
int yidlemax;            // stabilise no y input using an y idle max.                   (high-end=512)
int xyminmaxstabalizer;  // stabilise x,y minimum and max input                         (high-end=0)


Demonstration:
https://www.youtube.com/watch?v=2h-quhcb83I