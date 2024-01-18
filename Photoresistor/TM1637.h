/*
 * TM1637.h
 *
 */

#ifndef TM1637_H_
#define TM1637_H_

// Prototypes
static void TM1637_send_config(const int enable, const int brightness);
static void TM1637_send_command(const int value);
static void TM1637_start(void);
static void TM1637_stop(void);
static int TM1637_write_byte(int value);


#endif /* TM1637_H_ */

// Main Settings
#define TM1637_DIO_PIN          3 // Pin 1.3 and 1.4
#define TM1637_CLK_PIN          5
#define TM1637_DELAY_US         (5)
#define TM1637_BRIGHTNESS_MAX   (7)
#define TM1637_BRIGHTNESS_MIN   (1)
#define TM1637_POSITION_MAX     (4)

static int SEG_DIG_0            = 0x3F;
static int SEG_DIG_1            = 0x06;
static int SEG_DIG_2            = 0x5B;
static int SEG_DIG_3            = 0x4F;
static int SEG_DIG_4            = 0x66;
static int SEG_DIG_5            = 0x6D;
static int SEG_DIG_6            = 0x7D;
static int SEG_DIG_7            = 0x07;
static int SEG_DIG_8            = 0x7F;
static int SEG_DIG_9            = 0x6F;
static int SEG_TOP              = 0x01;
static int SEG_TOP_LEFT         = 0x20;
static int SEG_TOP_RIGHT        = 0x02;
static int SEG_BOTTOM           = 0x08;
static int SEG_BOTTOM_LEFT      = 0x10;
static int SEG_BOTTOM_RIGHT     = 0x04;
static int SEG_CENTRE           = 0x40;
static int SEG_CUBE_TOP         = 0x01 + 0x20 + 0x02 + 0x40;
static int SEG_CUBE_BOTTOM      = 0x08 + 0x10 + 0x04 + 0x40;
static int SEG_LINE_VERT_LEFT   = 0x20 + 0x10;
static int SEG_LINE_VERT_RIGHT  = 0x02 + 0x04;
static int SEG_ALPHA_A          = 0x01 + 0x20 + 0x02 + 0x10 + 0x04 + 0x40;
static int SEG_ALPHA_B          = 0x7F;
static int SEG_ALPHA_C          = 0x01 + 0x20 + 0x10 + 0x08;
static int SEG_ALPHA_D          = 0x3F;
static int SEG_ALPHA_E          = 0x01 + 0x20 + 0x10 + 0x08 + 0x40;
static int SEG_ALPHA_F          = 0x01 + 0x20 + 0x10 + 0x40;
static int SEG_ALPHA_G          = 0x01 + 0x20 + 0x10 + 0x08 + 0x04;
static int SEG_ALPHA_H          = 0x20 + 0x10 + 0x40 + 0x02 + 0x04;
static int SEG_ALPHA_I_LEFT     = 0x20 + 0x10;
static int SEG_ALPHA_I_RIGHT    = 0x02 + 0x04;
static int SEG_ALPHA_J          = 0x02 + 0x04 + 0x08;
static int SEG_ALPHA_K          = 0x40;
static int SEG_ALPHA_L          = 0x20 + 0x10 + 0x08;
static int SEG_ALPHA_M          = 0x40;
static int SEG_ALPHA_N          = 0x40;
static int SEG_ALPHA_O          = 0x3F;
static int SEG_ALPHA_P          = 0x01 + 0x20 + 0x10 + 0x02 + 0x40;
static int SEG_ALPHA_Q          = 0x40;
static int SEG_ALPHA_R          = 0x40;
static int SEG_ALPHA_S          = 0x01 + 0x20 + 0x40 + 0x04 + 0x08;
static int SEG_ALPHA_T          = 0x02 + 0x01 + 0x04;
static int SEG_ALPHA_U          = 0x20 + 0x04 + 0x08 + 0x10 + 0x02;
static int SEG_ALPHA_V          = 0x40;
static int SEG_ALPHA_W          = 0x40;
static int SEG_ALPHA_X          = 0x40;
static int SEG_ALPHA_Y          = 0x20 + 0x02 + 0x40 + 0x04;
static int SEG_ALPHA_Z          = 0x01 + 0x02 + 0x40 + 0x10 + 0x08;

static const int _shape2segments[] =
{
     0x01 + 0x20 + 0x02 + 0x40, // square top
     0x08 + 0x10 + 0x04 + 0x40  // square bottom
};

static const int _truesegments[] =
{
     0x20, // top left
     0x01, // top
     0x02, // top right
     0x40, // centre
     0x10, // bottom left
     0x08, // bottom
     0x04  // bottom right
};

static const int _digit2segments[] =
{
     0x3F, // 0
     0x06, // 1
     0x5B, // 2
     0x4F, // 3
     0x66, // 4
     0x6D, // 5
     0x7D, // 6
     0x07, // 7
     0x7F, // 8
     0x6F, // 9
};

static const int _alpha2segments[] =
{
     0x01 + 0x20 + 0x02 + 0x10 + 0x04 + 0x40, // A
     0x7F,                                    // B
     0x01 + 0x20 + 0x10 + 0x08,               // C
     0x3F,                                    // D
     0x01 + 0x20 + 0x10 + 0x08 + 0x40,        // E
     0x01 + 0x20 + 0x10 + 0x40,               // F
     0x01 + 0x20 + 0x10 + 0x08 + 0x04,        // G
     0x20 + 0x10 + 0x40 + 0x02 + 0x04,        // H
     0x20 + 0x10,                             // I
     0x02 + 0x04,                             // J
     0x02 + 0x04 + 0x08,                      // K
     0x20 + 0x10 + 0x08,                      // L
//     0x40,                                    // M (-)
//     0x40,                                    // N (-)
     0x3F,                                    // O
     0x01 + 0x20 + 0x10 + 0x02 + 0x40,        // P
//     0x40,                                    // Q (-)
//     0x40,                                    // R (-)
     0x01 + 0x20 + 0x40 + 0x04 + 0x08,        // S
     0x02 + 0x01 + 0x04,                      // T
     0x20 + 0x04 + 0x08 + 0x10 + 0x02,        // U
//     0x40,                                    // V (-)
//     0x40,                                    // W (-)
//     0x40,                                    // X (-)
     0x20 + 0x02 + 0x40 + 0x04,               // Y
     0x01 + 0x02 + 0x40 + 0x10 + 0x08         // Z
};
